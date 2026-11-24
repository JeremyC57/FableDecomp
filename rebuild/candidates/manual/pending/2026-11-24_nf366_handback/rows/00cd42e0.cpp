// CCountedPointer<CScriptDefinitionManager>::DeleteData(void* data) -- the counted pointer's per-type deleter: null-guarded virtual destroy
//   (vtable slot 0, flag arg 1), i.e. `delete (T*)data` through the scalar deleting destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptDefinitionManager@@@@SIXPAX@Z
// __fastcall pointer=ecx; the callee is the object's own slot 0.
// Names (NF366, 2026-11-24; was the behavioural extern "C" @DeleteIfSet@4, catalog sub_00cd42e0): FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VCScriptDefinitionManager@@@@SIXPAX@Z (static __fastcall void(void*)); retail stores this address as the
//   count block's DeleteFunc (`mov dword ptr [eax+4], 0x00CD42E0`) in CCountedPointer<CScriptDefinitionManager>::Reset 0x00CD43A0, whose
//   only caller is the 12-byte CScriptDefinitionManager::Set 0x00CD3F40 (FableWin.pdb + Ego_r.pdb
//   ?Set@CScriptDefinitionManager@@SIXPAV1@@Z, Ego_r 12 bytes: `push ecx; mov ecx, offset PClass; call Reset; ret`), called by
//   CScriptInfoManager::Init 0x00CB5D80 with a fresh CScriptDefinitionManager (constructor 0x00CD3F00, vtable 0x012C2648 without RTTI);
//   FableWin.pdb also names Reset(CScriptDefinitionManager*) and the static CScriptDefinitionManager::PClass (0x0143E920).
// Respelled in place in the 0x00428B10 form: an explicit specialization of engine/CCountedPointer.h's member (a strong definition, as the
//   landed extern "C" function was) with the landed body over its local slot-0 model; T is only declared by its shared header.
//   FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (the landed NF222 / NF282 opt-in) keeps the member declaration-only: with the
//   header's in-class body VC7.1 emits no symbol for the specialization (NF360 measurement).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"

#include "fable_string.h"
#include "engine/CScriptDefinitionManager.h"

struct Obj { virtual void Destroy(int flags); };

template <>
void FABLE_FASTCALL CCountedPointer<CScriptDefinitionManager>::DeleteData(void* data)
{
    if (data)
        static_cast<Obj*>(data)->Destroy(1);
}
