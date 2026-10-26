// NPlayerGui::GFToScreenScaled(const C2DBoxF& box) -- scale a 640x480 GUI box to the real render target
// Leaf: ?GFToScreenScaled@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643BFC, 152 bytes, plain `ret` (__fastcall: hidden C2DBoxF result in ecx,
// box in edx). Size-optimised NPlayerGui TU (`leave` epilogue).
// Identity: Ego_r.pdb `?GFToScreenScaled@NPlayerGui@@YI?AVC2DBoxF@@ABV2@@Z`.
// Callees: CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 four times (display manager 0x013B8390). Floats
// 1/640 (0x01238178) and 1/480 (0x0123887C).
// Shape: C2DBoxF's four-float constructor with each coordinate `dimension * coordinate * (1/640 or 1/480)`; right to left,
// so BRY, BRX and TLY park in frame words and TLX stays on the FPU for the last store.
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

C2DBoxF __fastcall GFToScreenScaled(const C2DBoxF& box)
{
    return C2DBoxF((float)g_pDisplayManager->GetRenderTargetDimensions().Width * box.TLX * (1.0f / 640.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Height * box.TLY * (1.0f / 480.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Width * box.BRX * (1.0f / 640.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Height * box.BRY * (1.0f / 480.0f));
}

}