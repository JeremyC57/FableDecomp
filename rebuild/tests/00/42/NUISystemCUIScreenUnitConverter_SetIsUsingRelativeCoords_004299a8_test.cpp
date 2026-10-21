#include <cstdio>
class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI() {}
};
class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();       // 0x009BEDC0
};
class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;                    // +0x60 CCountedPointer<CDisplayManager>::Data
};
CSystemManager* __fastcall GFGetSystemManager();         // 0x009A4EC0
namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static bool  IsUsingRelativeCoords;                  // 0x013B8768
    static float BackBufferWidth;                        // 0x013B876C
    static float BackBufferHeight;                       // 0x013B8770
    static void __fastcall SetIsUsingRelativeCoords(bool relative);
};
void __fastcall CUIScreenUnitConverter::SetIsUsingRelativeCoords(bool relative)
{
    if (IsUsingRelativeCoords != relative)
    {
        IsUsingRelativeCoords = relative;
        if (relative)
        {
            CSystemManager* system = GFGetSystemManager();
            C2DExtentsI dimensions = system->PDisplayManager->GetRenderTargetDimensions();
            BackBufferWidth = (float)dimensions.X;
            BackBufferHeight = (float)dimensions.Y;
        }
    }
}
}

static CSystemManager g_sys; static CDisplayManager g_disp; static long g_x = 640, g_y = 480; static int g_calls = 0;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { ++g_calls; C2DExtentsI r; r.X = g_x; r.Y = g_y; return r; }
CSystemManager* __fastcall GFGetSystemManager() { g_sys.PDisplayManager = &g_disp; return &g_sys; }
bool NUISystem::CUIScreenUnitConverter::IsUsingRelativeCoords = false;
float NUISystem::CUIScreenUnitConverter::BackBufferWidth = 0.0f;
float NUISystem::CUIScreenUnitConverter::BackBufferHeight = 0.0f;
int main()
{
    using NUISystem::CUIScreenUnitConverter;
    CUIScreenUnitConverter::SetIsUsingRelativeCoords(true);
    bool ok = CUIScreenUnitConverter::IsUsingRelativeCoords && g_calls == 1
        && CUIScreenUnitConverter::BackBufferWidth == 640.0f && CUIScreenUnitConverter::BackBufferHeight == 480.0f;
    g_x = 800; CUIScreenUnitConverter::SetIsUsingRelativeCoords(true);
    ok = ok && g_calls == 1 && CUIScreenUnitConverter::BackBufferWidth == 640.0f;
    CUIScreenUnitConverter::SetIsUsingRelativeCoords(false);
    ok = ok && !CUIScreenUnitConverter::IsUsingRelativeCoords && g_calls == 1;
    CUIScreenUnitConverter::SetIsUsingRelativeCoords(true);
    ok = ok && g_calls == 2 && CUIScreenUnitConverter::BackBufferWidth == 800.0f;
    if (ok) std::printf("004299a8_TEST PASS\n");
    return 0;
}