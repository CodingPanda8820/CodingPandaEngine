#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	m_MeshManager(nullptr),
	m_ShaderManager(nullptr),
	m_MaterialManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MeshManager);	
	SAFE_DELETE(m_MaterialManager);
}

bool CResourceManager::Init()
{
	m_MeshManager = new CMeshManager;

	if (!m_MeshManager->Init())
		return false;

	m_ShaderManager = new CShaderManager;

	if (!m_ShaderManager->Init())
		return false;

	m_MaterialManager = new CMaterialManager;

	if (!m_MaterialManager->Init())
		return false;

	return true;
}

CMesh * CResourceManager::FindMesh(const std::string & Name)
{
	return m_MeshManager->FindMesh(Name);
}

void CResourceManager::ReleaseMesh(const std::string & Name)
{
	m_MeshManager->ReleaseMesh(Name);
}

CShader * CResourceManager::FindShader(const std::string & Name)
{
	return m_ShaderManager->FindShader(Name);
}

void CResourceManager::ReleaseShader(const std::string & Name)
{
	m_ShaderManager->ReleaseShader(Name);
}

bool CResourceManager::CreateConstantBuffer(const std::string & Name, int Size, int Register, int ConstantBufferShaderType)
{
	return m_ShaderManager->CreateConstantBuffer(Name, Size, Register, ConstantBufferShaderType);
}

CConstantBuffer * CResourceManager::FindConstantBuffer(const std::string & Name)
{
	return m_ShaderManager->FindConstantBuffer(Name);
}

CMaterial * CResourceManager::FindMaterial(const std::string & Name)
{
	return m_MaterialManager->FindMaterial(Name);
}

void CResourceManager::ReleaseMaterial(const std::string & Name)
{
	m_MaterialManager->ReleaseMaterial(Name);
}
