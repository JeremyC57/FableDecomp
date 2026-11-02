// CTCFlashOnHit::~CTCFlashOnHit() -- if the parent thing is alive, take this component's tint off its graphic appearance; then ~CTCBase (tail jump)
// Leaf: ??1CTCFlashOnHit@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3420, 42 bytes (int3 padding 0x007E344A..F), thiscall, stores vtable 0x0123E034 first,
//   tail jmp ~CTCBase.
// Identity (proven, two sources): (1) the vtable it stores, 0x0123E034, is RTTI .?AVCTCFlashOnHit@@ (rtti-vtables.tsv); the only
//   caller is that vtable's slot-0 scalar deleting destructor 0x004D62D1 (call at 0x004D62D4); (2) Ego_r.pdb and FableWin.pdb
//   both list a user ??1CTCFlashOnHit@@UAE@XZ (Ego_r donor 0x007DECA0 right after FrameUpdate 0x007DEC30, as retail
//   0x007E3420 follows FrameUpdate 0x007E33B0, slot 9). FrameUpdate's last frame calls the same RemoveTintColour(this).
// Callees: NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void*) 0x004BFAE0 (Ego_r 0x004C49F0 / FableWin 0x01D0A000
//   ?RemoveTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAX@Z; the tint id is this component), CTCBase::~CTCBase
//   0x00686830 (tail).
// Layout: engine/CTCBase.h retail branch (FABLE_RETAIL_CTCBASE) and engine/CClass.h retail branch (FABLE_RETAIL_CCLASS, 0x14);
//   CThing is restated over CClass with every Ego_r.pdb member at its retail offset (Ego_r less 0x10: bitset 0x24, CVectorMap
//   0x10, list 4), non-touched members as opaque storage: PTCGraphicAppearance +0x64, Dead = bit 0 of +0x91, sizeof 0x98.
//   CTCFlashOnHit members (Ego_r.pdb, no containers): FadeInTime +0x0C, FadeOutTime +0x10, LifeTime +0x14, Timer +0x18.
// Shape: if (ParentThing && !ParentThing->IsDead()) through the inline PDB accessors IsDead() / GetTCGraphicAppearance()
//   (FableWin). The PDB template IS_THING_ALIVE(ParentThing) (by const reference) gives the same instructions but keeps the
//   vtable store ahead of the ParentThing load (42v42); retail loads and tests ParentThing before the store, which the plain
//   member test reproduces.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830
#define FABLE_RETAIL_CCLASS
#include "engine/CClass.h"                               // CClass : CBaseObject : CBaseClass, sizeof 0x14

namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase {
public:
    void RemoveTintColour(void* id);                     // 0x004BFAE0
};
}
class CTCDriverBase;
class CTCPhysicsBase;

class CThing : public CClass {                           // RTTI CThing : CClass; Ego_r.pdb layout less 0x10 (STLport containers)
public:
    unsigned __int64 UniqueID;                                       // +0x18
    unsigned char    UsingComponents_[0x24];                         // +0x20 std::bitset<274> UsingComponents
    unsigned char    Components_[0x10];                              // +0x44 CVectorMap<ETCInterfaceType,CTCBase*,...> Components
    unsigned char    UpdateComponentsList_[0x4];                     // +0x54 std::list<CTCBase*> UpdateComponentsList
    unsigned char    SerialisationSectionName_[0x4];                 // +0x58 CCharString SerialisationSectionName
    CTCDriverBase*   PTCDriver;                                      // +0x5c
    CTCPhysicsBase*  PhysicsTC;                                      // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;  // +0x64
    long             WorldFrameCreatedAt;                            // +0x68
    unsigned long    ThingTypeFlags;                                 // +0x6c
    unsigned char    PBaseDef_[0x4];                                 // +0x70 CDefPointer<const CThingBaseDef> PBaseDef
    unsigned char    ScriptName_[0x4];                               // +0x74 CCharString ScriptName
    unsigned char    ScriptData_[0x4];                               // +0x78 CCharString ScriptData
    CThing*          RenderListPrev;                                 // +0x7c
    CThing*          RenderListNext;                                 // +0x80
    CThing*          UpdateListPrev;                                 // +0x84
    CThing*          UpdateListNext;                                 // +0x88
    unsigned short   DefGlobalIndex;                                 // +0x8c
    short            CurrentMapNumber;                               // +0x8e
    signed char      PlayerNumber;                                   // +0x90
    unsigned char    Dead : 1;                                       // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans : 1;
    unsigned char    Activated : 1;
    unsigned char    GloballyLoaded : 1;
    unsigned char    InLimbo : 1;
    unsigned char    KillLocked : 1;
    unsigned char    CreatedBySerialisation : 1;
    unsigned char    DeletedOnOwnerLevelOutOfScope : 1;              // +0x92
    unsigned char    Drawable : 1;
    unsigned char    Dying : 1;
    unsigned char    ForceFrameUpdate : 1;
    unsigned char    LevelPersistent : 1;
    unsigned char    GameStatePersisted : 1;
    unsigned char    PersistOnUnloadWhenInLimbo : 1;
    unsigned char    PausedDuringCutscenes : 1;
    unsigned char    NotPausedDuringCutscenes : 1;                   // +0x93
    unsigned char    DeathPersistent : 1;
    unsigned char    KillOnLevelUnload : 1;
    unsigned char    ActivatedScript : 1;
    unsigned char    LevelIsUnloading : 1;
    unsigned char    IsAffectedByBulletTimeOverride : 1;
    unsigned char    AddingTCsPermitted : 1;
    unsigned char    InitialisationStage : 3;                        // +0x94
    bool IsDead() const { return Dead; }                             // FableWin ?IsDead@CThing@@QBE_NXZ (inline)
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }   // FableWin QAE (inline)
};
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);

class CTCFlashOnHit : public CTCBase {                   // vtable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@ : CTCBase)
public:
    CTCFlashOnHit(CThing& thing);
    virtual ~CTCFlashOnHit();
    virtual bool HasFrameUpdate() const;                 // slot 8  0x004D62CE
    virtual void FrameUpdate();                          // slot 9  0x007E33B0
    virtual ETCInterfaceType GetInterfaceType() const;   // slot 21 0x004D62B8
    virtual CCharString GetClassTypeName() const;        // slot 22 0x004D62BE
    void SetLifeTime(long time);                         // 0x007E33A0
    long FadeInTime;                                     // +0x0c
    long FadeOutTime;                                    // +0x10
    long LifeTime;                                       // +0x14
    long Timer;                                          // +0x18
};

CTCFlashOnHit::~CTCFlashOnHit()
{
    if (ParentThing && !ParentThing->IsDead())
        ParentThing->GetTCGraphicAppearance()->RemoveTintColour(this);
}