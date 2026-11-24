// NUISystem::CDeletion::CDeletion() -- default-constructs a UI component deletion record: Method left unset, empty AssociatedParents list
// Leaf: ??0CDeletion@NUISystem@@QAE@XZ
// Retail: Fable.exe @ 0x0042BED4, 22 bytes, returns this: constructs the list at +4 through _List_base<CComponent*>::_List_base(const allocator&)
//   0x0042ABAF (list's own inline constructor expanded) with a default allocator temporary (`lea eax,[esp+7]`). Catalog label CBankFile::GetBankHandle is wrong.
// Identity (sweep2): was landed as NUISystem::CAction::CAction, but (a) the object is 8 bytes and goes BY VALUE to
//   CComponent::SetDeletion(CDeletion) (NUISystem::CManager 0x0041E922: ctor into [ebp-0x248], `mov [ebp-0x248],1`, copy via
//   CDeletion's copy ctor 0x0042CD84, `call [edi+0xf8]` = vtable slot 62 SetDeletion), and its partner dtor 0x0042BEEA
//   destroys the +4 list with ~list<NUISystem::CComponent*> 0x0042ABCA; (b) Ego_r.pdb CDeletion is {EDeletionMethod Method;
//   list<CComponent*> AssociatedParents} (one list), while CAction holds two lists and a bool (16 bytes in retail).
//   Name set: ??0CDeletion@NUISystem@@QAE@XZ.
// Port (NF241, 2026-11-22, the list<CComponent*> family pass): the row-local std::allocator / std::list / CDeletion models are
//   replaced by the shared stlport/fable_containers.h and engine/NUISystem_CDeletion.h (FABLE_RETAIL_CDELETION: the user-declared
//   out-of-line constructors). The call binds 0x0042ABAF as _List_base<CComponent*>'s constructor (was ??0?$list@..): retail
//   0x0042B6A6 is list<CComponent*>'s own size-optimised list(const allocator&) COMDAT, `push [esp+8] ; mov esi,ecx ; call
//   0x0042ABAF` (STLport 4.6 _list.h: the sentinel allocation lives in _List_base); FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE declares
//   that constructor out of line, as this TU calls it.
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/NUISystem_CDeletion.h"
NUISystem::CDeletion::CDeletion()
{
}
