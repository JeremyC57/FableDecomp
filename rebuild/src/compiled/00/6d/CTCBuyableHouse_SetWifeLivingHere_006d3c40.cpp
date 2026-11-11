// CTCBuyableHouse::SetWifeLivingHere(CThing* wife) -- WifeLivingHere = CIntelligentPointer<CThing>(wife)
// Leaf: ?SetWifeLivingHere@CTCBuyableHouse@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D3C40, oracle length 71 bytes, thiscall `ret 4`.
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset CIntelligentPointer::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0x38] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// Name (naming agent NF21, 2026-11-11): FableWin.pdb ?SetWifeLivingHere@CTCBuyableHouse@@QAEXPAVCThing@@@Z (0x0248B580, beside
//   ?GetWifeLivingHere@CTCBuyableHouse@@QBEPAVCThing@@XZ); Ego_r.pdb CTCBuyableHouse member CIntelligentPointer<CThing> WifeLivingHere
//   at +0x38, the member this body assigns; the caller CTCWife::Unmarry 0x006CFF10 passes 0 on the interface-0xD5 component
//   (TCI_BUYABLE_HOUSE). It was the generated stand-in CClass_006d3c40::AssignTarget_006d3c40.
// Callees: CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10;
//   CIntelligentPointer<CThing> vftable 0x01238C6C.
// Layout: engine/CTCBuyableHouse.h (FABLE_RETAIL_CTCBUYABLEHOUSE + the WIFE opt-in), engine/CIntelligentPointer.h (retail, ASSIGN:
//   the item read into a local, then SetPItem).
// Shape: the temporary CIntelligentPointer<CThing>(wife) (inline ctor: base ctor, vptr, SetPItem), the inline operator=, the
//   temporary's destructor.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCBuyableHouse.h"

void CTCBuyableHouse::SetWifeLivingHere(CThing* wife)
{
    WifeLivingHere = CIntelligentPointer<CThing>(wife);
}
