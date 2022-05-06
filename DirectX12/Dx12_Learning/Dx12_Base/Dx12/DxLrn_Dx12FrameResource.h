#include "../Util/DxLrn_Windows.h"
#include "DxLrn_Dx12.h"
#include "DxLrn_DxConstantResource.h"

constexpr UINT	NumContexts			= 3;
constexpr int	CommandListCount	= 3;
constexpr int	CommandListPre		= 0;
constexpr int	CommandListMid		= 1;
constexpr int	CommandListPost		= 2;

class DxLrnDx12FrameResource
{
public:
	 DxLrnDx12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, ID3D12DescriptorHeap* pDsvHeap, ID3D12DescriptorHeap* pCbvSrvHeap, D3D12_VIEWPORT* pViewport, UINT frameResourceIndex);
	~DxLrnDx12FrameResource();

	void Initialize();
	void Bind(ID3D12GraphicsCommandList* pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle);
	void WriteObjectConstantBuffer();

	ID3D12CommandList* m_pBatchSubmit[NumContexts * 2 + CommandListCount];

	ComPtr<ID3D12CommandAllocator>		m_pCommandAllocators		[CommandListCount];
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandLists				[CommandListCount];

	ComPtr<ID3D12CommandAllocator>		m_pObjectCommandAllocators	[NumContexts];
	ComPtr<ID3D12GraphicsCommandList>	m_pObjectCommandLists		[NumContexts];

	UINT64 m_nFenceValue;

protected:

private:
	void CreateBatchList();


	ComPtr<ID3D12PipelineState> m_pPipelineState;
	ComPtr<ID3D12Resource>		m_pObjectConstantBuffer;

	ConstantObject* m_pConstantObject;


	D3D12_GPU_DESCRIPTOR_HANDLE m_hObjectCbv;
};