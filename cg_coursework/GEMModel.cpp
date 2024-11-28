#include "GEMModel.h"
GEMModel::GEMModel(const std::string& filename, ID3D11Device* device) {
    Load(filename);
    CreateBuffers(device);
}

void GEMModel::Load(const std::string& filename) {
    std::cout << "1" << std::endl;
    GEMLoader::GEMModelLoader loader;
    animated = loader.isAnimatedModel(filename);
    if (animated) {
        loader.load(filename, meshes, animation);
    }
    else {
        loader.load(filename, meshes);
    }
}

void GEMModel::CreateBuffers(ID3D11Device* device) {
    for (const auto& mesh : meshes) {
        // Vertex buffer
        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        vbDesc.ByteWidth = mesh.isAnimated()
            ? static_cast<UINT>(mesh.verticesAnimated.size() * sizeof(GEMLoader::GEMAnimatedVertex))
            : static_cast<UINT>(mesh.verticesStatic.size() * sizeof(GEMLoader::GEMStaticVertex));
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vbData = {};
        vbData.pSysMem = mesh.isAnimated()
            ? static_cast<const void*>(mesh.verticesAnimated.data())
            : static_cast<const void*>(mesh.verticesStatic.data());

        ID3D11Buffer* vb = nullptr;
        device->CreateBuffer(&vbDesc, &vbData, &vb);
        vertexBuffers.push_back(vb);

        // Index buffer
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.ByteWidth = static_cast<UINT>(mesh.indices.size() * sizeof(unsigned int));
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = mesh.indices.data();

        ID3D11Buffer* ib = nullptr;
        device->CreateBuffer(&ibDesc, &ibData, &ib);
        indexBuffers.push_back(ib);
    }
}

void GEMModel::ReleaseBuffers() {
    for (auto vb : vertexBuffers) vb->Release();
    for (auto ib : indexBuffers) ib->Release();
    vertexBuffers.clear();
    indexBuffers.clear();
}

const std::vector<ID3D11Buffer*>& GEMModel::GetVertexBuffers() const {
    return vertexBuffers;
}

const std::vector<ID3D11Buffer*>& GEMModel::GetIndexBuffers() const {
    return indexBuffers;
}

const std::vector<GEMLoader::GEMMesh>& GEMModel::GetMeshes() const {
    return meshes;
}

bool GEMModel::IsAnimated() const {
    return animated;
}