#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CAudioOwner ctor: member initialisation from the record, global steering settings, agent store choice and the four helpers bound to the group


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

static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
static int g_order = 0;
static int g_treeUnique = -1, g_listBuilt = 0, g_timerOn = 0, g_timerOff = 0, g_opt[2], g_mode[3], g_range[2];
static void* g_world = 0; static void* g_timerArg = 0; static float g_step = 0;
static int g_agentsCount = -1, g_defaultN = -1;
static CAudioOwner* g_h34 = 0; static CAudioOwner* g_h68 = 0; static CAudioOwner* g_h6c = 0;
static CAudioOwner* g_solverGroup = 0; static float g_solverArgs[5]; static CSteeringWorldEntry* g_solverEntry = 0;
static int g_timerOrder = 0, g_worldOrder = 0, g_solverOrder = 0, g_agentsOrder = 0;
CTreeHeader::CTreeHeader(int unique) { g_treeUnique = unique; }
CTreeHeader::~CTreeHeader() {}
CSteeringTree::~CSteeringTree() {}
CSteeringList::CSteeringList() { ++g_listBuilt; }
CSteeringList::~CSteeringList() {}
CScopedSteeringTimer::CScopedSteeringTimer(void* timer) : Timer(timer), Start(0) { g_timerArg = timer; ++g_timerOn; g_timerOrder = ++g_order; }
CScopedSteeringTimer::~CScopedSteeringTimer() { ++g_timerOff; }
CSBAgents::CSBAgents(int count) { g_agentsCount = count; g_agentsOrder = ++g_order; }
CSBDefaultAgents::CSBDefaultAgents(int n) { g_defaultN = n; g_agentsOrder = ++g_order; }
CAudioParamBlock::CAudioParamBlock(CAudioOwner* g) { g_h34 = g; }
CSoundEventManager::CSoundEventManager(CAudioOwner* g, float a4, float a0, float a8, float w, float h, CSteeringWorldEntry* entry)
{ g_solverGroup = g; g_solverArgs[0] = a4; g_solverArgs[1] = a0; g_solverArgs[2] = a8; g_solverArgs[3] = w; g_solverArgs[4] = h; g_solverEntry = entry; g_solverOrder = ++g_order; }
CSteeringHelper68::CSteeringHelper68(CAudioOwner* g) { g_h68 = g; }
CSteeringHelper6C::CSteeringHelper6C(CAudioOwner* g) { g_h6c = g; }
void __fastcall SetSteeringWorld(void* world) { g_world = world; g_worldOrder = ++g_order; }
void __fastcall SetSteeringOption(int option, bool on) { g_opt[option] = on ? 1 : 0; }
void __fastcall SetSteeringMode(int mode, int value, int on) { g_mode[0] = mode; g_mode[1] = value; g_mode[2] = on; }
void __stdcall SetSteeringTimeStep(float step) { g_step = step; }
void __stdcall SetSteeringRange(int lo, int hi) { g_range[0] = lo; g_range[1] = hi; }
void __fastcall NullAssertHook(const char*) {}
char g_lock_0143bb14[8];
void (__fastcall* g_pfnAssertHook)(const char*) = 0;
bool g_SteeringActive = false;
CAudioOwner::~CAudioOwner() {}
int main()
{
    CSteeringSaveRecord r;
    r.A0 = 1.5f; r.A4 = 2.5f; r.A8 = 3.5f; r.LimitB = 7; r.LimitA = 9; r.AgentCount = 12; r.TimeStep = 0.25f;
    r.RangeMin = -4; r.RangeMax = 44; r.WorldKey = 0; r.World = &r;
    CSteeringWorldEntry* entry = (CSteeringWorldEntry*)&g_order;
    CAudioOwner* g = new CAudioOwner(&r, entry);
    Check(g->Tree.Data == (void*)&g->Tree.Header && g_treeUnique == 0 && g_listBuilt == 1, __LINE__);
    Check(g->LimitA == 9 && g->LimitB == 7 && g->P78 == 0 && g->P7C == 0 && g->Capacity == 0x1000, __LINE__);
    Check(g->P60 == 0 && g->P64 == 0, __LINE__);
    Check(g_world == (void*)&r && g_worldOrder < g_timerOrder && g_timerArg == (void*)g_lock_0143bb14 && g_timerOn == 1 && g_timerOff == 1, __LINE__);
    Check(g_pfnAssertHook == NullAssertHook && g_SteeringActive, __LINE__);
    Check(g_opt[0] == 1 && g_opt[1] == 1 && g_mode[0] == 2 && g_mode[1] == 8 && g_mode[2] == 1, __LINE__);
    Check(g_step == 0.25f && g_range[0] == -4 && g_range[1] == 44, __LINE__);
    Check(g_agentsCount == 12 && g_defaultN == -1 && g->Agents != 0 && g_agentsOrder < g_solverOrder, __LINE__);
    Check(g_h34 == g && g_h68 == g && g_h6c == g && g->Params && g->EventManager && g->Helper68 && g->Helper6C, __LINE__);
    Check(g_solverGroup == g && g_solverArgs[0] == 2.5f && g_solverArgs[1] == 1.5f && g_solverArgs[2] == 3.5f, __LINE__);
    Check(g_solverArgs[3] == 100.0f && g_solverArgs[4] == 100.0f && g_solverEntry == entry, __LINE__);
    // no agent count: the default store with one slot
    r.AgentCount = 0; g_agentsCount = -1;
    CAudioOwner* g2 = new CAudioOwner(&r, entry);
    Check(g_defaultN == 1 && g_agentsCount == -1 && g2->Agents != 0 && g_timerOff == 2, __LINE__);
    if (fails == 0) std::printf("00c094a0_TEST PASS\n");
    return fails;
}