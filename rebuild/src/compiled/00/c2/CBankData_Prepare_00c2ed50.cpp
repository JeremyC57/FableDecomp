// CBankData::Prepare() -- prepares the bank data's three record lists (virtual slot 5 on the sample list at +0x00, the driver list at +0x10 and the event list at +0x20) and returns the last one's result
// Leaf: ?Prepare@CBankData@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2ED50, 27 bytes: two discarded slot-5 calls, then a tail jump into
//   the third.
// Naming (sweep1, 2026-10-25): was landed as `NScript::CIsThingAlive::operator()` (a propagated manifest label; the
//   source called it `operatorCall`); descriptive (no PDB covers libpk). Its only caller is CBankProcessor::Load
//   0x00C2EB00 (`m_pData->Prepare()` before the first step, on the bank data at +0 of the processor), and the three
//   lists are the ones the processor's steps walk: samples at +0x00 (AddSamples 0x00C2E270), drivers at +0x10
//   (AddDrivers 0x00C2E480, ProcessAllDrivers 0x00C2EAB0) and events at +0x20 (AddEvents 0x00C2EA30, AddSubEvents
//   0x00C2EA70). The slot-5 operation itself is not identified: the list classes are named for their records.
struct CSampleList {
    virtual int v0(); virtual int v1(); virtual int v2(); virtual int v3(); virtual int v4();
    virtual bool Prepare();                     // slot 5 (+0x14)
};

struct CDriverList {
    virtual int v0(); virtual int v1(); virtual int v2(); virtual int v3(); virtual int v4();
    virtual bool Prepare();                     // slot 5 (+0x14)
};

struct CEventList {
    virtual int v0(); virtual int v1(); virtual int v2(); virtual int v3(); virtual int v4();
    virtual bool Prepare();                     // slot 5 (+0x14)
};

struct CBankData {
    CSampleList m_samples;                      // +0x00
    char pad[12];                               // fill the sample list up to +0x10
    CDriverList m_drivers;                      // +0x10
    char padB[12];                              // fill the driver list up to +0x20
    CEventList m_events;                        // +0x20
    bool Prepare();
};

bool CBankData::Prepare()
{
    m_samples.Prepare();
    m_drivers.Prepare();
    return m_events.Prepare();
}
