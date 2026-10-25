// CBankProcessor::AddNumberTriggers() -- when a number-trigger registry is attached (+0x70), give every driver record of the bank data an event trigger under its number holding that driver; logs unknown drivers and duplicate trigger numbers
// Leaf: AddNumberTriggers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E4D0, length 208 bytes (204 + int3 padding), __fastcall, plain `ret`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EB42. Callees: the driver list's GetCount / GetItem (slots 15 / 13), the driver map's Find @ 0x00C3ABD0, the
//   registry's Find(number) @ 0x00C10140 (slot 17 by key address; landed label "CDataOutputStream::WriteCBYTE" is propagated),
//   the trigger reference's constructor @ 0x00C1EE60 (registry, number: finds the trigger or `new`s a 16-byte one, ctor
//   0x00C1F720, and registers it), GetDriverCount @ 0x00C1EE30 and InsertDriver(position, driver) @ 0x00C1F4E0 (grows the
//   trigger's driver list and retries when the insert fails), the log's AddMessage @ 0x00C307E0 and the stream operators.
// Name: the catalog's `EventTrigger_ProcessDrivers` is a guess; descriptive (the strings are "Could not find driver number "
//   and "Duplicate event trigger for number ").
// Shape: the 8-byte trigger reference is a loop-local whose address is taken, which makes VC7.1 build the `and esp,-8`
//   aligned frame (ebp then also serves as the loop index); its destructor is trivial. The append is
//   `InsertDriver(GetDriverCount(), driver)`: the driver is pushed first, then the count is computed (right-to-left
//   arguments). The two messages share their `<< number` tail (VC cross-jumps the second into the first).
struct CDebugStream {                        // libpk message text
    char* m_pBuffer;
    CDebugStream& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CLoadLog {
    CDebugStream& AddMessage();              // 0x00C307E0
};

struct SSampleHeader;
struct CEventRecord;
struct SDriverRecord {
    unsigned long m_number;                  // +0x00
};

template <class T>
struct CRecordList {                         // the bank data's polymorphic record lists (0x10 bytes)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual T* GetItem(unsigned long index);   // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();          // slot 15 (+0x3c)
    unsigned char _body[0x0c];
};

struct CBankData {
    CRecordList<SSampleHeader> m_samples;    // +0x00
    CRecordList<SDriverRecord> m_drivers;    // +0x10
    CRecordList<CEventRecord>  m_events;     // +0x20
};

struct CDriver;
struct CEventTrigger;

struct CNumberTriggerRegistry {
    CEventTrigger* Find(unsigned long number);   // 0x00C10140
};

struct CEventTriggerRef {                    // 8 bytes: the registry and the trigger it found or created
    CEventTriggerRef(CNumberTriggerRegistry* registry, unsigned long number);   // 0x00C1EE60
    unsigned long GetDriverCount();          // 0x00C1EE30
    void InsertDriver(unsigned long position, CDriver* driver);   // 0x00C1F4E0
    void*          m_pRegistry;              // +0x00
    CEventTrigger* m_pTrigger;               // +0x04
};

struct CDriverMap {                          // number -> driver (0x30 bytes)
    CDriver* Find(unsigned long number);     // 0x00C3ABD0
    unsigned char _body[0x30];
};

struct CBankProcessor {
    CBankData*              m_pData;         // +0x00
    void*                   m_pManager;      // +0x04
    CLoadLog*               m_pLog;          // +0x08
    CDriverMap              m_drivers;       // +0x0C
    unsigned char           _pad_0x3c[0x30]; // +0x3C the sample map
    void*                   m_pCriteriaTriggers;   // +0x6C
    CNumberTriggerRegistry* m_pNumberTriggers;     // +0x70
    void AddNumberTriggers();
};

void CBankProcessor::AddNumberTriggers()
{
    if (!m_pNumberTriggers)
        return;
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        SDriverRecord* record = m_pData->m_drivers.GetItem(i);
        CDriver* driver = m_drivers.Find(record->m_number);
        if (!driver)
            m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
        else if (m_pNumberTriggers->Find(record->m_number))
            m_pLog->AddMessage() << "Duplicate event trigger for number " << record->m_number;
        else {
            CEventTriggerRef trigger(m_pNumberTriggers, record->m_number);
            trigger.InsertDriver(trigger.GetDriverCount(), driver);
        }
    }
}