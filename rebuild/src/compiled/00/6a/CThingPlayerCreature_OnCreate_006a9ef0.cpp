// CThingPlayerCreature::OnCreate() -- creature-base create; mark the hero level/game-state persistent, record the initial position, set up the movement speeds and reset every control/movement/collision vector, name the script thing, take the hero-combat def and look up the player-movement def; validate, then create the default weapons when the def names a melee weapon
// Leaf: ?OnCreate@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9EF0, length 528 bytes, `ret`, bool in al. Vtable 0x012457FC slot 8
//   (the catalog's "CThingCreatureBase::OnCreateHero" is a propagated label).
// Callees: CThingCreatureBase::OnCreate @ 0x00666310, CThing::SetAsGameStatePersisted(bool) @ 0x004C7880,
//   CThingCreatureBase::GetPosition @ 0x006A4D00, CThingPlayerCreature::InitialiseMovementSpeeds @ 0x006A9A00,
//   CCharString::operator=(const char*) @ 0x0099EFE0, CThingCreatureBase::GetHeroCombatDef @ 0x00663340 (CDefPointer by
//   hidden pointer), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetDefinitionManager
//   @ 0x004C7990, CDefinitionManager::GetDef<CPlayerMovementDef>(const CCharString&, CDefPointer&) @ 0x006AE1C0,
//   CThingPhysical::ValidateInitialisation @ 0x006A4D60, CThingCreatureBase::CreateDefaultWeapons @ 0x00664680.
// Globals: the hero creation state word @ 0x0137F940 (-1 until the first hero is created); strings "SCRIPT_NAME_HERO"
//   0x0125D1C8 and "MOVEMENT_DEFAULT" 0x0125D1B4.
// Layout (retail; Ego_r.pdb in brackets): CThing ScriptName +0x74 [+0x84], LevelPersistent bit 4 of +0x92 [+0xa2];
//   CThingCreatureBase PDef +0xe0, InitialPos +0xe8 [+0x104], IdleCounter +0x124 [+0x148]; CThingPlayerCreature
//   ControlMoveByVector +0x16c, MovedByPlayer bit 0 of +0x178, ControlledMovementType +0x17c, MovementAcceleration +0x180,
//   RelativeMovementAcceleration +0x1b4, LastRelativeMovementAcceleration +0x1c0, CollidedWithThing +0x1cc, the two
//   unaligned collision vectors +0x1cd/+0x1d9, PHeroCombatDef +0x1e8, PPlayerMovementDef +0x1ec, ImpulseVelocity +0x1f0,
//   LeanAngle +0x1fc (landed CThingPlayerCreature models); CThingCreatureDef PrimaryMeleeWeaponDef +0xd8.
// Shape: each def pointer is cleared as `if (P) { P->Release(); P = 0; } P = 0;` (both stores kept); the vectors are
//   zeroed with `X = Y = Z = 0` (Z stored first); the hero-combat def is a CDefPointer temporary assigned through
//   CDefPointer::operator= and released through the def's inline ReleaseTemporary(); the body nests under the base
//   OnCreate and ValidateInitialisation tests so both failures share one final `return false` (ebx pushed at entry).
class C3DVector { public:
    float X, Y, Z;
    void Clear() { X = Y = Z = 0.0f; }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    void Clear()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
    }
    T* operator->() const { return Object; }
};
class CHeroCombatDef : public CDefObject { public:};
class CPlayerMovementDef : public CDefObject { public:};
class CThingCreatureDef { public:
    unsigned char _pad00[0xd8];
    long PrimaryMeleeWeaponDef;                         // +0xd8
};
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer<CPlayerMovementDef>& out);   // 0x006AE1C0
};

extern long g_HeroCreationState;                        // 0x0137F940

class CThing
{
public:
    void SetAsGameStatePersisted(bool);   // 0x004C7880
};
class CThingPhysical : public CThing
{
public:
};
class CThingGameObject : public CThingPhysical
{
public:
};

class CThingCreatureBase : public CThingGameObject { public:
    void* __vftable;
    unsigned char _pad04[0x74 - 0x04];
    CCharString ScriptName;                             // +0x74
    unsigned char _pad78[0x92 - 0x78];
    unsigned char _bits92 : 4;
    unsigned char LevelPersistent : 1;                  // +0x92 bit 4
    unsigned char _bits92b : 3;
    unsigned char _pad93[0xe0 - 0x93];
    CThingCreatureDef* PDef;                            // +0xe0 CDefPointer<const CThingCreatureDef>
    unsigned char _pade4[0xe8 - 0xe4];
    C3DVector InitialPos;                               // +0xe8
    unsigned char _padf4[0x124 - 0xf4];
    long IdleCounter;                                   // +0x124
    unsigned char _pad128[0x16c - 0x128];
    CDefinitionManager* GetDefinitionManager();         // 0x004C7990 (CThing)
    bool ValidateInitialisation();                      // 0x006A4D60 (CThingPhysical)
    const C3DVector& GetPosition() const;               // 0x006A4D00
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;   // 0x00663340
    void CreateDefaultWeapons();                        // 0x00664680
    bool OnCreate();                                    // 0x00666310
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    C3DVector ControlMoveByVector;                      // +0x16c
    unsigned char MovedByPlayer : 1;                    // +0x178 bit 0
    unsigned char Jumping : 1;
    long ControlledMovementType;                        // +0x17c
    C3DVector MovementAcceleration;                     // +0x180
    unsigned char _pad18c[0x1b4 - 0x18c];
    C3DVector RelativeMovementAcceleration;             // +0x1b4
    C3DVector LastRelativeMovementAcceleration;         // +0x1c0
    bool CollidedWithThing;                             // +0x1cc
#pragma pack(push, 1)
    struct { C3DVector Normal; C3DVector Position; } LastThingCollision;   // +0x1cd / +0x1d9
#pragma pack(pop)
    unsigned char _pad1e5[0x1e8 - 0x1e5];
    CDefPointer<CHeroCombatDef> PHeroCombatDef;         // +0x1e8
    CDefPointer<CPlayerMovementDef> PPlayerMovementDef; // +0x1ec
    C3DVector ImpulseVelocity;                          // +0x1f0
    float LeanAngle;                                    // +0x1fc
    void InitialiseMovementSpeeds();                    // 0x006A9A00
    bool OnCreate();
};

bool CThingPlayerCreature::OnCreate()
{
    if (CThingCreatureBase::OnCreate()) {
        LevelPersistent = true;
        SetAsGameStatePersisted(true);
        InitialPos = GetPosition();
        InitialiseMovementSpeeds();
        PPlayerMovementDef.Clear();
        PHeroCombatDef.Clear();
        ControlMoveByVector.Clear();
        MovementAcceleration.Clear();
        RelativeMovementAcceleration.Clear();
        LastRelativeMovementAcceleration.Clear();
        IdleCounter = 0;
        LeanAngle = 0.0f;
        ImpulseVelocity.Clear();
        MovedByPlayer = true;
        ScriptName = "SCRIPT_NAME_HERO";
        ControlledMovementType = 1;
        CollidedWithThing = false;
        LastThingCollision.Normal.Clear();
        LastThingCollision.Position.Clear();
        PHeroCombatDef = GetHeroCombatDef();
        GetDefinitionManager()->GetDef("MOVEMENT_DEFAULT", PPlayerMovementDef);
        if (g_HeroCreationState == -1)
            g_HeroCreationState = 1;
        if (ValidateInitialisation()) {
            if (PDef->PrimaryMeleeWeaponDef)
                CreateDefaultWeapons();
            return true;
        }
    }
    return false;
}