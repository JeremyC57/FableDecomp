// NScript::CQ_SunnyvaleMasterData::Alloc() -- the script data allocator of Q_SunnyvaleMaster: new 0x144 bytes + the inline constructor of NScript::CQ_SunnyvaleMasterData
// Leaf: ?Alloc@CQ_SunnyvaleMasterData@NScript@@SIPAVCScriptDataBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDBD20, 79 bytes, static __fastcall with no arguments, plain `ret`.
// Name fix (agentNF24, land1011de): was the free stand-in pSunnyvaleDataAlloc_func (FSE FableAPI.cpp's variable name for this
//   pointer, not a C++ symbol). Ego_r.pdb ?Alloc@CQ_SunnyvaleMasterData@NScript@@SIPAVCScriptDataBase@@XZ; the body is new 0x144
//   (= sizeof CQ_SunnyvaleMasterData) + the inlined 0x00CDB9E0 body storing vtable 0x012C2748; the script-info registration passes it
//   as the master-data allocator (ebp at 0x00CD532D, agentFAM08). Header opt-in FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_ALLOC.
// Callees: scalar operator new @ 0x00BFEA1A (0x144 bytes), CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0,
//   CCharString::CCharString() @ 0x0099E4B0 (TeddySolution, +0x54).
// Shape: speed TU. The ctor is inline: out-of-line base ctor, vtable store, TeddySolution's ctor, then the empty BoastRegions
//   vector's three pointers zeroed; the plain-data members are not initialised.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_ALLOC
#include "engine/CQ_SunnyvaleMasterData.h"

CScriptDataBase* FABLE_FASTCALL NScript::CQ_SunnyvaleMasterData::Alloc()
{
    return new CQ_SunnyvaleMasterData;
}
