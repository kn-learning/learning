#include "DxLrn_Dx12Render.h"

void DxLrnDx12Render::Build()
{
	try 
	{
		CreateDevice();
		CreateFence();
		CreateRootSignature();
		CreatePipelineState();
	}
	catch (std::runtime_error& error) 
	{
		RuntimeError(error);
	}
	finally
	{
		RuntimeLog("Build Finish");
	}
}

void DxLrnDx12Render::Begin()
{
	m_pCurrentFrameResource->Initialize();
	//swap barriar
	const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{}, dsvHandle{};
	rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart() + m_nFrameIndex * m_nRtvDescSize;

	m_pCurrentFrameResource->m_pCommandLists[CommandListPre]->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_pCurrentFrameResource->m_pCommandLists[CommandListPre]->ClearDepthStencilView(m_pDsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0. 0, nullptr);

	ThrowIfFailed(m_pCurrentFrameResource->m_pCommandLists[CommandListPre]->Close());
}

void DxLrnDx12Render::End()
{
	//swap barriar
	ThrowIfFailed(m_pCurrentFrameResource->m_pCommandLists[CommandListPost]->Close());
}


void DxLrnDx12Render::CreateDevice()
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

				if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), featureLevel, IID_PPV_ARGS(&m_pDevice)))) break;
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

void DxLrnDx12Render::CreateFence()
{
	ThrowIfFailed(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
	
	m_nFenceValue = 1;
	m_hFenceEvent = CreateEvent(nullptr, false, false, nullptr);

	if (m_hFenceEvent = nullptr) 
	{
		ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	WaitForPreviousFrame();
}

void DxLrnDx12Render::CreateRootSignature()
{
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	D3D12_ROOT_PARAMETER	rootParams[2]{};
	D3D12_DESCRIPTOR_RANGE	camRange{}, objRange{};

	camRange.NumDescriptors						= 1;
	camRange.BaseShaderRegister					= 0;
	camRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	camRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	objRange.NumDescriptors						= 1;
	objRange.BaseShaderRegister					= 1;
	objRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	objRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	rootParams[0].ParameterType			= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[0].ShaderVisibility		= D3D12_SHADER_VISIBILITY_ALL;
	rootParams[0].DescriptorTable		= {1, &camRange};

	rootParams[1].ParameterType			= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].ShaderVisibility		= D3D12_SHADER_VISIBILITY_ALL;
	rootParams[1].DescriptorTable		= {1, &objRange};


	D3D12_STATIC_SAMPLER_DESC samplerDescDef{}, samplerDesc[2];

	samplerDesc.Filter					= D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MipLODBias				= 0.0f;
	samplerDesc.MaxAnisotropy			= 16;
	samplerDesc.ComparisonFunc			= D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.BorderColor				= D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.MinLOD					= 0.0f;
	samplerDesc.MaxLOD					= D3D12_FLOAT32_MAX;
	samplerDesc.RegisterSpace			= 0;
	samplerDesc.ShaderVisibility		= D3D12_SHADER_VISIBILITY_PIXEL;

	samplerDesc[0]						= samplerDescDef;
	samplerDesc[0].AddressU				= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV				= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW				= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].ShaderRegister		= 0;

	samplerDesc[1]						= samplerDescDef;
	samplerDesc[1].AddressU				= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressV				= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].AddressW				= D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc[1].ShaderRegister		= 1;


	D3D12_ROOT_SIGNATURE_DESC1 rootSignatureDesc{};
	rootSignatureDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters		= rootParams;
	rootSignatureDesc.NumParameters		= _countof(rootParams);
	rootSignatureDesc.pStaticSamplers	= samplerDesc;
	rootSignatureDesc.NumStaticSamplers = _countof(samplerDesc);

	ComPtr<ID3DBlob> signature;

	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, nullptr));
	ThrowIfFailed(m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature)));
}

void DxLrnDx12Render::CreatePipelineState(HashKey VSName, HashKey PSName)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	pipelineDesc.pRootSignature = m_pRootSignature.Get();

	{
		D3D12_SHADER_BYTECODE shaderBytecode{};

		pipelineDesc.VS = shaderBytecode;
	}

	{
		D3D12_SHADER_BYTECODE shaderBytecode{};

		pipelineDesc.PS = shaderBytecode;
	}

	{
		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.FillMode					= D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode					= D3D12_CULL_MODE_NONE;
		rasterizerDesc.FrontCounterClockwise	= false;
		rasterizerDesc.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerDesc.DepthBiasClamp			= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerDesc.SlopeScaledDepthBias		= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.DepthClipEnable			= true;
		rasterizerDesc.MultisampleEnable		= false;
		rasterizerDesc.AntialiasedLineEnable	= false;
		rasterizerDesc.ForcedSampleCount		= 0;
		rasterizerDesc.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		
		pipelineDesc.RasterizerState = rasterizerDesc;
	}

	{
		D3D12_BLEND_DESC blendStateDesc{};
		blendStateDesc.AlphaToCoverageEnable	= false;
		blendStateDesc.IndependentBlendEnable	= false;

		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		{
			blendStateDesc.RenderTarget[i].BlendEnable				= false;
			blendStateDesc.RenderTarget[i].LogicOpEnable			= false;
			blendStateDesc.RenderTarget[i].SrcBlend					= D3D12_BLEND_ONE;
			blendStateDesc.RenderTarget[i].DestBlend				= D3D12_BLEND_ZERO;
			blendStateDesc.RenderTarget[i].BlendOp					= D3D12_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[i].SrcBlendAlpha			= D3D12_BLEND_ONE;
			blendStateDesc.RenderTarget[i].DestBlendAlpha			= D3D12_BLEND_ZERO;
			blendStateDesc.RenderTarget[i].BlendOpAlpha				= D3D12_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[i].LogicOp					= D3D12_LOGIC_OP_NOOP;
			blendStateDesc.RenderTarget[i].RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;
		}
		pipelineDesc.BlendState = blendStateDesc;
	}

	{
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

		depthStencilDesc.DepthEnable					= true;
		depthStencilDesc.DepthFunc						= D3D12_COMPARISON_FUNC_LESS;
		depthStencilDesc.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.StencilEnable					= false;
		depthStencilDesc.StencilReadMask				= D3D12_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask				= D3D12_DEFAULT_STENCIL_WRITE_MASK;

		depthStencilDesc.FrontFace.StencilFailOp		= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp		= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp			= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp	= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp			= D3D12_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC_ALWAYS;

		pipelineDesc.DepthStencilState = depthStencilDesc;
	}

	pipelineDesc.PrimitiveTopologyType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets		= 1;
	pipelineDesc.RTVFormats[0]			= DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.DSVFormat				= DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.SampleMask				= UINT_MAX;
	pipelineDesc.SampleDesc				= {1,0};

	ThrowIfFailed(m_pDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&m_pPipelineState)));
}

void DxLrnDx12Render::CreateThreadPool()
{
}


void DxLrnDx12Render::SetCommonPipelineState(ID3D12GraphicsCommandList* pCommandList)
{
	pCommandList->SetGraphicsRootSignature	(m_pRootSignature.Get());
	pCommandList->SetDescriptorHeaps		(1, m_pCbvHeap.GetAddressOf());
	pCommandList->RSSetViewports			();
	pCommandList->RSSetScissorRects			();
	pCommandList->IASetPrimitiveTopology	(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->OMSetStencilRef			(0);
}

void DxLrnDx12Render::WaitForPreviousFrame()
{
	const UINT64 fence = m_nFenceValue;
	ThrowIfFailed(m_pCommandQueue->Signal(m_pFence.Get(), fence));

	++m_nFenceValue;

	if (m_pFence->GetCompletedValue() < fence) 
	{
		ThrowIfFailed(m_pFence->SetEventOnCompletion(fence, m_hFenceEvent));
		WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

	m_nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
}

void DxLrnDx12Render::WorkerThread(int threadIndex)
{
	assert(threadIndex >= 0);
	assert(threadIndex < NumContexts);

	while (threadIndex >= 0 && threadIndex < NumContexts)
	{
		WaitForSingleObject(m_hWorkerBeginFrame[threadIndex], INFINITE);

		ID3D12GraphicsCommandList* pObjectCommandList = m_pCurrentFrameResource->m_pObjectCommandLists[threadIndex].Get();

		SetCommonPipelineState(pObjectCommandList);
	
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{}, dsvHandle{};
		rtvHandle.ptr = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart() + m_nFrameIndex * m_nRtvDescSize;
		dsvHandle.ptr = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

		m_pCurrentFrameResource->Bind(pObjectCommandList, &rtvHandle, &dsvHandle);

		D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle{};
		cbvHandle.ptr = m_pCbvHeap->GetGPUDescriptorHandleForHeapStart();

		const UINT nCbvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// ‚±‚±‚Ådraw

		ThrowIfFailed(pObjectCommandList->Close());

		SetEvent(m_hWorkerEndFrame[threadIndex]);
	}
}
