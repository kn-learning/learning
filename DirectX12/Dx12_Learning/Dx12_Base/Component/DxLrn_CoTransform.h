#include "../Independent/DxLrn_Utility.h"

struct Vertex
{
	Vertex() {}
	Vertex(FLOAT4 position, FLOAT4 normal, FLOAT4 color, FLOAT2 texCoord)
	{
		this->position	= position;
		this->normal	= normal;
		this->color		= color;
		this->texCoord	= texCoord;
	}

	FLOAT4 position{};
	FLOAT4 normal{};
	FLOAT4 color{};
	FLOAT2 texCoord{};
};

struct Transform
{
	Transform() {}

	FLOAT4 position	{ 0.0f, 0.0f, 0.0f, 1.0f };
	FLOAT4 rotation	{ 0.0f, 0.0f, 0.0f, 0.0f };
	FLOAT4 scale	{ 1.0f, 1.0f, 1.0f, 1.0f };
};

struct Camera
{
	Camera() {}

	FLOAT3 position	{ 0.0f, 0.0f, 0.0f };
	FLOAT3 at		{ 0.0f, 0.0f, 0.0f };
	FLOAT3 up		{ 0.0f, 1.0f, 0.0f };

	float fFovY	= 45.0f;
	float fNear = 0.1f;
	float fFar	= 1000.0f;
};


using Vertices	= std::vector<Vertex>;
using Indices	= std::vector<unsigned int>;