// NPlayerGui::GFAnchorToBottomRight(const C2DVector& pos) -- a 640x480 GUI position anchored to the bottom-right corner of the real render target
// Leaf: ?GFAnchorToBottomRight@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643D0C, 81 bytes, plain `ret` (__fastcall: hidden C2DVector result in ecx,
// pos in edx). Size-optimised NPlayerGui TU (`leave` epilogue).
// Identity: Ego_r.pdb `?GFAnchorToBottomRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z`.
// Callees: CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 twice (one per coordinate) on the display manager
// 0x013B8390. Floats 640.0f (0x01238168) and 480.0f (0x01238874).
// Shape: C2DVector(width + pos.X - 640, height + pos.Y - 480); right to left, so Y (and its dimensions call) comes first and
// is parked in a frame word across the second call.
#pragma optimize("s", on)
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};

struct C2DBoxF {
    float TLX;
    float TLY;
    float BRX;
    float BRY;
    C2DBoxF() {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};

struct C2DExtentsI {
    long Width;
    long Height;
};

struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions();                 // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;                    // 0x013B8390

namespace NPlayerGui {

C2DVector __fastcall GFAnchorToBottomRight(const C2DVector& pos)
{
    return C2DVector((float)g_pDisplayManager->GetRenderTargetDimensions().Width + pos.X - 640.0f,
                     (float)g_pDisplayManager->GetRenderTargetDimensions().Height + pos.Y - 480.0f);
}

}