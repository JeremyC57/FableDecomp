// CAudioOwner::CAudioOwner(CSteeringSaveRecord* record, CSteeringWorldEntry* entry) -- libpk audio owner: build the owner from its setup record: the two owned containers, the record's limits, the library globals (lock 0x0143BB14, the assert hook 0x0143BB2C set to the empty 0x00C089E0, the active flag 0x0143BAF8) and settings, then the agent store (record-sized or default), the parameter block (0x00C0B200), the sound-event manager (0x00C11D00) and two more helpers
// Flags: /EHsc
// Leaf: ??0CAudioOwner@@QAE@PAUCSteeringSaveRecord@@PAUCSteeringWorldEntry@@@Z
// Naming (sweep1, 2026-10-25): was landed as CSteeringGroup::CSteeringGroup (manifest "CSteeringGroup::Initialize");
//   descriptive (no PDB covers this library). The object is chunk 431's audio owner (CAudioOwner), not a steering
//   group: it stores vtable 0x012B1E24, which only this constructor and ~CAudioOwner 0x00C09380 store (that destructor
//   deletes the +0x34 and +0x38 objects built here); +0x34 is new CAudioParamBlock(this) (0x00C0B200, 0x3c8 bytes,
//   vtable 0x012B1E84) and +0x38 the CSoundEventManager (0x00C11D00 stores vtable 0x012B2748, the vtable ~CSoundEventManager
//   0x00C13470 restores); 0x0143BB14 is the library lock every libpk CScopedLock takes and 0x0143BB2C the library
//   assert hook (the landed CString rows' g_pfnAssertHook, `__fastcall`). The libpk CSteeringGroup of chunk 436 is a
//   different class (vtable 0x012B40A4). The record, world-entry, container, agent and setting names below are the
//   earlier landing's descriptive names and are unverified.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C094A0, 573 bytes, `ret 8`, EH frame (handler 0x01210C1E). Catalog name
//   "CSteeringGroup::Initialize" is the constructor (the landed CWorldManager::InitializeSteeringAfterSave 0x00C096E0 news
//   0x84 bytes and calls it).
// Callees: tree node ctor 0x00C194F0, container ctor 0x00C0A6D0, world setter 0x00C1BEB0 (__fastcall), scoped timer
//   0x00C0FFC0 / ~ 0x00C0FFE0 on 0x0143BB14, settings 0x00C1BE20 / 0x00C1BE00 (__fastcall) / 0x00C1BD80 / 0x00C1B9A0
//   (__stdcall), operator new 0x00BFEA1A, the helper ctors 0x00C1B950, 0x00C1AC40, 0x00C0B200, 0x00C11D00, 0x00C19E40,
//   0x00C165A0. Globals: callback 0x0143BB2C (set to the empty 0x00C089E0), flag 0x0143BAF8.
// Layout: library code with no RTTI or PDB type: descriptive model sized from the 0x84-byte allocation; vtables
//   0x012B1E24 (group) and 0x012B1C98 (the +4 container; its base stores the data pointer first, as the out-of-line
//   sibling container ctor 0x00C0A6D0 does).
struct CSteeringSaveRecord {
    float  A0;              // +0x00
    float  A4;              // +0x04
    float  A8;              // +0x08
    int    LimitB;          // +0x0c
    int    LimitA;          // +0x10
    int    AgentCount;      // +0x14
    float  TimeStep;        // +0x18
    int    RangeMin;        // +0x1c
    int    RangeMax;        // +0x20
    void*  WorldKey;        // +0x24
    void*  World;           // +0x28
};
struct CSteeringWorldEntry;
struct CAudioOwner;

struct CTreeHeader {
    CTreeHeader(int unique);        // 0x00C194F0
    ~CTreeHeader();
    char Pad[0x28];
};
struct CSteeringContainerBase {
    CSteeringContainerBase(void* data) : Data(data) {}
    virtual ~CSteeringContainerBase() {}
    void* Data;
};
struct CSteeringTree : CSteeringContainerBase {
    CSteeringTree() : CSteeringContainerBase(&Header), Header(0) {}
    virtual ~CSteeringTree();
    CTreeHeader Header;
};
struct CSteeringList {
    CSteeringList();                // 0x00C0A6D0
    ~CSteeringList();
    char Pad[0x20];
};
struct CScopedSteeringTimer {
    CScopedSteeringTimer(void* timer);   // 0x00C0FFC0
    ~CScopedSteeringTimer();             // 0x00C0FFE0
    void* Timer;
    unsigned long Start;
};
struct CSBAgentsBase {};
struct CSBAgents : CSBAgentsBase {
    CSBAgents(int count);               // 0x00C1B950
    char Pad[8];
};
struct CSBDefaultAgents : CSBAgentsBase {
    CSBDefaultAgents(int n);            // 0x00C1AC40
    char Pad[0x88];
};
struct CAudioParamBlock { CAudioParamBlock(CAudioOwner* g); char Pad[0x3c8]; };   // 0x00C0B200
struct CSoundEventManager {
    CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry);   // 0x00C11D00
    char Pad[0x890];
};
struct CSteeringHelper68 { CSteeringHelper68(CAudioOwner* g); char Pad[8]; };     // 0x00C19E40
struct CSteeringHelper6C { CSteeringHelper6C(CAudioOwner* g); char Pad[0xc]; };   // 0x00C165A0

void __fastcall SetSteeringWorld(void* world);                 // 0x00C1BEB0
void __fastcall SetSteeringOption(int option, bool on);        // 0x00C1BE20
void __fastcall SetSteeringMode(int mode, int value, int on);  // 0x00C1BE00
void __stdcall SetSteeringTimeStep(float step);                // 0x00C1BD80
void __stdcall SetSteeringRange(int lo, int hi);               // 0x00C1B9A0
void __fastcall NullAssertHook(const char* message);        // 0x00C089E0 (empty: `ret`)
extern char g_lock_0143bb14[];                                 // 0x0143BB14
extern void (__fastcall* g_pfnAssertHook)(const char* message);   // 0x0143BB2C
extern bool g_SteeringActive;                                  // 0x0143BAF8

struct CAudioOwnerBase {
    virtual ~CAudioOwnerBase() {}
};
struct CAudioOwner : CAudioOwnerBase {
    CSteeringTree       Tree;           // +0x04
    CAudioParamBlock*  Params;       // +0x34
    CSoundEventManager*    EventManager;         // +0x38
    CSteeringList       List;           // +0x3c
    CSBAgentsBase*      Agents;         // +0x5c
    void*               P60;            // +0x60
    void*               P64;            // +0x64
    CSteeringHelper68*  Helper68;       // +0x68
    CSteeringHelper6C*  Helper6C;       // +0x6c
    int                 LimitA;         // +0x70
    int                 LimitB;         // +0x74
    int                 P78;            // +0x78
    int                 P7C;            // +0x7c
    int                 Capacity;       // +0x80
    CAudioOwner(CSteeringSaveRecord* record, CSteeringWorldEntry* entry);
    virtual ~CAudioOwner();
};

CAudioOwner::CAudioOwner(CSteeringSaveRecord* record, CSteeringWorldEntry* entry)
    : Params(0), EventManager(0), Agents(0), P60(0), P64(0), Helper68(0), Helper6C(0),
      LimitA(record->LimitA), LimitB(record->LimitB), P78(0), P7C(0), Capacity(0x1000)
{
    SetSteeringWorld(record->World);
    CScopedSteeringTimer timer(g_lock_0143bb14);
    g_pfnAssertHook = NullAssertHook;
    SetSteeringOption(0, true);
    SetSteeringOption(1, true);
    SetSteeringMode(2, 8, 1);
    g_SteeringActive = true;
    SetSteeringTimeStep(record->TimeStep);
    SetSteeringRange(record->RangeMin, record->RangeMax);
    if (record->AgentCount)
        Agents = new CSBAgents(record->AgentCount);
    else
        Agents = new CSBDefaultAgents(1);
    Params = new CAudioParamBlock(this);
    EventManager = new CSoundEventManager(this, record->A4, record->A0, record->A8, 100.0f, 100.0f, entry);
    Helper68 = new CSteeringHelper68(this);
    Helper6C = new CSteeringHelper6C(this);
}