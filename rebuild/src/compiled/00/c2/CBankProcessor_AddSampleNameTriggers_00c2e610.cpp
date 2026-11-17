// CBankProcessor::AddSampleNameTriggers() -- when a criteria-trigger registry is attached and the option is on, give every driver that plays its own sample an event trigger named after that sample's file (upper-cased, no extension); missing drivers and duplicate names are logged
// Leaf: ?AddSampleNameTriggers@CBankProcessor@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2E610, 395 bytes, thiscall, plain `ret`; EH frame (handler 0x01213C22) for the name string.
// Identity (descriptive: libpk has no PDB or RTTI): called once by CBankProcessor::Load 0x00C2EB00 (0x00C2EB68) between
//   ProcessAllDrivers and AddEvents; the criteria-string twin of the landed AddNumberTriggers 0x00C2E4D0 (same driver loop, same
//   "Could not find driver number " text 0x012B488C) and of AddEventTrigger 0x00C2E7A0, whose model and callee spellings this
//   row reuses. Strings: "Duplicate event trigger for string '" 0x012B48C4, "'. Hash code = " 0x012B48B4. The catalog label
//   "AnimationSet_ProcessEventTriggers" is a guess. `AddSampleNameTriggers`, m_bSampleNameTriggers (+0x79), SDriverRecord::m_sample
//   and SSampleHeader::m_fileName are descriptive names (NOTES).
// Callees: CRecordList slots 13 / 15 (GetItem / GetCount), the sample list's slot 17 Find (+0x44), CDriverMap::Find 0x00C3ABD0,
//   CString::GetFileNameWithoutExtension 0x00C0E360 (catalog name), CString::MakeUpper 0x00C0C910 (copy-on-write 0x00C0BE20, then
//   MSVCR71 _strupr through the import 0x0144002C; landed under a propagated list-destructor label), CItemDispatcher::Find
//   0x00C101D0, CString::GetHash 0x00C0DBB0, CLoadLog::AddMessage 0x00C307E0, the CString << operators, CString(const char*)
//   0x00C0BFE0, CEventTriggerRef 0x00C1EEE0 / GetDriverCount 0x00C1EE30 / InsertDriver 0x00C1F4E0, ~CString 0x00C0BA40.
// NF53: every class is the shared libpk one: libpk/CBankProcessor.h (FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23: CBankProcessor with
//   m_drivers CIndexMap_00c3abd0 (Get 0x00C3ABD0), m_pCriteriaTriggers CNamedSet (Find 0x00C101D0), the CBankRecordList<T> slots 13
//   GetItemByIndex / 15 GetCount / 17 FindByKey, CEventTriggerRef 0x00C1EEE0 / GetDriverCount 0x00C1EE30 / InsertDriver 0x00C1F4E0),
//   libpk/CScriptHost.h CLoadLog::AddMessage 0x00C307E0 and engine/CString.h (BANK_TRIGGER / MAKEUPPER members). Was local stand-ins.
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#include "libpk/CBankProcessor.h"

void CBankProcessor::AddSampleNameTriggers()
{
    if (!m_pCriteriaTriggers || !m_bSampleNameTriggers)
        return;
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        SDriverRecord* record = m_pData->m_drivers.GetItemByIndex(i);
        void* driver = m_drivers.Get(record->m_number);
        if (!driver) {
            m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
        }
        else if (record->m_number == record->m_sample) {
            SSampleHeader* sample = m_pData->m_samples.FindByKey(record->m_sample);
            CString name = sample->m_fileName.GetFileNameWithoutExtension();
            name.MakeUpper();
            if (m_pCriteriaTriggers->Find(name, 0)) {
                m_pLog->AddMessage() << "Duplicate event trigger for string '" << name << "'. Hash code = " << name.GetHash();
            }
            else {
                CEventTriggerRef trigger(m_pCriteriaTriggers, (const char*)name, m_bNamedTriggers);
                trigger.InsertDriver(trigger.GetDriverCount(), driver);
            }
        }
    }
}