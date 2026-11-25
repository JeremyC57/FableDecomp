// CTCCarriedActionUseActivateQuest::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarriedActionUseActivateQuest::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarriedActionUseActivateQuest@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB114, 16 bytes: mov ecx,[esp+4]; call CTCCarriedActionUseActivateQuest::StatGetClassTypeName 0x004D5043; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarriedActionUseActivateQuest@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124036C (RTTI .?AVCTCCarriedActionUseActivateQuest@@) slot 22 (0x012403C4 -> 0x004DB114) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5043, the landed CTCCarriedActionUseActivateQuest::StatGetClassTypeName (?StatGetClassTypeName@CTCCarriedActionUseActivateQuest@@SI?AVCCharString@@XZ; literal "CTCCarriedActionUseActivateQuest").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarriedActionUseActivateQuest_GetClassTypeName@4 calling the made-up ?Init_004d5043@T@@QAEXXZ.
// Header: engine/CTCCarriedActionUseActivateQuest.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCCarriedActionUseActivateQuest CTCCarriedActionUseActivateQuestLayout
#include "engine/CTCCarriedActionUseActivateQuest.h"
#undef CTCCarriedActionUseActivateQuest

class CTCCarriedActionUseActivateQuest : public CTCCarriedActionUseActivateQuestLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5043
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarriedActionUseActivateQuest::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
