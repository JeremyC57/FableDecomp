// CBankProcessor::Load() -- runs the bank processor's steps in order (bank data preparation, samples, drivers, number triggers, driver groups, event triggers, events, sub-events), checking the load log before each step and stopping at the first failure
// Leaf: ?Load@CBankProcessor@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2EB00, 158 bytes, __fastcall `ret` (the last status test is a tail jump).
// Naming (sweep1, 2026-10-25): was landed as CChildListLoader::Load (chunk 437's descriptive class, which followed
//   the then-landed CChildListLoader::AddChildren 0x00C2E270); descriptive (no PDB covers libpk; the catalog's
//   `NScript_ProcessDeletedTrackNode` is propagated). Chunk 436 identified the owner: the bank loader 0x00C16CE0 runs
//   it (0x00C16D5A) on the stack CBankProcessor that 0x00C2E0C0 built and 0x00C2E140 destroys, and its steps are the
//   landed CBankProcessor::AddSamples 0x00C2E270, AddDrivers 0x00C2E480, AddNumberTriggers 0x00C2E4D0,
//   ProcessAllDrivers 0x00C2EAB0 (-> CBankProcessor::AddDriverToGroup 0x00C2E5A0), 0x00C2E610, AddEvents 0x00C2EA30
//   and AddSubEvents 0x00C2EA70.
// Callees: CLoadLog::IsOk @ 0x00C307C0 (`mov al,[ecx+30h]`), the bank data's Prepare @ 0x00C2ED50 (slot 5 on each of
//   its three record lists), the steps above.
// Shape: a void member: every `if (!m_pLog->IsOk()) return;` shares one `pop esi ; ret` exit, and the check
//   after the last step (nothing follows it) collapses to the tail jump into IsOk. A bool version returning false
//   adds `xor al,al` to the shared exit.
struct CLoadLog {
    unsigned char _pad[0x30];
    bool m_ok;                                     // +0x30
    bool IsOk() const;                             // 0x00C307C0
};

struct CBankData {
    bool Prepare();                                // 0x00C2ED50
};

struct CBankProcessor {
    CBankData* m_pData;                        // +0x00
    void*        m_pManager;                        // +0x04
    CLoadLog* m_pLog;                        // +0x08
    void AddSamples();                             // 0x00C2E270
    void AddDrivers();                             // 0x00C2E480
    void AddNumberTriggers();                      // 0x00C2E4D0
    void ProcessAllDrivers();                      // 0x00C2EAB0
    void ProcessEventTriggers();                   // 0x00C2E610
    void AddEvents();                              // 0x00C2EA30
    void AddSubEvents();                           // 0x00C2EA70
    void Load();
};

void CBankProcessor::Load()
{
    if (!m_pLog->IsOk())
        return;
    m_pData->Prepare();
    AddSamples();
    if (!m_pLog->IsOk())
        return;
    AddDrivers();
    if (!m_pLog->IsOk())
        return;
    AddNumberTriggers();
    if (!m_pLog->IsOk())
        return;
    ProcessAllDrivers();
    if (!m_pLog->IsOk())
        return;
    ProcessEventTriggers();
    if (!m_pLog->IsOk())
        return;
    AddEvents();
    if (!m_pLog->IsOk())
        return;
    AddSubEvents();
    if (!m_pLog->IsOk())
        return;
}