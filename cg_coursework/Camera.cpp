#include "Camera.h"
#include "cgmath.h"

Camera::Camera(float fovY, float aspectRatio, float nearZ, float farZ, float radius, Shaders& shader) : pitch(0.f), yaw(0.f), radius(radius){
	projectionMatrix = Matrix(1 / (tan(fovY*M_PI/360) * aspectRatio), 0, 0, 0, 0, 1 / tan(fovY * M_PI / 360), 0, 0, 0, 0, farZ / (farZ - nearZ), -1 * farZ * nearZ / (farZ - nearZ), 0, 0, 1, 0);
	init(shader);
}

void Camera::Update(Shaders& shader) {
	//Create the view matrix
	viewMatrix.ViewMatrix(from, lookat); //look at = to - from
	Matrix VP = projectionMatrix.mul(viewMatrix);
	shader.updateConstantVS("viewProjection", "VP", &VP);
	shader.uploadConstantVS("viewProjection", "VP");
}

void Camera::fromControl(Window& window, float dt) {
	float speed = 10 * dt;
	if (window.keyPressed('W')) {
		from += lookat * speed;
	}
	if (window.keyPressed('S')) {
		from -= lookat * speed;
	}
	if (window.keyPressed('A')) {
		Vec3 right(viewMatrix[0], viewMatrix[1], viewMatrix[2]);
		from -= right * speed;
	}
	if (window.keyPressed('D')) {
		Vec3 right(viewMatrix[0], viewMatrix[1], viewMatrix[2]);
		from += right * speed;
	}
}

void Camera::lookatControl(Window& window) {
	if (window.dirty) {
		yaw = window.deltaX * mouseSensitivity;
		pitch = 1 * window.deltaY * mouseSensitivity;
		window.dirty = false;
	}
	else {
		yaw = 0;
		pitch = 0;
	}
	Matrix rx, ry;
	rx = rx.Rotation(Vec3(viewMatrix[0], viewMatrix[1], viewMatrix[2]), pitch); //It should rotate by the right vector
	ry = ry.Rotation(1, yaw);
	Matrix ro = rx.mul(ry);
	lookat = ro.mulVec(lookat);
}

void Camera::gameloop(Window& window, float dt, Shaders& shader) {
	lookatControl(window);
	fromControl(window, dt);
	Update(shader);
}