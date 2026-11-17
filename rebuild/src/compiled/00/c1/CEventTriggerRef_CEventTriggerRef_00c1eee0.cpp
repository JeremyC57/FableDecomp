// CEventTriggerRef::CEventTriggerRef(CNamedSet* registry, CString name, bool named) -- binds the ref to the registry's trigger item for the name; when there is none, a new item is created from the name's hash (a named item that also keeps the name when `named`, else a hashed one) and attached to the registry
// Flags: /EHsc
// Leaf: ??0CEventTriggerRef@@QAE@PAVCNamedSet@@VCString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1EEE0, 217 bytes (`ret 0Ch`). Built under /EHsc. Descriptive name (no PDB covers this library):
//   libpk/CBankProcessor.h CEventTriggerRef(CNamedSet*, CString, bool) ("NativeC1EEE0 two pointer words; actual217B ctor"), the spelling the
//   ported 0x00C2E610 CBankProcessor::AddSampleNameTriggers binds; was CItemAction::CItemAction (the manifest's
//   "CAnimationSet_LoadAndUnsheatheWeapons" is propagated). Sibling of the keyed constructor 0x00C1EE60; callers 0x00C2E74A / 0x00C2E82E.
// Callees: CNamedSet::Find @ 0x00C101D0 ((name, collision counter = 0)) on the registry, scalar operator new @ 0x00BFEA1A,
//   CString::GetHash @ 0x00C0DBB0, CNamedItem(const CString&, unsigned long) @ 0x00C20230 (0x14 bytes) or CHashedItem(unsigned long)
//   @ 0x00C1F640 (0x10 bytes), the new item's Attach slot 6 (+0x18), ~CString @ 0x00C0BA40 for the by-value name.
// Shape: the by-value CString parameter puts the frame in EH state 0 from entry (the callee destroys it at the end,
//   state -1); each `new` raises the state to 1 / 2 around its constructor and the shared attach drops it back to 0.
//   The new pointer is homed in the dead `named` slot.
// NF65 (round 7): ported onto libpk/CBankProcessor.h (CEventTriggerRef, CNamedSet::Find), libpk/CItem.h (CItem slot 6 Attach; overlay
//   opt-ins FABLE_RETAIL_CHASHEDITEM_CTOR / FABLE_RETAIL_CNAMEDITEM_CTOR for the two item ctors and their 0x10 / 0x14 layouts) and
//   engine/CString.h (GetHash via FABLE_RETAIL_CSTRING_BANK_TRIGGER_PASS23, ~CString); the local CString / CItem / CNamedItem /
//   CHashedItem / CItemDispatcher / CItemAction models are gone. The registry is libpk/CBankProcessor.h's CNamedSet while libpk/CItem.h's
//   slot 6 takes the same object as CItemDispatcher (two shared models of the 0x00C101D0 class; NOTES.md), so the virtual Attach call
//   converts the pointer (no relocation).
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#define FABLE_RETAIL_CNAMEDITEM_CTOR
#define FABLE_RETAIL_CSTRING_BANK_TRIGGER_PASS23
#define FABLE_RETAIL_CSTRING_MAKEUPPER
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "libpk/CBankProcessor.h"
#include "libpk/CItem.h"

CEventTriggerRef::CEventTriggerRef(CNamedSet* registry, CString name, bool named)
    : m_pRegistry(registry), m_pTrigger(registry->Find(name, 0))
{
    if (!m_pTrigger) {
        if (named)
            m_pTrigger = new CNamedItem(name, name.GetHash());
        else
            m_pTrigger = new CHashedItem(name.GetHash());
        m_pTrigger->Attach(reinterpret_cast<CItemDispatcher*>(m_pRegistry));
    }
}
