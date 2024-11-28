#include "Camera.h"
#include "shader.h"

Camera::Camera(float fovY, float aspectRatio, float nearZ, float farZ, float radius) :position(Vec4(0.f, 0.f, -1 * radius, 1)), pitch(0.f), yaw(0.f), radius(radius) {
	projectionMatrix = Matrix(1 / (tan(fovY*M_PI/360) * aspectRatio), 0, 0, 0, 0, 1 / tan(fovY * M_PI / 360), 0, 0, 0, 0, -1 * farZ / (farZ - nearZ), -1 * farZ * nearZ / (farZ - nearZ), 0, 0, -1, 0);
}

void Camera::Update() {

	//Calculate the new position based on spherical coordinates
	float x = position.x;
	float y = position.y;
	float z = position.z;
	
	//Create the view matrix
	Vec3 lookat = position - Vec3(0,0,0); // lookat
	lookat = lookat.normalize();

	Vec3* frame = lookat.Schmit_orthono();
	lookat = frame[0];
	Vec3 up = frame[1];
	Vec3 right = frame[2];

	viewMatrix[0] = right.x;
	viewMatrix[1] = right.y;
	viewMatrix[2] = right.z;
	viewMatrix[3] = -1 * right.dot(position);
	viewMatrix[4] = up.x;
	viewMatrix[5] = up.y;
	viewMatrix[6] = up.z;
	viewMatrix[7] = -1 * up.dot(position);
	viewMatrix[8] = lookat.x;
	viewMatrix[9] = lookat.y;
	viewMatrix[10] = lookat.z;
	viewMatrix[11] = -1 * lookat.dot(position);
	viewMatrix[15] = 1;

}