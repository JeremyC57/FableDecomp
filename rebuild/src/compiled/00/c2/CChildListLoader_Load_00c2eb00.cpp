// CChildListLoader::Load() -- runs the loader's seven steps in order (source preparation, children, two chunk passes, drivers, event triggers, events, event mappings), checking the shared load status before each step and stopping at the first failure
// Leaf: ?Load@CChildListLoader
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2EB00, 158 bytes, __fastcall `ret` (the last status test is a tail jump).
//   Descriptive name (no PDB covers this library): the catalog's `NScript_ProcessDeletedTrackNode` is propagated.
//   Caller: the libpk bank loader 0x00C16D5A (a stack loader {source, target, status}); the class name follows the
//   landed CChildListLoader::AddChildren 0x00C2E270 (step 1).
// Callees: CLoadStatus::IsOk @ 0x00C307C0 (`mov al,[ecx+30h]`), the source's Prepare @ 0x00C2ED50 (landed as
//   "CIsThingAlive::operator()": slot 5 on each of the source's three lists), the steps AddChildren 0x00C2E270,
//   0x00C2E480, 0x00C2E4D0, ProcessAllDrivers 0x00C2EAB0, 0x00C2E610, 0x00C2EA30, 0x00C2EA70.
// Shape: a void member: every `if (!m_pStatus->IsOk()) return;` shares one `pop esi ; ret` exit, and the check
//   after the last step (nothing follows it) collapses to the tail jump into IsOk. A bool version returning false
//   adds `xor al,al` to the shared exit.
struct CLoadStatus {
    unsigned char _pad[0x30];
    bool m_ok;                                     // +0x30
    bool IsOk() const;                             // 0x00C307C0
};

struct CLoadSource {
    void Prepare();                                // 0x00C2ED50
};

struct CChildListLoader {
    CLoadSource* m_pSource;                        // +0x00
    void*        m_pTarget;                        // +0x04
    CLoadStatus* m_pStatus;                        // +0x08
    void AddChildren();                            // 0x00C2E270
    void ProcessChunks();                          // 0x00C2E480
    void ProcessChunkDrivers();                    // 0x00C2E4D0
    void ProcessAllDrivers();                      // 0x00C2EAB0
    void ProcessEventTriggers();                   // 0x00C2E610
    void ProcessAllEvents();                       // 0x00C2EA30
    void ProcessAllEventMappings();                // 0x00C2EA70
    void Load();
};

void CChildListLoader::Load()
{
    if (!m_pStatus->IsOk())
        return;
    m_pSource->Prepare();
    AddChildren();
    if (!m_pStatus->IsOk())
        return;
    ProcessChunks();
    if (!m_pStatus->IsOk())
        return;
    ProcessChunkDrivers();
    if (!m_pStatus->IsOk())
        return;
    ProcessAllDrivers();
    if (!m_pStatus->IsOk())
        return;
    ProcessEventTriggers();
    if (!m_pStatus->IsOk())
        return;
    ProcessAllEvents();
    if (!m_pStatus->IsOk())
        return;
    ProcessAllEventMappings();
    if (!m_pStatus->IsOk())
        return;
}