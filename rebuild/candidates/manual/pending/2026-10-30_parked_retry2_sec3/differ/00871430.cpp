// NCreatureAction::CActionEventShot::Dispatch(const CCreatureActionBase&) -- fire the strike action's shots at its target: throw the carried weapon when it is a throwable, otherwise create the shot at the weapon focus dummy or the strike helper point
// Grade: DIFFER(863v871) -- 291 difflines (base 859v871/298); the SIMPLE_NOISE string as a named block local passed by reference (retail pushes its lea after the constructor, not the constructor's eax). Left: this in ebx (ours edi) with the target/def zero stores scheduled after GetPItem, frame 0x40 vs 0x44, and register choices through the shot block.
// Leaf: ?Dispatch@CActionEventShot
// Layout: retail CThing view as the landed 0x008717A0 / 0x008C0FB0 (bitset +0x20, map +0x44, physics +0x60, appearance +0x64,
//   def +0x70, flags +0x91); CCreatureActionBase PTarget intelligent pointer at +0xa8 (retail disassembly).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet {
    C3DVector First, Second;
    CRightHandedSet(const CRightHandedSet& other);      // 0x0049CF80
};
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
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
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (*type > middle->type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();                             // slot 0
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
struct CWeaponSubDef : CDefBase {                       // the GetSubDef 0x0042B1A6 sub-definition
    unsigned char _pad08[0x2c - 0x08];
    long Type;                                          // +0x2c (3: throwable)
    unsigned char _pad30[0xa0 - 0x30];
    long NumberOfShots;                                 // +0xa0
};
struct CWeaponSubDefPointer {
    CWeaponSubDef* m_p;
    CWeaponSubDefPointer() : m_p(0) { }
    ~CWeaponSubDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CWeaponSubDef* operator->() const { return m_p; }
};
struct CMeleeCombatAbilityDef : CDefBase {
    __forceinline long Release() const { return --const_cast<CMeleeCombatAbilityDef*>(this)->m_refCount; }
};
struct CMeleeCombatAbilityDefPointer {                  // CDefPointer<const CMeleeCombatAbilityDef>
    const CMeleeCombatAbilityDef* m_p;
    CMeleeCombatAbilityDefPointer() : m_p(0) { }
    ~CMeleeCombatAbilityDefPointer() { if (m_p && m_p->Release() == 0) const_cast<CMeleeCombatAbilityDef*>(m_p)->Destroy(); }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CWeaponSubDefPointer& out);          // 0x0042B1A6
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CWeaponSubDefPointer& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (CWeaponSubDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefClassReference {                             // a counted CDefClassBase reference (AddRef / Release)
    CDefClassBase* m_p;
    __forceinline CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    __forceinline ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
class CThing;
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, float scale);   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                    // 0x006A1440
    void RemoveThing(CThing* thing);                    // 0x006A19B0
};
struct CTCShotCount {                                   // interface 0x30
    unsigned char _pad00[0x7c];
    long NumberOfShots;                                 // +0x7c
};
struct CPhysics {
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
    virtual void v73(); virtual void v74();
    virtual void SetOrientation(const CRightHandedSet& orientation);   // slot 75 (+0x12c)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CPhysics* PPhysics;                                 // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                              // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                           // +0x91 bit 0: dead
    void MakeNoise(const CCharString& noise);           // 0x00662110 (CThingCreatureBase)
    long GetShotAccuracyPercentage();                   // 0x00663B10 (CThingCreatureBase)
    void CreateShot(CThing* source, CThing* target, float accuracy, float scale, float spread, CMeleeCombatAbilityDefPointer* ability, long count);   // 0x007C0E80
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
#pragma auto_inline(off)
bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    if (thing && !(thing->m_flags91 & 1))
        return true;
    return false;
}
#pragma auto_inline(on)
CThing* __fastcall CreateShot(long count, CThing* source, CThing* through, CThing* target, float accuracy, float scale, float spread,
    CMeleeCombatAbilityDefPointer* ability, C3DVector* position, CRightHandedSet* orientation);   // 0x007C0AD0
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsStrikeAction() const;                // slot 15 (+0x3c)
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22();
    virtual CCharString GetStrikeEventHelperName() const;   // slot 23 (+0x5c)
    unsigned char _pad04[0xa8 - 0x04];
    CBaseIntelligentPointer PTarget;                    // +0xa8
    CThing* GetWeaponUsedInStrikeAction() const;        // 0x00857070
};
struct CActionEventShot {
    unsigned char _pad00[0x0c];
    CThing* SourceThing;                                // +0x0c
    void Dispatch(const CCreatureActionBase& action);
};

void CActionEventShot::Dispatch(const CCreatureActionBase& action)
{
    CThing* target = 0;
    if (action.IsStrikeAction()) {
        target = action.PTarget.GetPItem();
        if (target && !(target->m_flags91 & 1)) {
            CWeaponSubDefPointer def;
            if (SourceThing->m_pDef->GetSubDef(def)) {
                {
                    CCharString noise("SIMPLE_NOISE");
                    SourceThing->MakeNoise(noise);
                }
                CThing* source = SourceThing;
                long count;
                CTCShotCount* shotCount;
                if (source->GetInterface(&shotCount, 0x30) && shotCount->NumberOfShots > 0)
                    count = shotCount->NumberOfShots;
                else
                    count = def->NumberOfShots;
                if (count > 0) {
                    long accuracy = source->GetShotAccuracyPercentage();
                    CThing* weapon = 0;
                    CTCCarrying* carrying;
                    if (SourceThing->GetInterface(&carrying, 0x46)) {
                        weapon = carrying->GetThingInPrimarySlot();
                        if (weapon) {
                            CWeaponSubDefPointer weaponDef;
                            bool throwable = CDefClassReference(weapon->m_pDef)->GetSubDef(weaponDef) && weaponDef->Type == 3;
                            if (throwable) {
                                CRightHandedSet orientation(weapon->PPhysics->GetOrientation());
                                carrying->RemoveThing(weapon);
                                weapon->PPhysics->SetOrientation(orientation);
                                CMeleeCombatAbilityDefPointer ability;
                                weapon->CreateShot(SourceThing, target, (float)accuracy, 1.0f, 0.0f, &ability, count);
                            } else
                                weapon = 0;
                        }
                    }
                    if (!weapon) {
                        const C3DVector& origin = SourceThing->PPhysics->Position;
                        C3DVector position(origin.X, origin.Y, origin.Z + 1.0f);
                        CRightHandedSet orientation(SourceThing->PPhysics->GetOrientation());
                        CThing* strikeWeapon = action.GetWeaponUsedInStrikeAction();
                        CCharString focus("WEAPON_FOCUS_01");
                        CCharString helper = action.GetStrikeEventHelperName();
                        if (!IS_THING_ALIVE(strikeWeapon) || !strikeWeapon->PTCGraphicAppearance->GetDummyObject(focus, 0, 0, &position, &orientation, 1.0f))
                            SourceThing->PTCGraphicAppearance->GetHelperPoint(helper, 0, 0, &position, 1.0f);
                        CMeleeCombatAbilityDefPointer ability;
                        CreateShot(count, SourceThing, 0, target, (float)accuracy, 1.0f, 0.0f, &ability, &position, &orientation);
                    }
                }
            }
        }
    }
}
