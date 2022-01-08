#pragma once

#include "EngineMath.h"

struct Vector4
{
	float x, y, z, w;

	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(const Vector4& v);
	Vector4(const XMVECTOR& v);

	Vector4& operator = (const Vector4& v);
	Vector4& operator = (const XMVECTOR& v);
	Vector4& operator = (float f);

	bool operator == (const Vector4& v)		const;
	bool operator == (const XMVECTOR& v)	const;
	bool operator != (const Vector4& v)		const;
	bool operator != (const XMVECTOR& v)	const;

	float& operator [] (int Index);

	Vector4 operator + (const Vector4& v)	const;
	Vector4 operator + (const XMVECTOR& v)	const;
	Vector4 operator + (const float f)		const;

	Vector4 operator += (const Vector4& v);
	Vector4 operator += (const XMVECTOR& v);
	Vector4 operator += (const float f);

	Vector4 operator - (const Vector4& v)	const;
	Vector4 operator - (const XMVECTOR& v)	const;
	Vector4 operator - (const float f)		const;

	Vector4 operator -= (const Vector4& v);
	Vector4 operator -= (const XMVECTOR& v);
	Vector4 operator -= (const float f);

	Vector4 operator * (const Vector4& v)	const;
	Vector4 operator * (const XMVECTOR& v)	const;
	Vector4 operator * (const float f)		const;

	Vector4 operator *= (const Vector4& v);
	Vector4 operator *= (const XMVECTOR& v);
	Vector4 operator *= (const float f);

	Vector4 operator / (const Vector4& v)	const;
	Vector4 operator / (const XMVECTOR& v)	const;
	Vector4 operator / (const float f)		const;

	Vector4 operator /= (const Vector4& v);
	Vector4 operator /= (const XMVECTOR& v);
	Vector4 operator /= (const float f);

	Vector4 operator ++ ();
	Vector4 operator ++ (int);

	Vector4 operator -- ();
	Vector4 operator -- (int);

	XMVECTOR Convert()	const;
	void Convert(const XMVECTOR& v);

	static Vector4 Black;
	static Vector4 White;
	static Vector4 Red;
	static Vector4 Green;
	static Vector4 Blue;

};

