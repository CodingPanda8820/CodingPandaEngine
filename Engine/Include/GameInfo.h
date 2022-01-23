#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr;}

#define ROOT_PATH		"Root"
#define SHADER_PATH		"Shader"

#define DECLARE_SINGLE(Type)\
private:\
	static Type* m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type();\
	~Type();\

#define DEFINITION_SINGLE(Type) Type* Type::m_Inst = nullptr;

struct Resolution
{
	unsigned int Width;
	unsigned int Height;
};

struct VertexColor
{
	Vector3 Pos;
	Vector4 Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _Pos, const Vector4& _Color) :
		Pos(_Pos),
		Color(_Color)
	{
	}
};

// Buffer : 임시 메모리 공간.
struct VertexBuffer
{
	ID3D11Buffer* Buffer;
	int Size;
	int Count;

	VertexBuffer():
		Buffer(nullptr),
		Size(0),
		Count(0)
	{
	}

	~VertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* Buffer;
	int Size;
	int Count;
	DXGI_FORMAT Format;

	IndexBuffer():
		Buffer(nullptr),
		Size(0),
		Count(0),
		Format(DXGI_FORMAT_UNKNOWN)
	{
	}

	~IndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

struct MeshContainer
{
	VertexBuffer				VB;
	std::vector<IndexBuffer>	vecIB;
	D3D11_PRIMITIVE_TOPOLOGY	Primitive;
};

struct TransformCBuffer
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	Matrix	matWV;
	Matrix	matWVP;
	Matrix	matVP;
	Vector3 Pivot;
	float	Empty0;
	Vector3	MeshSize;
	float	Empty1;
};