#pragma once

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class Buffer_Type
{
	Vertex,
	Index
};

enum class Shader_Type
{
	Graphic,
	Compute
};

enum class ConstantBuffer_Shader_Type
{
	Vertex = 0x1,
	Hull = 0x2,
	Domain = 0x4,
	Geometry = 0x8,
	Pixel = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Hull | Domain | Geometry | Pixel,
	All = Vertex | Hull | Domain | Geometry | Pixel | Compute
};

enum class Component_Type
{
	SceneComponent,
	ObjectComponent
};