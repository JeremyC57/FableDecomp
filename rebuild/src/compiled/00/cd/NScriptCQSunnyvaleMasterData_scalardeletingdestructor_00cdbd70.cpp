// NScript::CQ_SunnyvaleMasterData::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CQ_SunnyvaleMasterData inlined (destroying the vector<CCharString> BoastRegions at +0x134, the CCharString TeddySolution at +0x54, then ~CBaseClassNonCopyable), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCQ_SunnyvaleMasterData@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDBD70, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Name fix (agentNF24, land1011de): was the descriptive CNoRttiObject_012C2748 (??_GCNoRttiObject_012C2748@@UAEPAXI@Z). Vtable 0x012C2748 is
//   NScript::CQ_SunnyvaleMasterData's: NScript::CQ_SunnyvaleMasterData::CQ_SunnyvaleMasterData 0x00CDB9E0 and Alloc 0x00CDBD20 store it,
//   its slots 1/2 are the landed OnPersist 0x00CDC070 / Init 0x00CDBA10 (agentFAM08); the destroyed members are BoastRegions +0x134 and
//   TeddySolution +0x54 (engine/CQ_SunnyvaleMasterData.h, Ego_r layout). Ego_r.pdb / FableWin.pdb ??_GCQ_SunnyvaleMasterData@NScript@@UAEPAXI@Z.
// Callees: vector<CCharString>::~vector 0x00414EA0, CCharString::~CCharString 0x0099EAE0, CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300, operator delete 0x00BFE9BC.
// Shape: no vtable store -- every destructor between this class and CBaseClassNonCopyable is implicit; member destructors run in
//   reverse declaration order. The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class:
//   MakeOne below is only that construction (a TU device, as in the landed row; it binds no retail address).
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_SunnyvaleMasterData.h"

void* MakeOne()
{
    return new NScript::CQ_SunnyvaleMasterData;
}
