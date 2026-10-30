// CCreatureAction_SpiritAttack::CreateAttackEffect() -- look up the lightning-spell ability def and its special-effects
// Grade: DIFFER(762v723) -- header-moved (CSubDefClassBase/CDefClassBase + the engine/CDefPointer.h verify copy with the proposed ENullDef ctor; PDB callee names CGameDefinitionManager::Get / GetDefGlobalIndex / CDefinitionManager::GetDef<CSpecialAbilitiesLightningSpellDef> / CSpecialEffectsDef::GetEffectFromString): 154 difflines (local model 748v723, 161); retail holds ability.Object in edi from GetDef to the release, ours re-reads it
//   sub-def; take the effect named by the co-op spirit's action-use animation group, create it at the target and keep it
//   as the action's effect; when it is alive and has a particle emitter, attach the emitter to the spirit and feed it
//   LIGHTNING_START (the spirit's focal point) and LIGHTNING_END (the target's helper point).
// Leaf: CreateAttackEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008548B0, length 723 bytes, plain `ret`.
// Callees: GetPItem 0x00A01B50, CDefinitionManager::Get 0x0044C6B0 (static) / GetDefIndexFromName 0x009AD390 / GetDef
//   0x007A35D0, GetSubDef<CSpecialEffectsDef> 0x00669580, LowerBound 0x0040F020, CTCCoopSpirit::
//   GetActionUseAnimationGroupName 0x00670010 / GetFocalPos 0x0066FFD0, CSpecialEffectsDef lookup 0x00669D30,
//   CreateEffectThing 0x006E0880 (__fastcall), CIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor
//   0x00A01C10 (vtable 0x01238C6C), CTCDParticleEmitter::AttachToThing 0x006E0BE0 / SetEmitterUpdateParameters
//   0x006E11A0, CTCGraphicAppearance helper point 0x004C1060, CRC Calc 0x004014A0 (__fastcall), CParticleUpdateParameters
//   ctor 0x006E24D0 / Update 0x0076C750 / dtor 0x006E2500, vector insert-overflow 0x0076ADA0 / push_back 0x0076B120,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Globals: the emitter helper name (a CCharString) 0x013BAC48.
// Layout: CParticleUpdateParameters per the landed model of 0x007BE3E0 (0x24 bytes, VectorParams +0x04, Dirty +0x11);
//   action +0x08 PCreature, +0xa8 PTarget, +0xb0 PEffect; thing +0x20 present bits, +0x44 interface map, +0x64 graphic
//   appearance, +0x91 bit 0 dead (retail bytes).
struct C3DVector { float X, Y, Z; };
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
extern CCharString g_SpiritEmitterHelper;                   // 0x013BAC48
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
struct __false_type { };
struct SVectorParam {                                        // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) { }
};
struct CVectorParamVector {                                  // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    void push_back(const SVectorParam& value);               // 0x0076B120
    void _M_insert_overflow(SVectorParam* pos, const SVectorParam& x, const __false_type&, unsigned int fill,
                            bool atend);                     // 0x0076ADA0
    __forceinline void push_back_inline(const SVectorParam& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SVectorParam(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CVectorParamMap {
    CVectorParamVector Vector;                               // +0x00
    bool               Sorted;                               // +0x0c
    bool               Dirty;                                // +0x0d
};
struct CParticleUpdateParameters {                           // <unsigned long, unsigned long>, 0x24 bytes
    void*           __vftable;                               // +0x00
    CVectorParamMap VectorParams;                            // +0x04
    unsigned char   UpdateParams[0x10];                      // +0x14
    CParticleUpdateParameters();                             // 0x006E24D0
    ~CParticleUpdateParameters();                            // 0x006E2500
    void Update();                                           // 0x0076C750
};
class CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetEmitterUpdateParameters(const CParticleUpdateParameters& parameters);   // 0x006E11A0
};
struct CTCGraphicAppearance {
    C3DVector GetTargetPoint(float fraction);                // 0x004C1060
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];                  // +0x20
    CInterfaceMap         m_interfaces;                      // +0x44
    unsigned char         _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;                // +0x64
    unsigned char         _pad68[0x91 - 0x68];
    unsigned char         m_flags91;                         // +0x91 bit 0: dead
    const C3DVector* GetPosition() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long defIndex, const C3DVector* position, bool flag);   // 0x006E0880
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CSpecialEffectsDef : public CDefClassBase {
public:
    bool GetEffectFromString(const CCharString& name, long& effectIndex) const;   // 0x00669D30
};
class CSpecialAbilitiesLightningSpellDef : public CSubDefClassBase { };
class CDefinitionManager {
public:
    long GetDefGlobalIndex(const CCharString& name, bool required);              // 0x009AD390
    template <class T> bool GetDef(long index, CDefPointer<T>* out) const;        // 0x007A35D0 <CSpecialAbilitiesLightningSpellDef>
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                              // 0x0044C6B0
};
inline long GetDefIndex(const CCharString& name) { return CGameDefinitionManager::Get().GetDefGlobalIndex(name, true); }
struct CTCCoopSpirit {
    CCharString GetActionUseAnimationGroupName();            // 0x00670010
    C3DVector GetFocalPos();                                 // 0x0066FFD0
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    ~CIntelligentPointerThing();                             // 0x00A01C10
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct CCreatureAction_SpiritAttack {
    virtual void v00();
    unsigned char            _pad04[4];
    CBaseIntelligentPointer  PCreature;                      // +0x08
    unsigned char            _pad10[0xa8 - 0x10];
    CIntelligentPointerThing PTarget;                        // +0xa8
    CIntelligentPointerThing PEffect;                        // +0xb0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    void CreateAttackEffect();
};

void CCreatureAction_SpiritAttack::CreateAttackEffect()
{
    CThing* target = PTarget.GetPItem();
    long abilityIndex = GetDefIndex(CCharString("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF"));
    CDefPointer<CSpecialAbilitiesLightningSpellDef> ability(NULL_DEF);
    CGameDefinitionManager::Get().GetDef(abilityIndex, &ability);
    CDefPointer<const CSpecialEffectsDef> effects(NULL_DEF);
    if (ability->GetSubDef(&effects)) {
        CTCCoopSpirit* spirit;
        GetCreature()->GetInterface(&spirit, 0xc7);
        long effectIndex;
        effects->GetEffectFromString(spirit->GetActionUseAnimationGroupName(), effectIndex);
        CThing* effect = CreateEffectThing(effectIndex, target->GetPosition(), false);
        PEffect = CIntelligentPointerThing(effect);
        CTCDParticleEmitter* emitter;
        if (PEffect.GetPItem() && !(PEffect.GetPItem()->m_flags91 & 1)
            && PEffect.GetPItem()->GetInterface(&emitter, 0x4b) && emitter) {
            emitter->AttachToThing(GetCreature(), 0xc, g_SpiritEmitterHelper, 0, 0.0f);
            C3DVector start = spirit->GetFocalPos();
            C3DVector end = target->PGraphicAppearance->GetTargetPoint(1.0f);
            SVectorParam startParam(Calc_004014a0(0, "LIGHTNING_START", 15), start);
            SVectorParam endParam(Calc_004014a0(0, "LIGHTNING_END", 13), end);
            CParticleUpdateParameters parameters;
            parameters.VectorParams.Vector.push_back_inline(startParam);
            parameters.VectorParams.Dirty = true;
            parameters.VectorParams.Vector.push_back(endParam);
            parameters.Update();
            emitter->SetEmitterUpdateParameters(parameters);
        }
    }
}
