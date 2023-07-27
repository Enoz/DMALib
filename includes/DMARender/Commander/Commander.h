#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#include <string>
#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif




namespace DMARender {

    struct FrameContext
    {
        ID3D12CommandAllocator* CommandAllocator;
        UINT64                  FenceValue;
    };

    // Data
    static int const                    NUM_FRAMES_IN_FLIGHT = 3;
    static FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
    static UINT                         g_frameIndex = 0;

    static int const                    NUM_BACK_BUFFERS = 3;
    static ID3D12Device* g_pd3dDevice = nullptr;
    static ID3D12DescriptorHeap* g_pd3dRtvDescHeap = nullptr;
    static ID3D12DescriptorHeap* g_pd3dSrvDescHeap = nullptr;
    static ID3D12CommandQueue* g_pd3dCommandQueue = nullptr;
    static ID3D12GraphicsCommandList* g_pd3dCommandList = nullptr;
    static ID3D12Fence* g_fence = nullptr;
    static HANDLE                       g_fenceEvent = nullptr;
    static UINT64                       g_fenceLastSignaledValue = 0;
    static IDXGISwapChain3* g_pSwapChain = nullptr;
    static HANDLE                       g_hSwapChainWaitableObject = nullptr;
    static ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
    static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

    // Forward declarations of helper functions
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    void WaitForLastSubmittedFrame();
    FrameContext* WaitForNextFrameResources();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    //My Functions
    void initializeWindow();
}