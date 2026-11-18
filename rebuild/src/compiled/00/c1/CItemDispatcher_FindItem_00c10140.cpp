// CItemDispatcher::FindItem(long key) -- the id lookup: the dispatcher's own slot 17 FindByKey on the key's stack slot
// Leaf: ?FindItem@CItemDispatcher@@QAEPAUCItem@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C10140, 13 bytes, thiscall `ret 4`: `mov eax,[ecx] ; lea edx,[esp+4] ; push edx ; call [eax+44h]`.
// Name (NF70, q10757): the spelling libpk/CItem.h gives it (FABLE_RETAIL_CITEMDISPATCHER_FINDITEM, NF65; descriptive, libpk has no PDB)
//   and its landed binder 0x00C1EE60 uses; was the stand-in CNamedSet::FindById(unsigned long) on a local model. The dispatcher's slots 16
//   / 17 come from the overlay opt-in FABLE_RETAIL_CITEMDISPATCHER_FINDBYKEY (the CNamedIndex sibling's TransferAllFrom / FindByKey).
#define FABLE_RETAIL_CITEMDISPATCHER_FINDITEM
#define FABLE_RETAIL_CITEMDISPATCHER_FINDBYKEY
#include "libpk/CItem.h"
CItem* CItemDispatcher::FindItem(long key)
{
    return FindByKey(reinterpret_cast<const unsigned long&>(key));
}
