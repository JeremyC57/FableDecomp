// Behaviour test for 008ddd70: the source verbatim (CRT free wrapped, MSVC keywords defined away under g++), every declared callee
// stubbed so the bodyguard search, the per-bodyguard HEAL effect and the one-time CTCHealLife add are observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static void ReleaseBlock(void* p) { std::free(p); }
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
extern "C" void fable_free(void* p) { ReleaseBlock(p); }
#define free fable_free
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCreatureMode::CModeGuildmasterPowerUpHeal::OnActivate() -- stand-mode activation; suspend a player's animation; for every live guild bodyguard within 20 units of the guildmaster play the HEAL effect (attached through its particle emitter) and make sure the guildmaster has a CTCHealLife component
// Leaf: ?OnActivate@CModeGuildmasterPowerUpHeal
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DDD70, 565 bytes (`ret`).
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CThingAnimatable::SetSuspendAnimation @ 0x00833A10, CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName
//   @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CModeBase::GetWorld @ 0x0073A680 (labelled
//   "CViewBase::GetDisplayEngine"), CThingSearchTools::GetAllThingsInRoughArea<..., CThingFilter_HasDefGlobalIndex> @ 0x008E3EB0,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CCRC::CalcAndLookup @ 0x00669D30, the special-effect Create @ 0x006E0880,
//   CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CThing::AddTC @ 0x004C9D60, CTCHealLife state setter @ 0x007C3EA0 (`[ecx+18h] = arg`;
//   labelled "CObservable::AddExclusiveObserver"), free @ 0x00BFEA14 (the vector buffer).
// Data: "CREATURE_GUILDMASTER_BODYGUARD" 0x01285C70, "HEAL" 0x01280BA4, "" 0x0122D70E, "CTCHealLife" 0x0123D19C.
// Layout: CModeBase ParentThing +4; CThing type +0x10, presence bits +0x20 (0x4b emitter, 0x99 heal-life), emitter driver +0x5c,
//   def +0x70, dead flag +0x91 bit 0; the world's thing-search tools +0x20.
// Shape: the landed CAIStateGroup_Guildmaster::FindAndStoreBodyguards 0x00977B10 search (zeroed vector, the def-index filter and a
//   C2DBoxI of +-20 in a block, a same-TU stand-in for the search so the count stays in ebp) plus the landed JackDeathInto
//   0x008C0EA0 effect block (by-value CDefPointer copy, same-TU GetSubDef stand-in, zeroed sub-def and effect id per bodyguard) and a same-TU
//   CalcAndLookup stand-in that places the effect-id zero store.
//   The effect and the component go on the guildmaster (ParentThing), once per bodyguard, exactly as retail does.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
class CTCParamsBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
class CDefClassBase { public:
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
// Same-TU stand-in for CSpecialEffectsDef::CalcAndLookup 0x00669D30 (advisor C1's 0x008E0060 lever: it reads the name and writes
// through the out pointer, nothing escapes), so VC7.1 sinks the per-bodyguard `effect = 0` store after the sub-def zero, as retail does.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    const char* text = (const char*)name.PStringData;
    if (text == 0)
        return false;
    *value = (long)text[0];
    return true;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> 0x00669580 (its landed body; see 0x008C0EA0).
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    long* pIndex;
    {
        CCharString name("CSpecialEffectsDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CSpecialEffectsDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

namespace NCreatureMode { struct CModeBase; }
class CThing { public:
    // (presence bits, emitter driver, def and dead flag as in the landed 0x008DAE10 model)
    unsigned char _pad00[0x10];
    unsigned char m_type;                             // +0x10 (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                   // +0x20
    unsigned char _pad44[0x5c - 0x44];
    CTCBase*      PTCDriver;                          // +0x5c
    unsigned char _pad60[0x70 - 0x60];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    void SetSuspendAnimation(bool suspend, NCreatureMode::CModeBase* mode);   // 0x00833A10
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

extern "C" void __cdecl free(void* p);                // 0x00BFEA14
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};
class CThingFilter_HasDefGlobalIndex { public:
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) {}
};
class CThingAICreature : public CThing { public:
};
struct CCreatureVector {                              // STLport vector<CThingAICreature*> {start, finish, end_of_storage}
    CThingAICreature** _M_start;
    CThingAICreature** _M_finish;
    CThingAICreature** _M_end_of_storage;
    CCreatureVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCreatureVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThingAICreature* operator[](unsigned long i) const { return _M_start[i]; }
};
class CThingSearchTools { public:
    // GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex> @ 0x008E3EB0
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CCreatureVector& out) const;
};
// Stand-in for the 0x008E3EB0 instantiation (as in the landed 0x00977B10): same TU, the vector's bounds pass by value and come back.
void GatherRoughArea_008e3eb0(const CThingSearchTools* tools, long minX, long minY, long maxX, long maxY, const CThingFilter_HasDefGlobalIndex& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CCreatureVector& out) const
{
    CThingAICreature** start = out._M_start;
    CThingAICreature** finish = out._M_finish;
    CThingAICreature** endOfStorage = out._M_end_of_storage;
    GatherRoughArea_008e3eb0(this, area.MinX, area.MinY, area.MaxX, area.MaxY, filter, &start, &finish, &endOfStorage);
    out._M_start = start;
    out._M_finish = finish;
    out._M_end_of_storage = endOfStorage;
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;              // +0x20
};
class CTCHealLife { public:
    void SetState(long state);                        // 0x007C3EA0
};

namespace NCreatureMode {
struct CModeBase {                                    // vtable 0x012673CC
    virtual ~CModeBase();
    CThing*       m_pThing;                           // +0x04
    unsigned char _pad08[0x14];
    CWorld* GetWorld() const;                         // 0x0073A680
};
struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                       // +0x1c .. +0x68
    void OnActivate();                                // 0x008621A0
};
struct CModeGuildmasterPowerUpHeal : CModeStandBase {
    virtual void OnActivate();
};

void CModeGuildmasterPowerUpHeal::OnActivate()
{
    CModeStandBase::OnActivate();
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(true, this);
    CCreatureVector bodyguards;
    long bodyguardDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_GUILDMASTER_BODYGUARD");
    {
        CThingFilter_HasDefGlobalIndex filter(bodyguardDef);
        const C3DVector& pos = m_pThing->GetPosition();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        C2DBoxI area(centre.X - 20, centre.Y - 20, centre.X + 20, centre.Y + 20);
        CWorld* world = GetWorld();
        world->ThingSearchTools->GetAllThingsInRoughArea(area, filter, bodyguards);
    }
    for (unsigned long i = 0; i < bodyguards.size(); ++i) {
        CThingAICreature* guard = bodyguards[i];
        if (guard && !(guard->m_flags91 & 1)) {
            CDefPointer<CSpecialEffectsDef> effects;
            long effect = 0;
            m_pThing->GetDef()->GetSubDef(&effects);
            if (effects.Object) {
                bool found;
                {
                    CCharString name("HEAL");
                    found = effects->CalcAndLookup(name, &effect);
                }
                if (found) {
                    CThing* healEffect = CreateSpecialEffect(effect, m_pThing->GetPosition(), false);
                    if (healEffect && !(healEffect->m_flags91 & 1) && healEffect->HasTC(0x4b)) {
                        CTCDParticleEmitter* emitter = (CTCDParticleEmitter*)healEffect->PTCDriver;
                        emitter->AttachToThing(*m_pThing, 10, "", 0, 0.0f);
                    }
                }
            }
            if (!m_pThing->HasTC(0x99)) {
                CTCHealLife* heal = (CTCHealLife*)m_pThing->AddTC("CTCHealLife", true, 0);
                heal->SetState(1);
            }
        }
    }
}
}

// ---- stubs ----
#undef free
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
void CSpecialEffectsDef::v00() {}
static int g_effectsDestroyed = 0;
void CSpecialEffectsDef::Destroy() { ++g_effectsDestroyed; }
void CDefClassBase::v00() {} void CDefClassBase::Destroy() {} void CDefClassBase::v08() {} void CDefClassBase::v0c() {}
void CDefClassBase::v10() {} void CDefClassBase::v14() {} void CDefClassBase::v18() {} void CDefClassBase::v1c() {}
void CDefClassBase::v20() {} void CDefClassBase::v24() {} void CDefClassBase::v28() {} void CDefClassBase::v2c() {}
void CDefClassBase::v30() {} void CDefClassBase::v34() {}
static long g_subIndex = 5; static char g_subName[64];
long* CDefClassBase::GetSubDefIndex(const CCharString& name) { std::strcpy(g_subName, (const char*)name.PStringData); return &g_subIndex; }
static CSpecialEffectsDef g_effects; static long g_lookupIndex = -1;
CDefPointer<CSpecialEffectsDef> CDefinitionManager::GetPDefFromGlobalIndex(long index) { g_lookupIndex = index; return CDefPointer<CSpecialEffectsDef>(&g_effects); }
static C3DVector g_parentPos = { 103.7f, 58.2f, 4.0f };
static CThing* g_parent = 0;
const C3DVector& CThing::GetPosition() const { return g_parentPos; }
static int g_suspendCalls = 0; static bool g_suspendArg = false; static void* g_suspendMode = 0;
void CThing::SetSuspendAnimation(bool suspend, NCreatureMode::CModeBase* mode) { ++g_suspendCalls; g_suspendArg = suspend; g_suspendMode = mode; }
static int g_addTCCalls = 0; static char g_addTCName[64]; static bool g_addTCActivate = false; static CTCHealLife* g_heal = (CTCHealLife*)&g_subIndex;
CTCBase* CThing::AddTC(const CCharString& name, bool activate, const CTCParamsBase*)
{
    ++g_addTCCalls; std::strcpy(g_addTCName, (const char*)name.PStringData); g_addTCActivate = activate;
    m_presentBits[0x99 >> 5] |= 1UL << (0x99 & 31);
    return (CTCBase*)g_heal;
}
static int g_setState = -1, g_setStateCalls = 0;
void CTCHealLife::SetState(long state) { ++g_setStateCalls; g_setState = (int)state; }
static CThing g_effectThing; static int g_createCalls = 0; static long g_createEffect = -1; static bool g_createAttached = true;
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{
    ++g_createCalls; g_createEffect = effect; g_createAttached = attached;
    return &position == &g_parentPos ? &g_effectThing : 0;
}
static int g_attachCalls = 0; static CThing* g_attachThing = 0; static long g_attachBone = -1; static char g_attachName[16];
void CTCDParticleEmitter::AttachToThing(CThing& thing_ref, long boneIndex, const CCharString& boneName, long, float)
{ CThing* thing = &thing_ref;
    ++g_attachCalls; g_attachThing = thing; g_attachBone = boneIndex; std::strcpy(g_attachName, (const char*)boneName.PStringData);
}
static CGameDefinitionManager g_gdm; static char g_defName[64];
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { std::strcpy(g_defName, (const char*)name.PStringData); return 77; }
static CThingAICreature g_alive1, g_alive2, g_dead;
static long g_box[4]; static long g_filterIndex = -1;
void GatherRoughArea_008e3eb0(const CThingSearchTools*, long minX, long minY, long maxX, long maxY, const CThingFilter_HasDefGlobalIndex& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage)
{
    g_box[0] = minX; g_box[1] = minY; g_box[2] = maxX; g_box[3] = maxY; g_filterIndex = filter.DefGlobalIndex;
    CThingAICreature** v = (CThingAICreature**)std::malloc(4 * sizeof(CThingAICreature*));
    v[0] = &g_alive1; v[1] = &g_dead; v[2] = 0; v[3] = &g_alive2;
    *start = v; *finish = v + 4; *endOfStorage = v + 4;
}
static CThingSearchTools g_tools; static CWorld g_world;
namespace NCreatureMode {
CModeBase::~CModeBase() {}
CWorld* CModeBase::GetWorld() const { return &g_world; }
static int g_baseActivate = 0;
void CModeStandBase::OnActivate() { ++g_baseActivate; }
}

int main()
{
    static CThing parent; static CDefClassBase def; static CDefinitionManager manager;
    std::memset(&parent, 0, sizeof(parent));
    parent.m_type = 1; parent.m_pDef = &def; def.m_pManager = &manager; def.RefCount = 1; g_effects.RefCount = 1;
    g_dead.m_flags91 = 1;
    std::memset(&g_effectThing, 0, sizeof(g_effectThing));
    static CTCDParticleEmitter emitter; g_effectThing.PTCDriver = (CTCBase*)&emitter;
    g_effectThing.m_presentBits[0x4b >> 5] |= 1UL << (0x4b & 31);
    g_world.ThingSearchTools = &g_tools;
    NCreatureMode::CModeGuildmasterPowerUpHeal mode;
    mode.m_pThing = &parent;
    mode.OnActivate();
    CHECK(NCreatureMode::g_baseActivate == 1);
    CHECK(g_suspendCalls == 1 && g_suspendArg && g_suspendMode == (void*)&mode);
    CHECK(std::strcmp(g_defName, "CREATURE_GUILDMASTER_BODYGUARD") == 0 && g_filterIndex == 77);
    CHECK(g_box[0] == 83 && g_box[1] == 38 && g_box[2] == 123 && g_box[3] == 78);
    // two live bodyguards: one HEAL effect each, attached to the guildmaster on bone 10
    CHECK(g_createCalls == 2 && g_createEffect == 'H' && !g_createAttached);
    CHECK(std::strcmp(g_subName, "CSpecialEffectsDef") == 0 && g_lookupIndex == 5);
    CHECK(g_attachCalls == 2 && g_attachThing == &parent && g_attachBone == 10 && g_attachName[0] == 0);
    // the heal-life component is added once (the second bodyguard finds it present)
    CHECK(g_addTCCalls == 1 && std::strcmp(g_addTCName, "CTCHealLife") == 0 && g_addTCActivate);
    CHECK(g_setStateCalls == 1 && g_setState == 1);
    // the effects def references are balanced (one held by the stub's original only)
    CHECK(g_effects.RefCount == 1 && g_effectsDestroyed == 0 && def.RefCount == 1);
    // a non-player guildmaster does not suspend its animation
    parent.m_type = 0; g_suspendCalls = 0;
    mode.OnActivate();
    CHECK(g_suspendCalls == 0);
    if (fails == 0) printf("008ddd70_TEST PASS\n");
    return fails;
}