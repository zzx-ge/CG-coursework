#pragma once
#include <d3d11.h>
#include "cgmath.h"

class Camera
{
private:
	Matrix viewMatrix;
	Matrix projectionMatrix;
	Vec3 position;
	float pitch; //Rotation around X
	float yaw;   //Rotation around Y
	float radius;//Distance from the center;
public:
	Camera(float fovY, float aspectRario, float nearZ, float farZ, float radius);
	void init() {
		SetPosition(Vec3(1,1,1)); //The viewMatrix and ProjectionMatrix has been established
		Update();
	}
	//void Update(float deltaTime, float _x, float _y, float _z);
	void Update();
	void SetPosition(const Vec3& _position) {
		position = _position;
	}
	const Matrix& GetViewMatrix() const { return viewMatrix; }
	const Matrix& GetProjectionMatrix() const { return projectionMatrix; }
	const Vec3& Getposition() const { return position; }
};