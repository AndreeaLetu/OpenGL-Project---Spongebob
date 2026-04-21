#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "model_loader.h"
#include "texture.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

static std::string GetBaseDirectory(const std::string& path) {
    size_t pos1 = path.find_last_of('/');
    size_t pos2 = path.find_last_of('\\');
    size_t pos = std::string::npos;

    if (pos1 != std::string::npos && pos2 != std::string::npos)
        pos = (pos1 > pos2) ? pos1 : pos2;
    else if (pos1 != std::string::npos)
        pos = pos1;
    else
        pos = pos2;

    if (pos == std::string::npos)
        return "";

    return path.substr(0, pos + 1);
}

bool LoadOBJModel(const char* filename, OBJModel& outModel) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    outModel.subMeshes.clear();

    std::string inputFile = filename;
    std::string baseDir = GetBaseDirectory(inputFile);

    bool ret = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &err,
        filename,
        baseDir.c_str()
    );

    if (!warn.empty()) {
        std::cout << "OBJ warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "OBJ error: " << err << std::endl;
    }

    if (!ret) {
        std::cout << " OBJ model not loaded : " << filename << std::endl;
        return false;
    }

    std::vector<GLuint> materialTextures(materials.size(), 0);

    for (size_t i = 0; i < materials.size(); i++) {
        if (!materials[i].diffuse_texname.empty()) {
            std::string texPath = baseDir + materials[i].diffuse_texname;
            materialTextures[i] = LoadTexture(texPath.c_str());
        }
    }

    std::map<int, int> materialToSubmesh;

    for (const auto& shape : shapes) {
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            int materialID = -1;

            if (f < shape.mesh.material_ids.size()) {
                materialID = shape.mesh.material_ids[f];
            }

            if (materialToSubmesh.find(materialID) == materialToSubmesh.end()) {
                SubMesh sm;

                if (materialID >= 0 && materialID < (int)materialTextures.size()) {
                    sm.textureID = materialTextures[materialID];
                    sm.r = materials[materialID].diffuse[0];
                    sm.g = materials[materialID].diffuse[1];
                    sm.b = materials[materialID].diffuse[2];
                }

                outModel.subMeshes.push_back(sm);
                materialToSubmesh[materialID] = (int)outModel.subMeshes.size() - 1;
            }

            int subMeshIndex = materialToSubmesh[materialID];
            SubMesh& currentSubMesh = outModel.subMeshes[subMeshIndex];

            if (fv != 3) {
                index_offset += fv;
                continue;
            }

            for (int v = 0; v < 3; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                SimpleVertex vert{};

                vert.x = attrib.vertices[3 * idx.vertex_index + 0];
                vert.y = attrib.vertices[3 * idx.vertex_index + 1];
                vert.z = attrib.vertices[3 * idx.vertex_index + 2];

                if (idx.normal_index >= 0) {
                    vert.nx = attrib.normals[3 * idx.normal_index + 0];
                    vert.ny = attrib.normals[3 * idx.normal_index + 1];
                    vert.nz = attrib.normals[3 * idx.normal_index + 2];
                }
                else {
                    vert.nx = 0.0f;
                    vert.ny = 1.0f;
                    vert.nz = 0.0f;
                }

                if (idx.texcoord_index >= 0) {
                    vert.u = attrib.texcoords[2 * idx.texcoord_index + 0];
                    vert.v = attrib.texcoords[2 * idx.texcoord_index + 1];
                }
                else {
                    vert.u = 0.0f;
                    vert.v = 0.0f;
                }

                currentSubMesh.vertices.push_back(vert);
            }

            index_offset += fv;
        }
    }

    std::cout << "Model incarcat cu succes: " << filename << std::endl;
    std::cout << "Numar submesh-uri: " << outModel.subMeshes.size() << std::endl;

    return true;
}

void DrawOBJModel(const OBJModel& model) {
    if (model.subMeshes.empty()) return;

    glDisable(GL_LIGHTING);

    for (const auto& sm : model.subMeshes) {
        if (sm.textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, sm.textureID);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else {
            glDisable(GL_TEXTURE_2D);
            glColor3f(sm.r, sm.g, sm.b);
        }
        glBegin(GL_TRIANGLES);
        for (const auto& v : sm.vertices) {
            glTexCoord2f(v.u, v.v);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

void DrawOBJModelShadow(const OBJModel& model)
{
    if (model.subMeshes.empty()) return;
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    for (const auto& sm : model.subMeshes) {
        glBegin(GL_TRIANGLES);
        for (const auto& v : sm.vertices)
            glVertex3f(v.x, v.y, v.z);
        glEnd();
    }
}
