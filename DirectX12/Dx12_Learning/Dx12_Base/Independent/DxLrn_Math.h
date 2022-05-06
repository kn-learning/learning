#pragma once
#include <vector>

using real = float;

struct FLOAT2
{
	FLOAT2() {};
	FLOAT2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float x = 0;
	float y = 0;


	void operator=		(const float& oth) {
		x = oth;
		y = oth;
	}
	void operator+=		(const float& oth) {
		x += oth;
		y += oth;
	}
	void operator-=		(const float& oth) {
		x -= oth;
		y -= oth;
	}
	void operator*=		(const float& oth) {
		x *= oth;
		y *= oth;
	}
	void operator/=		(const float& oth) {
		x /= oth;
		y /= oth;
	}

	FLOAT2 operator+	(const float& oth) {

		FLOAT2 tmp;

		tmp.x = x + oth;
		tmp.y = y + oth;

		return tmp;
	}
	FLOAT2 operator-	(const float& oth) {

		FLOAT2 tmp;

		tmp.x = x - oth;
		tmp.y = y - oth;

		return tmp;
	}
	FLOAT2 operator*	(const float& oth) {

		FLOAT2 tmp;

		tmp.x = x * oth;
		tmp.y = y * oth;

		return tmp;
	}
	FLOAT2 operator/	(const float& oth) {

		FLOAT2 tmp;

		tmp.x = x / oth;
		tmp.y = y / oth;

		return tmp;
	}

	void operator=		(const FLOAT2& oth) {
		x = oth.x;
		y = oth.y;
	}
	void operator+=		(const FLOAT2& oth) {
		x += oth.x;
		y += oth.y;
	}
	void operator-=		(const FLOAT2& oth) {
		x -= oth.x;
		y -= oth.y;
	}
	void operator*=		(const FLOAT2& oth) {
		x *= oth.x;
		y *= oth.y;
	}
	void operator/=		(const FLOAT2& oth) {
		x /= oth.x;
		y /= oth.y;
	}

	FLOAT2 operator+	(const FLOAT2& oth) {

		FLOAT2 tmp;

		tmp.x = x + oth.x;
		tmp.y = y + oth.y;

		return tmp;
	}
	FLOAT2 operator-	(const FLOAT2& oth) {

		FLOAT2 tmp;

		tmp.x = x - oth.x;
		tmp.y = y - oth.y;

		return tmp;
	}
	FLOAT2 operator*	(const FLOAT2& oth) {

		FLOAT2 tmp;

		tmp.x = x * oth.x;
		tmp.y = y * oth.y;

		return tmp;
	}
	FLOAT2 operator/	(const FLOAT2& oth) {

		FLOAT2 tmp;

		tmp.x = x / oth.x;
		tmp.y = y / oth.y;

		return tmp;
	}
};

struct FLOAT3
{
	FLOAT3() {};
	FLOAT3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float x = 0;
	float y = 0;
	float z = 0;


	void operator=		(const float& oth) {
		x = oth;
		y = oth;
		z = oth;
	}
	void operator+=		(const float& oth) {
		x += oth;
		y += oth;
		z += oth;
	}
	void operator-=		(const float& oth) {
		x -= oth;
		y -= oth;
		z -= oth;
	}
	void operator*=		(const float& oth) {
		x *= oth;
		y *= oth;
		z *= oth;
	}
	void operator/=		(const float& oth) {
		x /= oth;
		y /= oth;
		z /= oth;
	}

	FLOAT3 operator+	(const float& oth) {

		FLOAT3 tmp;

		tmp.x = x + oth;
		tmp.y = y + oth;
		tmp.z = z + oth;

		return tmp;
	}
	FLOAT3 operator-	(const float& oth) {

		FLOAT3 tmp;

		tmp.x = x - oth;
		tmp.y = y - oth;
		tmp.z = z - oth;

		return tmp;
	}
	FLOAT3 operator*	(const float& oth) {

		FLOAT3 tmp;

		tmp.x = x * oth;
		tmp.y = y * oth;
		tmp.z = z * oth;

		return tmp;
	}
	FLOAT3 operator/	(const float& oth) {

		FLOAT3 tmp;

		tmp.x = x / oth;
		tmp.y = y / oth;
		tmp.z = z / oth;

		return tmp;
	}

	void operator=		(const FLOAT3& oth) {
		x = oth.x;
		y = oth.y;
		z = oth.z;
	}
	void operator+=		(const FLOAT3& oth) {
		x += oth.x;
		y += oth.y;
		z += oth.z;
	}
	void operator-=		(const FLOAT3& oth) {
		x -= oth.x;
		y -= oth.y;
		z -= oth.z;
	}
	void operator*=		(const FLOAT3& oth) {
		x *= oth.x;
		y *= oth.y;
		z *= oth.z;
	}
	void operator/=		(const FLOAT3& oth) {
		x /= oth.x;
		y /= oth.y;
		z /= oth.z;
	}

	FLOAT3 operator+	(const FLOAT3& oth) {

		FLOAT3 tmp;

		tmp.x = x + oth.x;
		tmp.y = y + oth.y;
		tmp.z = z + oth.z;

		return tmp;
	}
	FLOAT3 operator-	(const FLOAT3& oth) {

		FLOAT3 tmp;

		tmp.x = x - oth.x;
		tmp.y = y - oth.y;
		tmp.z = z - oth.z;

		return tmp;
	}
	FLOAT3 operator*	(const FLOAT3& oth) {

		FLOAT3 tmp;

		tmp.x = x * oth.x;
		tmp.y = y * oth.y;
		tmp.z = z * oth.z;

		return tmp;
	}
	FLOAT3 operator/	(const FLOAT3& oth) {

		FLOAT3 tmp;

		tmp.x = x / oth.x;
		tmp.y = y / oth.y;
		tmp.z = z / oth.z;

		return tmp;
	}
};

struct FLOAT4 {

	FLOAT4() {};
	FLOAT4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	void operator=		(const float& oth) {
		x = oth;
		y = oth;
		z = oth;
		w = oth;
	}
	void operator+=		(const float& oth) {
		x += oth;
		y += oth;
		z += oth;
		w += oth;
	}
	void operator-=		(const float& oth) {
		x -= oth;
		y -= oth;
		z -= oth;
		w -= oth;
	}
	void operator*=		(const float& oth) {
		x *= oth;
		y *= oth;
		z *= oth;
		w *= oth;
	}
	void operator/=		(const float& oth) {
		x /= oth;
		y /= oth;
		z /= oth;
		w /= oth;
	}

	FLOAT4 operator+	(const float& oth) {

		FLOAT4 tmp;

		tmp.x = x + oth;
		tmp.y = y + oth;
		tmp.z = z + oth;
		tmp.w = w + oth;

		return tmp;
	}
	FLOAT4 operator-	(const float& oth) {

		FLOAT4 tmp;

		tmp.x = x - oth;
		tmp.y = y - oth;
		tmp.z = z - oth;
		tmp.w = w - oth;

		return tmp;
	}
	FLOAT4 operator*	(const float& oth) {

		FLOAT4 tmp;

		tmp.x = x * oth;
		tmp.y = y * oth;
		tmp.z = z * oth;
		tmp.w = w * oth;

		return tmp;
	}
	FLOAT4 operator/	(const float& oth) {

		FLOAT4 tmp;

		tmp.x = x / oth;
		tmp.y = y / oth;
		tmp.z = z / oth;
		tmp.w = w / oth;

		return tmp;
	}

	void operator=		(const FLOAT4& oth) {
		x = oth.x;
		y = oth.y;
		z = oth.z;
		w = oth.w;
	}
	void operator+=		(const FLOAT4& oth) {
		x += oth.x;
		y += oth.y;
		z += oth.z;
		w += oth.w;
	}
	void operator-=		(const FLOAT4& oth) {
		x -= oth.x;
		y -= oth.y;
		z -= oth.z;
		w -= oth.w;
	}
	void operator*=		(const FLOAT4& oth) {
		x *= oth.x;
		y *= oth.y;
		z *= oth.z;
		w *= oth.w;
	}
	void operator/=		(const FLOAT4& oth) {
		x /= oth.x;
		y /= oth.y;
		z /= oth.z;
		w /= oth.w;
	}

	FLOAT4 operator+	(const FLOAT4& oth) {

		FLOAT4 tmp;

		tmp.x = x + oth.x;
		tmp.y = y + oth.y;
		tmp.z = z + oth.z;
		tmp.w = w + oth.w;

		return tmp;
	}
	FLOAT4 operator-	(const FLOAT4& oth) {

		FLOAT4 tmp;

		tmp.x = x - oth.x;
		tmp.y = y - oth.y;
		tmp.z = z - oth.z;
		tmp.w = w - oth.w;

		return tmp;
	}
	FLOAT4 operator*	(const FLOAT4& oth) {

		FLOAT4 tmp;

		tmp.x = x * oth.x;
		tmp.y = y * oth.y;
		tmp.z = z * oth.z;
		tmp.w = w * oth.w;

		return tmp;
	}
	FLOAT4 operator/	(const FLOAT4& oth) {

		FLOAT4 tmp;

		tmp.x = x / oth.x;
		tmp.y = y / oth.y;
		tmp.z = z / oth.z;
		tmp.w = w / oth.w;

		return tmp;
	}
};

struct FLAOT4X4
{
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
};

