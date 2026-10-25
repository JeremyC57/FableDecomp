// CBankProcessor::AddDrivers() -- for every driver record of the bank data (the list at +0x10), AddDriver(record); stops as soon as the log reports a failure
// Leaf: AddDrivers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E480, length 80 bytes (70 + int3 padding), __fastcall, plain `ret`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EB2F. Callees: the driver list's GetCount (slot 15, +0x3c) / GetItem (slot 13, +0x34), AddDriver @ 0x00C2E2B0
//   (landed as "ResourceProcessor_00c2e2b0::Process": looks the record's sample up, builds the driver, adds it to the driver
//   map), CLoadLog::IsOk @ 0x00C307C0 (`mov al,[ecx+30h]`).
// Name: the catalog's `ProcessResourceChunksUntilQuickAssignFails` is propagated (the IsOk callee is labelled
//   CTCInventoryBase::GetCanItemBeQuickAssigned).
// Shape: the landed 0x00C2E270 loop (an unsigned index against the re-called virtual count, rotated by VC) over the list
//   embedded at +0x10 of the data object, so each call recomputes `lea ecx,[eax+10h]`; the failure exit is a `return`.
struct SSampleHeader;
struct SDriverRecord;
struct CEventRecord;

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

struct CLoadLog {
    bool IsOk() const;                       // 0x00C307C0
};

struct CBankProcessor {
    CBankData* m_pData;                      // +0x00
    void*      m_pManager;                   // +0x04
    CLoadLog*  m_pLog;                       // +0x08
    void AddDriver(const SDriverRecord* record);   // 0x00C2E2B0
    void AddDrivers();
};

void CBankProcessor::AddDrivers()
{
    for (unsigned long i = 0; i < m_pData->m_drivers.GetCount(); ++i) {
        AddDriver(m_pData->m_drivers.GetItem(i));
        if (!m_pLog->IsOk())
            return;
    }
}