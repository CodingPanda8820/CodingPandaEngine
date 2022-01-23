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

	Desc.ByteWidth = Size * Count; // 정점 버퍼의 크기(바이트 단위)
	Desc.Usage = Usage; // 버퍼가 쓰이는 방식, 용도

	// BindFlags : Resource가 파이프라인에 묶이는 용도
	if (Type == Buffer_Type::Vertex)
		Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	else
		Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (Usage == D3D11_USAGE_DYNAMIC) // 응용 프로그램(CPU)가 Resource의 내용을 자주 변경
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (Usage == D3D11_USAGE_STAGING) // 응용 프로그램(CPU)가 Resource의 복사본을 읽어야 한다면
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

	//Desc.MiscFlags;
	//Desc.StructureByteStride : 구조적 버퍼(Structured Buffer)에 저장된 원소 하나의 크기(바이트 단위)

	D3D11_SUBRESOURCE_DATA BufferData = {};

	// 정점 버퍼를 초기화할 자료를 담은 시스템 메모리 배열을 가리키는 포인터
	// 정점 n개를 담는 버퍼를 초기화하는 경우, 초기화가 제대로 일어나려면
	// 그 배열의 원소가 적어도 n개이어야 한다.
	BufferData.pSysMem = Data;
	
	// BufferData.SysMemPitch; 정점 버퍼에는 쓰이지 않음
	// BufferData.SysMemSlicePitch; 정점 버퍼에는 쓰이지 않음

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
		unsigned int Stride = m_vecContainer[i]->VB.Size; // 해당 정점 버퍼의 한 원소의 바이트 단위 크기
		unsigned int Offset = 0; // 오프셋들의 배열의 첫 원소를 가리키는 포인터

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
