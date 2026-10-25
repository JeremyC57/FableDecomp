// CBankProcessor::AddDriverToGroup(unsigned long number, unsigned long group) -- when the record names a group, add the driver with that number to the manager's group (created on first use); logs "Could not find driver number <n>" for an unknown driver
// Leaf: AddDriverToGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E5A0, length 112 bytes (106 + int3 padding), __fastcall, `ret 8`.
// Owner: the libpk bank processor (constructed at 0x00C2E0C0, run step by step by 0x00C2EB00, which tests the log's
//   IsOk @ 0x00C307C0 after each step): data +0x00, sound manager +0x04, log +0x08, driver map +0x0C, sample map +0x3C,
//   criteria-trigger registry +0x6C, number-trigger registry +0x70, option bytes +0x74..+0x7A. No PDB covers libpk: names are descriptive.
// Caller: 0x00C2EADA (the step 0x00C2EAB0: one call per driver record, passing the record's number (+0x00) and its
//   zero-extended group word (+0x0C)).
// Callees: the driver map's Find(number) @ 0x00C3ABD0 (slot 17 by key address, the stored driver or 0), the log's AddMessage
//   @ 0x00C307E0, the stream's `__fastcall operator<<` (text @ 0x00C0DEC0, unsigned @ 0x00C0DF10), the manager's FindGroup
//   @ 0x00C0E950 (landed as "CIsThingAlive::operator()": its +0x74 map's Find) and CreateGroup @ 0x00C0E8E0 (landed as
//   "CFactory_00c0e8e0::Create": `new` of a 0x20-byte group, ctor 0x00C1E6D0), the group's AddDriver @ 0x00C1E780.
// Name: the catalog's `EventTrigger_ProcessSingleDriver` is a guess; descriptive.
// Shape: the group test comes before ebx/ebp are saved (shrink-wrapped early return); the number lives in ebp for the
//   lookup and the message; the found-or-created group is one local, so both calls feed the single AddDriver call.
struct CDebugStream {                        // libpk message text
    char* m_pBuffer;
    CDebugStream& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CLoadLog {
    CDebugStream& AddMessage();              // 0x00C307E0
};

struct CDriver;

struct CDriverGroup {
    void AddDriver(CDriver* driver);         // 0x00C1E780
};

struct CSoundManager {
    CDriverGroup* FindGroup(unsigned long group);     // 0x00C0E950
    CDriverGroup* CreateGroup(unsigned long group);   // 0x00C0E8E0
};

struct CDriverMap {                          // number -> driver (0x30 bytes)
    CDriver* Find(unsigned long number);     // 0x00C3ABD0
    unsigned char _body[0x30];
};

struct CBankProcessor {
    void*          m_pData;                  // +0x00
    CSoundManager* m_pManager;               // +0x04
    CLoadLog*      m_pLog;                   // +0x08
    CDriverMap     m_drivers;                // +0x0C
    void AddDriverToGroup(unsigned long number, unsigned long group);
};

void CBankProcessor::AddDriverToGroup(unsigned long number, unsigned long group)
{
    if (!group)
        return;
    CDriver* driver = m_drivers.Find(number);
    if (!driver) {
        m_pLog->AddMessage() << "Could not find driver number " << number;
        return;
    }
    CDriverGroup* driverGroup = m_pManager->FindGroup(group);
    if (!driverGroup)
        driverGroup = m_pManager->CreateGroup(group);
    driverGroup->AddDriver(driver);
}