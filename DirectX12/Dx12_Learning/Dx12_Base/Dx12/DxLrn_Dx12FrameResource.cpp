#include "DxLrn_Dx12FrameResource.h"

DxLrnDx12FrameResource::DxLrnDx12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, ID3D12DescriptorHeap* pDsvHeap, ID3D12DescriptorHeap* pCbvSrvHeap, D3D12_VIEWPORT* pViewport, UINT frameResourceIndex)
{
	m_nFenceValue		= 0;
	m_pPipelineState	= pPso;

	for (UINT i = 0; i < CommandListCount; ++i)
	{
		ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocators[i])));
		ThrowIfFailed(pDevice->CreateCommandList	 (0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocators[i].Get(), m_pPipelineState.Get(), IID_PPV_ARGS(&m_pCommandLists[i])));

		ThrowIfFailed(m_pCommandLists[i]->Close());
	}

	for (UINT i = 0; i < NumContex; ++i)
	{
		ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pObjectCommandAllocators[i])));
		ThrowIfFailed(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pObjectCommandAllocators[i].Get(), m_pPipelineState.Get(), IID_PPV_ARGS(&m_pObjectCommandLists[i])));

		ThrowIfFailed(m_pObjectCommandLists[i]->Close());
	}
















	CreateBatchList();
}

DxLrnDx12FrameResource::~DxLrnDx12FrameResource()
{
}

void DxLrnDx12FrameResource::Initialize()
{
	for (int i = 0; i < CommandListCount; ++i)
	{
		ThrowIfFailed(m_pCommandAllocators[i]->Reset());
		ThrowIfFailed(m_pCommandLists[i]->Reset(m_pCommandAllocators[i].Get(), m_pPipelineState.Get()));
	}

	for (int i = 0; i < NumContexts; ++i) 
	{
		ThrowIfFailed(m_pObjectCommandAllocators[i]->Reset());
		ThrowIfFailed(m_pObjectCommandLists[i]->Reset(m_pObjectCommandAllocators[i].Get(), m_pPipelineState.Get()));
	}
}

void DxLrnDx12FrameResource::Bind(ID3D12GraphicsCommandList* pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle)
{
	pCommandList->SetGraphicsRootDescriptorTable(1, m_hObjectCbv));

	assert(pRtvHandle != nullptr);
	assert(pDsvHandle != nullptr);

	pCommandList->OMSetRenderTargets(1, pRtvHandle, false, pDsvHandle);
}

void DxLrnDx12FrameResource::WriteObjectConstantBuffer()
{
	memcpy(m_pConstantObject, nullptr, sizeof(ConstantObject));
}

void DxLrnDx12FrameResource::CreateBatchList()
{
	const UINT batchSize = _countof(m_pObjectCommandLists);

	m_pBatchSubmit[0] = m_pCommandLists[CommandListPre].Get();

	memcpy(
		m_pBatchSubmit + 1,
		m_pObjectCommandLists,
		_countof(m_pObjectCommandLists) * sizeof(ID3D12CommandList*)
	);
}
