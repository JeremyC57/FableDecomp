// Behaviour test for 008e0060: final.cpp copied verbatim (CRT names wrapped and `long` forced to 32 bits under g++; build
// with `g++ -fpermissive`), every declared callee stubbed below so the calls, their arguments and the vectors are observable.
// NCreatureMode::CModeGuildmasterPowerUpShield::OnActivate()
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* CopyBlock(void* d, const void* s, unsigned int n) { return std::memmove(d, s, n); }
static void* AllocBlock(unsigned int n) { return std::malloc(n); }
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
extern "C" void* fable_memmove(void* d, const void* s, unsigned int n) { return CopyBlock(d, s, n); }
extern "C" void* fable_malloc(unsigned int n) { return AllocBlock(n); }
#define memmove fable_memmove
#define malloc fable_malloc
#define long int          // the model's longs are 32-bit words
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NCreatureMode::CModeGuildmasterPowerUpShield::OnActivate() -- probe
// Leaf: ?OnActivate@CModeGuildmasterPowerUpShield
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0060, 839 bytes, frame 0x158.
// Shape (the frame layout): three same-TU `#pragma auto_inline(off)` / `__declspec(noinline)` stand-ins for callees whose
//   retail bodies sat in this TU -- GetSubDef<CSpecialEffectsDef> (agent291's), the two vector<CIntelligentPointer<T>>
//   _M_insert_overflow instantiations (read the element and the tag, rewrite the vector words by value) and
//   CSpecialEffectsDef::CalcAndLookup (writes through its out pointer). With the overflow bodies visible the push_back
//   tag (an uninitialised, ctor-less `__false_type isPOD;`) no longer escapes, so both inlined push_backs share one
//   byte slot (`lea eax,[esp+1Fh]`) and the strings / def pointer / vector take retail's slots 0x10..0x20 (frame 0x158,
//   not 0x160); with CalcAndLookup visible the `effect = 0` store sinks next to the def-pointer zero as retail's.
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
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
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
struct CThingFilter_IsAlive { };
template <class A, class B> class CFilter_Bind2 { public:
    const A* First;
    const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
typedef CThingFilter_HasDefGlobalIndex CBodyguardFilter;
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    void SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingAICreature : public CThingCreatureBase { public: };
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
    // GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex> @ 0x008E4100
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CBodyguardFilter& filter, CCreatureVector& out) const;
};
// Stand-in for the 0x008E3EB0 instantiation (as in the landed 0x00977B10): same TU, the vector's bounds pass by value and come back.
void GatherRoughArea_008e3eb0(const CThingSearchTools* tools, long minX, long minY, long maxX, long maxY, const CBodyguardFilter& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CBodyguardFilter& filter, CCreatureVector& out) const
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
    unsigned long* GetRandomSeed();                   // 0x0049D810
};
class CTCHealLife { public:
    void SetState(long state);                        // 0x007C3EA0
};


class CCreatureActionBase { public: unsigned char nativeStorage[0xa8]; };
class CCombatActionBase : public CCreatureActionBase { public: unsigned char nativeTail[0x6c]; };
class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:
    bool ImmediateCast;                  // +0x114
    bool OneAnimCast;                    // +0x115
    unsigned char padding116[2];
    long LevelToCast;                    // +0x118
    long TimeLimit;                      // +0x11c
    float ShieldHealth;                  // +0x120
    CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase& creature, long level, bool immediate, const CCharString& anim,
                                            long timeLimit, float health);   // 0x008BB6F0
    ~CCreatureAction_CastPhysicalShieldSpell();                               // 0x0062E900
};
extern long g_FramesPerSecond;                        // 0x01375550
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerThing2 : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing2(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing2(const CIntelligentPointerThing2& other) { SetPItem(other.GetPItem()); }
};
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* where) { return where; }
#else
#include <new>
#endif
struct __false_type { };
template <class T> struct CPointerVector {          // STLport vector<CIntelligentPointer<T>>, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(T* position, const T& x, const __false_type& tag, unsigned long fillLength, bool atEnd);
    void push_back(const T& x)
    {
        __false_type isPOD;
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, isPOD, 1, true);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// Same-TU stand-ins for the two _M_insert_overflow instantiations (0x004AC940 / 0x007AFD10): the retail bodies sat in this
// TU, so VC7.1 knows the element and the tag are only read and the vector words are rewritten by value.
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CPointerVector<T>::_M_insert_overflow(T* position, const T& x, const __false_type& tag, unsigned long fillLength, bool atEnd)
{
    T* start = _M_start;
    T* finish = _M_finish;
    unsigned long count = finish - start;
    unsigned long capacity = count ? count * 2 : 1;
    T* block = (T*)malloc(capacity * sizeof(T));
    T* dst = block;
    T* src = start;
    for (; src != position; ++src, ++dst)
        new (dst) T(*src);
    for (unsigned long i = 0; i < fillLength; ++i, ++dst)
        new (dst) T(x);
    if (!atEnd)
        for (; src != finish; ++src, ++dst)
            new (dst) T(*src);
    if (start)
        free(start);
    _M_start = block;
    _M_finish = dst;
    _M_end_of_storage = block + capacity;
}
#pragma auto_inline(on)
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
inline void EraseCreature(CCreatureVector& v, CThingAICreature** position)
{
    CThingAICreature** next = position + 1;
    if (next != v._M_finish)
        memmove(position, next, (char*)v._M_finish - (char*)next);
    --v._M_finish;
}
unsigned long __fastcall GFROR13(unsigned long value);       // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}

// Same-TU stand-in for CSpecialEffectsDef::CalcAndLookup 0x00669D30 (reads the name, writes the looked-up value through
// the pointer, nothing escapes): VC7.1 then sinks the `effect = 0` store with the def-pointer zero as retail does.
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
struct CModeGuildmasterPowerUpShield : CModeStandBase {
    unsigned char _pad68[0x10];                       // +0x68
    CPointerVector<CIntelligentPointerThing2> Effects;          // +0x78 (overflow 0x004AC940)
    CPointerVector<CIntelligentPointerCreature> ShieldTargets;   // +0x84 (overflow 0x007AFD10)
    virtual void OnActivate();
};

void CModeGuildmasterPowerUpShield::OnActivate()
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
    {
        long effect = 0;
        CDefPointer<CSpecialEffectsDef> effects;
        m_pThing->GetDef()->GetSubDef(&effects);
        {
            CCharString name("POWER_UP_BEAM");
            effects->CalcAndLookup(name, &effect);
        }
        while (bodyguards.size() > 0 && ShieldTargets.size() < 3) {
            unsigned long count = bodyguards.size();
            unsigned long r = GFRandom(GetWorld()->GetRandomSeed());
            unsigned long index = count ? r % count : 0;
            long timeLimit = (long)((float)g_FramesPerSecond * 12.0f);
            bodyguards._M_start[index]->SetCurrentAction(CCreatureAction_CastPhysicalShieldSpell(*bodyguards._M_start[index], 2, true, "", timeLimit, 10.0f));
            ShieldTargets.push_back(CIntelligentPointerCreature(bodyguards._M_start[index]));
            EraseCreature(bodyguards, &bodyguards._M_start[index]);
            CThing* fx = CreateSpecialEffect(effect, m_pThing->GetPosition(), false);
            if (fx && !(fx->m_flags91 & 1))
                Effects.push_back(CIntelligentPointerThing2(fx));
        }
    }
}
}

// ---- stubs ----
using namespace NCreatureMode;
static int g_standActivate, g_suspend, g_defNameCalls, g_gather, g_actions, g_effectsCreated, g_destroys, g_subDefIndexCalls, g_getPDef;
static CModeBase* g_suspendMode; static bool g_suspendFlag; static const char* g_defName; static long g_bodyguardIndex = 77;
static long g_gatherBox[4]; static long g_gatherFilter; static CThingAICreature* g_pool[8]; static int g_poolCount;
static CThingCreatureBase* g_actionCreature[4]; static long g_actionLevel[4]; static bool g_actionImmediate[4]; static const char* g_actionAnim[4]; static long g_actionTime[4]; static float g_actionHealth[4];
static long g_fxEffect[4]; static const C3DVector* g_fxPos[4]; static bool g_fxAttached[4]; static CThing g_fxThings[4]; static bool g_fxDead;
static unsigned long g_seed; static long g_subDefIndex = 9; static CSpecialEffectsDef g_effectsDef; static CDefinitionManager g_defManager;
static C3DVector g_position = { 100.6f, -40.2f, 7.0f };
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() { PStringData = 0; }
void CSpecialEffectsDef::v00() { }
void CSpecialEffectsDef::Destroy() { ++g_destroys; }
CDefPointer<CSpecialEffectsDef> CDefinitionManager::GetPDefFromGlobalIndex(long index) { ++g_getPDef; CHECK(index == g_subDefIndex); return CDefPointer<CSpecialEffectsDef>(&g_effectsDef); }
void CDefClassBase::v00() { } void CDefClassBase::Destroy() { ++g_destroys; } void CDefClassBase::v08() { } void CDefClassBase::v0c() { } void CDefClassBase::v10() { }
void CDefClassBase::v14() { } void CDefClassBase::v18() { } void CDefClassBase::v1c() { } void CDefClassBase::v20() { } void CDefClassBase::v24() { }
void CDefClassBase::v28() { } void CDefClassBase::v2c() { } void CDefClassBase::v30() { } void CDefClassBase::v34() { }
long* CDefClassBase::GetSubDefIndex(const CCharString& name) { ++g_subDefIndexCalls; CHECK(strcmp((const char*)name.PStringData, "CSpecialEffectsDef") == 0); return &g_subDefIndex; }
const C3DVector& CThing::GetPosition() const { return g_position; }
void CThing::SetSuspendAnimation(bool suspend, CModeBase* mode) { ++g_suspend; g_suspendFlag = suspend; g_suspendMode = mode; }
CTCBase* CThing::AddTC(const CCharString&, bool, CTCParamsBase*) { return 0; }
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached)
{
    if (g_effectsCreated < 4) { g_fxEffect[g_effectsCreated] = effect; g_fxPos[g_effectsCreated] = &position; g_fxAttached[g_effectsCreated] = attached; }
    CThing* fx = &g_fxThings[g_effectsCreated % 4]; fx->m_flags91 = g_fxDead ? 1 : 0; ++g_effectsCreated; return fx;
}
void CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { m_pItem = 0; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
void CIntelligentPointerCreature::v00() { }
void CIntelligentPointerThing2::v01() { }
static CGameDefinitionManager g_gameDefs;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gameDefs; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { ++g_defNameCalls; g_defName = (const char*)name.PStringData; return g_bodyguardIndex; }
void GatherRoughArea_008e3eb0(const CThingSearchTools*, long minX, long minY, long maxX, long maxY, const CBodyguardFilter& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage)
{
    ++g_gather; g_gatherBox[0] = minX; g_gatherBox[1] = minY; g_gatherBox[2] = maxX; g_gatherBox[3] = maxY; g_gatherFilter = filter.DefGlobalIndex;
    CThingAICreature** block = (CThingAICreature**)AllocBlock(8 * sizeof(CThingAICreature*));
    for (int i = 0; i < g_poolCount; ++i) block[i] = g_pool[i];
    *start = block; *finish = block + g_poolCount; *endOfStorage = block + 8;
}
static unsigned long g_seedStore;
unsigned long* CWorld::GetRandomSeed() { return &g_seedStore; }
void CTCHealLife::SetState(long) { }
CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase& creature, long level, bool immediate, const CCharString& anim, long timeLimit, float health)
{
    if (g_actions < 4) { g_actionCreature[g_actions] = &creature; g_actionLevel[g_actions] = level; g_actionImmediate[g_actions] = immediate; g_actionAnim[g_actions] = (const char*)anim.PStringData; g_actionTime[g_actions] = timeLimit; g_actionHealth[g_actions] = health; }
    ++g_actions; LevelToCast = level; TimeLimit = timeLimit; ShieldHealth = health; ImmediateCast = immediate; OneAnimCast = false;
}
CCreatureAction_CastPhysicalShieldSpell::~CCreatureAction_CastPhysicalShieldSpell() { }
long g_FramesPerSecond = 30;
unsigned long __fastcall GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
CModeBase::~CModeBase() { }
static CWorld g_world; static CThingSearchTools g_tools;
CWorld* CModeBase::GetWorld() const { return &g_world; }
void CModeStandBase::OnActivate() { ++g_standActivate; }
void CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) { }
static CDefClassBase g_thingDef;

struct Fixture {
    CModeGuildmasterPowerUpShield mode; CThing thing; CThingAICreature guards[8];
    Fixture(int guardCount, unsigned char thingType)
    {
        memset(&mode, 0, sizeof(mode)); mode.m_pThing = &thing;
        memset(&thing, 0, sizeof(thing)); thing.m_type = thingType; thing.m_pDef = &g_thingDef;
        memset(guards, 0, sizeof(guards)); g_poolCount = guardCount; for (int i = 0; i < guardCount; ++i) g_pool[i] = &guards[i];
        g_thingDef.RefCount = 1; g_thingDef.m_pManager = &g_defManager; g_effectsDef.RefCount = 1; g_world.ThingSearchTools = &g_tools;
        g_standActivate = g_suspend = g_defNameCalls = g_gather = g_actions = g_effectsCreated = g_destroys = g_subDefIndexCalls = g_getPDef = 0;
        g_seedStore = 12345; g_fxDead = false; g_FramesPerSecond = 30;
    }
    ~Fixture() { if (mode.Effects._M_start) free(mode.Effects._M_start); if (mode.ShieldTargets._M_start) free(mode.ShieldTargets._M_start); }
};
static long ExpectedIndex(unsigned long& seed, long count) { seed = seed * 0x24a1 + 0x24df; seed = GFROR13(seed); return count ? (long)(seed % (unsigned long)count) : 0; }

int main()
{
    {   // the player creature with four bodyguards: three get the shield action and an effect each
        Fixture f(4, 1);
        unsigned long seed = 12345; long order[3]; CThingAICreature* remaining[8]; int n = 4; for (int i = 0; i < 4; ++i) remaining[i] = &f.guards[i];
        CThingAICreature* chosen[3];
        for (int k = 0; k < 3; ++k) { order[k] = ExpectedIndex(seed, n); chosen[k] = remaining[order[k]]; for (int j = order[k]; j + 1 < n; ++j) remaining[j] = remaining[j + 1]; --n; }
        f.mode.OnActivate();
        CHECK(g_standActivate == 1 && g_suspend == 1 && g_suspendFlag && g_suspendMode == &f.mode);
        CHECK(g_defNameCalls == 1 && strcmp(g_defName, "CREATURE_GUILDMASTER_BODYGUARD") == 0);
        CHECK(g_gather == 1 && g_gatherFilter == 77 && g_gatherBox[0] == 80 && g_gatherBox[1] == -60 && g_gatherBox[2] == 120 && g_gatherBox[3] == -20);
        CHECK(g_subDefIndexCalls == 1 && g_getPDef == 1 && g_effectsDef.RefCount == 1 && g_thingDef.RefCount == 1 && g_destroys == 0);
        CHECK(g_actions == 3 && f.mode.ShieldTargets.size() == 3 && g_effectsCreated == 3 && f.mode.Effects.size() == 3);
        for (int k = 0; k < 3; ++k) {
            CHECK(g_actionCreature[k] == chosen[k] && g_actionLevel[k] == 2 && g_actionImmediate[k] && g_actionAnim[k][0] == 0 && g_actionTime[k] == 360 && g_actionHealth[k] == 10.0f);
            CHECK(f.mode.ShieldTargets._M_start[k].GetPItem() == chosen[k]);
            CHECK(g_fxEffect[k] == (long)'P' && g_fxPos[k] == &g_position && !g_fxAttached[k] && f.mode.Effects._M_start[k].GetPItem() == &g_fxThings[k]);
        }
        CHECK(g_seedStore == seed);
    }
    {   // not the player, two bodyguards, dead effect things: both shielded, no suspend, no effects kept
        Fixture f(2, 3); g_fxDead = true; f.mode.OnActivate();
        CHECK(g_suspend == 0 && g_actions == 2 && f.mode.ShieldTargets.size() == 2 && g_effectsCreated == 2 && f.mode.Effects.size() == 0);
    }
    {   // nobody around: nothing shielded, no random draw
        Fixture f(0, 1); f.mode.OnActivate();
        CHECK(g_actions == 0 && f.mode.ShieldTargets.size() == 0 && g_effectsCreated == 0 && g_seedStore == 12345 && g_gather == 1);
    }
    if (fails) { printf("008e0060_TEST FAIL (%d)\n", fails); return 1; }
    printf("008e0060_TEST PASS\n");
    return 0;
}