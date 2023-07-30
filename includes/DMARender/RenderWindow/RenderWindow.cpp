#include "RenderWindow.h"

std::map<HWND, DMARender::RenderWindow*> DMARender::hwndMap = std::map<HWND, DMARender::RenderWindow*>();

void DMARender::RenderWindow::drawOverlayHandler()
{
    static bool identifyWindows = false;
    static bool overlayEnabled = false;
    auto pIO = ImGui::GetPlatformIO();
    static int monitor_current_idx = 0;
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::Begin("Overlay Handler", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
    if (overlayEnabled) {
        auto io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f FPS", io.Framerate);
        if (ImGui::Button("Disable Overlay"))
            overlayEnabled = false;
    }
    else {
        ImGui::Checkbox("Identify Windows", &identifyWindows);
        std::string comboPreview = std::format("Window {}", monitor_current_idx);

        if (ImGui::BeginCombo("Overlay Monitor", comboPreview.c_str())) {
            for (int i = 0; i < pIO.Monitors.size(); i++) {
                auto pMonitor = pIO.Monitors[i];
                const bool isSelected = (monitor_current_idx == i);
                auto monitorName = std::format("Monitor {}", i);
                if (ImGui::Selectable(monitorName.c_str(), isSelected)) {
                    monitor_current_idx = i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Selected Resolution: %.0fx%.0f", pIO.Monitors[monitor_current_idx].MainSize.x, pIO.Monitors[monitor_current_idx].MainSize.y);
        if (ImGui::Button("Begin Overlay")) {
            overlayEnabled = true;
            identifyWindows = false;
        }
    }
    ImGui::End();

    if (identifyWindows) {
        for (int i = 0; i < pIO.Monitors.size(); i++) {
            auto pMonitor = pIO.Monitors[i];
            ImGui::SetNextWindowPos(ImVec2(pMonitor.MainPos.x + 40, pMonitor.MainPos.y + 40));
            ImGui::SetNextWindowSize(ImVec2(150, 250));
            auto windowName = std::format("{}", i);
            ImGui::Begin(windowName.c_str(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

            auto drawList = ImGui::GetWindowDrawList();
            auto p = ImGui::GetCursorScreenPos();

            ImGui::PushFont(windowIdentifyFont);
            drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(p.x, p.y), IM_COL32(255, 0, 0, 255), windowName.c_str());
            ImGui::PopFont();
            ImGui::End();
        }
    }

    if (overlayEnabled) {
        auto selectedMonitor = pIO.Monitors[monitor_current_idx];
        ImGui::SetNextWindowSize(ImVec2(selectedMonitor.MainSize.x, selectedMonitor.MainSize.y));
        ImGui::SetNextWindowPos((ImVec2(selectedMonitor.MainPos.x, selectedMonitor.MainPos.y)));
        ImGui::Begin("ESP Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
        this->bridge->getOverlay()->DrawOverlay();
        ImGui::End();
    }
}

void DMARender::RenderWindow::drawMapHandler()
{

    //Map Selection
    auto maps = bridge->getMapManager()->getMaps();
    if (maps.size() == 0)
        return;
    static int map_current_index = 0;
    static MapTransform mTrans = MapTransform();
    ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
    if (bridge->getMapManager()->isMapSelected()) {
        if (ImGui::Button("Stop Radar"))
            bridge->getMapManager()->clearMap();
    }
    else {
        auto previewString = maps[map_current_index]->getName();
        if (ImGui::BeginCombo("Selected Map", previewString.c_str())) {
            for (int i = 0; i < maps.size(); i++) {
                bool isSelected = i == map_current_index;
                if (ImGui::Selectable(maps[i]->getName().c_str(), isSelected))
                    map_current_index = i;
            }

            ImGui::EndCombo();
        }
        if (ImGui::Button("Start Radar")) {
            mTrans.mapZoom = -1;
            mTrans.dragOffsetX = 0;
            mTrans.dragOffsetY = 0;
            bridge->getMapManager()->selectMap(map_current_index);
        }

    }
    ImGui::End();

    //Radar Logic
    if (!bridge->getMapManager()->isMapSelected())
        return;
    auto gameMap = bridge->getMapManager()->getSelectedGameMap();
    auto allocator = bridge->getMapManager()->getSelectedAllocator();
    if (!gameMap || !allocator)
        return;

    RECT rect;
    GetWindowRect(hwnd, &rect);


    if (mTrans.mapZoom < 0) {
        float mapSize = fmaxf(allocator->getWidth(), allocator->getHeight());
        float screenSize = fminf(rect.right - rect.left, rect.bottom - rect.top);
        mTrans.mapZoom = screenSize / mapSize;
        mTrans.mapHeight = allocator->getHeight();
        mTrans.mapWidth = allocator->getWidth();
    }

    auto mousePos = ImGui::GetMousePos();
    static float lastMousePosX = mousePos.x;
    static float lastMousePosY = mousePos.y;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::GetIO().WantCaptureMouse) {
        mTrans.dragOffsetX += mousePos.x - lastMousePosX;
        mTrans.dragOffsetY += mousePos.y - lastMousePosY;
    }
    if (ImGui::GetIO().MouseWheel != 0.0f) {
        float oldZoom = mTrans.mapZoom;
        //Zoom in/out
        mTrans.mapZoom += ImGui::GetIO().MouseWheel * .01;
        if (mTrans.mapZoom < 0.01)
            mTrans.mapZoom = 0.01;

        //Zoom toward cursor
        float deltaX = (allocator->getWidth() * oldZoom) - (allocator->getWidth() * mTrans.mapZoom);
        float deltaY = (allocator->getHeight() * oldZoom) - (allocator->getHeight() * mTrans.mapZoom);

        float percX = (mousePos.x - rect.left - mTrans.dragOffsetX) / ((allocator->getWidth() * mTrans.mapZoom));
        float percY = (mousePos.y - rect.top - mTrans.dragOffsetY) / ((allocator->getHeight() * mTrans.mapZoom));

        mTrans.dragOffsetX += (deltaX * percX);
        mTrans.dragOffsetY += (deltaY * percY);
    }

    lastMousePosX = mousePos.x;
    lastMousePosY = mousePos.y;



    ImDrawList* fgDrawList = ImGui::GetBackgroundDrawList();
    fgDrawList->AddImage(
        allocator->getImage(),
        ImVec2(
            rect.left + mTrans.dragOffsetX,
            rect.top + mTrans.dragOffsetY
        ),
        ImVec2(
            rect.left + mTrans.dragOffsetX + (allocator->getWidth() * mTrans.mapZoom),
            rect.top + mTrans.dragOffsetY + (allocator->getHeight() * mTrans.mapZoom)
        )
    );
    this->bridge->getRadar()->DrawOverlay(bridge->getMapManager()->getSelectedGameMap().get(), mTrans);
    
}

DMARender::RenderWindow::RenderWindow()
{
    g_pd3dDevicePtr = new ID3D11Device*;
    this->bridge = std::shared_ptr<RenderBridge>(new RenderBridge(g_pd3dDevicePtr));
}

void DMARender::RenderWindow::_setResizeParams(UINT width, UINT height)
{
    this->g_ResizeWidth = width;
    this->g_ResizeHeight = height;
}

void DMARender::RenderWindow::initializeWindow()
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, DMARender::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowW(wc.lpszClassName, L"DMA Commander",  WS_OVERLAPPEDWINDOW , 0, 0, 1280, 720, nullptr, nullptr, wc.hInstance, nullptr);
    DMARender::hwndMap[hwnd] = this;

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    CreateFonts();
    this->bridge->createFonts();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    *g_pd3dDevicePtr = g_pd3dDevice;


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (this->bridge != nullptr) {
            drawMapHandler();
            if (this->bridge->getOverlay() != nullptr) {
                drawOverlayHandler();
            }
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        //g_pSwapChain->Present(1, 0); // Present with vsync
        g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return;
}

std::shared_ptr<DMARender::RenderBridge> DMARender::RenderWindow::getBridge()
{
    return this->bridge;
}

// Helper functions
bool DMARender::RenderWindow::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void DMARender::RenderWindow::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void DMARender::RenderWindow::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != NULL) {
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

}

void DMARender::RenderWindow::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void DMARender::RenderWindow::CreateFonts()
{
    ImGui::GetIO().Fonts->AddFontDefault();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 1;
    config.GlyphExtraSpacing.x = 1.0f;
    windowIdentifyFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 256, &config);
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI DMARender::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED)
            return 0;
        RenderWindow* rndPtr = DMARender::hwndMap[hWnd];
        rndPtr->_setResizeParams((UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); // Queue resize
        return 0;
    }
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
