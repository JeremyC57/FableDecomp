// CCreatureAction_DropWeapon::DropWeapon(CThing& weapon, CTCCarrying& carrying, CThing& creature) -- take the weapon out of the creature's carrying component: an inventory item goes back to its passive (sheathed) carry slot (evicting a living occupant to limbo or killing it) unless the weapon def names a sheathe object or the weapon is dead, recording it as the previously wielded melee/ranged weapon; anything else is dropped upright facing its current XY angle and made not-solid to the creature; finally the creature's mode 4 is removed
// Leaf: ?DropWeapon@CCreatureAction_DropWeapon@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00845CE0, 763 bytes (+ int3 padding), static __fastcall, `ret 4`.
// Callees: CTCCarrying::RemoveThing 0x006A19B0, GetThingInCarrySlot 0x006A11F0 (manifest "CThing_Array_FindByID"),
//   PutThingInCarrySlot 0x006A1F80; CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E / GetSubDef<CWeaponDef> 0x0042B1A6
//   (manifest "<CAbilityDef>" is propagated); CThing::SetInLimbo 0x004C8CF0, Kill 0x004C9B80, AddTC 0x004C9D60;
//   CVectorMap::LowerBound 0x0040F020 (non-const) / 0x004365B0 (const); CTCWeapon::IsMeleeWeapon 0x005DAA00; the physics
//   slots 63 (+0xfc, facing angle XY as a fraction of a turn) and 75 (+0x12c, SetRHSet); CTCSolidToThing::
//   AddThingNotSolidTo 0x00770FA0; CTCCreatureModeManager::RemoveMode 0x0073ABA0; CCharString 0x0099EBF0 / 0x0099EAE0.
// Layout: CThing interface bitset +0x20 (0x1a inventory item, 0x13 inventory weapons, 0x48 weapon, 0x4d solid-to-thing,
//   0x31 mode manager), map +0x44, PhysicsTC +0x60, def +0x70, DefGlobalIndex (ushort) +0x8c, dead bit +0x91;
//   CCarryableDef PassiveCarrySlot +0x30; CWeaponDef SheatheObject +0x38; CTCInventoryWeapons
//   PreviouslyWieldedMeleeWeaponDefIndex +0x168 / PreviouslyWieldedRangedWeaponDefIndex +0x16c.
// Shape: the PickUpGoods 0x00848730 TU model -- same-TU GetSubDef and LowerBound stand-ins (the weapon def held in edi,
//   the carryable def released without a null test); component lookups through an arrow-ternary GetTC; the weapon
//   component an if/else-assigned pointer (null when absent); the forward vector `(float)sin(a * 2pi) * cosPitch` as a
//   float product into a named C3DVector copied into a named CRightHandedSet; the AddTC name a block-scoped CCharString.
#include <math.h>
#pragma intrinsic(sin, cos)

class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public:
    C3DVector Up;                                     // +0x00
    C3DVector Forward;                                // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
};
class CCarryableDef : public CDefBase { public:
    unsigned char _pad08[0x30 - 0x08];
    long PassiveCarrySlot;                            // +0x30
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0x38 - 0x08];
    long SheatheObject;                               // +0x38
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
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
    template<class T> CDefPointer<T> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out);  // 0x005CC10E
    bool GetSubDef(CDefPointer<CWeaponDef>* out);     // 0x0042B1A6
};
#pragma auto_inline(off)
// Same-TU stand-ins for GetSubDef<CCarryableDef> 0x005CC10E and GetSubDef<CWeaponDef> 0x0042B1A6 (the landed GetSubDef
// shape): VC7.1 then knows neither keeps the def pointer's address, so the weapon def stays in edi across the calls.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex<CCarryableDef>(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
bool CDefClassBase::GetSubDef(CDefPointer<CWeaponDef>* out)
{
    long* pIndex;
    {
        CCharString name("CWeaponDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CWeaponDef> result = m_pManager->GetPDefFromGlobalIndex<CWeaponDef>(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
    const SEntry* LowerBound(const long* type) const; // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-ins for CVectorMap::LowerBound 0x0040F020 / 0x004365B0 (STLport lower_bound over the 8-byte
// {type, component} entries): the lookup key's address does not escape, so the keys share one frame word.
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
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CTCPhysicsBase { public:
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
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY() const;           // slot 63 (+0xfc), a fraction of a full turn
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rhs); // slot 75 (+0x12c)
};
class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                 // +0x20
    CInterfaceMap   m_interfaces;                     // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                        // +0x60
    unsigned char   _pad64[0x70 - 0x64];
    CDefClassBase*  m_pDef;                           // +0x70
    unsigned char   _pad74[0x8c - 0x74];
    unsigned short  DefGlobalIndex;                   // +0x8c
    unsigned char   _pad8e[0x91 - 0x8e];
    unsigned char   m_flags91;                        // +0x91 bit 0: dead
    void SetInLimbo(bool inLimbo);                    // 0x004C8CF0
    void Kill(bool immediately);                      // 0x004C9B80
    CTCBase* AddTC(const CCharString& name, bool activate, void* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
class CTCCarrying {
public:
    void RemoveThing(CThing* thing);                                         // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);    // 0x006A1F80
public:
    CThing* GetThingInCarrySlot(long) const;   // 0x006A11F0
};
class CTCWeapon { public:
    bool IsMeleeWeapon() const;                       // 0x005DAA00
};
class CTCInventoryWeapons { public:
    unsigned char _pad[0x168];
    long PreviouslyWieldedMeleeWeaponDefIndex;        // +0x168
    long PreviouslyWieldedRangedWeaponDefIndex;       // +0x16c
};
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing);           // 0x00770FA0
};
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);                       // 0x0073ABA0
};

class CCreatureAction_DropWeapon {
public:
    static void __fastcall DropWeapon(CThing& weapon, CTCCarrying& carrying, CThing& creature);
};

void __fastcall CCreatureAction_DropWeapon::DropWeapon(CThing& weapon, CTCCarrying& carrying, CThing& creature)
{
    if (weapon.HasTC(0x1a)) {
        carrying.RemoveThing(&weapon);
        CDefPointer<CCarryableDef> carryableDef;
        weapon.m_pDef->GetSubDef(&carryableDef);
        long passiveSlot = carryableDef->PassiveCarrySlot;
        CThing* sheathed = carrying.GetThingInCarrySlot(passiveSlot);
        if (sheathed && !(sheathed->m_flags91 & 1)) {
            carrying.RemoveThing(sheathed);
            if (sheathed->HasTC(0x1a))
                sheathed->SetInLimbo(true);
            else
                sheathed->Kill(true);
        }
        CDefPointer<CWeaponDef> weaponDef;
        if ((!weapon.m_pDef->GetSubDef(&weaponDef) || weaponDef->SheatheObject < 0) && !(weapon.m_flags91 & 1)) {
            CTCInventoryWeapons* inventoryWeapons;
            if (creature.GetInterface(&inventoryWeapons, 0x13)) {
                const CTCWeapon* weaponTC;
                if (weapon.HasTC(0x48))
                    weaponTC = (const CTCWeapon*)weapon.GetConstTC(0x48);
                else
                    weaponTC = 0;
                if (weaponTC) {
                    if (weaponTC->IsMeleeWeapon())
                        inventoryWeapons->PreviouslyWieldedMeleeWeaponDefIndex = weapon.DefGlobalIndex;
                    else
                        inventoryWeapons->PreviouslyWieldedRangedWeaponDefIndex = weapon.DefGlobalIndex;
                }
            }
            carrying.PutThingInCarrySlot(&weapon, passiveSlot, true);
        }
    }
    else {
        CTCPhysicsBase* physics = weapon.PhysicsTC;
        float angle = physics->GetFacingAngleXY();
        float cosPitch = (float)cos(0.0);
        C3DVector forward((float)sin(angle * 6.2831854820251465) * cosPitch,
                          (float)cos(angle * 6.2831854820251465) * cosPitch, (float)sin(0.0));
        CRightHandedSet rhs(C3DVector(0.0f, 0.0f, 1.0f), forward);
        physics->SetRHSet(rhs);
        carrying.RemoveThing(&weapon);
        CTCSolidToThing* solidToThing;
        if (!weapon.GetInterface(&solidToThing, 0x4d)) {
            {
                CCharString name("CTCSolidToThing");
                weapon.AddTC(name, false, 0);
            }
            weapon.GetInterface(&solidToThing, 0x4d);
        }
        solidToThing->AddThingNotSolidTo(&creature);
    }
    CTCCreatureModeManager* modes;
    creature.GetInterface(&modes, 0x31);
    modes->RemoveMode(4);
}