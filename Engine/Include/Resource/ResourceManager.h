#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"

class CResourceManager
{
	DECLARE_SINGLE(CResourceManager)

private:
	CMeshManager* m_MeshManager;
	CShaderManager* m_ShaderManager;
	CMaterialManager* m_MaterialManager;

public:
	bool Init();

public: // ====== Mesh =============================
	class CMesh* FindMesh(const std::string& Name);

	void ReleaseMesh(const std::string& Name);

public: // ====== Shader ===========================
	class CShader* FindShader(const std::string& Name);

	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		return m_ShaderManager->CreateShader<T>(Name);
	}

	void ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
							  int ConstantBufferShaderType = (int)ConstantBuffer_Shader_Type::All);

	class CConstantBuffer* FindConstantBuffer(const std::string& Name);

public: // ====== Material =========================
	class CMaterial* FindMaterial(const std::string& Name);

	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		return m_MaterialManager->CreateMaterial<T>(Name);
	}

	void ReleaseMaterial(const std::string& Name);
};

