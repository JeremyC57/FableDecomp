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
class CString {                              // libpk's MFC-style string (one pointer, 12-byte header in front)
public:
    char* m_pchData;
    CString(const char* lpsz);               // 0x00C0BFE0
    ~CString();                              // 0x00C0BA40
    unsigned long GetHash() const;           // 0x00C0DBB0
    CString GetFileNameWithoutExtension() const;   // 0x00C0E360
    void MakeUpper();                        // 0x00C0C910
    operator const char*() const { return m_pchData; }
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(const CString& text);    // 0x00C0DEA0
    CString& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CLoadLog {
    CString& AddMessage();                   // 0x00C307E0 ?AddMessage@CLoadLog@@QAEAAVCString@@XZ
};

struct SSampleHeader {
    unsigned long m_number;                  // +0x00
    CString       m_fileName;                // +0x04
};
struct CEventRecord;
struct SDriverRecord {
    unsigned long m_number;                  // +0x00
    unsigned long m_sample;                  // +0x04 the sample the driver plays
};

template <class T>
struct CRecordList {                         // the bank data's polymorphic record lists (0x10 bytes)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual T* GetItem(unsigned long index);   // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();          // slot 15 (+0x3c)
    virtual void v16();
    virtual T* Find(const unsigned long& key); // slot 17 (+0x44)
    unsigned char _body[0xc];
};

struct CBankData {
    CRecordList<SSampleHeader> m_samples;    // +0x00
    CRecordList<SDriverRecord> m_drivers;    // +0x10
    CRecordList<CEventRecord>  m_events;     // +0x20
};

struct CItem;
struct CDriver;

struct CItemDispatcher {                     // the criteria-string trigger registry
    CItem* Find(const CString& name, long* pCollisions);   // 0x00C101D0
};

struct CEventTriggerRef {                    // 8 bytes: the registry and the trigger it found or created
    CEventTriggerRef(CItemDispatcher* registry, CString name, bool named);   // 0x00C1EEE0
    unsigned long GetDriverCount();          // 0x00C1EE30
    void InsertDriver(unsigned long position, CDriver* driver);   // 0x00C1F4E0
    void*  m_pRegistry;                      // +0x00
    CItem* m_pTrigger;                       // +0x04
};

struct CDriverMap {                          // number -> driver (0x30 bytes)
    CDriver* Find(unsigned long number);     // 0x00C3ABD0
    unsigned char _body[0x30];
};

struct CBankProcessor {
    CBankData*       m_pData;                // +0x00
    void*            m_pManager;             // +0x04
    CLoadLog*        m_pLog;                 // +0x08
    CDriverMap       m_drivers;              // +0x0C
    unsigned char    _pad_0x3c[0x30];        // +0x3C the sample map
    CItemDispatcher* m_pCriteriaTriggers;    // +0x6C
    void*            m_pNumberTriggers;      // +0x70
    unsigned char    _pad_0x74[5];           // +0x74 option bytes
    bool             m_bSampleNameTriggers;  // +0x79
    bool             m_bNamedTriggers;       // +0x7A
    void AddSampleNameTriggers();
};

void CBankProcessor::AddSampleNameTriggers()
{
    if (!m_pCriteriaTriggers || !m_bSampleNameTriggers)
        return;
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        SDriverRecord* record = m_pData->m_drivers.GetItem(i);
        CDriver* driver = m_drivers.Find(record->m_number);
        if (!driver) {
            m_pLog->AddMessage() << "Could not find driver number " << record->m_number;
        }
        else if (record->m_number == record->m_sample) {
            SSampleHeader* sample = m_pData->m_samples.Find(record->m_sample);
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