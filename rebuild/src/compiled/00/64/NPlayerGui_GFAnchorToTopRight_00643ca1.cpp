// NPlayerGui::GFAnchorToTopRight(const C2DVector& pos) -- a 640x480 GUI position anchored to the top-right corner of the real render target
// Leaf: ?GFAnchorToTopRight@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643CA1, 57 bytes, plain `ret` (__fastcall: hidden C2DVector result in ecx,
// pos in edx). Size-optimised NPlayerGui TU (no function alignment, `leave` epilogue).
// Identity: Ego_r.pdb `?GFAnchorToTopRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z`.
// Callees: CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 (C2DExtentsI by hidden pointer) on the display manager
// 0x013B8390. Float 640.0f from pool 0x01238168.
// Shape: the result is built with C2DVector's (x, y) constructor; arguments evaluate right to left, so pos.Y is read into a
// frame word before the call; X = width + pos.X - 640.
#pragma optimize("s", on)
class C2DVector { public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};

class C2DBoxF { public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
    C2DBoxF() {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};

class C2DExtentsI { public:
    long Width;
    long Height;
};

class CDisplayManager { public:
    C2DExtentsI GetRenderTargetDimensions();                 // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;                    // 0x013B8390

namespace NPlayerGui {

C2DVector __fastcall GFAnchorToTopRight(const C2DVector& pos)
{
    return C2DVector((float)g_pDisplayManager->GetRenderTargetDimensions().Width + pos.X - 640.0f, pos.Y);
}

}