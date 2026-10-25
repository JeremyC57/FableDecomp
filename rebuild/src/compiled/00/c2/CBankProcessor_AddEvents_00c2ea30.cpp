// CBankProcessor::AddEvents() -- for every event record of the bank data (the list at +0x20), AddEvent(record)
// Leaf: AddEvents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2EA30, length 64 bytes (58 + int3 padding), __fastcall, plain `ret`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EB7B. Callees: the event list's GetCount (slot 15) / GetItem (slot 13), AddEvent @ 0x00C2E8D0.
// Name: the catalog's `ProcessAllAnimationEvents` is a guess with no anchor; descriptive.
// Shape: the AddDrivers loop without the log test; the list is embedded at +0x20 of the data object.
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

struct CBankProcessor {
    CBankData* m_pData;                      // +0x00
    void*      m_pManager;                   // +0x04
    void*      m_pLog;                       // +0x08
    void AddEvent(const CEventRecord& record);     // 0x00C2E8D0
    void AddEvents();
};

void CBankProcessor::AddEvents()
{
    for (unsigned long i = 0; i < m_pData->m_events.GetCount(); ++i)
        AddEvent(*m_pData->m_events.GetItem(i));
}