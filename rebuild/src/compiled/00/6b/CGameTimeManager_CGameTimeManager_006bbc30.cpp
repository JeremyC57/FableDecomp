// CGameTimeManager::CGameTimeManager(CMainGameComponent& gameComponent, CWorld& world) -- (manifest: "CCombatManager
// Init") member defaults, an empty stop-time set, the start time and time step from the world's environment def, and
// the SetTimeOfDay / SetDaySpeed console commands registered with the global console (made on first use)
// Leaf: Init
// Layout: Ego_r.pdb CGameTimeManager (0x40): World +0, GameComponent +4, Time +8, PrevTime +0xc, TimeStep +0x10, DayCount
//   +0x14, UsingVillageTime +0x18, VillageTime +0x1c, VillageDay +0x20, FastForwardingToTime +0x21, TimeFastForwardingTo
//   +0x24, SecondsLeftToFastForwardTime +0x28, StopTimeIndexes +0x2c (retail STLport set is 12 bytes, PDB 16),
//   NextStopTimeIndex +0x38 (PDB +0x3c). CConsoleCommandBase is 0x14 in retail (landed 0x00414F30).
// Shape: FableWin.pdb names the class and both console callbacks (?ConsoleSetTimeOfDay@CGameTimeManager@@CIX...);
//   the commands are `new CConsoleCommand<CGameTimeManager>(...)` with the name a conditional temporary (flag bits 1, 2)
//   and the inline console getter evaluated after the new-expression; Time, then PrevTime = Time (st0) and
//   VillageTime = Time (integer copy).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);            // 0x0099EC30
    ~CCharString();                                   // 0x0099EAE0
};
class CConsoleInputBase { public:                            // vtable 0x0122E5B0
    virtual ~CConsoleInputBase();
    CCharString Name;                                 // +0x04
    CConsoleInputBase(const CCharString& name) : Name(name) { }
};
class CConsoleCommandParameters { public:                    // 12-byte vector of parameter types
    long* first; long* last; long* end_of_storage;
    CConsoleCommandParameters(long type);             // 0x009EBB30
};
enum EGenericVarType { GVT_FLOAT = 3 };
struct CConsoleCommandBase : CConsoleInputBase {      // vtable 0x0122E638
    CConsoleCommandParameters ParamTypes;             // +0x08
    virtual ~CConsoleCommandBase();
    CConsoleCommandBase(const CCharString& name, const EGenericVarType& type) : CConsoleInputBase(name), ParamTypes(type) { }
};
struct CGenericVarVector;
template<class T> struct CConsoleCommand : CConsoleCommandBase {   // vtable 0x0125D668 for CGameTimeManager
    typedef void (__fastcall* Function)(T& object, const CGenericVarVector& args);
    T*       Object;                                  // +0x14
    Function Callback;                                // +0x18
    CConsoleCommand(const CCharString& name, T& object, Function callback, const EGenericVarType& type)
        : CConsoleCommandBase(name, type), Object(&object), Callback(callback) { }
    virtual ~CConsoleCommand();
};
class CConsole { public:                                     // vtable 0x0129C600, 0x9c bytes
    unsigned char _pad[0x9c];
    CConsole();                                       // 0x009ECD80
    void AddInput(CConsoleInputBase* input);          // 0x009EC5E0
};
struct CCountedPointer_CConsole {
    CConsole* m_pObject;
    void*     m_pCount;
    void Reset(CConsole* object);                     // 0x00413520
};
extern CCountedPointer_CConsole g_pConsole;           // 0x013CAA40
inline CConsole* GetConsole()
{
    if (g_pConsole.m_pObject == 0)
        g_pConsole.Reset(new CConsole());
    return g_pConsole.m_pObject;
}
struct SSetNode {                                     // STLport _Rb_tree_node<long>
    char      Color;
    SSetNode* Parent;
    SSetNode* Left;
    SSetNode* Right;
    long      Value;
};
struct CLongSet {                                     // STLport set<long> (12 bytes)
    SSetNode*     _M_header;
    unsigned long _M_node_count;
    char          _M_key_compare;
    CLongSet() : _M_header(0)
    {
        _M_header = (SSetNode*)malloc(sizeof(SSetNode));
        _M_node_count = 0;
        _M_header->Color = 0;
        _M_header->Parent = 0;
        _M_header->Left = _M_header;
        _M_header->Right = _M_header;
    }
};
class CEnvironmentDef { public:
    unsigned char _pad[0xb4];
    float StartTime;                                  // +0xb4 (hours)
    float DaySpeed;                                   // +0xb8
};
class CEnvironment { public:
    unsigned char _pad[0xd8];
    const CEnvironmentDef* Def;                       // +0xd8
};
class CWorld { public:
    unsigned char _pad[0x10];
    CEnvironment* Environment;                        // +0x10
};
class CMainGameComponent;
class CGameTimeManager {
public:
    CWorld*             World;                        // +0x00
    CMainGameComponent* GameComponent;                // +0x04
    float               Time;                         // +0x08
    float               PrevTime;                     // +0x0c
    float               TimeStep;                     // +0x10
    long                DayCount;                     // +0x14
    bool                UsingVillageTime;             // +0x18
    float               VillageTime;                  // +0x1c
    bool                VillageDay;                   // +0x20
    bool                FastForwardingToTime;         // +0x21
    float               TimeFastForwardingTo;         // +0x24
    float               SecondsLeftToFastForwardTime; // +0x28
    CLongSet            StopTimeIndexes;              // +0x2c
    long                NextStopTimeIndex;            // +0x38
    CGameTimeManager(CMainGameComponent& gameComponent, CWorld& world);
private:
    static void __fastcall ConsoleSetTimeOfDay(CGameTimeManager& manager, const CGenericVarVector& args);   // 0x006BB860
    static void __fastcall ConsoleSetDaySpeed(CGameTimeManager& manager, const CGenericVarVector& args);    // 0x006BB940
};

CGameTimeManager::CGameTimeManager(CMainGameComponent& gameComponent, CWorld& world)
    : World(&world)
    , GameComponent(&gameComponent)
    , DayCount(0)
    , UsingVillageTime(false)
    , VillageDay(true)
    , FastForwardingToTime(false)
    , TimeFastForwardingTo(-1.0f)
    , SecondsLeftToFastForwardTime(0)
    , NextStopTimeIndex(1)
{
    const CEnvironmentDef* def = World->Environment->Def;
    Time = def->StartTime * (1.0f / 24.0f);
    PrevTime = Time;
    VillageTime = Time;
    TimeStep = 1.0f / def->DaySpeed;
    GetConsole()->AddInput(new CConsoleCommand<CGameTimeManager>("SetTimeOfDay", *this, &ConsoleSetTimeOfDay, GVT_FLOAT));
    GetConsole()->AddInput(new CConsoleCommand<CGameTimeManager>("SetDaySpeed", *this, &ConsoleSetDaySpeed, GVT_FLOAT));
}