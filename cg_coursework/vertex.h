#pragma once
#include "adapter.h"
class Vec3
{
public:
	float x;
	float y;
	float z;
	Vec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vec3 operator+(Vec3 p) { return Vec3(x + p.x, y + p.y, z + p.z); }
	Vec3 operator-(Vec3 p) { return Vec3(x - p.x, y - p.y, z - p.z); }
	Vec3 operator*(float v) { return Vec3(x * v, y * v, z * v); }
	void write(FILE* file)
	{
		fwrite(&x, sizeof(float), 1, file);
		fwrite(&y, sizeof(float), 1, file);
		fwrite(&z, sizeof(float), 1, file);
	}
};

struct Colour
{
	float r;
	float g;
	float b;

	Colour()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
	}


	Colour(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

};

struct Vertex
{
	Vec3 position;
	Colour colour;
};

class Triangle
{
public:
	ID3D11Buffer* vertexBuffer; //Does that means each triangle is stored in a buffer?

	void init(DXcore& dx) {
		Vertex vertices[3];
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);


		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA uploadData;
		bd.ByteWidth = sizeof(Vertex) * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		uploadData.pSysMem = vertices;
		uploadData.SysMemPitch = 0;
		uploadData.SysMemSlicePitch = 0;
		dx.device->CreateBuffer(&bd, &uploadData, &vertexBuffer);
	}

	void draw(DXcore& dx) {
		UINT offsets;
		offsets = 0;
		UINT strides = sizeof(Vertex);
		dx.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dx.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dx.devicecontext->Draw(3, 0);
	}
};