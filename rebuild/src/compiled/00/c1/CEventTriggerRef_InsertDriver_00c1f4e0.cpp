// CEventTriggerRef::InsertDriver(unsigned long index, void* driver) -- passes (index, driver) to the trigger item's table; on a negative result it resets the trigger's type (-1: SetFamily2, else SetVariant2) and asks the table again, returning that result
// Leaf: ?InsertDriver@CEventTriggerRef@@QAEJKPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F4E0, 76 bytes (`ret 8`). Descriptive name (no PDB covers this library): libpk/CBankProcessor.h
//   CEventTriggerRef::InsertDriver ("76B insertion dispatcher"), which the ported 0x00C2E610 CBankProcessor::AddSampleNameTriggers binds;
//   was CItemAction::Handle (the manifest's "CQueueProcessor_HandleProcessError" is propagated). Callers 0x00C2E57B / 0x00C2E75E /
//   0x00C2E8B2 (the event-trigger drivers 0x00C2E4D0 / 0x00C2E610 / 0x00C2E7A0, which also build the refs 0x00C1EE60 / 0x00C1EEE0).
// Callees: the trigger item's GetTable slot 5 (+0x14) and the table's slot 3 (+0xC); the type setters @ 0x00C1F3C0 (sets type byte 2
//   to 2) and @ 0x00C1F380 (sets type byte 1 to 2), both thiscall without arguments.
// Shape: index and driver stay in ebx/edi for both table calls; the setter calls share one `mov ecx,esi`.
// NF65 (round 7): ported onto libpk/CBankProcessor.h (CEventTriggerRef: m_pRegistry +0, m_pTrigger +4; overlay opt-in
//   FABLE_RETAIL_CEVENTTRIGGERREF_TYPESETTERS for the two setters) and libpk/CItem.h (CItem slot 5 GetTable, CItemTable slot 3 under the
//   overlay opt-in FABLE_RETAIL_CITEMTABLE_INSERT); the local CItemTarget / CItem / CItemAction models are gone.
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#define FABLE_RETAIL_CITEMTABLE_INSERT
#define FABLE_RETAIL_CEVENTTRIGGERREF_TYPESETTERS
#include "libpk/CItem.h"
#include "libpk/CBankProcessor.h"

long CEventTriggerRef::InsertDriver(unsigned long index, void* driver)
{
    long result = m_pTrigger->GetTable()->Insert(index, driver);
    if (result < 0) {
        if (result == -1)
            SetFamily2();
        else
            SetVariant2();
        result = m_pTrigger->GetTable()->Insert(index, driver);
    }
    return result;
}
