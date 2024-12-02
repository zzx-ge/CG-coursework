#pragma once
#include "adapter.h"
#include "Gemloader.h"
#include <iostream>
#include "vertex.h"
#include "shader.h"

class GEMModel
{
public:
    Vec3 position;
    std::vector<Mesh> meshes;
    Animation animation;
    std::vector<GEMLoader::GEMMesh> gemmeshes;
    GEMLoader::GEMAnimation gemanimation;
    GEMLoader::GEMModelLoader loader;
    bool animated = false;


    void init(string filename, DXcore& dx, Vec3 vec) {
        position = vec;
        loader.load(filename, gemmeshes, gemanimation);
        memcpy(&(animation.skeleton.globalInverse), &(gemanimation.globalInverse), sizeof(Matrix));
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh mesh;
            if (gemmeshes[i].isAnimated()) {
                std::vector<ANIMATED_VERTEX> vertices;
                for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
                    ANIMATED_VERTEX v;
                    memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(GEMLoader::GEMAnimatedVertex));
                    vertices.push_back(v);
                }
                mesh.init(vertices, gemmeshes[i].indices, dx);
                meshes.push_back(mesh);
            }
            else {
                std::vector<STATIC_VERTEX> vertices;
                for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                    STATIC_VERTEX v;
                    memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(GEMLoader::GEMStaticVertex));
                    vertices.push_back(v);
                }
                mesh.init(vertices, gemmeshes[i].indices, dx);
                meshes.push_back(mesh);
            }
            
        }
        for (int i = 0; i < gemanimation.bones.size(); i++)
        {
            Bone bone;
            bone.name = gemanimation.bones[i].name;
            memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
            bone.parentIndex = gemanimation.bones[i].parentIndex;
            animation.skeleton.bones.push_back(bone);
        }
        for (int i = 0; i < gemanimation.animations.size(); i++)
        {
            std::string name = gemanimation.animations[i].name;
            AnimationSequence aseq;
            aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
            for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
            {
                AnimationFrame frame;
                for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
                {
                    Vec3 p;
                    Quaternion q;
                    Vec3 s;
                    memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
                    frame.positions.push_back(p);
                    memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
                    frame.rotations.push_back(q);
                    memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
                    frame.scales.push_back(s);
                }
                aseq.frames.push_back(frame);
            }
            animation.animations.insert({ name, aseq });
        }
    }

    void updateWorld(Shaders& shader, Matrix& w) {
        w = Matrix::Translation(position.x, position.y, position.z);
        shader.updateConstantVS("World", "W", &w);
        shader.uploadConstantVS("World", "W");
    }

    void updateBones(Shaders& shader, AnimationInstance& instance, float dt) {
        instance.update("Run", dt);
        shader.updateConstantVS("animatedMeshBuffer", "bones", instance.matrices);
        shader.uploadConstantVS("animatedMeshBuffer", "bones");
    }

    void Draw(DXcore& dx, Shaders& shader, AnimationInstance& instance, float dt, Matrix& world) {
        updateBones(shader, instance, dt);
        updateWorld(shader, world);
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(dx);
        }

    }
};