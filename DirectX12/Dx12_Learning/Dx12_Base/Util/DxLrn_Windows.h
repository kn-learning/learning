#include <windows.h>
#include <wrl/client.h>
#include "DxLrn_Utility.h"

using Microsoft::WRL::ComPtr;

constexpr int WINDOW_WIDTH	= 960;
constexpr int WINDOW_HEIGHT	= 540;

inline bool ThrowIfFailed(HRESULT hr) {

	if (FAILED(hr))
	{
		throw std::runtime_error(hr);
		return false;
	}

	return true;
}