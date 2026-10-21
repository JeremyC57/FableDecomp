// Behaviour test for 0074e560: src/0074e560.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCContainerRewardHero::CreateNewObject
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCContainerRewardHero::CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration, const CCharString& scriptName) -- spawn a reward object popping out of the container: push it, give it a random spin, make it not solid to the container, copy the container's lighting, and optionally give it a 90-second life timer
// Leaf: CreateNewObject
// Retail 0x0074E560, 336 bytes, `ret 10h`.
// Callees: the thing factory Create(long, const C3DVector*, long, CRightHandedSet*, ulong, const CCharString*) 0x00703210
// (`__fastcall`), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) 0x006B0210, the TC's game accessor 0x00686A80 and its
// random-seed accessor 0x0049D810, GFFloatRandom(float, ulong*) 0x004978A0 (`__fastcall`), the physics' slot 66 (+0x108,
// float spin), CCharString(const char*, long) 0x0099EBF0 ("CTCSolidToThing" 0x0123B47C, "CTCLifeTimer" 0x0123E424) / ~
// 0x0099EAE0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, CTCSolidToThing::AddThingNotSolidTo
// 0x00770FA0, CTCGraphicAppearance::SetLightingChannel 0x004BFB40 / SetAsChanged 0x004BF9E0, CVectorMap::LowerBound
// 0x0040F020 (interface 0x103), the life timer's setter 0x007E5950 (90.0f).
// Layout: Ego_r.pdb CTCContainerRewardHero - 4: CreateObjectsThatTimeOut +0x19, ParentThing +4; CThing physics +0x60,
// graphic appearance +0x64 (lighting channel +0x40), dead flag +0x91; the life timer's flags +0x10/+0x11.
// Shape: the random spin is a named float local passed on as a dword; the AddTC names are temporaries; the life timer is
// `if (HasTC) t = GetTC; else t = AddTC(...)` with the string slot address formed before the branch.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
struct CTCBase;
struct CTCParamsBase;
struct CRightHandedSet;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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

struct CEnginePrimitiveThemeChannelDesc;
struct CTCGraphicAppearance {
    unsigned char _pad00[0x40];
    unsigned char LightingChannel[4];                   // +0x40
    void SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b);   // 0x004BFB40
    void SetAsChanged(unsigned long what);              // 0x004BF9E0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetSpin(float spin);                   // slot 66 (+0x108)
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CThing {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];             // +0x20
    CInterfaceMap         m_interfaces;                 // +0x44
    unsigned char         _pad4c[0x60 - 0x4c];
    CTCPhysicsBase*       Physics;                      // +0x60
    CTCGraphicAppearance* Graphic;                      // +0x64
    unsigned char         _pad68[0x91 - 0x68];
    unsigned char         m_flags;                      // +0x91 bit 0: dead
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
CThing* __fastcall CreateThing(long def, const C3DVector* position, long kind, CRightHandedSet* orientation,
                               unsigned long flags, const CCharString* scriptName);   // 0x00703210
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CGameComponents {
    unsigned long* GetRandomSeed();                     // 0x0049D810
};
struct CTCSolidToThing {
    void AddThingNotSolidTo(CThing* thing);             // 0x00770FA0
};
struct CTCLifeTimer {
    unsigned char _pad00[0x10];
    bool          Active;                               // +0x10
    bool          Expired;                              // +0x11
    void SetLifeTime(float seconds);                    // 0x007E5950
};
class CTCContainerRewardHero {
public:
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x18 - 0x08];
    bool          Enabled;                              // +0x18
    bool          CreateObjectsThatTimeOut;             // +0x19
    CGameComponents* GetGame();                         // 0x00686A80
    CThing* CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration, const CCharString& scriptName);
};

CThing* CTCContainerRewardHero::CreateNewObject(long def, const C3DVector& position, const C3DVector& acceleration,
                                                const CCharString& scriptName)
{
    CThing* thing = CreateThing(def, &position, 5, 0, 0, &scriptName);
    if (thing && !(thing->m_flags & 1)) {
        thing->Physics->ApplyAcceleration(acceleration);
        float spin = GFFloatRandom(1.0f, GetGame()->GetRandomSeed());
        thing->Physics->SetSpin(spin);
        CTCSolidToThing* solid = (CTCSolidToThing*)thing->AddTC("CTCSolidToThing", false, 0);
        solid->AddThingNotSolidTo(ParentThing);
        CTCGraphicAppearance* containerGraphic = ParentThing->Graphic;
        if (containerGraphic && thing->Graphic) {
            thing->Graphic->SetLightingChannel((CEnginePrimitiveThemeChannelDesc*)containerGraphic->LightingChannel, true, true);
            thing->Graphic->SetAsChanged(4);
        }
        if (CreateObjectsThatTimeOut) {
            CTCLifeTimer* timer;
            if (thing->HasTC(0x103))
                timer = thing->GetTC<CTCLifeTimer>(0x103);
            else
                timer = (CTCLifeTimer*)thing->AddTC("CTCLifeTimer", false, 0);
            timer->SetLifeTime(90.0f);
            timer->Active = true;
            timer->Expired = false;
        }
    }
    return thing;
}

// ---- stubs ----
static int g_strs; static const char* g_strText[8];
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; g_strText[g_strs++ & 7] = text; }
CCharString::~CCharString() { --g_strs; }
void CTCPhysicsBase::v00() {}
void CTCPhysicsBase::v01() {}
void CTCPhysicsBase::v02() {}
void CTCPhysicsBase::v03() {}
void CTCPhysicsBase::v04() {}
void CTCPhysicsBase::v05() {}
void CTCPhysicsBase::v06() {}
void CTCPhysicsBase::v07() {}
void CTCPhysicsBase::v08() {}
void CTCPhysicsBase::v09() {}
void CTCPhysicsBase::v10() {}
void CTCPhysicsBase::v11() {}
void CTCPhysicsBase::v12() {}
void CTCPhysicsBase::v13() {}
void CTCPhysicsBase::v14() {}
void CTCPhysicsBase::v15() {}
void CTCPhysicsBase::v16() {}
void CTCPhysicsBase::v17() {}
void CTCPhysicsBase::v18() {}
void CTCPhysicsBase::v19() {}
void CTCPhysicsBase::v20() {}
void CTCPhysicsBase::v21() {}
void CTCPhysicsBase::v22() {}
void CTCPhysicsBase::v23() {}
void CTCPhysicsBase::v24() {}
void CTCPhysicsBase::v25() {}
void CTCPhysicsBase::v26() {}
void CTCPhysicsBase::v27() {}
void CTCPhysicsBase::v28() {}
void CTCPhysicsBase::v29() {}
void CTCPhysicsBase::v30() {}
void CTCPhysicsBase::v31() {}
void CTCPhysicsBase::v32() {}
void CTCPhysicsBase::v33() {}
void CTCPhysicsBase::v34() {}
void CTCPhysicsBase::v35() {}
void CTCPhysicsBase::v36() {}
void CTCPhysicsBase::v37() {}
void CTCPhysicsBase::v38() {}
void CTCPhysicsBase::v39() {}
void CTCPhysicsBase::v40() {}
void CTCPhysicsBase::v41() {}
void CTCPhysicsBase::v42() {}
void CTCPhysicsBase::v43() {}
void CTCPhysicsBase::v44() {}
void CTCPhysicsBase::v45() {}
void CTCPhysicsBase::v46() {}
void CTCPhysicsBase::v47() {}
void CTCPhysicsBase::v48() {}
void CTCPhysicsBase::v49() {}
void CTCPhysicsBase::v50() {}
void CTCPhysicsBase::v51() {}
void CTCPhysicsBase::v52() {}
void CTCPhysicsBase::v53() {}
void CTCPhysicsBase::v54() {}
void CTCPhysicsBase::v55() {}
void CTCPhysicsBase::v56() {}
void CTCPhysicsBase::v57() {}
void CTCPhysicsBase::v58() {}
void CTCPhysicsBase::v59() {}
void CTCPhysicsBase::v60() {}
void CTCPhysicsBase::v61() {}
void CTCPhysicsBase::v62() {}
void CTCPhysicsBase::v63() {}
void CTCPhysicsBase::v64() {}
void CTCPhysicsBase::v65() {}
static float g_spin; static CTCPhysicsBase* g_spinPhys; static int g_spins;
void CTCPhysicsBase::SetSpin(float spin) { g_spin = spin; g_spinPhys = this; ++g_spins; }
static const C3DVector* g_acc; static CTCPhysicsBase* g_accPhys;
void CTCPhysicsBase::ApplyAcceleration(const C3DVector& acceleration) { g_acc = &acceleration; g_accPhys = this; }
static CThing* g_created; static long g_def; static const C3DVector* g_pos; static long g_kind; static CRightHandedSet* g_orient;
static unsigned long g_cflags; static const CCharString* g_script;
CThing* __fastcall CreateThing(long def, const C3DVector* position, long kind, CRightHandedSet* orientation,
                               unsigned long flags, const CCharString* scriptName)
{ g_def = def; g_pos = position; g_kind = kind; g_orient = orientation; g_cflags = flags; g_script = scriptName; return g_created; }
static unsigned long g_seed; static float g_range; static unsigned long* g_seedArg;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_range = range; g_seedArg = seed; return 0.25f; }
unsigned long* CGameComponents::GetRandomSeed() { return &g_seed; }
static CGameComponents g_game; static CTCContainerRewardHero* g_gameThis;
CGameComponents* CTCContainerRewardHero::GetGame() { g_gameThis = this; return &g_game; }
static int g_adds; static const char* g_addName[4]; static CThing* g_addThing[4]; static bool g_addFlag; static CTCParamsBase* g_addParams;
static CTCSolidToThing g_solid; static CTCLifeTimer g_newTimer;
CTCBase* CThing::AddTC(const CCharString& name, bool flag, CTCParamsBase* params)
{
    const char* n = (const char*)name.PStringData; g_addName[g_adds & 3] = n; g_addThing[g_adds & 3] = this; ++g_adds;
    g_addFlag = flag; g_addParams = params;
    if (!strcmp(n, "CTCSolidToThing")) return (CTCBase*)&g_solid;
    if (!strcmp(n, "CTCLifeTimer")) return (CTCBase*)&g_newTimer;
    return 0;
}
static CThing* g_notSolid; static CTCSolidToThing* g_notSolidThis;
void CTCSolidToThing::AddThingNotSolidTo(CThing* thing) { g_notSolid = thing; g_notSolidThis = this; }
static int g_light; static CEnginePrimitiveThemeChannelDesc* g_lightCh; static CTCGraphicAppearance* g_lightThis; static bool g_la, g_lb;
void CTCGraphicAppearance::SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b)
{ ++g_light; g_lightCh = channel; g_lightThis = this; g_la = a; g_lb = b; }
static unsigned long g_changed; static CTCGraphicAppearance* g_changedThis;
void CTCGraphicAppearance::SetAsChanged(unsigned long what) { g_changed = what; g_changedThis = this; }
static float g_life; static CTCLifeTimer* g_lifeThis; static int g_lifes;
void CTCLifeTimer::SetLifeTime(float seconds) { g_life = seconds; g_lifeThis = this; ++g_lifes; }
struct TestPhys : CTCPhysicsBase {};
static void Reset()
{
    g_strs = 0; g_spin = 0; g_spinPhys = 0; g_spins = 0; g_acc = 0; g_accPhys = 0; g_adds = 0; g_addFlag = true; g_addParams = (CTCParamsBase*)1;
    g_notSolid = 0; g_notSolidThis = 0; g_light = 0; g_lightCh = 0; g_lightThis = 0; g_changed = 0; g_changedThis = 0;
    g_life = 0; g_lifeThis = 0; g_lifes = 0; g_range = 0; g_seedArg = 0; g_gameThis = 0;
    memset(&g_newTimer, 0, sizeof g_newTimer);
}
static long g_thingBuf[3][sizeof(CThing) / 4 + 1];
static CThing* NewThing(int i) { memset(g_thingBuf[i], 0, sizeof g_thingBuf[i]); return (CThing*)g_thingBuf[i]; }
int main()
{
    CThing* container = NewThing(0); CThing* reward = NewThing(1);
    TestPhys phys; CTCGraphicAppearance contGfx, rewardGfx;
    container->Graphic = &contGfx; reward->Physics = &phys; reward->Graphic = &rewardGfx;
    CTCContainerRewardHero h; memset(&h, 0, sizeof h); h.ParentThing = container; h.CreateObjectsThatTimeOut = true;
    C3DVector pos = { 1, 2, 3 }, acc = { 4, 5, 6 }; CCharString script("scr"); g_strs = 0;
    // full path, no existing life timer: AddTC for both, lighting copied, 90 s timer
    Reset(); g_created = reward;
    CThing* r = h.CreateNewObject(7, pos, acc, script);
    CHECK(r == reward && g_def == 7 && g_pos == &pos && g_kind == 5 && g_orient == 0 && g_cflags == 0 && g_script == &script);
    CHECK(g_accPhys == &phys && g_acc == &acc);
    CHECK(g_range == 1.0f && g_seedArg == &g_seed && g_gameThis == &h && g_spins == 1 && g_spin == 0.25f && g_spinPhys == &phys);
    CHECK(g_adds == 2 && !strcmp(g_addName[0], "CTCSolidToThing") && !strcmp(g_addName[1], "CTCLifeTimer"));
    CHECK(g_addThing[0] == reward && g_addThing[1] == reward && !g_addFlag && g_addParams == 0);
    CHECK(g_notSolid == container && g_notSolidThis == &g_solid);
    CHECK(g_light == 1 && g_lightThis == &rewardGfx && g_lightCh == (CEnginePrimitiveThemeChannelDesc*)contGfx.LightingChannel && g_la && g_lb);
    CHECK(g_changed == 4 && g_changedThis == &rewardGfx);
    CHECK(g_lifes == 1 && g_lifeThis == &g_newTimer && g_life == 90.0f && g_newTimer.Active && !g_newTimer.Expired);
    CHECK(g_strs == 0);
    // existing life timer (interface 0x103) found through the map, not added
    static CTCLifeTimer existing; existing.Active = false; existing.Expired = true;
    CInterfaceMap::SEntry e[] = { { 4, 0 }, { 0x103, (CTCBase*)&existing }, { 0x110, 0 } };
    for (int i = 0; i < 3; ++i) reward->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    reward->m_interfaces.first = e; reward->m_interfaces.last = e + 3;
    Reset();
    r = h.CreateNewObject(7, pos, acc, script);
    CHECK(r == reward && g_adds == 1 && g_lifes == 1 && g_lifeThis == &existing && existing.Active && !existing.Expired);
    // no time-out: no timer at all
    Reset(); h.CreateObjectsThatTimeOut = false;
    r = h.CreateNewObject(7, pos, acc, script);
    CHECK(r == reward && g_adds == 1 && g_lifes == 0);
    // container without graphic: lighting skipped; reward without graphic likewise
    Reset(); container->Graphic = 0;
    h.CreateNewObject(7, pos, acc, script);
    CHECK(g_light == 0 && g_changed == 0 && g_adds == 1);
    Reset(); container->Graphic = &contGfx; reward->Graphic = 0;
    h.CreateNewObject(7, pos, acc, script);
    CHECK(g_light == 0 && g_changed == 0);
    // dead reward: returned untouched
    Reset(); reward->m_flags = 1;
    r = h.CreateNewObject(7, pos, acc, script);
    CHECK(r == reward && g_accPhys == 0 && g_spins == 0 && g_adds == 0);
    // nothing created
    Reset(); g_created = 0;
    r = h.CreateNewObject(7, pos, acc, script);
    CHECK(r == 0 && g_accPhys == 0 && g_adds == 0);
    if (fails == 0) printf("0074e560_TEST PASS\n"); else printf("0074e560_TEST FAIL\n");
    return fails;
}