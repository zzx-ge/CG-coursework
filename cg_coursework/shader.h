#pragma once
#include "adapter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <D3Dcompiler.h>
#include <d3d11shader.h>

#pragma comment(lib, "dxguid.lib")
class Shaders
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

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
	void loadPS(DXcore& dx, std::string psfile) {
		ID3DBlob* compiledPixelShader;
		ID3DBlob* status_pixel;
		HRESULT hr = D3DCompile(psfile.c_str(), strlen(psfile.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &compiledPixelShader, &status_pixel); //PS is the entry point of the shader;
		if (FAILED(hr)) {
			(char*)status_pixel->GetBufferPointer();
			exit(0);
		}
		dx.device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), NULL, &pixelShader);
		compiledPixelShader->Release();
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create Pixel Shader");
		}

		
	}

	void loadVS(DXcore& dx, std::string vsfile) {
		ID3DBlob* compiledVertexShader;
		ID3DBlob* status_vertex;
		HRESULT hr = D3DCompile(vsfile.c_str(), strlen(vsfile.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &compiledVertexShader, &status_vertex); //VS is the entry point of the shader
		if (FAILED(hr)) {
			MessageBoxA(NULL, (char*)status_vertex->GetBufferPointer(), "Error", 0);
			exit(0);
		}
		dx.device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), NULL, &vertexShader);
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		dx.device->CreateInputLayout(layoutDesc, 2, compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(), &layout);

		compiledVertexShader->Release();

	}

	void apply(DXcore& dx) {
		dx.devicecontext->IASetInputLayout(layout);
		dx.devicecontext->VSSetShader(vertexShader, NULL, 0);
		dx.devicecontext->PSSetShader(pixelShader, NULL, 0);

	}

	void init(std::string PSfilename, std::string VSfilename, DXcore& dx) {
		std::string code_ps = LoadShaders(PSfilename);
		std::string code_vs = LoadShaders(VSfilename);
		loadVS(dx, code_vs);
		loadPS(dx, code_ps);
	}
};