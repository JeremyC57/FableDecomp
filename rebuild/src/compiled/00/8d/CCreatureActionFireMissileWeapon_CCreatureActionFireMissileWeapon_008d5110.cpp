// CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(CThingCreatureBase&, const CThing*) -- short constructor: base action on the "PROJECTILE_WEAPON_FIRE" animation, target pointer, accuracy from the creature, damage and build-up 1.0, no decapitation, no special shot
// Leaf: ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D5110, 192 bytes, thiscall, `ret 8`. Vtable 0x01285424.
// Identity (proven): stores vtable 0x01285424 (RTTI .?AVCCreatureAction_FireMissileWeapon@@); `ret 8` = two arguments =
//   Ego_r.pdb ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@@Z (donor 0x0090BE10, right after
//   the nine-argument ctor 0x0090BD10 = retail 0x008D5020); literal 0x0127F254 "PROJECTILE_WEAPON_FIRE".
// Callees: as 0x008D5020, plus CBaseIntelligentPointer::GetPItem 0x00A01B50 (on PCreature +8) and
//   CThingCreatureBase::GetShotAccuracyPercentage() const 0x00663B10 (returns long; `fild`).
// Shape: DelayTimer / PSpecialShotDef in the initialiser list (stored before the calls); the accuracy, the two 1.0f
//   factors and Decapitate are assigned in the body.
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
    long GetShotAccuracyPercentage() const;                  // 0x00663B10
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                               // 0x00A01B10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    void* PData;                                             // +4
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
    T* operator->() const { return (T*)GetPItem(); }
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
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Delete(); }
};

class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _pad_0x04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _body[0xa8 - 0x10];                        // retail sizeof(CCreatureActionBase) 0xA8
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
    CCreatureAction_FireMissileWeapon(CThingCreatureBase& creature, const CThing* target);
    virtual ~CCreatureAction_FireMissileWeapon();
};

CCreatureAction_FireMissileWeapon::CCreatureAction_FireMissileWeapon(CThingCreatureBase& creature, const CThing* target)
    : CCreatureActionBase(creature, -1.0f, 0.0f, GetRandomAnimFromName(creature, "PROJECTILE_WEAPON_FIRE"), CSoundPair(), 0, 0)
    , PTarget(target)
    , DelayTimer(0)
    , PSpecialShotDef()
{
    ShotAccuracyPercentage = (float)PCreature->GetShotAccuracyPercentage();
    ShotDamageMultiplier = 1.0f;
    ShotBuildupTime = 1.0f;
    Decapitate = false;
}