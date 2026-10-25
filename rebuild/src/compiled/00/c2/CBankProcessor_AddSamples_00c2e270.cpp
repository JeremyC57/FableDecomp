// CBankProcessor::AddSamples() -- for every sample record of the bank data (the list at +0x00), AddSample(record); stops as soon as the load log reports a failure
// Leaf: ?AddSamples@CBankProcessor@@QAEXXZ
// Retail: Fable.exe @ 0x00C2E270, 60 bytes (+ int3 padding), __thiscall, plain `ret`.
// Naming (sweep1, 2026-10-25): was landed as CChildListLoader::AddChildren (descriptive, owner unknown at the time;
//   the manifest's "CTextContainer::AddChild" is propagated). Descriptive (no PDB covers libpk). Chunk 436 identified
//   the owner: its only caller is the bank processor's step sequence 0x00C2EB00 (step 1), whose other steps are the
//   landed CBankProcessor::AddDrivers 0x00C2E480 / AddNumberTriggers 0x00C2E4D0 / AddEvents 0x00C2EA30 /
//   AddSubEvents 0x00C2EA70 with the same loop over the lists at +0x10 / +0x20 of the bank data; its per-item callee
//   0x00C2E190 is the landed CBankProcessor::AddSample (sample record -> CAudioFormat -> CSoundManager::CreateSample ->
//   the sample map). Agent436 (chunk 436) model.
// Callees: the sample list's GetItem(index) (vtable slot 13, +0x34) and GetCount() (slot 15, +0x3c, re-called every
//   pass), CBankProcessor::AddSample(record) @ 0x00C2E190, CLoadLog::IsOk() @ 0x00C307C0 (`mov al,[ecx+30h] ; ret`).
// Shape: a `for` loop over an unsigned index against a re-evaluated virtual count (VC rotates it: the count is tested
//   once before the loop and again at the bottom); the early exit is a `return` on the byte flag. The sample list sits
//   at +0 of the bank data, so no `lea` precedes the virtual calls (AddDrivers' list at +0x10 adds one).
struct SSampleHeader;
struct SDriverRecord;
struct CEventRecord;

template <class T>
struct CRecordList {                            // the bank data's polymorphic record lists (0x10 bytes)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual T* GetItem(unsigned long index);    // slot 13 (+0x34)
    virtual void v14();
    virtual unsigned long GetCount();           // slot 15 (+0x3c)
    unsigned char _body[0x0c];
};

struct CBankData {
    CRecordList<SSampleHeader> m_samples;       // +0x00
    CRecordList<SDriverRecord> m_drivers;       // +0x10
    CRecordList<CEventRecord>  m_events;        // +0x20
};

struct CLoadLog {
    bool IsOk() const;                          // 0x00C307C0
};

struct CBankProcessor {
    CBankData* m_pData;                         // +0x00
    void*      m_pManager;                      // +0x04
    CLoadLog*  m_pLog;                          // +0x08
    void AddSample(const SSampleHeader* header);   // 0x00C2E190
    void AddSamples();
};

void CBankProcessor::AddSamples()
{
    for (unsigned long i = 0; i < m_pData->m_samples.GetCount(); ++i) {
        AddSample(m_pData->m_samples.GetItem(i));
        if (!m_pLog->IsOk())
            return;
    }
}
