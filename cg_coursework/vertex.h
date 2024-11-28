#pragma once
#include "adapter.h"
#include "cgmath.h"

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};


class Mesh
{
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize;
	UINT strides;

	void init(void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, DXcore& dx) {
		D3D11_BUFFER_DESC bd;
		memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;
		dx.device->CreateBuffer(&bd, &data, &indexBuffer);
		bd.ByteWidth = vertexSizeInBytes * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data.pSysMem = vertices;
		dx.device->CreateBuffer(&bd, &data, &vertexBuffer);
		indicesSize = numIndices;
		strides = vertexSizeInBytes;
	}

	void init(std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices, DXcore& dx)
	{
		init(&vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size(), dx);
	}

	void Draw(DXcore& dx) {
		UINT offsets = 0;
		dx.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dx.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		dx.devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		dx.devicecontext->DrawIndexed(indicesSize, 0, 0);
	}
};

class Plane
{
public:
	std::vector<STATIC_VERTEX> vertices;
	Mesh mesh;
	Matrix world;
	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		//Frame frame; //shading frame
		//frame.fromVector(n);
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(DXcore& dx) {
		world = world.Rotation(1, 2); //Rotate by y
		vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(vertices, indices, dx);
	}

	void Draw(DXcore& dx) {
		mesh.Draw(dx);
	}
};

//class Triangle
//{
//public:
//	ID3D11Buffer* vertexBuffer; //Does that means each triangle is stored in a buffer?
//
//	void init(DXcore& dx) {
//		Vertex vertices[3];
//		vertices[0].position = Vec3(0, 1.0f, 0);
//		vertices[0].colour = Colour(0, 1.0f, 0);
//		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
//		vertices[1].colour = Colour(1.0f, 0, 0);
//		vertices[2].position = Vec3(1.0f, -1.0f, 0);
//		vertices[2].colour = Colour(0, 0, 1.0f);
//
//
//		D3D11_BUFFER_DESC bd;
//		bd.Usage = D3D11_USAGE_DEFAULT;
//		bd.CPUAccessFlags = 0;
//		bd.MiscFlags = 0;
//		D3D11_SUBRESOURCE_DATA uploadData;
//		bd.ByteWidth = sizeof(Vertex) * 3;
//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//		uploadData.pSysMem = vertices;
//		uploadData.SysMemPitch = 0;
//		uploadData.SysMemSlicePitch = 0;
//		dx.device->CreateBuffer(&bd, &uploadData, &vertexBuffer);
//	}
//
//	void draw(DXcore& dx) {
//		UINT offsets;
//		offsets = 0;
//		UINT strides = sizeof(Vertex);
//		dx.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		dx.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
//		dx.devicecontext->Draw(3, 0);
//	}
//};