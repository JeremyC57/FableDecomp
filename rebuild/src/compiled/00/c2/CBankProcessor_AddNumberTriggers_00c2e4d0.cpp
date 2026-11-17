// CBankProcessor::AddNumberTriggers() -- when a number-trigger registry is attached (+0x70), give every driver record of the bank data an event trigger under its number holding that driver; logs unknown drivers and duplicate trigger numbers
// Leaf: AddNumberTriggers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E4D0, length 208 bytes (204 + int3 padding), __fastcall, plain `ret`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EB42. Callees: the driver list's GetCount / GetItem (slots 15 / 13), the driver map's Get @ 0x00C3ABD0, the
//   registry's FindItem(number) @ 0x00C10140 (slot 17 by key address; landed label "CDataOutputStream::WriteCBYTE" is propagated),
//   the trigger reference's constructor @ 0x00C1EE60 (registry, number: finds the trigger or `new`s a 16-byte one, ctor
//   0x00C1F720, and registers it), GetDriverCount @ 0x00C1EE30 and InsertDriver(position, driver) @ 0x00C1F4E0 (grows the
//   trigger's driver list and retries when the insert fails), the log's AddMessage @ 0x00C307E0 and the stream operators.
// Name: the catalog's `EventTrigger_ProcessDrivers` is a guess; descriptive (the strings are "Could not find driver number "
//   and "Duplicate event trigger for number ").
// Shape: the 8-byte trigger reference is a loop-local whose address is taken, which makes VC7.1 build the `and esp,-8`
//   aligned frame (ebp then also serves as the loop index); its destructor is trivial. The append is
//   `InsertDriver(GetDriverCount(), driver)`: the driver is pushed first, then the count is computed (right-to-left
//   arguments). The two messages share their `<< number` tail (VC cross-jumps the second into the first).
// NF65 (round 7): ported onto libpk/CBankProcessor.h (CBankProcessor / CBankData / CBankRecordList<SDriverRecord> GetCount / GetItemByIndex,
//   CIndexMap_00c3abd0::Get, CEventTriggerRef GetDriverCount / InsertDriver; overlay opt-ins FABLE_RETAIL_CBANKPROCESSOR_NUMBERTRIGGERS (the
//   +0x70 registry typed CItemDispatcher*) and FABLE_RETAIL_CEVENTTRIGGERREF_NUMBERCTOR (the 0x00C1EE60 ctor)), libpk/CItem.h (CItemDispatcher;
//   overlay opt-in FABLE_RETAIL_CITEMDISPATCHER_FINDITEM for 0x00C10140), libpk/CScriptHost.h CLoadLog and engine/CString.h; the local
//   CString / CLoadLog / SDriverRecord / CRecordList / CBankData / CNumberTriggerRegistry / CEventTriggerRef / CDriverMap / CBankProcessor
//   models are gone. InsertDriver now binds ?InsertDriver@CEventTriggerRef@@QAEJKPAX@Z (was ...QAEXKPAUCDriver@@@Z).
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#define FABLE_RETAIL_CBANKPROCESSOR_NUMBERTRIGGERS
#define FABLE_RETAIL_CBANKPROCESSOR_STEPS
#define FABLE_RETAIL_CEVENTTRIGGERREF_NUMBERCTOR
#define FABLE_RETAIL_CITEMDISPATCHER_FINDITEM
#include "libpk/CItem.h"
#include "libpk/CBankProcessor.h"

void CBankProcessor::AddNumberTriggers()
{
    if (!m_pNumberTriggers)
        return;
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        SDriverRecord* record = m_pData->m_drivers.GetItemByIndex(i);
        void* driver = m_drivers.Get(record->m_number);
        if (!driver)
            m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
        else if (m_pNumberTriggers->FindItem(record->m_number))
            m_pLog->AddMessage() << "Duplicate event trigger for number " << record->m_number;
        else {
            CEventTriggerRef trigger(m_pNumberTriggers, record->m_number);
            trigger.InsertDriver(trigger.GetDriverCount(), driver);
        }
    }
}