// CCreatureAction_BanditKingThrowWeapons::PerformAction() -- a living bandit king takes the things out of its right and left hand carry slots and throws each living one at the target as a shot (the right-hand weapon's def index is remembered by the boss component); the thrown thing gets physics state 2 and its navigator enabled
// Leaf: ?PerformAction@CCreatureAction_BanditKingThrowWeapons
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C89D0, 595 bytes (+ int3 padding), virtual PerformAction, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PTarget +0xa8), CVectorMap::LowerBound 0x0040F020
//   (inlined GetInterface: 0x46 CTCCarrying bit 6 of +0x28, 0xAD boss weapons component bit 13 of +0x34, 0x77
//   CTCPhysicsNavigator bit 23 of +0x2c), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCreatureActionBase::GetPDefinitionManager 0x00692910, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CTCCarrying::RemoveThingInCarrySlot(long, bool) 0x006A16A0, CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 (manifest
//   "<CAbilityDef>" is propagated; the landed 0x0042B1A6 body names 'CWeaponDef'), CThingCreatureBase::
//   GetShotAccuracyPercentage 0x00663B10, CreateShot(CThing*, CThing*, CThing*, CThing*, float, float, float,
//   CDefPointer<CMeleeCombatAbilityDef>*, long) 0x007C0E80 (__fastcall, ret 0x1c), the physics TC's slot 43 (+0xac),
//   CTCPhysicsNavigator::EnableNavigator 0x007BED30. Strings "CARRY_SLOT_RIGHT_HAND" 0x01265440, "CARRY_SLOT_LEFT_HAND"
//   0x01265428.
// Layout: retail CCreatureActionBase: PCreature +0x08, PTarget +0xa8 (modelled locally). CThing: interface bitset +0x20,
//   map +0x44, PhysicsTC +0x60, def +0x70, dead bit +0x91. CWeaponDef's global index +0xc8 is stored at +0x1c of the 0xAD
//   component (the word the landed BanditKingChangeWeapons 0x008C8960 recreates the weapons from).
// Shape: the slot names are block-scoped named CCharStrings (their address pushed, not the ctor's return); the slot
//   indices are a two-long array indexed by the memory-held loop counter; the weapon def pointer is zeroed after the
//   PBaseDef load and released in the `i == 0` block (GetSubDef has a same-TU out-only stand-in, non-template); the
//   ability pointer lives in a block around the CreateShot call only, so it is released (TrollProjectileAttack
//   ReleaseTemporary model, null store kept) before the shot is touched; target and creature are named locals fetched
//   before the argument pushes, the accuracy converted inline at its argument.
class CTCBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0xc8 - 0x08];
    long          GlobalIndex;                          // +0xc8
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CWeaponDef>* out);       // 0x0042B1A6
};
void* FindSubDef_CWeaponDef(CDefClassBase* defClass);  // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWeaponDef>* out)
{
    void* def = FindSubDef_CWeaponDef(this);
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CRefCounted {                                    // second base of the ability def: the count at +4
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
class CMeleeCombatAbilityDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CAbilityPointer {
    T* m_p;
    CAbilityPointer() : m_p(0) { }
    ~CAbilityPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetPhysicsState(long state);           // slot 43 (+0xac)
};
struct CTCPhysicsNavigator {                            // interface 0x77
    void EnableNavigator(bool enable);                  // 0x007BED30
};
struct CTCBanditKingWeapons {                           // interface 0xAD
    unsigned char _pad[0x1c];
    long m_weaponDef;                                   // +0x1c
};
class CThing { public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* m_pPhysics;                         // +0x60
    unsigned char   _pad64[0x70 - 0x64];
    CDefClassBase*  m_pDef;                             // +0x70
    unsigned char   _pad74[0x91 - 0x74];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
public:
    long GetShotAccuracyPercentage() const;   // 0x00663B10
};
class CTCCarrying { public:                                    // interface 0x46
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
};
CThing* __fastcall CreateShot(CThing* shot, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                              CAbilityPointer<CMeleeCombatAbilityDef>* ability, long d);   // 0x007C0E80

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
__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThingCreatureBase>& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThingCreatureBase> PTarget;                    // +0xa8
    CDefinitionManager* GetPDefinitionManager() const;  // 0x00692910
};
class CCreatureAction_BanditKingThrowWeapons : public CCreatureActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_BanditKingThrowWeapons::PerformAction()
{
    if (!IS_THING_ALIVE(PCreature))
        return;
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    long slots[2];
    {
        CCharString name("CARRY_SLOT_RIGHT_HAND");
        slots[0] = GetPDefinitionManager()->GetDefGlobalIndexFromName(name);
    }
    {
        CCharString name("CARRY_SLOT_LEFT_HAND");
        slots[1] = GetPDefinitionManager()->GetDefGlobalIndexFromName(name);
    }
    for (long i = 0; i < 2; ++i) {
        CThing* weapon = carrying->RemoveThingInCarrySlot(slots[i], false);
        if (weapon && !(weapon->m_flags91 & 1)) {
            if (i == 0) {
                CDefPointer<CWeaponDef> def;
                CDefClassBase* defClass = weapon->m_pDef;
                if (defClass && defClass->GetSubDef(&def)) {
                    CTCBanditKingWeapons* boss;
                    if (PCreature.GetPItem()->GetInterface(&boss, 0xad))
                        boss->m_weaponDef = def->GlobalIndex;
                }
            }
            CThing* shot;
            {
                CAbilityPointer<CMeleeCombatAbilityDef> ability;
                CThing* target = PTarget.GetPItem();
                CThing* creature = PCreature.GetPItem();
                shot = CreateShot(weapon, creature, weapon, target, (float)PCreature.GetPItem()->GetShotAccuracyPercentage(),
                                  1.0f, 1.0f, &ability, -1);
            }
            if (shot) {
                shot->GetPhysics()->SetPhysicsState(2);
                CTCPhysicsNavigator* navigator;
                if (shot->GetInterface(&navigator, 0x77))
                    navigator->EnableNavigator(true);
            }
        }
    }
}