#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
}

bool CMesh::Init()
{
	return true;
}

bool CMesh::CreateBuffer(Buffer_Type Type, void * Data, int Size, int Count,
						 D3D11_USAGE Usage, ID3D11Buffer ** Buffer)
{
	D3D11_BUFFER_DESC Desc = {};

	Desc.ByteWidth = Size * Count; // ���� ������ ũ��(����Ʈ ����)
	Desc.Usage = Usage; // ���۰� ���̴� ���, �뵵

	// BindFlags : Resource�� ���������ο� ���̴� �뵵
	if (Type == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	else
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC) // ���� ���α׷�(CPU)�� Resource�� ������ ���� ����
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (Usage == D3D11_USAGE_STAGING) // ���� ���α׷�(CPU)�� Resource�� ���纻�� �о�� �Ѵٸ�
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	//Desc.MiscFlags;
	//Desc.StructureByteStride : ������ ����(Structured Buffer)�� ����� ���� �ϳ��� ũ��(����Ʈ ����)

	D3D11_SUBRESOURCE_DATA BufferData = {};

	// ���� ���۸� �ʱ�ȭ�� �ڷḦ ���� �ý��� �޸� �迭�� ����Ű�� ������
	// ���� n���� ��� ���۸� �ʱ�ȭ�ϴ� ���, �ʱ�ȭ�� ����� �Ͼ����
	// �� �迭�� ���Ұ� ��� n���̾�� �Ѵ�.
	BufferData.pSysMem = Data;
	
	// BufferData.SysMemPitch; ���� ���ۿ��� ������ ����
	// BufferData.SysMemSlicePitch; ���� ���ۿ��� ������ ����

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&Desc, &BufferData, Buffer)))
		return false;

	if (Type == Buffer_Type::Vertex)
	{
		char* VertexData = (char*)Data;

		for (int i = 0; i < Count; ++i)
		{
			Vector3* Pos = (Vector3*)VertexData;
			VertexData += Size;

			if (m_Min.x > Pos->x)
				m_Min.x = Pos->x;

			if (m_Min.y > Pos->y)
				m_Min.y = Pos->y;

			if (m_Min.z > Pos->z)
				m_Min.z = Pos->z;

			if (m_Max.x < Pos->x)
				m_Max.x = Pos->x;

			if (m_Max.y < Pos->y)
				m_Max.y = Pos->y;

			if (m_Max.z < Pos->z)
				m_Max.z = Pos->z;
		}
	}

	return true;
}

void CMesh::Render()
{
	size_t Size = m_vecContainer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned int Stride = m_vecContainer[i]->VB.Size; // �ش� ���� ������ �� ������ ����Ʈ ���� ũ��
		unsigned int Offset = 0; // �����µ��� �迭�� ù ���Ҹ� ����Ű�� ������

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecContainer[i]->VB.Buffer, &Stride, &Offset);

		size_t IdxCount = m_vecContainer[i]->vecIB.size();

		if (IdxCount > 0)
		{
			for (size_t j = 0; j < IdxCount; ++j)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecContainer[i]->vecIB[j].Buffer,
																   m_vecContainer[i]->vecIB[j].Format, 0);
				CDevice::GetInst()->GetContext()->DrawIndexed(m_vecContainer[i]->vecIB[j].Size, 0, 0);
			}
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(m_vecContainer[i]->VB.Count, 0);
		}
	}
}
