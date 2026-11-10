// CThing::GetTC<CTCJackPillar>(CTCJackPillar** result) -- if the thing has a Jack-of-Blades pillar component (TCI_JACK_PILLAR = 37), store it through result and return true, else return false
// Leaf: ??$GetTC@VCTCJackPillar@@@CThing@@QAE_NPAPAVCTCJackPillar@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00976E20, 67 bytes, thiscall `ret 4`. Not called or referenced in retail: the instance's
//   own COMDAT (every other use in the Jack-of-Blades AI TU is expanded), kept because retail links without /OPT:REF.
// Identity: FableWin.pdb ??$GetTC@VCTCJackPillar@@@CThing@@QAE_NPAPAVCTCJackPillar@@@Z (with Ego_r.pdb / FableWin.pdb
//   ?StatGetInterfaceType@CTCJackPillar@@SI?AW4ETCInterfaceType@@XZ); the body tests UsingComponents bit 37 (`test byte
//   [ecx+24h],20h`) and looks up key 0x25 = TCI_JACK_PILLAR (engine/ETCInterfaceType.h) in Components (+0x44) through
//   CVectorMap::LowerBound 0x0040F020: exactly the shared engine/CThing.h GetTC<T> body (FABLE_RETAIL_CTHING_GETTC_EXPANDED),
//   key copied into the local inside the test, map end read through the map pointer. The TU is the Jack-of-Blades AI one
//   (neighbours CAIStateGroup_JackOfBladesBase::SetupArena 0x00972830, CCreatureAction_JackFlash 0x00976E80).
// Shape: explicit instantiation emits the member template as its own COMDAT.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"

class CTCJackPillar {                                       // only the static type query is needed here
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_JACK_PILLAR; }
};

template bool CThing::GetTC<CTCJackPillar>(CTCJackPillar** result);