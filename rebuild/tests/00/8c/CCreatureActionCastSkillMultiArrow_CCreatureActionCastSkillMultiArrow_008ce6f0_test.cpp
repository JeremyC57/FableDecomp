#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#define __forceinline inline
#endif
// CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase*) -- combat action 0x91 with no sounds and Level 0; animation ST_MULTI_ARROW_CAST; load SPECIAL_ABILITIES_MULTI_ARROW_DEF, play its cast sound through the creature's sound component and spawn its CAST_EFFECT particle effect at the creature, attached to the creature's left hand
// Leaf: ??0CCreatureAction_CastSkillMultiArrow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE6F0, length 522 bytes (+ int3 padding), `ret 4`, no EH frame.
// Callees: CCombatActionBase ctor 0x00858030, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall),
//   GetDefIndexFromName 0x009AD390, GetDef<> 0x007CC8E0, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CDefStringTable::GetString 0x009D49B0 (g_DefStringTable
//   0x013CA828), the sound component's PlaySound 0x0066BD00, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateParticleEffect 0x0072AB70 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0 (bone 10, g_LeftHandAttachmentName 0x013BAC34).
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18): Level +0x114 (PDB +0x12c); the multi-arrow def's cast sound
//   (CDefString) at +0x60; CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
// Shape: the landed one-argument-longer sibling 0x008CE900 model (same-TU GetDef / GetSubDef / GetEffectFromString /
//   LowerBound stand-ins so every out slot lands in the dead argument slots). The new tail is a single && chain
//   `effect && !IsDead() && GetInterface(&emitter, 0x4b)`: a separate HasTC test before GetInterface tests the bit twice.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CDefString { public: long Index; };
struct CDefStringTable {
    CCharString GetString(CDefString name);           // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;              // 0x013CA828
struct CDefVirtual {
    virtual ~CDefVirtual();                           // slot 0
    virtual void Destroy();                           // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                  // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
    T* operator->() const { return m_p; }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CSpecialAbilitiesMultiArrowDef : public CDefClassBase { public:
    unsigned char _pad08[0x60 - 0x08];
    CDefString    CastSound;                          // +0x60
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
    long GetDefIndexFromName(const CCharString& name, long column);       // 0x009AD390
    bool GetDef(long index, CDefPointer<CSpecialAbilitiesMultiArrowDef>* out);   // 0x007CC8E0
};
void* FindDef_CGameDefinitionManager_GetDef(long index);
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CSpecialAbilitiesMultiArrowDef>* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef(index);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CTCSound {                                     // the interface-0x10 component
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
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

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    bool HasTC(long interfaceType) const { return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b);   // 0x006E0BE0
};
extern CCharString g_LeftHandAttachmentName;          // 0x013BAC34
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position);   // 0x0072AB70
class CSoundPair { public:
    long First, Second;
    CSoundPair() : First(0), Second(0) { }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                  // +0x04
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase* creature, long type, const CSoundPair& sounds);   // 0x00858030
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_CastSkillMultiArrow : public CCombatActionBase {
public:
    long Level;                                       // +0x114 (PDB +0x12c)
    CCreatureAction_CastSkillMultiArrow(CThingCreatureBase* creature);
    virtual void v00();
};

CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase* creature)
    : CCombatActionBase(creature, 0x91, CSoundPair()), Level(0)
{
    SetCombatAnimation("ST_MULTI_ARROW_CAST");
    CDefPointer<CSpecialAbilitiesMultiArrowDef> def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_ARROW_DEF");
        index = CGameDefinitionManager::Get().GetDefIndexFromName(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (GetCreature()->GetInterface(&sound, 0x10)) {
        CCharString soundName = g_DefStringTable.GetString(def->CastSound);
        sound->PlaySound(soundName, 1, 0, 0, -1);
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    if (def->GetSubDef(effects)) {
        if (effects->GetEffectFromString("CAST_EFFECT", effectType)) {
            CThing* effect = CreateParticleEffect(effectType, &GetCreature()->GetPosition());
            CTCDParticleEmitter* emitter;
            if (effect && !effect->IsDead() && effect->GetInterface(&emitter, 0x4b))
                emitter->AttachToThing(GetCreature(), 10, g_LeftHandAttachmentName, 0, 0);
        }
    }
}
// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_strings, g_stringDtors, g_destroyed, g_plays, g_effects, g_lookups, g_attaches;
static const char* g_anim; static const char* g_defName; static long g_defColumn; static long g_getDefIndex;
static const char* g_playName; static long g_playArgs[4]; static void* g_playThis;
static long g_effectType; static const C3DVector* g_effectPos; static const char* g_lookupName;
static void* g_defObj; static void* g_subDefObj; static long g_found = -1; static CThing* g_spawned;
static void* g_attachThis; static CThing* g_attachThing; static long g_attachBone; static const CCharString* g_attachName; static long g_attachA, g_attachB;
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { g_strings++; (void)length; }
CCharString::~CCharString() { g_stringDtors++; }
CCharString g_LeftHandAttachmentName("LEFT_HAND", -1);
CDefStringTable g_DefStringTable;
CCharString CDefStringTable::GetString(CDefString name) { return CCharString(name.Index == 42 ? "SND_MULTI_ARROW" : "?"); }
CDefVirtual::~CDefVirtual() {}
void CDefVirtual::Destroy() { g_destroyed++; }
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name) { g_lookups++; g_lookupName = (const char*)name.PStringData; (void)def; return g_found; }
void* Find_CDefClassBase_GetSubDef(const void* owner) { (void)owner; return g_subDefObj; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_mgr; }
long CGameDefinitionManager::GetDefIndexFromName(const CCharString& name, long column) { g_defName = (const char*)name.PStringData; g_defColumn = column; return 1234; }
void* FindDef_CGameDefinitionManager_GetDef(long index) { g_getDefIndex = index; return g_defObj; }
unsigned long CTCSound::PlaySound(const CCharString& name, long a, long b, long c, long d) { g_plays++; g_playThis = this; g_playName = (const char*)name.PStringData; g_playArgs[0] = a; g_playArgs[1] = b; g_playArgs[2] = c; g_playArgs[3] = d; return 1; }
void CTCDParticleEmitter::AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, long b) { g_attaches++; g_attachThis = this; g_attachThing = thing; g_attachBone = boneIndex; g_attachName = &boneName; g_attachA = a; g_attachB = b; }
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position) { g_effects++; g_effectType = type; g_effectPos = position; return g_spawned; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static long g_baseType; static long g_baseSounds;
CCombatActionBase::CCombatActionBase(CThingCreatureBase* creature, long type, const CSoundPair& sounds) { PCreature.m_pItem = creature; g_baseType = type; g_baseSounds = sounds.First | sounds.Second; }
void CCombatActionBase::v00() {}
void CCombatActionBase::SetCombatAnimation(const CCharString& name) { g_anim = (const char*)name.PStringData; }
void CCreatureAction_CastSkillMultiArrow::v00() {}
static void Reset() { g_strings = g_stringDtors = g_destroyed = g_plays = g_effects = g_lookups = g_attaches = 0; g_anim = g_defName = g_playName = g_lookupName = 0; g_effectPos = 0; g_effectType = -5; g_baseType = 0; g_baseSounds = -1; g_attachThing = 0; g_attachThis = 0; }
int main()
{
    static CThingCreatureBase creatureObj; CThingCreatureBase* creature = &creatureObj;
    static CTCSound sound;
    CInterfaceMap::SEntry e[] = { { 0x05, 0 }, { 0x10, (CTCBase*)&sound } };
    creature->m_presentBits[0] = 1UL << 0x10; creature->m_interfaces.first = e; creature->m_interfaces.last = e + 2;
    static CSpecialAbilitiesMultiArrowDef def; def.m_refCount = 1; def.CastSound.Index = 42; g_defObj = &def;
    static CSpecialEffectsDef effects; effects.m_refCount = 1; g_subDefObj = &effects; g_found = 17;
    static CThing spawned; static CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry se[] = { { 0x20, 0 }, { 0x4b, (CTCBase*)&emitter }, { 0x50, 0 } };
    spawned.m_presentBits[2] = 1UL << (0x4b & 31); spawned.m_interfaces.first = se; spawned.m_interfaces.last = se + 3; spawned.m_flags91 = 0;
    g_spawned = &spawned;
    // full path: sound played, effect spawned at the creature and attached to its left hand
    Reset();
    {
        CCreatureAction_CastSkillMultiArrow action(creature);
        CHECK(action.Level == 0 && g_baseType == 0x91 && g_baseSounds == 0 && strcmp(g_anim, "ST_MULTI_ARROW_CAST") == 0);
    }
    CHECK(strcmp(g_defName, "SPECIAL_ABILITIES_MULTI_ARROW_DEF") == 0 && g_defColumn == 1 && g_getDefIndex == 1234);
    CHECK(g_plays == 1 && g_playThis == &sound && strcmp(g_playName, "SND_MULTI_ARROW") == 0 && g_playArgs[0] == 1 && g_playArgs[1] == 0 && g_playArgs[2] == 0 && g_playArgs[3] == -1);
    CHECK(g_lookups == 1 && strcmp(g_lookupName, "CAST_EFFECT") == 0 && g_effects == 1 && g_effectType == 17 && g_effectPos == &g_pos);
    CHECK(g_attaches == 1 && g_attachThis == &emitter && g_attachThing == creature && g_attachBone == 10 && g_attachName == &g_LeftHandAttachmentName && g_attachA == 0 && g_attachB == 0);
    CHECK(g_destroyed == 2);   // both def pointers released (ref 1 -> 0)
    // spawned effect already dead: no attach
    def.m_refCount = 2; effects.m_refCount = 2; spawned.m_flags91 = 1; Reset();
    { CCreatureAction_CastSkillMultiArrow action(creature); }
    CHECK(g_effects == 1 && g_attaches == 0 && g_destroyed == 0 && def.m_refCount == 1 && effects.m_refCount == 1);
    // no emitter interface: no attach
    def.m_refCount = 1; effects.m_refCount = 1; spawned.m_flags91 = 0; spawned.m_presentBits[2] = 0; Reset();
    { CCreatureAction_CastSkillMultiArrow action(creature); }
    CHECK(g_effects == 1 && g_attaches == 0 && g_destroyed == 2);
    // spawn failed (null): no attach
    def.m_refCount = 1; effects.m_refCount = 1; g_spawned = 0; Reset();
    { CCreatureAction_CastSkillMultiArrow action(creature); }
    CHECK(g_effects == 1 && g_attaches == 0);
    // no sound component, effect name unknown: nothing spawned
    def.m_refCount = 2; effects.m_refCount = 2; creature->m_presentBits[0] = 0; g_found = -1; Reset();
    { CCreatureAction_CastSkillMultiArrow action(creature); }
    CHECK(g_plays == 0 && g_lookups == 1 && g_effects == 0 && g_destroyed == 0 && def.m_refCount == 1 && effects.m_refCount == 1 && g_strings == g_stringDtors);
    // no special-effects sub-def: no lookup
    g_subDefObj = 0; Reset();
    { CCreatureAction_CastSkillMultiArrow action(creature); }
    CHECK(g_lookups == 0 && g_effects == 0 && g_destroyed == 1);
    if (fails == 0) printf("008ce6f0_TEST PASS\n"); else printf("008ce6f0_TEST FAIL\n");
    return fails;
}