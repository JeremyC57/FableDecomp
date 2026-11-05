// CThingCreatureDef::~CThingCreatureDef() -- the compiler-generated destructor: free the NavigatorTypes block, then the inline CThingBaseDef (Components small vector delete[]d and reset) and CParentDefClassBase (SubDefInfoMap block freed) destructors, the CDefPointeeBase vtable reset and ~CResource
// Leaf: ??1CThingCreatureDef@@UAE@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00671100, 75 bytes, thiscall, tail `jmp ~CResource`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x006710E0 of vtable 0x0125B2DC (RTTI .?AVCThingCreatureDef@@ :
//   CThingBaseDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase); slot 20
//   (GetSizeofClass, 0x006710D0) returns 0x14C = the Ego_r.pdb size 0x154 less the two 4-byte-smaller STLport containers
//   (CParentDefClassBase's CVectorMap, NavigatorTypes). Ego_r.pdb (and FableWin.pdb) spell ??1CThingCreatureDef@@UAE@XZ.
//   No 0x0125B2DC store: the destructor is the implicit one. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Members destroyed, in reverse order (Ego_r.pdb names, retail offsets): CThingCreatureDef::NavigatorTypes
//   (vector<ENavigatorType>, +0xC8, Ego_r +0xCC); CThingBaseDef::Components (CThingComponentSet = CSmallVector<CEntry,8>:
//   Start +0x39, Size +0x3D; Ego_r +0x3D); CParentDefClassBase::SubDefInfoMap (CVectorMap, +0x29 -- the header's ctor
//   evidence 0x00430370).
// Callees: free 0x00BFEA14 (the two ~_Vector_base deallocates), operator delete[] 0x00BFEB1C (CSmallVector::Clear's
//   `delete[] Start`), CResource::~CResource 0x009FC550.
// Shape: every class in the chain below CDefClassBase declares no destructor, so VC7.1 expands them all here; the
//   CSmallVector destructor is the PDB's `~CSmallVector() { Clear(); }` with the protected Clear (`delete[] Start; Start = 0;
//   Size = 0;`, the landed 0x006FCB90 model: two zero stores after the call); CDefClassBase's inline `virtual ~CDefClassBase()
//   {}` store is dead (dropped) and CDefPointeeBase's (0x01230BA0) is kept before the out-of-line ~CResource -- the same tail
//   as the implicit ~CParentDefClassBase 0x00430300. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1`.
// Layout: local CParentDefClassBase -- engine/CParentDefClassBase.h declares `virtual ~CParentDefClassBase();` out of line,
//   but retail's is implicit (0x00430300 stores no 0x01230CBC vtable) and is expanded inline here (header debt); members
//   and virtual overrides copied from the header. CThingBaseDef / CThingCreatureDef have no shared header.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
void __cdecl operator delete[](void* block);                    // 0x00BFEB1C (MSVCR71 import thunk)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CArray.h"


class CDefString {                                      // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                      // +0x00
};
#include "engine/CVectorMap.h"
template <class T, int N> class CSmallVector {          // Ego_r.pdb, 8 bytes
public:
    CSmallVector();
    ~CSmallVector() { Clear(); }
protected:
    void Clear()
    {
        delete[] Start;
        Start = 0;
        Size = 0;
    }
public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
};
class CThingComponentSet {                              // Ego_r.pdb, 8 bytes
public:
#pragma pack(push, 1)
    class CEntry {                                      // Ego_r.pdb, 9 bytes (`lea edx,[eax+eax*8]` stride, landed 0x00870480)
    public:
        CDefString Name;                                // +0x00
        long       DriverType;                          // +0x04
        bool       Override;                            // +0x08
    };
#pragma pack(pop)
    CSmallVector<CEntry, 8> Entries;                    // +0x00
};
class CCharString;
class CDataInputStream;
class CDataOutputStream;
enum ESex { };
enum ECreatureType { };
enum ENavigatorType { };
enum ECreatureInteractionType { };
enum EThingCreatureProperty { };
enum EControlledMovementType { };

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {      // vtable 0x01230CBC, 0x39 bytes (copy of the shared header, implicit dtor)
public:
    CParentDefClassBase();                                               // 0x00430370
    CParentDefClassBase(const CParentDefClassBase&);                     // 0x0046EE98
    CParentDefClassBase& operator=(const CParentDefClassBase&);          // 0x00431ED0
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x004303B0
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x004303C0
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x009B00D0
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x009B0170
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x009AD510
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x009AD4A0
    virtual bool IsParentDef() const;                                    // slot 16 0x004303D0
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009B0200
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009AED70
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x004303E0
    CDefString InstantiationName;                                        // +0x25
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   // +0x29
};

class CThingBaseDef : public CParentDefClassBase {      // vtable-bearing, 0x70 bytes in retail (Ego_r 0x74)
public:
    CThingComponentSet Components;                      // +0x39
    bool AvailableInEditor : 1;                         // +0x41
    bool AlwaysFrameUpdate : 1;
    bool StartKillLocked : 1;
    bool LevelLess : 1;
    bool IsDrawable : 1;
    bool IsDrawableInEditor : 1;
    bool ForceNoSerialise : 1;
    bool InMapSearches : 1;
    bool IsLoadableGlobal : 1;                          // +0x42
    bool GameStatePersisted : 1;
    bool CanComeBetweenCameraAndObservedThing : 1;
    bool DrawWeaponTrails : 1;
    bool DrawProjectileWeaponTrails : 1;
    bool IsVulnerableToMelee : 1;
    bool Damageable : 1;
    bool AlwaysIncludeInObjectStrikeScans : 1;
    bool DisableOcclusionTesting : 1;                   // +0x43
    bool DrawAfterWater : 1;
    bool ForceRenderedLastFrameOverride : 1;
    bool AddToComboMultiplierOnHit : 1;
    bool GiveHeroStatChangesOnBeingHit : 1;
    long       GroupDef;                                // +0x44
    float      RenderFadeDistance;                      // +0x48
    long       PersistenceFlags;                        // +0x4c
    float      Health;                                  // +0x50
    float      MinHealth;                               // +0x54
    float      MeshHeightOffset;                        // +0x58
    float      MeshHeight;                              // +0x5c
    float      MeshRadius;                              // +0x60
    float      ApproxMaxMeshHeight;                     // +0x64
    CDefString DefaultScriptName;                       // +0x68
    CDefString DefaultScriptData;                       // +0x6c
};

class CEngineGraphic {                                  // Ego_r.pdb, 14 bytes
public:
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)

class CThingCreatureDef : public CThingBaseDef {        // vtable 0x0125B2DC, 0x14C bytes in retail (Ego_r 0x154)
public:
    float  StunTimeMin;                                 // +0x70
    float  StunTimeMax;                                 // +0x74
    float  StunHealthThreshold;                         // +0x78
    float  DazedTimeMin;                                // +0x7c
    float  DazedTimeMax;                                // +0x80
    long   DazedHealthThreshold;                        // +0x84
    float  NumSecondsToPersistOnDeath;                  // +0x88
    float  MaxSlowWalkingSpeed;                         // +0x8c
    float  MaxWalkingSpeed;                             // +0x90
    float  MaxJoggingSpeed;                             // +0x94
    float  MaxRunningSpeed;                             // +0x98
    float  MaxPushingSpeed;                             // +0x9c
    float  MaxFlyingSpeed;                              // +0xa0
    float  MaxHoverSpeedSlow;                           // +0xa4
    float  MaxHoverSpeedMedium;                         // +0xa8
    float  MaxHoverSpeedFast;                           // +0xac
    float  SpeedRandomisationFactor;                    // +0xb0
    float  MaxAcceleration;                             // +0xb4
    float  HearingRadius;                               // +0xb8
    float  FOV;                                         // +0xbc
    ESex   Sex;                                         // +0xc0
    ECreatureType CreatureType;                         // +0xc4
    std::vector<ENavigatorType> NavigatorTypes;         // +0xc8
    long   InitialWeaponDef;                            // +0xd4
    long   PrimaryMeleeWeaponDef;                       // +0xd8
    long   SecondaryMeleeWeaponDef;                     // +0xdc
    long   RangedWeaponDef;                             // +0xe0
    long   InitialSheathedWeapon;                       // +0xe4
    long   BrainDef;                                    // +0xe8
    ECreatureInteractionType CreatureInteractionType;   // +0xec
    EThingCreatureProperty   Property;                  // +0xf0
    float  Damage;                                      // +0xf4
    long   DamageEffectDef;                             // +0xf8
    long   StunnedParticle;                             // +0xfc
    CEngineGraphic Graphic;                             // +0x100
    CEngineGraphic EyeGraphic;                          // +0x10e
    long   DefaultOwner;                                // +0x11c
    EControlledMovementType ControlledMovementType;     // +0x120
    long   CombatTypeDef;                               // +0x124
    bool   ResetCombatTypeOnUnsheatheDefaultWeapons;    // +0x128
    long   HeroCombatDef;                               // +0x12c
    long   HeroCombatPCDef;                             // +0x130
    float  CombatDiameter;                              // +0x134
    bool   DeferHitsIfBehindHitter : 1;                 // +0x138
    long   LeaderTypeDef;                               // +0x13c
    float  RecoverEvadeHitsPercent;                     // +0x140
    long   DeadBodyDecayEffect;                         // +0x144
    bool   LeaveDeadCreature : 1;                       // +0x148
    bool   AbleToStrafe : 1;
    bool   CreateDummyOnDeath : 1;
    bool   ImmuneToAttack : 1;
    bool   UseMeleeWeaponAsDefault : 1;
    bool   UnsheatheWeaponsAutomatically : 1;
    bool   IsWillable : 1;
    bool   IsIndestructible : 1;
    bool   IsInvulnerableDuringGenericResponse : 1;     // +0x149
    bool   IsPushedByExplosions : 1;
    virtual void Transfer(CPersistContext& context);    // slot 18
    virtual void Copy(const CDefClassBase* other);      // slot 19
    virtual unsigned long GetSizeofClass();             // slot 20 0x006710D0 (`mov eax,14Ch`)
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CThingCreatureDef* def)
{
    def->CThingCreatureDef::~CThingCreatureDef();
}