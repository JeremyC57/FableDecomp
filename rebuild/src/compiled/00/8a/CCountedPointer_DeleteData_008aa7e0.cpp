// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CScriptGameResourceObjectScriptedThingBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA7E0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x008AA7E0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_008aa7e0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@SIXPAX@Z; stored by CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::Reset 0x008AB230 (reloc_bindings, caller 0x008AB960); Ego_r.pdb CScriptGameResourceObjectScriptedThingBase::PImp.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"


template void FABLE_FASTCALL CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::DeleteData(void*);

