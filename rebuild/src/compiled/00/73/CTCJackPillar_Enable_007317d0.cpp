// CTCJackPillar::Enable(bool enable) -- switch the pillar's physics and its switchable navigation; enabling plays the RISE animation, spawns the def's RISE_UP_EFFECT at the pillar and shows it, disabling hides it unless the game is loading
// Leaf: Enable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007317D0, 413 bytes (+int3), `ret 4`, esp frame (speed TU).
// Callees: the physics' slots 36/43/44 (+0x90 / +0xAC / +0xB0; names descriptive), CVectorMap::LowerBound @ 0x0040F020
//   (inline GetInterface 0x79 / 0x83 over the bitset words +0x2C / +0x30), CTCSwitchableNavigation 0x00787580 (manifest
//   "InitialActivate"; takes the bool), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CTCAnimatingObject::PlayAnimation @ 0x007F90B0, CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CThing::GetPosition @ 0x004C73D0, CreateEffectThing @ 0x006E0880
//   (`__fastcall`), the thing's slot 28 (+0x70), the main game component's slot 9 (+0x24) (engine 0x013B86A0).
// Shape: the navigation lookup is the out-parameter GetInterface (one bitset test); the animating component is a named local
//   taken from GetTC before the "RISE" temporary (miss path reads the dead key slot); GetSubDef / GetEffectFromString are the
//   same-TU noinline stand-ins of the landed CCreatureActionJackFloatUp::PerformAction 0x008C0FB0 (the effect type shares the
//   frame word left by the first push); the sub-def is released by hand after SetVisible(true).
// Strings: "RISE" @ 0x01266F98, "RISE_UP_EFFECT" @ 0x01266F88.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
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
struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetCollisionEnabled(bool enabled);   // slot 36 (+0x90) (name descriptive)
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Deactivate(long flags);              // slot 43 (+0xAC) (name descriptive)
    virtual void Activate();                          // slot 44 (+0xB0) (name descriptive)
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetVisible(bool visible);           // slot 28 (+0x70) (name descriptive)
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CPhysics*      PPhysics;                          // +0x60
    unsigned char  _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                            // +0x70
    const C3DVector& GetPosition() const;             // 0x004C73D0
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
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* component;
        GetInterface(&component, interfaceType);
        return component;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
class CTCSwitchableNavigation { public:                      // interface 0x79
    void SetEnabled(bool enabled);                    // 0x00787580 (manifest "InitialActivate")
};
class CTCAnimatingObject { public:                           // interface 0x83
    void PlayAnimation(const CCharString& name, long a, bool b);   // 0x007F90B0
};
class CMainGameComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsLoading();                         // slot 9 (+0x24) (name descriptive)
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

class CTCJackPillar { public:
    virtual void v00();
    CThing* ParentThing;                              // +0x04
    void Enable(bool enable);
};

void CTCJackPillar::Enable(bool enable)
{
    ParentThing->PPhysics->SetCollisionEnabled(enable);
    if (enable)
        ParentThing->PPhysics->Activate();
    else
        ParentThing->PPhysics->Deactivate(0);
    CTCSwitchableNavigation* navigation;
    if (ParentThing->GetInterface(&navigation, 0x79))
        navigation->SetEnabled(enable);
    if (enable) {
        CTCAnimatingObject* animating = ParentThing->GetTC<CTCAnimatingObject>(0x83);
        animating->PlayAnimation("RISE", 0, false);
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        CDefClassReference(ParentThing->m_pDef)->GetSubDef(effectsSlot);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (effects) {
            effects->GetEffectFromString("RISE_UP_EFFECT", effectType);
            if (effectType)
                CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
        }
        ParentThing->SetVisible(true);
        if (effects && --effects->m_refCount == 0)
            effects->Destroy();
    } else {
        if (!g_pMainGameComponent->IsLoading())
            ParentThing->SetVisible(false);
    }
}