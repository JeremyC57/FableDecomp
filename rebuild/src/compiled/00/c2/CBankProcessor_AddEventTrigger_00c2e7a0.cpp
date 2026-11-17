// CBankProcessor::AddEventTrigger(const CEventRecord& record, bool subEvent) -- register the bank event's criteria-string trigger: when a criteria-trigger registry is attached (+0x6C), log a duplicate criteria string (unless this is a sub-event), else bind a trigger reference to the record's name and insert every driver the record numbers at the trigger's current driver count, logging driver numbers the driver map does not know
// Leaf: ?AddEventTrigger@CBankProcessor@@QAEXABUCEventRecord@@_N@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2E7A0, 297 bytes, thiscall `ret 8`; built under /EHsc like its TU siblings AddEvent / AddSubEvent,
//   although it needs no EH frame: the only trace is the by-value name temporary's address saved in the dead subEvent slot
//   (`mov [esp+28h],esp`), which plain /O2 omits (286v297).
// Identity (descriptive: libpk has no PDB or RTTI): the landed CBankProcessor::AddEvent 0x00C2E8D0 and AddSubEvent
//   0x00C2E960 call this row as `AddEventTrigger(record, false / true)` and already bind exactly this spelling
//   (reloc_bindings.tsv); its strings are the bank processor's ("Duplicate event trigger for criteria string '", "', hash
//   code ", "Could not find driver ", " attach to criteria string '", "'"); the number-trigger twin is the landed
//   AddNumberTriggers 0x00C2E4D0, whose callee spellings this row reuses. The manifest label
//   "Thiscall_AnimationEventTriggerProcess" is a guess.
// Callees: CNamedSet::Find(const CString&, long*) 0x00C101D0, CString::GetHash
//   0x00C0DBB0, CLoadLog::AddMessage 0x00C307E0, CString::operator<< 0x00C0DEC0 (text) / 0x00C0DF10 (unsigned long) /
//   0x00C0DEA0 (CString), CString(const char*) 0x00C0BFE0 (the by-value name built in its argument slot),
//   CEventTriggerRef::CEventTriggerRef(CNamedSet*, CString, bool) 0x00C1EEE0 (the criteria-string overload of the
//   number constructor 0x00C1EE60), CEventTriggerRef::GetDriverCount
//   0x00C1EE30, PKCAutoDeleteArray<unsigned long>::operator[] 0x00C2EBC0 (the range-checked accessor whose own trace
//   names PKCAutoDeleteArray.h; landed under a placeholder, NAME_FIXES.tsv), the driver map's Get 0x00C3ABD0 (libpk/CBankProcessor.h CIndexMap_00c3abd0, the header's placeholder class),
//   CEventTriggerRef::InsertDriver 0x00C1F4E0.
// Layout: CBankProcessor as the landed siblings (log +0x08, driver map +0x0C, criteria-trigger registry +0x6C, option byte
//   +0x7A = the trigger reference's `named` flag); CEventRecord = {CString name, PKCAutoDeleteArray<unsigned long> drivers}
//   (the landed siblings flatten the array into +4 data / +8 count).
// Shape: the trigger reference is a function-scope 8-byte local with a trivial destructor; the insert position is read
//   once before the driver loop (VC7.1 homes it in the dead subEvent slot); the duplicate hash is computed before the log
//   call (right-to-left evaluation of the outermost <<).
// NF65 (round 7): ported onto libpk/CBankProcessor.h (CBankProcessor: m_pLog +0x08, m_drivers +0x0C, m_pCriteriaTriggers +0x6C,
//   m_bNamedTriggers +0x7A; CNamedSet, CEventTriggerRef, CIndexMap_00c3abd0::Get; overlay opt-in FABLE_RETAIL_CBANKPROCESSOR_ADDEVENTTRIGGER
//   declares this member), libpk/CScriptHost.h (CLoadLog::AddMessage) and engine/CString.h (FABLE_RETAIL_CSTRING_BUILDERS / the bank-trigger
//   members); the local CString / CLoadLog / CItemDispatcher / CEventTriggerRef / CDriverMap / CBankProcessor models are gone. CEventRecord
//   and PKCAutoDeleteArray have no shared header (CBankProcessor.h only forward-declares CEventRecord) and stay local. The trigger ctor now
//   binds ??0CEventTriggerRef@@QAE@PAVCNamedSet@@VCString@@_N@Z (was ??0CEventTriggerRef@@QAE@PAUCItemDispatcher@@VCString@@_N@Z).
#define FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CBANKPROCESSOR_ADDEVENTTRIGGER
#include "libpk/CBankProcessor.h"
#include "libpk/CItem.h"   // the CItem definition (struct): CNamedSet::Find binds ?Find@CNamedSet@@QAEPAUCItem@@ABVCString@@PAJ@Z, as 0x00C1EEE0 does

template <class T>
struct PKCAutoDeleteArray {                  // libpk PKCAutoDeleteArray.h: an owned array and its count
    T*            m_pData;                   // +0x00
    unsigned long m_count;                   // +0x04
    T& operator[](unsigned long index);      // 0x00C2EBC0 (unsigned long): range-checked
};

struct CEventRecord {                        // 12 bytes
    CString                           m_name;      // +0x00 criteria string
    PKCAutoDeleteArray<unsigned long> m_drivers;   // +0x04 driver numbers
};

void CBankProcessor::AddEventTrigger(const CEventRecord& record, bool subEvent)
{
    if (!m_pCriteriaTriggers)
        return;
    if (!subEvent && m_pCriteriaTriggers->Find(record.m_name, 0)) {
        m_pLog->AddMessage() << "Duplicate event trigger for criteria string '" << record.m_name << "', hash code "
                             << record.m_name.GetHash();
        return;
    }
    CEventTriggerRef trigger(m_pCriteriaTriggers, (const char*)record.m_name, m_bNamedTriggers);
    unsigned long position = trigger.GetDriverCount();
    for (unsigned long i = 0; i < record.m_drivers.m_count; ++i) {
        unsigned long number = const_cast<PKCAutoDeleteArray<unsigned long>&>(record.m_drivers)[i];
        void* driver = m_drivers.Get(number);
        if (!driver)
            m_pLog->AddMessage() << "Could not find driver " << number << " attach to criteria string '" << record.m_name << "'";
        else
            trigger.InsertDriver(position, driver);
    }
}