#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"
#include "../ImageAllocator/ImageAllocator.h"
#include "../IOverlay/IOverlay.h"
#include "RenderBridge/RenderBridge.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <map>
#include <memory>
#include <format>



namespace DMARender {
    class RenderWindow;
    extern std::map<HWND, RenderWindow*> hwndMap;
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    class RenderWindow {
        //WindowCreation
        ID3D11Device*           g_pd3dDevice = nullptr;
        ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;
        IDXGISwapChain*         g_pSwapChain = nullptr;
        ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        void CreateFonts();

        std::shared_ptr<RenderBridge> bridge = nullptr;
        ImageAllocator* testAllocator;

        //Fonts
        ImFont* windowIdentifyFont;

        void drawOverlayHandler();
        UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    public:
        RenderWindow();
        void _setResizeParams(UINT width, UINT height);
        void initializeWindow();
        
        //Adapter
        std::shared_ptr<RenderBridge> getBridge();
    };



}