// CTCActionUseActivateQuest::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseActivateQuest::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseActivateQuest@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB1DA, 16 bytes: mov ecx,[esp+4]; call CTCActionUseActivateQuest::StatGetClassTypeName 0x004D57FA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseActivateQuest@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012404AC (RTTI .?AVCTCActionUseActivateQuest@@) slot 22 (0x01240504 -> 0x004DB1DA) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D57FA, the landed CTCActionUseActivateQuest::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseActivateQuest@@SI?AVCCharString@@XZ; literal "CTCActionUseActivateQuest").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseActivateQuest_GetClassTypeName@4 calling the made-up ?Init_004d57fa@T@@QAEXXZ.
// Header: engine/CTCActionUseActivateQuest.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCActionUseActivateQuest CTCActionUseActivateQuestLayout
#include "engine/CTCActionUseActivateQuest.h"
#undef CTCActionUseActivateQuest

class CTCActionUseActivateQuest : public CTCActionUseActivateQuestLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D57FA
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseActivateQuest::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
