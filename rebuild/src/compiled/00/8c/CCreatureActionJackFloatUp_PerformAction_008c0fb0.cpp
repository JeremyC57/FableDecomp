// CCreatureAction_JackFloatUp::PerformAction() -- float speed = FloatHeight over the animation time left, start moving; then
// spawn the def's RISE_EFFECT at Jack, and its FLOAT_EFFECT, which (while it lives) joins Jack's 0xAD dependent things and
// has its particle emitter attached to Jack's TARGET_CURSOR_POINT helper
// Leaf: ?PerformAction@CCreatureAction_JackFloatUp
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18): FloatHeight +0x114, FloatSpeed +0x118, Moving +0x11c;
//   RunningTimeSeconds +0x40; CThing interface bitset +0x20, map +0x44, PTCDriver +0x5c, def +0x70, flags +0x91.
// Shape: the landed 0x008C4B80 model (counted def reference, same-TU GetSubDef / GetEffectFromString / LowerBound stand-ins,
//   explicit release inside if (effects)).
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
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
struct CDefClassReference {                           // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CTCDParticleEmitter {
public:
    void AttachToThing(class CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    CTCDParticleEmitter* PTCDriver;                   // +0x5c (the cached interface-0x4b component)
    unsigned char  _pad60[0x70 - 0x60];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    float GetAnimDuration(long animation);            // 0x006627F0 (CThingCreatureBase)
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CTCAD {                                        // interface 0xAD
    void AddDependentThing(CThing* thing);            // 0x007AF6C0
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
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual long GetAnimationIndex();                 // slot 7 (+0x1c)
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x40 - 0x10];
    float RunningTimeSeconds;                         // +0x40
    unsigned char _pad44[0x114 - 0x44];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    float FloatHeight;                                // +0x114 (PDB +0x12c)
    float FloatSpeed;                                 // +0x118
    bool  Moving;                                     // +0x11c
    float StartHeight;                                // +0x120
    void PerformAction();
};

void CCreatureAction_JackFloatUp::PerformAction()
{
    FloatSpeed = FloatHeight / (GetCreature()->GetAnimDuration(GetAnimationIndex()) - RunningTimeSeconds);
    Moving = true;
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassReference(GetCreature()->m_pDef)->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("RISE_EFFECT", effectType))
            CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
        if (effects->GetEffectFromString("FLOAT_EFFECT", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &GetCreature()->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1)) {
                CTCAD* dependents = 0;
                GetCreature()->GetInterface(&dependents, 0xad);
                dependents->AddDependentThing(effect);
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->PTCDriver;
                    emitter->AttachToThing(GetCreature(), 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}