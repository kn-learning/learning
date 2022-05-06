#include "DxLrn_Dx12Render.h"

void DxLrnDx12Render::Build()
{

#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		debugController->EnableDebugLayer();
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

	{
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		if (m_bUseWarpDevice)
		{
			ComPtr<IDXGIAdapter1> warpAdapter;

			ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
			ThrowIfFailed(warpAdapter.Get(), featureLevel, IID_PPV_ARGS(&m_pDevice));
		}
		else
		{
			ComPtr<IDXGIAdapter1> hardAdapter;
			ComPtr<IDXGIAdapter1> adapter;

			for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, adapter.GetAddressOf()); ++i)
			{
				DXGI_ADAPTER_DESC1 adapterDesc{};
				adapter->GetDesc1(&adapterDesc);

				if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

				if(ThrowIfFailed(D3D12CreateDevice(adapter.Get(), featureLevel, IID_PPV_ARGS(&m_pDevice)))) break;
			}

			hardAdapter = adapter.Detach();

			ThrowIfFailed(D3D12CreateDevice(hardAdapter.Get(), featureLevel, IID_PPV_ARGS(&m_pDevice)));
		}
	}

	{
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
		commandQueueDesc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDesc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(m_pDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue)));
	}

	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.BufferCount	= m_nFrameCount;
		swapChainDesc.Width			= WINDOW_WIDTH;
		swapChainDesc.Height		= WINDOW_HEIGHT;
		swapChainDesc.Format		= DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc	= { 1, 0 };

		ComPtr<IDXGISwapChain1> swapChain;

		ThrowIfFailed(factory->CreateSwapChainForHwnd(m_pCommandQueue.Get(), nullptr, &swapChainDesc, nullptr, nullptr, swapChain.GetAddressOf()));
		ThrowIfFailed(swapChain.As(&m_pSwapChain));

		m_nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	}


	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.NumDescriptors		= m_nFrameCount;
		rtvHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowIfFailed(m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRtvHeap)));
		m_nRtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		dsvHeapDesc.NumDescriptors		= m_nFrameCount;
		dsvHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		ThrowIfFailed(m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pDsvHeap)));
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.NumDescriptors		= m_nFrameCount * 2;
		cbvHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		ThrowIfFailed(m_pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvHeap)));
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{};
		samplerHeapDesc.NumDescriptors	= 2;
		samplerHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		samplerHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		ThrowIfFailed(m_pDevice->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_pSamplerHeap)));
	}

	ThrowIfFailed(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator)));
}

void DxLrnDx12Render::Begin()
{
}

void DxLrnDx12Render::End()
{
}
