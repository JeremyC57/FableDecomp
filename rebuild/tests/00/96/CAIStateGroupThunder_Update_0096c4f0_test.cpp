// Behaviour test for 0096c4f0: src/0096c4f0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_Thunder::Update
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_Thunder::Update() -- mark the target in danger, and in the idle combat state cast the lightning storm when the target is within the ability range
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096C4F0, 495 bytes.
// Shape: CDefinitionManager::GetDef(long, CDefPointer*) 0x008E31D0 has a same-TU stand-in body that writes the out slot
//   only (the def is then loaded once into esi and reused for the range test and the release), the CVectorMap LowerBound
//   instantiation 0x0040F020 has its same-TU binary-search body, and the static accessor CGameDefinitionManager::Get
//   0x0044C6B0 is `__fastcall` as in the landed headers (that convention puts the name temporary's `lea` after `push 1`).
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
#pragma pack(pop)
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSpecialAbilityDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
    unsigned char _pad[0x44 - 8];
    float Range;                                // +0x44
};
struct CDefPointer {
    CSpecialAbilityDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (--m_p->m_refs == 0) m_p->Destroy(); }
    CSpecialAbilityDef* operator->() const { return m_p; }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();       // 0x0044C6B0
    long GetDefGlobalIndex(const CCharString& name, bool b);   // 0x009AD390
    bool GetDef(long index, CDefPointer* out);  // 0x008E31D0
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingCreatureBase;
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThing* creature, long level);   // 0x00747B80
};
struct CTCThunderCombat {                       // interface 0xad
    long GetState();                            // 0x007AED10
    void Reset();                               // 0x007AED50
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCreatureAction_ThunderLightningStorm : CCreatureActionBase {
    CCreatureAction_ThunderLightningStorm(CThingCreatureBase* creature);   // 0x0096C6E0
    ~CCreatureAction_ThunderLightningStorm();   // 0x0062E900
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
};
__forceinline float Distance2D(CThing* target, CThing* me)
{
    const C3DVector& from = me->GetPosition();
    const C3DVector& to = target->GetPosition();
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    return (float)sqrt(dx * dx + dy * dy);
}
#pragma pack(push, 1)
struct CAIStateGroup_Thunder : CAIStateGroupBase {
    CBaseIntelligentPointer PArenaCentre;       // +0x14
    bool bIsRangeAttack;                        // +0x1c
    CBaseIntelligentPointer PTargetThing;       // +0x1d
    unsigned char _pad_0x25[3];
    long HitCount;                              // +0x28
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_Thunder::Update()
{
    CTCThunderCombat* combat;
    PBrain->PCreature->GetInterface(&combat, 0xad);
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* stealth;
        if (PTargetThing.GetPItem()->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(PBrain->PCreature, 3);
    }
    switch (combat->GetState()) {
    case 0:
        if (IS_THING_ALIVE(PTargetThing)) {
            long index = CGameDefinitionManager::Get().GetDefGlobalIndex("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", true);
            CDefPointer def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            float dist = Distance2D(PTargetThing.GetPItem(), PBrain->PCreature);
            if (dist < def->Range && dist > 1.0f) {
                long result = AIAction_PerformActionAndWait(&CCreatureAction_ThunderLightningStorm(GetCreatureBase()), false);
                if (result != 0) {
                    HitCount = 0;
                    combat->Reset();
                }
            }
        }
        Terminate();
        break;
    case 1:
        Terminate();
        break;
    }
}

// ---- stubs ----
static C3DVector g_pos[4]; static long g_tb[4][sizeof(CThing) / 4 + 1];
const C3DVector& CThing::GetPosition() const { return g_pos[(const long*)this == g_tb[0] ? 0 : (const long*)this == g_tb[1] ? 1 : 2]; }
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
static int g_strs; static const char* g_strText;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strs; g_strText = text; }
CCharString::~CCharString() { --g_strs; }
static int g_destroyed;
void CSpecialAbilityDef::v00() {}
void CSpecialAbilityDef::Destroy() { ++g_destroyed; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
static int g_lookups; static const char* g_lookupName; static bool g_lookupB; static int g_lookupLive;
long CGameDefinitionManager::GetDefGlobalIndex(const CCharString& name, bool b)
{ ++g_lookups; g_lookupName = (const char*)name.m_pData; g_lookupB = b; g_lookupLive = g_strs; return 42; }
static CSpecialAbilityDef g_def;
void* FindDef_CGameDefinitionManager_GetDef_0() { ++g_def.m_refs; return &g_def; }
static int g_danger; static CThing* g_dangerWho; static long g_dangerLevel; static CTCStealth* g_dangerThis;
void CTCStealth::SetAsInDanger(CThing* creature, long level) { ++g_danger; g_dangerWho = creature; g_dangerLevel = level; g_dangerThis = this; }
static long g_state; static int g_resets;
long CTCThunderCombat::GetState() { return g_state; }
void CTCThunderCombat::Reset() { ++g_resets; }
void CCreatureActionBase::v00() {}
static int g_actCtor, g_actDtor; static CThingCreatureBase* g_actCreature; static void* g_actThis;
CCreatureAction_ThunderLightningStorm::CCreatureAction_ThunderLightningStorm(CThingCreatureBase* creature) { ++g_actCtor; g_actCreature = creature; g_actThis = this; }
CCreatureAction_ThunderLightningStorm::~CCreatureAction_ThunderLightningStorm() { ++g_actDtor; }
CAIStateGroupBase::~CAIStateGroupBase() {}
static int g_term; void CAIStateGroupBase::Terminate() { ++g_term; }
static int g_perf; static long g_perfResult; static CCreatureActionBase* g_perfAction; static bool g_perfB; static int g_perfDtorsBefore;
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b)
{ ++g_perf; g_perfAction = action; g_perfB = b; g_perfDtorsBefore = g_actDtor; return g_perfResult; }
static void Reset()
{
    g_strs = 0; g_destroyed = 0; g_lookups = 0; g_lookupName = 0; g_lookupB = false; g_lookupLive = 0; g_danger = 0; g_dangerWho = 0;
    g_dangerLevel = 0; g_dangerThis = 0; g_resets = 0; g_actCtor = g_actDtor = 0; g_actCreature = 0; g_actThis = 0; g_term = 0; g_perf = 0;
    g_perfAction = 0; g_perfB = true; g_perfDtorsBefore = -1;
}
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    CThing* me = (CThing*)g_tb[0]; CThing* target = (CThing*)g_tb[1];
    static CTCThunderCombat combat; static CTCStealth stealth;
    CInterfaceMap::SEntry me_e[] = { { 3, 0 }, { 0xad, (CTCBase*)&combat } }; SetInterfaces(me, me_e, 2);
    CInterfaceMap::SEntry t_e[] = { { 0x36, (CTCBase*)&stealth }, { 0x40, 0 } }; SetInterfaces(target, t_e, 2);
    g_pos[0].x = 0; g_pos[0].y = 0; g_pos[1].x = 3; g_pos[1].y = 4;
    g_def.m_refs = 1; g_def.Range = 10.0f;
    CAIBrain brain; brain.PCreature = me;
    CAIStateGroup_Thunder g; g.PBrain = &brain; g.PTargetThing.m_pItem = target; g.HitCount = 9;
    // idle, target in range, the cast succeeds
    Reset(); g_state = 0; g_perfResult = 1;
    g.Update();
    CHECK(g_danger == 1 && g_dangerWho == me && g_dangerLevel == 3 && g_dangerThis == &stealth);
    CHECK(g_lookups == 1 && g_lookupB && g_lookupLive == 1 && !strcmp(g_lookupName, "SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF") && g_strs == 0);
    CHECK(g_actCtor == 1 && g_actCreature == (CThingCreatureBase*)me && g_perf == 1 && g_perfAction == (CCreatureActionBase*)g_actThis && !g_perfB);
    CHECK(g_perfDtorsBefore == 0 && g_actDtor == 1 && g.HitCount == 0 && g_resets == 1 && g_term == 1);
    CHECK(g_def.m_refs == 1 && g_destroyed == 0);
    // the cast fails: no reset
    Reset(); g.HitCount = 9; g_perfResult = 0;
    g.Update();
    CHECK(g_perf == 1 && g.HitCount == 9 && g_resets == 0 && g_term == 1);
    // out of range / too close: no cast
    Reset(); g_def.Range = 5.0f; g_perfResult = 1;
    g.Update();
    CHECK(g_perf == 0 && g_term == 1 && g_lookups == 1);
    g_def.Range = 10.0f; g_pos[1].x = 0.6f; g_pos[1].y = 0.8f;
    Reset();
    g.Update();
    CHECK(g_perf == 0 && g_term == 1);
    g_pos[1].x = 3; g_pos[1].y = 4;
    // last def reference released through Destroy
    Reset(); g_def.m_refs = 0;
    g.Update();
    CHECK(g_destroyed == 1 && g_def.m_refs == 0);
    g_def.m_refs = 1;
    // state 1: terminate only; other states: nothing
    Reset(); g_state = 1;
    g.Update();
    CHECK(g_term == 1 && g_lookups == 0 && g_danger == 1);
    Reset(); g_state = 2;
    g.Update();
    CHECK(g_term == 0 && g_lookups == 0);
    // dead target: no danger mark, no cast, still terminates in state 0
    target->m_flags91 = 1; Reset(); g_state = 0;
    g.Update();
    CHECK(g_danger == 0 && g_lookups == 0 && g_term == 1);
    // target without stealth: no danger mark
    target->m_flags91 = 0; SetInterfaces(target, t_e + 1, 1); Reset();
    g.Update();
    CHECK(g_danger == 0 && g_perf == 1);
    g.PTargetThing.m_pItem = 0;
    if (fails == 0) printf("0096c4f0_TEST PASS\n"); else printf("0096c4f0_TEST FAIL\n");
    return fails;
}