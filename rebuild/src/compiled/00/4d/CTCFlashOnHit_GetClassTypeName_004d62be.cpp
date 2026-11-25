// CTCFlashOnHit::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCFlashOnHit::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCFlashOnHit@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D62BE, 16 bytes: mov ecx,[esp+4]; call CTCFlashOnHit::StatGetClassTypeName 0x004D627F; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCFlashOnHit@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@) slot 22 (0x0123E08C -> 0x004D62BE) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D627F, the landed CTCFlashOnHit::StatGetClassTypeName (?StatGetClassTypeName@CTCFlashOnHit@@SI?AVCCharString@@XZ; literal "CTCFlashOnHit").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCFlashOnHit_GetClassTypeName@4 calling the made-up ?Init_004d627f@T@@QAEXXZ.
// Header: engine/CTCFlashOnHit.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCFlashOnHit CTCFlashOnHitLayout
#include "engine/CTCFlashOnHit.h"
#undef CTCFlashOnHit

class CTCFlashOnHit : public CTCFlashOnHitLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D627F
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCFlashOnHit::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
