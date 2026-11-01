#include <math.h>

#include "rebuild_abi.h"
// GFRoundVXToNearestPixel(double x) -- round a virtual-screen coordinate down to the nearest whole render-target pixel
// Leaf: ?GFRoundVXToNearestPixel@@YIMN@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E1D00. Callees: ?GFGetSystemManager@@YIPAVCSystemManager@@XZ,
//   ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ 0x009BEDC0 (PDB: returns const C2DExtentsI by value) on
//   CSystemManager::PDisplayManager (+0x60, ESTABLISHED_SPELLINGS 0x013B8390 evidence).
// Naming wave 18: landed as the CDisplayManagerPixelRoundView / CSystemManagerPixelRoundView stand-ins; respelled on the PDB
//   class, member and return spelling (name-only: the bytes are unchanged). The virtual-screen double keeps its row-local
//   spelling (VIRTUAL_SCREEN_* gated).

class C2DExtentsI
{ public:
    fable_i32 X;
    fable_i32 Y;
};

class CDisplayManager
{ public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};

class CSystemManager
{ public:
    fable_u8 unknown000[0x60];
    CDisplayManager* PDisplayManager;                // +0x60
};

extern double g_FableVirtualWidth_013961E8;

CSystemManager* FABLE_FASTCALL GFGetSystemManager();

float FABLE_FASTCALL GFRoundVXToNearestPixel(double x)
{
    CDisplayManager* displayManager =
        GFGetSystemManager()->PDisplayManager;
    const double onePixel =
        g_FableVirtualWidth_013961E8 /
        displayManager->GetRenderTargetDimensions().X;
    const fable_i32 pixel =
        static_cast<fable_i32>(floor(x / onePixel));
    return static_cast<float>(pixel * onePixel);
}
