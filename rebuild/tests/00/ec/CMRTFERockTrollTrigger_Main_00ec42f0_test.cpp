// CM_RTFERockTrollTrigger::Main() -- rock-troll trigger marker: alive filter on Me; unless the quest already spawned the troll, wait until the hero comes within the script def's trigger distance, then create the standing rock troll ("RTFE_RockTroll") at the "M_RTFERockTrollSpawnPos" marker, flag it spawned and remove this trigger
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC42F0, length 412 bytes (+ int3 padding).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840; the CIsThingAlive filter (vtable 0x012C32F0,
//   destructor 0x00E241B0) registered @ 0x00F35B10; IsDone @ 0x00F35B30; IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`);
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("M_RTFERockTrollSpawnPos" 0x012F16AC, "RTFE_RockTroll"
//   0x012F1644, "CREATURE_ROCK_TROLL_START_STANDING" 0x012F1688); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero,
//   72 GetThingWithScriptName, 91 CreateCreature, 108 RemoveThing. Script def @ 0x0143E90C (trigger distance +0x72c); zero vector
//   @ 0x0143E8E0 (the inline CScriptThing::GetPos).
// Layout: the entity-script base {vptr, Game +4, Me +8} with the quest object at +0x14 (spawned flag +0x4e), as the landed
//   CUndeadSoldier::Main 0x00D88F90 and CHTG_AmbushPointCond::Main 0x00D73970.
// Shape: the scoped filter object of the landed entity-script Mains; the creature's two names are implicit conversion temporaries; the in-loop
//   IsDone exit jumps to one shared `done:` return at the end, which moves the destroy-spawnPos exit block below the inline GetPos
//   ternary as in retail (a plain in-loop `return` keeps it inline after the post-loop IsDone test).
//   built before the position argument; the CreateCreature result is a discarded temporary (it reuses the filter's slot).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float x, y, z; };
extern C3DVector g_ZeroVector;                     // 0x0143E8E0
struct CCPPointerInfo;
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const;
    virtual void v04() const; virtual void v05() const;
    virtual __forceinline const C3DVector& GetPos() const   // slot 6 (+0x18), 0x004AA980
    {
        if (!PImp_Data)
            return g_ZeroVector;
        return PImp_Data->GetPos();
    }
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const;
    virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const;
    virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071() const;
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const;
    virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const;
    virtual void v089() const; virtual void v090() const;
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& position, const CCharString& scriptName, bool flag) const;   // slot 91 (+0x16c)
    virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const;
    virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
struct CIsThingAlive {                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
struct CScriptDef {
    unsigned char _pad[0x72c];
    float         RockTrollTriggerDistance;        // +0x72c (descriptive)
};
extern CScriptDef* g_pScriptDef;                   // 0x0143E90C
struct CQ_RTFEScript {
    unsigned char _pad[0x4e];
    bool          RockTrollSpawned;                // +0x4e (descriptive)
};

class CM_RTFERockTrollTrigger {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_RTFEScript*        ParentClass;             // +0x14
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CM_RTFERockTrollTrigger::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    if (ParentClass->RockTrollSpawned)
        return;
    if (IsDone())
        return;
    CScriptThing spawnPos = Game->GetThingWithScriptName("M_RTFERockTrollSpawnPos");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, g_pScriptDef->RockTrollTriggerDistance)) {
        Game->NewScriptFrame();
        if (IsDone())
            goto done;
    }
    if (IsDone())
        return;
    Game->CreateCreature("CREATURE_ROCK_TROLL_START_STANDING", spawnPos.GetPos(), "RTFE_RockTroll", false);
    ParentClass->RockTrollSpawned = true;
    Game->RemoveThing(Me, false, true);
    return;
done:
    return;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
void CScriptThing::v00() const {}
void CScriptThing::v01() const {}
void CScriptThing::v02() const {}
void CScriptThing::v03() const {}
void CScriptThing::v04() const {}
void CScriptThing::v05() const {}
void CGameScriptInterface::v000() const {}
void CGameScriptInterface::v001() const {}
void CGameScriptInterface::v002() const {}
void CGameScriptInterface::v003() const {}
void CGameScriptInterface::v004() const {}
void CGameScriptInterface::v005() const {}
void CGameScriptInterface::v006() const {}
void CGameScriptInterface::v008() const {}
void CGameScriptInterface::v009() const {}
void CGameScriptInterface::v010() const {}
void CGameScriptInterface::v011() const {}
void CGameScriptInterface::v012() const {}
void CGameScriptInterface::v013() const {}
void CGameScriptInterface::v014() const {}
void CGameScriptInterface::v015() const {}
void CGameScriptInterface::v016() const {}
void CGameScriptInterface::v017() const {}
void CGameScriptInterface::v018() const {}
void CGameScriptInterface::v019() const {}
void CGameScriptInterface::v020() const {}
void CGameScriptInterface::v021() const {}
void CGameScriptInterface::v022() const {}
void CGameScriptInterface::v023() const {}
void CGameScriptInterface::v024() const {}
void CGameScriptInterface::v025() const {}
void CGameScriptInterface::v026() const {}
void CGameScriptInterface::v027() const {}
void CGameScriptInterface::v028() const {}
void CGameScriptInterface::v029() const {}
void CGameScriptInterface::v030() const {}
void CGameScriptInterface::v031() const {}
void CGameScriptInterface::v032() const {}
void CGameScriptInterface::v033() const {}
void CGameScriptInterface::v034() const {}
void CGameScriptInterface::v035() const {}
void CGameScriptInterface::v036() const {}
void CGameScriptInterface::v037() const {}
void CGameScriptInterface::v038() const {}
void CGameScriptInterface::v039() const {}
void CGameScriptInterface::v040() const {}
void CGameScriptInterface::v041() const {}
void CGameScriptInterface::v042() const {}
void CGameScriptInterface::v043() const {}
void CGameScriptInterface::v044() const {}
void CGameScriptInterface::v045() const {}
void CGameScriptInterface::v046() const {}
void CGameScriptInterface::v047() const {}
void CGameScriptInterface::v048() const {}
void CGameScriptInterface::v049() const {}
void CGameScriptInterface::v050() const {}
void CGameScriptInterface::v051() const {}
void CGameScriptInterface::v052() const {}
void CGameScriptInterface::v053() const {}
void CGameScriptInterface::v054() const {}
void CGameScriptInterface::v055() const {}
void CGameScriptInterface::v056() const {}
void CGameScriptInterface::v057() const {}
void CGameScriptInterface::v058() const {}
void CGameScriptInterface::v059() const {}
void CGameScriptInterface::v060() const {}
void CGameScriptInterface::v061() const {}
void CGameScriptInterface::v062() const {}
void CGameScriptInterface::v063() const {}
void CGameScriptInterface::v064() const {}
void CGameScriptInterface::v065() const {}
void CGameScriptInterface::v066() const {}
void CGameScriptInterface::v067() const {}
void CGameScriptInterface::v068() const {}
void CGameScriptInterface::v069() const {}
void CGameScriptInterface::v071() const {}
void CGameScriptInterface::v073() const {}
void CGameScriptInterface::v074() const {}
void CGameScriptInterface::v075() const {}
void CGameScriptInterface::v076() const {}
void CGameScriptInterface::v077() const {}
void CGameScriptInterface::v078() const {}
void CGameScriptInterface::v079() const {}
void CGameScriptInterface::v080() const {}
void CGameScriptInterface::v081() const {}
void CGameScriptInterface::v082() const {}
void CGameScriptInterface::v083() const {}
void CGameScriptInterface::v084() const {}
void CGameScriptInterface::v085() const {}
void CGameScriptInterface::v086() const {}
void CGameScriptInterface::v087() const {}
void CGameScriptInterface::v088() const {}
void CGameScriptInterface::v089() const {}
void CGameScriptInterface::v090() const {}
void CGameScriptInterface::v092() const {}
void CGameScriptInterface::v093() const {}
void CGameScriptInterface::v094() const {}
void CGameScriptInterface::v095() const {}
void CGameScriptInterface::v096() const {}
void CGameScriptInterface::v097() const {}
void CGameScriptInterface::v098() const {}
void CGameScriptInterface::v099() const {}
void CGameScriptInterface::v100() const {}
void CGameScriptInterface::v101() const {}
void CGameScriptInterface::v102() const {}
void CGameScriptInterface::v103() const {}
void CGameScriptInterface::v104() const {}
void CGameScriptInterface::v105() const {}
void CGameScriptInterface::v106() const {}
void CGameScriptInterface::v107() const {}
void CIsThingAlive::v00() {}
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { }
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { }
CCharString::~CCharString() { }
C3DVector g_ZeroVector = { 0.0f, 0.0f, 0.0f };
static int g_thingDtors = 0;
CScriptThing::CScriptThing(const CScriptThing& other) : PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info) { }
CScriptThing::~CScriptThing() { ++g_thingDtors; }
static long g_raw[3];
static const CScriptThing& Proto() { return *(const CScriptThing*)g_raw; }
struct TThing : CScriptThing {
    C3DVector pos;
    TThing(float x, float y, float z) : CScriptThing(Proto()) { pos.x = x; pos.y = y; pos.z = z; }
    const C3DVector& GetPos() const { return pos; }
};
static TThing g_spawnMarker(5.0f, 6.0f, 7.0f);
static TThing g_hero(0.0f, 0.0f, 0.0f);
static int g_distCalls = 0, g_distTrueAt = 0;
static float g_distArg = 0.0f;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float distance)
{
    g_distArg = distance;
    return ++g_distCalls >= g_distTrueAt;
}
static int g_frames = 0, g_lookups = 0, g_creates = 0, g_removes = 0;
static const char* g_lookupName = 0; static const char* g_createDef = 0; static const char* g_createName = 0;
static C3DVector g_createPos; static bool g_createFlag = true; static bool g_removeA = true, g_removeB = false;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; }
const CScriptThing& CGameScriptInterface::GetHero() const { return g_hero; }
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& name) const
{
    ++g_lookups; g_lookupName = (const char*)name.m_pData;
    CScriptThing t(Proto()); t.PImp_Data = &g_spawnMarker; return t;
}
CScriptThing CGameScriptInterface::CreateCreature(const CCharString& def, const C3DVector& position, const CCharString& scriptName, bool flag) const
{
    ++g_creates; g_createDef = (const char*)def.m_pData; g_createName = (const char*)scriptName.m_pData; g_createPos = position; g_createFlag = flag;
    return CScriptThing(Proto());
}
void CGameScriptInterface::RemoveThing(const CScriptThing&, bool a, bool b) const { ++g_removes; g_removeA = a; g_removeB = b; }
CIsThingAlive::~CIsThingAlive() { }
static CScriptDef g_def;
CScriptDef* g_pScriptDef = &g_def;
static int g_registers = 0, g_isDoneCalls = 0, g_doneAt = 1000;
void CM_RTFERockTrollTrigger::RegisterFilter(CIsThingAlive&) { ++g_registers; }
bool CM_RTFERockTrollTrigger::IsDone() { return ++g_isDoneCalls >= g_doneAt; }
static void Reset(int doneAt, int distTrueAt)
{
    g_frames = g_lookups = g_creates = g_removes = g_registers = g_isDoneCalls = g_distCalls = g_thingDtors = 0;
    g_doneAt = doneAt; g_distTrueAt = distTrueAt; g_createDef = g_createName = g_lookupName = 0;
}
int main()
{
    static CGameScriptInterface game;
    static CQ_RTFEScript quest;
    static long sbuf[16];
    CM_RTFERockTrollTrigger& s = *(CM_RTFERockTrollTrigger*)sbuf;
    memset(&quest, 0, sizeof(quest));
    s.Game = &game; s.ParentClass = &quest;
    s.Me.PImp_Data = 0; s.Me.PImp_Info = 0;
    g_def.RockTrollTriggerDistance = 12.5f;
    // 1: troll not spawned, hero arrives after 3 checks
    Reset(1000, 3);
    s.Main();
    CHECK(g_registers == 1);
    CHECK(g_lookups == 1 && strcmp(g_lookupName, "M_RTFERockTrollSpawnPos") == 0);
    CHECK(g_distCalls == 3 && g_distArg == 12.5f);
    CHECK(g_frames == 1 + 2);
    CHECK(g_creates == 1 && strcmp(g_createDef, "CREATURE_ROCK_TROLL_START_STANDING") == 0 && strcmp(g_createName, "RTFE_RockTroll") == 0);
    CHECK(g_createPos.x == 5.0f && g_createPos.y == 6.0f && g_createPos.z == 7.0f && g_createFlag == false);
    CHECK(quest.RockTrollSpawned == true);
    CHECK(g_removes == 1 && g_removeA == false && g_removeB == true);
    // 2: already spawned -> nothing
    Reset(1000, 1);
    s.Main();
    CHECK(g_lookups == 0 && g_creates == 0 && g_removes == 0 && g_frames == 1);
    // 3: done inside the wait loop (4th IsDone call) -> no creature
    quest.RockTrollSpawned = false;
    Reset(4, 100);
    s.Main();
    CHECK(g_lookups == 1 && g_creates == 0 && g_removes == 0 && quest.RockTrollSpawned == false);
    CHECK(g_frames == 1 + 2);
    // 4: done right after the loop
    Reset(3, 1);
    s.Main();
    CHECK(g_lookups == 1 && g_distCalls == 1 && g_creates == 0 && g_removes == 0);
    // 5: done at the first check
    Reset(1, 1);
    s.Main();
    CHECK(g_lookups == 0 && g_frames == 1);
    if (fails == 0) printf("00ec42f0_TEST PASS\n");
    return fails;
}