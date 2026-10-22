// CCreatureAction_GenericFireProjectileWeapon::PerformAction() -- make a SIMPLE_NOISE, unload the carried weapon, play its PROJECTILE_WEAPON_FIRE animation, create the weapon's shot at the target and post a fired-shot event
// Grade: DIFFER(638v638) -- weapon lives in esi (retail edi, pushed at the carrying join) and the float accuracy shares the int slot (retail: own slot after `this`); everything else aligned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF280, length 638 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
//          CCharString::~CCharString @ 0x0099EAE0, CThingCreatureBase::MakeNoise(const CCharString&) @ 0x00662110,
//          CVectorMap::LowerBound @ 0x004365B0 (const) / 0x0040F020, CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
//          CTCWeapon::SetProjectileWeaponLoaded(bool, CThing*) @ 0x005DAC50,
//          CTCAnimatingObject::PlayAnimation(const CCharString&, long, bool) @ 0x007F90B0,
//          CThingCreatureBase::GetShotAccuracyPercentage @ 0x00663B10, CTCWeapon::GetCreatedShotDefGlobalIndex @ 0x005DA750,
//          CreateShot @ 0x007C0AD0 (`__fastcall`), operator new @ 0x00BFEA1A, CEventFiredShot::CEventFiredShot(CThing*)
//          @ 0x008BFE20, CTCEntityEvents::AddEvent(CEventBase*) @ 0x008113C0.
// Shape: the carrying component (0x46) lookup's bool is ignored, so the not-present path reads the never-written out
//   variable; the weapon component (0x48) defaults to null and is used unchecked for the shot index.
// Layout: CCombatActionBase (retail 0x114) with PCreature at +8 and PTarget at +0xa8 (as the landed
//   CCreatureAction_TrollProjectileAttack::PerformAction 0x008C6430); the generated class header is a flattened blob.
struct CTCBase;
struct C3DVector;
struct CRightHandedSet;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    SEntry* LowerBound(const long* type) const;        // 0x004365B0
};

struct CPhysicsFlags {                                 // the shot's +0x64 sub-object
    unsigned char _pad[0x59];
    unsigned char m_bit0 : 1;
    unsigned char m_launched : 1;                      // +0x59 bit 1
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CPhysicsFlags* m_pPhysics;                         // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CThingCreatureBase : CThing {
    void MakeNoise(const CCharString& noise);          // 0x00662110
    long GetShotAccuracyPercentage();                  // 0x00663B10
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                              CDefPointer<CMeleeCombatAbilityDef>* ability, C3DVector* position, CRightHandedSet* frame);   // 0x007C0AD0

struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                   // 0x006A1440
};
struct CTCWeapon {
    void SetProjectileWeaponLoaded(bool loaded, CThing* projectile);   // 0x005DAC50
    long GetCreatedShotDefGlobalIndex();               // 0x005DA750
};
struct CTCAnimatingObject {
    void PlayAnimation(const CCharString& name, long a, bool b);       // 0x007F90B0
};
struct CEventBase { };
struct CEventFiredShot : CEventBase {
    unsigned char _pad[0x10];
    CEventFiredShot(CThing* shooter);                  // 0x008BFE20
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                  // 0x008113C0
};

struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};

struct CCombatActionBase {
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

struct CCreatureAction_GenericFireProjectileWeapon : CCombatActionBase {
    virtual void PerformAction();
};

void CCreatureAction_GenericFireProjectileWeapon::PerformAction()
{
    if (PCreature.GetPItem() == 0 || (PCreature.GetPItem()->m_flags91 & 1))
        return;
    {
        CCharString noise("SIMPLE_NOISE");
        PCreature.GetPItem()->MakeNoise(noise);
    }
    CTCWeapon* weaponTC = 0;
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if (weapon == 0 || (weapon->m_flags91 & 1))
        return;
    if (weapon->HasTC(0x48)) {
        weaponTC = (CTCWeapon*)weapon->FindTC(0x48);
        weaponTC->SetProjectileWeaponLoaded(false, 0);
    }
    if (weapon->HasTC(0x83)) {
        CTCAnimatingObject* anim = (CTCAnimatingObject*)weapon->FindTC(0x83);
        CCharString animName("PROJECTILE_WEAPON_FIRE");
        anim->PlayAnimation(animName, 0, false);
    }
    float accuracy = (float)PCreature.GetPItem()->GetShotAccuracyPercentage();
    long shotIndex = weaponTC->GetCreatedShotDefGlobalIndex();
    if (shotIndex == 0)
        return;
    CThing* shot;
    {
        CDefPointer<CMeleeCombatAbilityDef> ability;
        CThing* shooter = PCreature.GetPItem();
        shot = CreateShot(shotIndex, shooter, weapon, PTarget.GetPItem(), accuracy, 1.0f, 1.0f, &ability, 0, 0);
    }
    if (shot == 0 || (shot->m_flags91 & 1))
        return;
    shot->m_pPhysics->m_launched = 1;
    CThingCreatureBase* creature = PCreature.GetPItem();
    if (creature->HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)creature->FindTC(0x42);
        events->AddEvent(new CEventFiredShot(PTarget.GetPItem()));
    }
}
