#pragma once

#include "../../Ref.h"

class CMesh:
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	std::vector<MeshContainer*>	m_vecContainer;

	Vector3 m_Min;
	Vector3 m_Max;
	
public:
	virtual bool Init();
	virtual void Render();

protected:
	bool CreateBuffer(Buffer_Type Type, void* Data, int Size, int Count,
					  D3D11_USAGE Usage, ID3D11Buffer** Buffer);	
};

