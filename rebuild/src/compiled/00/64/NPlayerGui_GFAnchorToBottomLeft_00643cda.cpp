// NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos) -- a 640x480 GUI position anchored to the bottom-left corner of the real render target
// Leaf: ?GFAnchorToBottomLeft@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643CDA, 50 bytes, plain `ret` (__fastcall: hidden C2DVector result in ecx,
// pos in edx). Size-optimised NPlayerGui TU; this one has no frame pointer (VC's size heuristic picks the ebp frame only for
// its siblings with more frame traffic).
// Identity: Ego_r.pdb `?GFAnchorToBottomLeft@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z`.
// Callees: CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on the display manager 0x013B8390. Float 480.0f from
// pool 0x01238874.
// Shape: C2DVector(pos.X, height + pos.Y - 480); the Y expression (the right-hand argument) is evaluated first.
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

C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& pos)
{
    return C2DVector(pos.X, (float)g_pDisplayManager->GetRenderTargetDimensions().Height + pos.Y - 480.0f);
}

}