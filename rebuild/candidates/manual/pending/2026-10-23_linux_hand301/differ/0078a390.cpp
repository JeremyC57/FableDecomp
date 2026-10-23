// Grade: DIFFER(563v563) -- same size; frame slots permuted (retail: GetTC key and accuracy at +0x14, current target and the aiming-time temp at +0x18; ours swaps those two) and the hero combat def loads through ecx instead of eax
// CTCHeroRangedAttacks::FireRangedWeapon() -- fires the queued shot of a loaded projectile weapon at the current target and posts the fire message
class CThing;
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
    SEntry* LowerBound(const long& type) const;         // 0x004365B0
};
class CPlayer {
public:
    long GetCurrentMode() const;                        // 0x00633BE0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91
    __forceinline bool HasTC(long type) const { return ((PresentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetInterface(long type)
    {
        CTCBase* result;
        const bool& present = HasTC(type);
        if (present)
            result = GetTC(type);
        return result;
    }
    __forceinline const CTCBase* GetInterface(long type) const
    {
        const CTCBase* result;
        const bool& present = HasTC(type);
        if (present)
            result = GetTC(type);
        return result;
    }
    CPlayer& GetPlayer();                               // 0x004C7A30
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CThingCreatureBase;
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};
class CMeleeCombatAbilityDef : public CDefClassBase { };
class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
class CCreatureAction_FireMissileWeapon : public CCreatureActionBase {
public:
    unsigned char _bodyA8[0xc8 - 0xa8];
    CCreatureAction_FireMissileWeapon(CThingCreatureBase& creature, const CThing* target, float damage, float accuracy,
                                      float buildup, bool decapitate, CDefPointer<const CMeleeCombatAbilityDef> special,
                                      long delay, long count);   // 0x008D5020
    virtual ~CCreatureAction_FireMissileWeapon();      // 0x0078ADF0
    virtual void PerformAction();                       // 0x008D5AA0
};
class CThingCreatureBase {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CMessageManager {
public:
    void AddMessage(long type, long frames, const C3DVector& pos, long a, long b, long c, long d, long e);   // 0x00687540
};
class CWorld {
public:
    unsigned char _pad00[0x60];
    CMessageManager* PMessages;                         // +0x60
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CWorld* GetWorld() const;                           // 0x00686A70
};
class CTCCarrying : public CTCBase {
public:
    CThing* GetThingInPrimarySlot() const;              // 0x006A1440
};
class CTCWeapon : public CTCBase {
public:
    bool IsProjectileWeapon() const;                    // 0x005DAA40
    bool IsProjectileWeaponLoaded() const;              // 0x005DA980
};
class CTCTargeting : public CTCBase {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                        // slot 33 (+0x84)
};
class CHeroCombatDef : public CDefClassBase {
public:
    unsigned char _pad08[0xe8 - 0x08];
    float MaxRange;                                     // +0xE8
};
class CThingBuilding;
bool __fastcall IS_THING_ALIVE(const CThing* const& thing);  // 0x0040DCE0
bool __fastcall IsValidRangedWeaponTargetForShooter(const CThing* shooter, const CThing* target, float range);   // 0x0085F8D0
extern float g_RangedAccuracyOverride;                  // 0x013BAE04
extern long g_framesPerSecond;                          // 0x01375550

class CTCHeroRangedAttacks : public CTCBase {
public:
    CThingCreatureBase* PParentCreature;                // +0x0C
    const CHeroCombatDef* PHeroCombatDef;               // +0x10
    bool FireCycleInProgress;                           // +0x14
    bool Aiming;                                        // +0x15
    bool FireQueued;                                    // +0x16
    bool Loading;                                       // +0x17
    long AimingTimer;                                   // +0x18
    long PullBackTimer;                                 // +0x1C
    float PullBackAmount;                               // +0x20
    float Accuracy;                                     // +0x24
    float DamageMultiplier;                             // +0x28
    bool Decapitate;                                    // +0x2C
    __forceinline float GetAimingTime() const { return (float)AimingTimer / g_framesPerSecond; }
    void FireRangedWeapon();
};

void CTCHeroRangedAttacks::FireRangedWeapon()
{
    if (!FireQueued || AimingTimer < 0)
        return;
    CThing* weapon = ((CTCCarrying*)Thing->GetInterface(0x46))->GetThingInPrimarySlot();
    if (!weapon || (weapon->Flags91 & 1))
        return;
    const bool& isWeapon = weapon->HasTC(0x48);
    if (!isWeapon)
        return;
    const CTCWeapon* weaponTC = (const CTCWeapon*)((const CThing*)weapon)->GetTC(0x48);
    if (!weaponTC->IsProjectileWeapon() || !weaponTC->IsProjectileWeaponLoaded())
        return;
    CTCTargeting* targeting = (CTCTargeting*)Thing->GetInterface(8);
    CThing* target;
    float accuracy;
    float damage;
    {
        CThing* const current = targeting->GetTarget();
        target = current;
        damage = DamageMultiplier;
        if (g_RangedAccuracyOverride >= 0.0f)
            accuracy = g_RangedAccuracyOverride;
        else
            accuracy = Accuracy;
        if (Thing->GetPlayer().GetCurrentMode() == 0x19)
            accuracy = 100.0f;
        if (IS_THING_ALIVE(current) && !IsValidRangedWeaponTargetForShooter(Thing, target, PHeroCombatDef->MaxRange))
            target = 0;
    }
    CCreatureAction_FireMissileWeapon action(*PParentCreature, target, damage, accuracy, GetAimingTime(),
                                             Decapitate, CDefPointer<const CMeleeCombatAbilityDef>(), 100, 4);
    if (!PParentCreature->SetCurrentAction(action))
        action.PerformAction();
    CThing* thing = Thing;
    long frames = g_framesPerSecond;
    CMessageManager* messages = GetWorld()->PMessages;
    messages->AddMessage(0x43, frames, thing->GetPosition(), 0, 0, 0, 0, 0);
    FireQueued = false;
}
