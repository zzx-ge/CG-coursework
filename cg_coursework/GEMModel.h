#pragma once
#include "adapter.h"
#include "Gemloader.h"
#include <iostream>

class GEMModel {
public:
    GEMModel(const std::string& filename, ID3D11Device* device);

    void Load(const std::string& filename);
    void CreateBuffers(ID3D11Device* device);
    void ReleaseBuffers();

    const std::vector<ID3D11Buffer*>& GetVertexBuffers() const;
    const std::vector<ID3D11Buffer*>& GetIndexBuffers() const;
    const std::vector<GEMLoader::GEMMesh>& GetMeshes() const;

    bool IsAnimated() const;

private:
    std::vector<GEMLoader::GEMMesh> meshes;
    GEMLoader::GEMAnimation animation;
    std::vector<ID3D11Buffer*> vertexBuffers;
    std::vector<ID3D11Buffer*> indexBuffers;
    bool animated = false;
};