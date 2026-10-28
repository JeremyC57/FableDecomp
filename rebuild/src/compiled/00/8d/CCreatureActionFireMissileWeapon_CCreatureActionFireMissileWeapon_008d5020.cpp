// CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(CThingCreatureBase&, const CThing*, float, float, float, bool, CDefPointer<const CMeleeCombatAbilityDef>, long, long) -- full constructor: base action on the "PROJECTILE_WEAPON_FIRE" animation, target pointer, shot parameters, special-shot def taken by value
// Leaf: ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@MMM_NV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D5020, 225 bytes, thiscall, `ret 24h`. Vtable 0x01285424.
// Identity (proven): stores vtable 0x01285424 (RTTI .?AVCCreatureAction_FireMissileWeapon@@, the landed copy ctor 0x008D6320
//   stores the same table); `ret 24h` = nine dword arguments = Ego_r.pdb
//   ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@MMM_NV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@JJ@Z
//   (donor 0x0090BD10, listed before the two-argument ctor 0x0090BE10 = retail 0x008D5110); literal 0x0127F254 is "PROJECTILE_WEAPON_FIRE".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName 0x006924B0
//   (`__fastcall`), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&,
//   const CSoundPair&, long, long) 0x00693B30, CCharString::~CCharString 0x0099EAE0, CBaseIntelligentPointer::
//   CBaseIntelligentPointer 0x00A01B10, CBaseIntelligentPointer::SetPItem 0x00A01B90 (CIntelligentPointer<const CThing>
//   vtable 0x01237F1C).
// Shape: the landed BanditKing constructor (0x008C7F60) base call; the by-value CDefPointer argument is copied into
//   PSpecialShotDef (count +1) and then destroyed by the callee (count -1, slot-1 Delete when it reaches zero), which is
//   retail's `inc edx ; mov eax,edx ; dec eax` pair. The second float argument goes to ShotAccuracyPercentage (+0xB0),
//   the first to ShotDamageMultiplier (+0xB4).
// Layout: Ego_r.pdb members at PDB offset - 0x0C (retail CCreatureActionBase 0xA8, PDB 0xB4), as the landed copy ctor.
class CBaseObject;
class CThing;
class CAnimationEntry;

class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};

class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                               // 0x00A01B10
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    void* PData;                                             // +4
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
};

class CDefPointeeBase {                                      // counted def: vptr, reference count at +4
public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                             // slot 1
    mutable long RefCount;                                   // +4 (descriptive)
};

class CMeleeCombatAbilityDef : public CDefPointeeBase { };

template <class T>
class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Delete(); }
};

class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase {
public:
    CIntelligentPointer<const CThing> PTarget;               // +0xA8
    float ShotAccuracyPercentage;                            // +0xB0
    float ShotDamageMultiplier;                              // +0xB4
    float ShotBuildupTime;                                   // +0xB8
    bool Decapitate;                                         // +0xBC
    long DelayTimer;                                         // +0xC0
    CDefPointer<const CMeleeCombatAbilityDef> PSpecialShotDef;   // +0xC4
    CCreatureAction_FireMissileWeapon(CThingCreatureBase& creature, const CThing* target, float damage_multiplier,
                                      float accuracy_percentage, float buildup_time, bool decapitate,
                                      CDefPointer<const CMeleeCombatAbilityDef> special_shot_def, long c, long d);
    virtual ~CCreatureAction_FireMissileWeapon();
};

CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(CThingCreatureBase& creature, const CThing* target,
    float damage_multiplier, float accuracy_percentage, float buildup_time, bool decapitate,
    CDefPointer<const CMeleeCombatAbilityDef> special_shot_def, long c, long d)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, "PROJECTILE_WEAPON_FIRE"), CSoundPair(), c, d)
    , PTarget(target)
    , ShotAccuracyPercentage(accuracy_percentage)
    , ShotDamageMultiplier(damage_multiplier)
    , ShotBuildupTime(buildup_time)
    , Decapitate(decapitate)
    , DelayTimer(0)
    , PSpecialShotDef(special_shot_def)
{
}