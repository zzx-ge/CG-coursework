#pragma once
#include <iostream>
#include <cmath>

#define SQ(x) (x) * (x)
#define _USE_MATH_DEFINES
#define M_PI 3.1415926

using namespace std;

template<typename T>
static T lerp(const T a, const T b, float t) {
	return a * (1.f - t) + (b * t);
}

struct Vec2 {
	float x, y;
	Vec2() {}
	Vec2(const float _x, const float _y) :x(_x), y(_y){}
};

struct Vec3
{
	float x, y, z;
	Vec3() {}
	Vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z){}

	float lengthsquare() {
		return x * x + y * y + z * z;
	}

	float length() {
		return sqrtf(lengthsquare());
	}

	Vec3 normalize() {
		float len = 1.f / length();
		return Vec3(x * len, y * len, z * len);
	}

	Vec3 Cross(const Vec3& v1)
	{
		return Vec3(y * v1.z - z * v1.y,
			z * v1.x - x * v1.z,
			x * v1.y - y * v1.x);
	}

	float dot(Vec3 v) {
		return x * v.x + y * v.y + z * v.z;
	}

	void set_value(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	bool operator==(const Vec3 v) {
		if (x == v.x && y == v.y && z == v.z)return true;
		else return false;
	}

	Vec3 operator-(const Vec3 v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator+(const Vec3 v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator*(const float a) const { return Vec3(x * a, y * a, z * a); }
	void operator-=(const Vec3 v) {
		x -= v.x; 
		y -= v.y;
		z -= v.z;
	}
	void operator+=(const Vec3 v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator*=(const float a) {
		x *= a;
		y *= a;
		z *= a;
	}


	Vec3* Schmit_orthono() {
		Vec3 u(0, 1, 0);
		Vec3 gaze = normalize();

		Vec3 right = u.Cross(gaze).normalize();
		Vec3 up = gaze.Cross(right);
		Vec3 frame[3] = { gaze, up, right };
		return frame;
	}
};

struct Vec4 : public Vec3
{
	float w;
	Vec4() {}
	Vec4(Vec3 v, float _w) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = _w;
	}
	Vec4(const float _x, const float _y, const float _z, const float _w) : Vec3(_x, _y, _z) {
		w = _w;
	}
	
};

class Matrix
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};

	Matrix() {
		for (int i = 0; i < 16; i++) {
			m[i] = 0;
		}
	}
	Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
		m[0] = m00;
		m[1] = m01;
		m[2] = m02;
		m[3] = m03;
		m[4] = m10;
		m[5] = m11;
		m[6] = m12;
		m[7] = m13;
		m[8] = m20;
		m[9] = m21;
		m[10] = m22;
		m[11] = m23;
		m[12] = m30;
		m[13] = m31;
		m[14] = m32;
		m[15] = m33;
	}
	Matrix(float* otherMatrix) {
		memcpy(&m, otherMatrix, 16 * sizeof(float));
	}


	float& operator[](int index) {
		return m[index];
	}

	void vectorstobase(const Vec4 v0, const Vec4 v1, const Vec4 v2) {
		m[0] = v0.x;
		m[1] = v1.x;
		m[2] = v2.x;
		m[4] = v0.y;
		m[5] = v1.y;
		m[6] = v2.y;
		m[8] = v0.z;
		m[9] = v1.z;
		m[10] = v2.z;
	}

	void translation_set(const float _x, const float _y, const float _z) {
		m[3] = _x;
		m[7] = _y;
		m[11] = _z;
	}

	void identity() {
		memset(this, 0, 16);
		m[0] = 1;
		m[5] = 1;
		m[10] = 1;
		m[15] = 1;
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;

		ret[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
		ret[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
		ret[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
		ret[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
		ret[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
		ret[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
		ret[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
		ret[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
		ret[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
		ret[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
		ret[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
		ret[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
		ret[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
		ret[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
		ret[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
		ret[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
		return ret;
	}


	Matrix operator*(const Matrix& matrix) {
		return mul(matrix);
	}


	Matrix Transpose() {
		Matrix _transpose;
		for (int i = 0; i < 4; i++) {
			for (int n = 0; n < 4; n++) {
				_transpose.a[n][i] = a[i][n];
			}
		}
		return _transpose;
	}

	float determinant() {
		return
			a[0][3] * a[1][2] * a[2][1] * a[3][0] - a[0][2] * a[1][3] * a[2][1] * a[3][0] -
			a[0][3] * a[1][1] * a[2][2] * a[3][0] + a[0][1] * a[1][3] * a[2][2] * a[3][0] +
			a[0][2] * a[1][1] * a[2][3] * a[3][0] - a[0][1] * a[1][2] * a[2][3] * a[3][0] -
			a[0][3] * a[1][2] * a[2][0] * a[3][1] + a[0][2] * a[1][3] * a[2][0] * a[3][1] +
			a[0][3] * a[1][0] * a[2][2] * a[3][1] - a[0][0] * a[1][3] * a[2][2] * a[3][1] -
			a[0][2] * a[1][0] * a[2][3] * a[3][1] + a[0][0] * a[1][2] * a[2][3] * a[3][1] +
			a[0][3] * a[1][1] * a[2][0] * a[3][2] - a[0][1] * a[1][3] * a[2][0] * a[3][2] -
			a[0][3] * a[1][0] * a[2][1] * a[3][2] + a[0][0] * a[1][3] * a[2][1] * a[3][2] +
			a[0][1] * a[1][0] * a[2][3] * a[3][2] - a[0][0] * a[1][1] * a[2][3] * a[3][2] -
			a[0][2] * a[1][1] * a[2][0] * a[3][3] + a[0][1] * a[1][2] * a[2][0] * a[3][3] +
			a[0][2] * a[1][0] * a[2][1] * a[3][3] - a[0][0] * a[1][2] * a[2][1] * a[3][3] -
			a[0][1] * a[1][0] * a[2][2] * a[3][3] + a[0][0] * a[1][1] * a[2][2] * a[3][3];
	}


	static Matrix Translation(float _x, float _y, float _z) {
		Matrix tr;
		tr.identity();
		tr[3] = _x;
		tr[7] = _y;
		tr[11] = _z;
		return tr;
	}

	static Matrix Rotation(int xyz, float _theta) {
		Matrix ro;
		float theta = _theta;
		double cos = std::cos(theta);
		double sin = std::sin(theta);
		ro.identity();
		if (xyz == 0) {
			ro.m[5] = cos;
			ro.m[6] = -1 * sin;
			ro.m[9] = sin;
			ro.m[10] = cos;
		}
		else if (xyz == 1) {
			ro.m[0] = cos;
			ro.m[2] = sin;
			ro.m[8] = -1 * sin;
			ro.m[10] = cos;
		}
		else if (xyz == 2) {
			ro.m[0] = cos;
			ro.m[1] = -sin;
			ro.m[4] = sin;
			ro.m[5] = cos;
		}
		return ro;
	}

	static Matrix Scale(float xratio, float yratio, float zratio) {
		Matrix sc;
		sc.m[0] = xratio;
		sc.m[5] = yratio;
		sc.m[10] = zratio;
		sc.m[15] = 1;
	}

	const void ViewMatrix(Vec3 from, Vec3 lookat) {
		lookat.normalize();
		Vec3* frame = lookat.Schmit_orthono();
		lookat = frame[0];
		Vec3 up = frame[1];
		Vec3 right = frame[2];

		m[0] = right.x;
		m[1] = right.y;
		m[2] = right.z;
		m[3] = -1 * right.dot(from);
		m[4] = up.x;
		m[5] = up.y;
		m[6] = up.z;
		m[7] = -1 * up.dot(from);
		m[8] = lookat.x;
		m[9] = lookat.y;
		m[10] = lookat.z;
		m[11] = -1 * lookat.dot(from);
		m[15] = 1;

	}


	Matrix invert()
	{
		Matrix inv;
		inv.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv.m[8] = m[4] * m[9] * m[15] - m[8] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

};