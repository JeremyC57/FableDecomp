// CTCKrakenTentacle::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCKrakenTentacle::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCKrakenTentacle@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB353, 16 bytes: mov ecx,[esp+4]; call CTCKrakenTentacle::StatGetClassTypeName 0x004D5DEB; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCKrakenTentacle@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012407BC (RTTI .?AVCTCKrakenTentacle@@) slot 22 (0x01240814 -> 0x004DB353) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5DEB, the landed CTCKrakenTentacle::StatGetClassTypeName (?StatGetClassTypeName@CTCKrakenTentacle@@SI?AVCCharString@@XZ; literal "CTCKrakenTentacle").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCKrakenTentacle_GetClassTypeName@4 calling the made-up ?Init_004d5deb@T@@QAEXXZ.
// Header: engine/CTCKrakenTentacle.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCKRAKENTENTACLE_TRACK_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#define CTCKrakenTentacle CTCKrakenTentacleLayout
#include "engine/CTCKrakenTentacle.h"
#undef CTCKrakenTentacle

class CTCKrakenTentacle : public CTCKrakenTentacleLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5DEB
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCKrakenTentacle::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
