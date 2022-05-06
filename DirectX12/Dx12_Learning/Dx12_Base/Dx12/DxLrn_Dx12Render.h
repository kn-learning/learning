#include "../Util/DxLrn_Windows.h"
#include "DxLrn_Dx12.h"
#include "DxLrn_Dx12FrameResource.h"

class DxLrnDx12Render 
{
public:
	void Build()	override;
	void Begin()	override;
	void End()		override;

protected:

private:
	void WorkerThread(int threadIndex);

	bool m_bUseWarpDevice = false;

	UINT m_nFrameCount;
	UINT m_nFrameIndex;
	UINT m_nRtvDescSize;

	ComPtr<IDXGISwapChain3>			m_pSwapChain;
	ComPtr<ID3D12Device>			m_pDevice;

	ComPtr<ID3D12CommandQueue>		m_pCommandQueue;
	ComPtr<ID3D12CommandAllocator>	m_pCommandAllocator;

	ComPtr<ID3D12Resource>			m_pRenderTargets[m_nFrameCount];
	ComPtr<ID3D12Resource>			m_pDepthStencil;

	ComPtr<ID3D12DescriptorHeap>	m_pRtvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_pDsvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_pCbvHeap;
	ComPtr<ID3D12DescriptorHeap>	m_pSmpHeap;

	ComPtr<ID3D12RootSignature>		m_pRootSignature;
	ComPtr<ID3D12PipelineState>		m_pPipelineState;

	static DxLrnDx12Render* s_pRender;

	struct ThreadParameter
	{
		int threadIndex;
	};
	ThreadParameter m_threadParameterd[NumContexts];

	std::unique_ptr<DxLrnDx12FrameResource> m_pFrameResource[m_nFrameCount];
	DxLrnDx12FrameResource*					m_pCurrentFrameResource;

	HANDLE	m_hWorkerBegin;
	HANDLE	m_hWorkerEnd;
	HANDLE	m_hThread;

	UINT				m_hFenceEvent;
	UINT64				m_nFenceValue;
	ComPtr<ID3D12Fence> m_pFence;
};