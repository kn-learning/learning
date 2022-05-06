#include <DirectXMath.h>
#pragma comment(lib, "DirectXMath.lib")

using namespace DirectX;

struct ConstantObject
{
	::XMFLOAT4X4	world;
};

struct ConstantCamera 
{
	::XMFLOAT4X4	directionLight;

	::XMFLOAT4X4	view;
	::XMFLOAT4X4	projection;

	::XMFLOAT4X4	inversView;
	::XMFLOAT4X4	inversProjection;
	::XMFLOAT4X4	inversVP;
};

struct ConstantPointLights 
{

};