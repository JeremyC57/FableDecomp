// CSteeringGroup::CSteeringGroup(CSteeringSaveRecord* record, CSteeringWorldEntry* entry) -- steering library: build a steering group from a saved record: the two owned containers, the record's limits, the world/callback/simulation settings, then the agent store (record-sized or default), the four helper objects and the solver
// Flags: /EHsc
// Leaf: ??0CSteeringGroup
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
struct CSteeringGroup;

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
struct CSteeringHelper34 { CSteeringHelper34(CSteeringGroup* g); char Pad[0x3c8]; };   // 0x00C0B200
struct CSteeringSolver {
    CSteeringSolver(CSteeringGroup* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry);   // 0x00C11D00
    char Pad[0x890];
};
struct CSteeringHelper68 { CSteeringHelper68(CSteeringGroup* g); char Pad[8]; };     // 0x00C19E40
struct CSteeringHelper6C { CSteeringHelper6C(CSteeringGroup* g); char Pad[0xc]; };   // 0x00C165A0

void __fastcall SetSteeringWorld(void* world);                 // 0x00C1BEB0
void __fastcall SetSteeringOption(int option, bool on);        // 0x00C1BE20
void __fastcall SetSteeringMode(int mode, int value, int on);  // 0x00C1BE00
void __stdcall SetSteeringTimeStep(float step);                // 0x00C1BD80
void __stdcall SetSteeringRange(int lo, int hi);               // 0x00C1B9A0
void SteeringNullCallback();                                   // 0x00C089E0
extern char g_SteeringTimer[];                                 // 0x0143BB14
extern void (*g_SteeringCallback)();                           // 0x0143BB2C
extern bool g_SteeringActive;                                  // 0x0143BAF8

struct CSteeringGroupBase {
    virtual ~CSteeringGroupBase() {}
};
struct CSteeringGroup : CSteeringGroupBase {
    CSteeringTree       Tree;           // +0x04
    CSteeringHelper34*  Helper34;       // +0x34
    CSteeringSolver*    Solver;         // +0x38
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
    CSteeringGroup(CSteeringSaveRecord* record, CSteeringWorldEntry* entry);
    virtual ~CSteeringGroup();
};

CSteeringGroup::CSteeringGroup(CSteeringSaveRecord* record, CSteeringWorldEntry* entry)
    : Helper34(0), Solver(0), Agents(0), P60(0), P64(0), Helper68(0), Helper6C(0),
      LimitA(record->LimitA), LimitB(record->LimitB), P78(0), P7C(0), Capacity(0x1000)
{
    SetSteeringWorld(record->World);
    CScopedSteeringTimer timer(g_SteeringTimer);
    g_SteeringCallback = SteeringNullCallback;
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
    Helper34 = new CSteeringHelper34(this);
    Solver = new CSteeringSolver(this, record->A4, record->A0, record->A8, 100.0f, 100.0f, entry);
    Helper68 = new CSteeringHelper68(this);
    Helper6C = new CSteeringHelper6C(this);
}
