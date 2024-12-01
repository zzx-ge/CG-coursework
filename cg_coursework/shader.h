#pragma once
#include "adapter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <D3Dcompiler.h>
#include <d3d11shader.h>
#include "GEMModel.h"
#include "adapter.h"
#include "ShaderReflection.h"

#pragma comment(lib, "dxguid.lib")

struct TransformBuffer {
	Matrix VP;
	Matrix World;
};

class Shaders
{
public:
	std::vector<ConstantBuffer> psConstantBuffers;
	std::vector<ConstantBuffer> vsConstantBuffers;
	DXcore* dx;

	void updateConstant(const std::string& constantBufferName, const std::string& variableName, void* data, std::vector<ConstantBuffer>& buffers) {
		for (auto& buffer : buffers)
		{
			if (buffer.name == constantBufferName)
			{
				buffer.update(variableName, data);
				return;
			}
		}
	}

	void uploadConstant(const std::string& constBufferName, const std::string& variableName, std::vector<ConstantBuffer>& buffers) {
		for (auto& buffer : buffers)
		{
			if (buffer.name == constBufferName)
			{
				buffer.upload(dx);
				return;
			}
		}
	}

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* constantBuffer;
	std::map<std::string, int> textureBindPointsVS;
	std::map<std::string, int> textureBindPointsPS;

	Shaders(DXcore* _dx) : dx(_dx) {}

	//add a constant buffer to the system
	void addConstantBuffer(const ConstantBuffer& buffer, ShaderStage stage) {
		if (stage == ShaderStage::VertexShader) vsConstantBuffers.push_back(buffer);
		else if (stage == ShaderStage::PixelShader) psConstantBuffers.push_back(buffer);
	}

	//Update a specific constant buffer variable for the vertex shader
	void updateConstantVS(const std::string& constantBufferName, const std::string& variableName, void* data) {
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}

	void uploadConstantVS(const std::string& constantBufferName, const std::string& variableName) {
		uploadConstant(constantBufferName, variableName, vsConstantBuffers);
	}

	//Update a specific constant buffer variable for the pixel shader
	void updateConstantPS(const std::string& constantBufferName, const std::string& variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}

	void uploadConstantPS(const std::string& constantBufferName, const std::string& variableName) {
		uploadConstant(constantBufferName, variableName, psConstantBuffers);
	}

	std::string LoadShaders(std::string filePath) {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open HLSL file");
		}

		//Read HLSL file
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string shaderCode = buffer.str();
		return shaderCode;

	}

	void loadPS(std::string psfile) {
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status_pixel;
		HRESULT hr = D3DCompile(psfile.c_str(), strlen(psfile.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status_pixel); //PS is the entry point of the shader;
		if (FAILED(hr)) {
			(char*)status_pixel->GetBufferPointer();
			exit(0);
		}
		dx->device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
		ConstantBufferReflection reflection;
		reflection.build(dx, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
		compiledPixelShader->Release();
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create Pixel Shader");
		}
		
	}

	void loadVS(std::string vsfile) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status_vertex;
		HRESULT hr = D3DCompile(vsfile.c_str(), strlen(vsfile.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status_vertex); //VS is the entry point of the shader
		if (FAILED(hr)) {
			MessageBoxA(NULL, (char*)status_vertex->GetBufferPointer(), "Error", 0);
			exit(0);
		}
		dx->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		dx->device->CreateInputLayout(layoutDesc, 4, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);
		ConstantBufferReflection reflection;
		reflection.build(dx, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
		compiledVertexShader->Release();

	}

	void apply() {
		dx->devicecontext->IASetInputLayout(layout);
		dx->devicecontext->VSSetShader(vertexShader, NULL, 0);
		dx->devicecontext->PSSetShader(pixelShader, NULL, 0);
	}

	void init(std::string PSfilename, std::string VSfilename) {
		std::string code_ps = LoadShaders(PSfilename);
		std::string code_vs = LoadShaders(VSfilename);
		loadVS(code_vs);
		loadPS(code_ps);
	}

	void DrawModel(const GEMModel& model, DXcore& dx) {
		const auto& meshes = model.GetMeshes();
		const auto& vertexBuffers = model.GetVertexBuffers();
		const auto& indexBuffers = model.GetIndexBuffers();
		
		for (size_t i = 0; i < meshes.size(); ++i) {
			UINT stride = model.IsAnimated() ? sizeof(GEMLoader::GEMAnimatedVertex) : sizeof(GEMLoader::GEMStaticVertex);
			UINT offset = 0;

			dx.devicecontext->IASetVertexBuffers(0, 1, &vertexBuffers[i], &stride, &offset);
			dx.devicecontext->IASetIndexBuffer(indexBuffers[i], DXGI_FORMAT_R32_UINT, 0);
			dx.devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			dx.devicecontext->VSSetShader(vertexShader, nullptr, 0);
			dx.devicecontext->PSSetShader(pixelShader, nullptr, 0);
			dx.devicecontext->DrawIndexed(static_cast<UINT>(meshes[i].indices.size()), 0, 0);
		}
	}
};