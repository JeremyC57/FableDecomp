// CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase*) -- combat action 0x91 with no sounds and Level 0; animation ST_MULTI_ARROW_CAST; load SPECIAL_ABILITIES_MULTI_ARROW_DEF, play its cast sound through the creature's sound component and spawn its CAST_EFFECT particle effect at the creature, attached to the creature's left hand
// Leaf: ??0CCreatureAction_CastSkillMultiArrow
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE6F0, length 522 bytes (+ int3 padding), `ret 4`, no EH frame.
// Callees: CCombatActionBase ctor 0x00858030, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall),
//   GetDefIndexFromName 0x009AD390, GetDef<> 0x007CC8E0, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CDefStringTable::GetString 0x009D49B0 (GDefStringTable
//   0x013CA828), the sound component's PlaySound 0x0066BD00, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateParticleEffect 0x0072AB70 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0 (bone 10, g_LeftHandAttachmentName 0x013BAC34).
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18): Level +0x114 (PDB +0x12c); the multi-arrow def's cast sound
//   (CDefString) at +0x60; CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
// Shape: the landed one-argument-longer sibling 0x008CE900 model (same-TU GetDef / GetSubDef / GetEffectFromString /
//   LowerBound stand-ins so every out slot lands in the dead argument slots). The new tail is a single && chain
//   `effect && !IsDead() && GetInterface(&emitter, 0x4b)`: a separate HasTC test before GetInterface tests the bit twice.
class CTCBase;
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CDefString { public: long Index; };
class CDefStringTable { public:
    CCharString GetString(CDefString name);           // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)              // 0x013CA828
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
class CDefinitionManager { public:
    long GetGlobalIndexFromClassIndex(CCharString const&a0, long a1) const;   // 0x009AD390 (Ego_r.pdb ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z)
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                      // 0x0044C6B0
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
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
};
#pragma auto_inline(on)
struct CTCSound {                                     // the interface-0x10 component
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    bool HasTC(long interfaceType) const { return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
extern CCharString g_LeftHandAttachmentName;          // 0x013BAC34
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position);   // 0x0072AB70
class CSoundPair { public:
    long First, Second;
    CSoundPair() : First(0), Second(0) { }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase {
public:
    CCombatActionBase(CThingCreatureBase* creature, long type, const CSoundPair& sounds);   // 0x00858030
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
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
        index = ((CDefinitionManager*)&(CGameDefinitionManager::Get()))->GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (GetCreature()->GetInterface(&sound, 0x10)) {
        CCharString soundName = GDefStringTable.GetString(def->CastSound);
        ((CTCSoundPlayer*)sound)->PlayAttachedCriteria(soundName, 1, 0, 0, -1);
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    if (def->GetSubDef(effects)) {
        if (effects->GetEffectFromString("CAST_EFFECT", effectType)) {
            CThing* effect = CreateParticleEffect(effectType, &GetCreature()->GetPos());
            CTCDParticleEmitter* emitter;
            if (effect && !effect->IsDead() && effect->GetInterface(&emitter, 0x4b))
                emitter->AttachToThing(*GetCreature(), 10, g_LeftHandAttachmentName, 0, 0);
        }
    }
}