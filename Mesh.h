#pragma once

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/ext.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec2 m_tex;
    glm::vec3 m_normal;

    Vertex() {}

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = normal;
    }

    Vertex(const glm::vec3& pos, const glm::vec2& tex)
    {
        m_pos = pos;
        m_tex = tex;
        m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
    }

};

class Mesh
{
private:
	bool initFromScene(const aiScene* pScene, const std::string& Filename);
	void initMesh(unsigned int Index, const aiMesh* paiMesh);
	bool initMaterials(const aiScene* pScene, const std::string& Filename);
	void clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

       void Init(const std::vector<Vertex>& Vertices,
          const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

  
    std::vector<MeshEntry> m_Entries;
    std::vector<unsigned int> m_Textures;


public:
	Mesh();

	~Mesh();

	bool loadMesh(const std::string& Filename);

	void Render();



};
#endif  /* MESH_H */
