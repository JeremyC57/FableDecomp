// CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase*) -- combat action 0x91 with no sounds and Level 0; animation ST_MULTI_ARROW_CAST; load SPECIAL_ABILITIES_MULTI_ARROW_DEF, play its cast sound through the creature's sound component and spawn its CAST_EFFECT particle effect at the creature, attached to the creature's left hand
// Leaf: ??0CCreatureAction_CastSkillMultiArrow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE6F0, length 522 bytes (+ int3 padding), `ret 4`, no EH frame.
// Callees: CCombatActionBase ctor 0x00858030, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetAnimationName 0x008574D0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall),
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
    const C3DVector& GetPos() const;             // 0x004C73D0
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
    void SetAnimationName(const CCharString& name);   // 0x008574D0
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
    SetAnimationName("ST_MULTI_ARROW_CAST");
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
            CThing* effect = CreateParticleEffect(effectType, &GetCreature()->GetPos());
            CTCDParticleEmitter* emitter;
            if (effect && !effect->IsDead() && effect->GetInterface(&emitter, 0x4b))
                emitter->AttachToThing(GetCreature(), 10, g_LeftHandAttachmentName, 0, 0);
        }
    }
}