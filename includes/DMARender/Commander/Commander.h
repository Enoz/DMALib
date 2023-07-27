#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>



namespace DMARender {
    // Data
    static ID3D11Device*           g_pd3dDevice = nullptr;
    static ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;
    static IDXGISwapChain*         g_pSwapChain = nullptr;
    static UINT                    g_ResizeWidth = 0, g_ResizeHeight = 0;
    static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    // Forward declarations of helper functions
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    //My Functions
    void initializeWindow();
}