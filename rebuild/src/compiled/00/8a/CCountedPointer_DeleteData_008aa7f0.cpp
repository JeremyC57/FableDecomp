// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CScriptGameResourceObjectMovieBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA7F0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x008AA7F0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_008aa7f0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@SIXPAX@Z; stored by CCountedPointer<CScriptGameResourceObjectMovieBase>::Reset 0x008AB2B0 (reloc_bindings, caller 0x00CF8850); Ego_r.pdb CScriptGameResourceObjectMovieBase::PImp.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"


template void FABLE_FASTCALL CCountedPointer<CScriptGameResourceObjectMovieBase>::DeleteData(void*);

