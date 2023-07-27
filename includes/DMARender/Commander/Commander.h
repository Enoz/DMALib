#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <map>



namespace DMARender {
    class Commander;
    extern std::map<HWND, Commander*> hwndMap;
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    class Commander {
        // Data
        ID3D11Device*           g_pd3dDevice = nullptr;
        ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;
        IDXGISwapChain*         g_pSwapChain = nullptr;

        ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

        // Forward declarations of helper functions
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();


        //My Functions
    public:
        UINT                    g_ResizeWidth = 0, g_ResizeHeight = 0;
        void initializeWindow();
    };



}