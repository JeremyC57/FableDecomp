// CTCTargetingPlayer::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTargetingPlayer::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTargetingPlayer@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E230A, 16 bytes: mov ecx,[esp+4]; call CTCTargetingPlayer::StatGetClassTypeName 0x0049D450; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTargetingPlayer@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124280C (RTTI .?AVCTCTargetingPlayer@@) slot 22 (0x01242864 -> 0x004E230A) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x0049D450, CTCTargetingPlayer::StatGetClassTypeName (?StatGetClassTypeName@CTCTargetingPlayer@@SI?AVCCharString@@XZ; literal "CTCTargetingPlayer"; respelt by NF417 batch s1, which this row needs).
// Respell NF417 (RULINGS r1 of NF412): was the extern "C" __stdcall stand-in _CTCTargetingPlayer_GetClassTypeName@4 calling the made-up ?Init@T@@QAEXXZ.
// Header: engine/CTCTargetingPlayer.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form), opt-in FABLE_RETAIL_CTCTARGETINGPLAYER
//   (the polymorphic retail class on CTCTargetingBase; the default branch is the generated flat layout, not derived from CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCTargetingPlayer CTCTargetingPlayerLayout
#include "engine/CTCTargetingPlayer.h"
#undef CTCTargetingPlayer

class CTCTargetingPlayer : public CTCTargetingPlayerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x0049D450
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTargetingPlayer::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
