// CModeStandBase::UpdateWalkingAnim(CAnimateThing& anim) -- choose and blend the movement animation pair for the creature's planar speed: keep the movement-update counter (2 while moving, else count down), then blend run->sprint when the hero special-movement component is sprinting past running speed, else jog->run / walk->jog / stand->walk (walk->run / stand->walk without a jog animation) by the speed band, with the existing-anim status picked from the main anim's type
// Leaf: ?UpdateWalkingAnim@CModeStandBase@@AAEXAAVCAnimateThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00862AA0, 1056 bytes incl. the two 4-entry status jump tables at 0x00862EA0 / 0x00862EB0,
//   `ret 4`. Ego_r.pdb: ?UpdateWalkingAnim@CModeStandBase@@AAEXAAVCAnimateThing@@@Z (private); caller AnimUpdate 0x00862EC0 (landed).
// Callees: CModeStandBase::SetUpMovementAnim 0x00861A80 (landed, Ego_r private const spelling), CThing::GetPos 0x004C73D0,
//   CTCPhysicsBase::GetFacingVector (slot 70, +0x118, on PhysicsTC +0x60), the creature's GetMaxWalkingSpeed / GetMaxJoggingSpeed /
//   GetMaxRunningSpeed / GetMaxSprintingSpeed(const C3DVector&) const (slots 79-82; CThingAICreature vtable order, as the landed
//   GetCurrentMaxMovementSpeed 0x006AB5D0), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (engine/CVectorMap.h).
//   Interface 0xDF = CTCHeroSpecialMovement (Sprinting +0x0C). Data: DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Layout: CModeStandBase members per engine/CModeStandBase.h (FABLE_RETAIL path; restated here as the landed AnimUpdate /
//   UpdateIdleAnim rows do, since the header's retail path needs CTCCreatureModeManager::CModeBase): PStand +0x38, PWalk +0x48,
//   PJog +0x50, PRun +0x58, PSprint +0x60 (CCountedPointer<CAnimEntryLite>); CAnimateThing +0x50 = CurrentAnim.MainAnimSet.Type
//   (Ego_r CAnimSet::Type +0x58 less two 4-byte-smaller STLport vectors). EExistingAnimStatus: values 0..2 as SetUpMovementAnim
//   uses them; the fourth value 3 is unnamed in the PDB name sets (descriptive STATUS_OTHER).
// Jump tables (checked by hand against retail): 0x00862EA0 (jog->run) type 2 -> NONE, 3 -> SAME, 4/5 -> REPLACE; 0x00862EB0
//   (walk->jog) 2/3 -> NONE, 4 -> SAME, 5 -> REPLACE; default OTHER.
// Shape: the PeekTC out-parameter form with the key copied into a local (bitset bit 0xDF = sign of word 6); the speed bands are
//   `max + epsilon < speed` (`fadd ; fcomp ; test ah,5 ; jp`); the run->sprint setups are built from then to, jog->run to then
//   from; each switch lists its REPLACE case first (VC7.1 lays the blocks out in source order) and the walk->run one is an if
//   chain (2/3/5 sparse); the two stand->walk status picks switch on a `long` copy of the type (movzx + signed compares, no
//   setne if-conversion) and share their tail; the stand setup comes from a one-argument inline CMovementSetup constructor
//   (speed 0, type 5) whose body stores lite, entry, type, speed (VC7.1 emits the three constant stores speed, type, entry as
//   retail). That constructor is not in the PDB name sets (inline-only, no symbol): its existence is inferred from the store
//   order, which the three-argument PDB constructor (MEABV...) cannot produce.
#include "fable_compiler.h"
#include <math.h>
extern const float DELTA_NOTIONAL_ZERO;     // 0x0129BA3C

#include "engine/C3DVector.h"
class CTCBase;
class CTCPhysicsBase {
public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacingVector() const;  // slot 70 (+0x118)
};
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCHeroSpecialMovement {                  // interface 0xDF (CTCBase-derived)
public:
    unsigned char _pad00[0x0c];
    bool          Sprinting;                    // +0x0c (Ego_r.pdb)
};
class CThing {
public:
    virtual void v000();
    unsigned char  _pad04[0x1c];
    std::bitset<274> UsingComponents;           // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char  _pad54[0x60 - 0x54];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char  _pad64[0x10c - 0x64];
    float          VelocityX;                   // +0x10c
    float          VelocityY;                   // +0x110
    const C3DVector& GetPos() const;            // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> FABLE_FORCEINLINE bool PeekTC(ETCInterfaceType id, T** ptc)
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    VF10(01) VF10(02) VF10(03) VF10(04) VF10(05) VF10(06) VF10(07)
    VF(080) VF(081) VF(082) VF(083) VF(084) VF(085) VF(086) VF(087)
    virtual float GetMaxWalkingSpeed(const C3DVector& direction) const;     // slot 79 (+0x13c)
    virtual float GetMaxJoggingSpeed(const C3DVector& direction) const;     // slot 80 (+0x140)
    virtual float GetMaxRunningSpeed(const C3DVector& direction) const;     // slot 81 (+0x144)
    virtual float GetMaxSprintingSpeed(const C3DVector& direction) const;   // slot 82 (+0x148)
};

namespace NAnimationEntryBodge { class CAnimEntryLite; }
#include "engine/CCountedPointer.h"

class CAnimateThing {
public:
    unsigned char _pad00[0x50];
    unsigned char MainAnimType;                 // +0x50 CurrentAnim.MainAnimSet.Type
};
class CAnimationEntry;

class CModeBase {
public:
    virtual ~CModeBase();
    CThingCreatureBase* ParentThing;            // +0x04
    unsigned char       _pad08[0x14];
};

class CModeStandBase : public CModeBase {
public:
    enum EExistingAnimStatus { STATUS_NONE, STATUS_SAME, STATUS_REPLACE, STATUS_OTHER };
    class CMovementSetup {
    public:
        float                                  MaxMovementSpeed;   // +0x00
        unsigned char                          Type;               // +0x04
        CAnimationEntry*                       PAnimEntry;         // +0x08
        const NAnimationEntryBodge::CAnimEntryLite* PAnimEntryLite; // +0x0c
        CMovementSetup(float maxSpeed, unsigned char type, const NAnimationEntryBodge::CAnimEntryLite& entry)
            : MaxMovementSpeed(maxSpeed), Type(type), PAnimEntry(0), PAnimEntryLite(&entry) { }
        CMovementSetup(const NAnimationEntryBodge::CAnimEntryLite& entry)
        {
            PAnimEntryLite = &entry;
            PAnimEntry = 0;
            Type = 5;
            MaxMovementSpeed = 0.0f;
        }
    };
    long        IdleCounter;                                             // +0x1c
    long        FramesToKeepUpdatingMovement;                            // +0x20
    bool        ForceAddNewAnims;                                        // +0x24
    float       MaxRunSpeed;                                             // +0x28
    float       MaxJogSpeed;                                             // +0x2c
    float       MaxWalkSpeed;                                            // +0x30
    void*       PoseAnimName;                                            // +0x34 CCharString
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PStandAnimation;     // +0x38
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PSlowWalkAnimation;  // +0x40
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PWalkAnimation;      // +0x48
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PJogAnimation;       // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PRunAnimation;       // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PSprintAnimation;    // +0x60
private:
    void SetUpMovementAnim(CAnimateThing& animate, EExistingAnimStatus status, float speed, const CMovementSetup& from,
                           const CMovementSetup& to) const;   // 0x00861A80
    void UpdateWalkingAnim(CAnimateThing& anim);
};

void CModeStandBase::UpdateWalkingAnim(CAnimateThing& anim)
{
    if (PWalkAnimation.IsNull() || PRunAnimation.IsNull())
        return;
    CThingCreatureBase* creature = ParentThing;
    float speed = (float)sqrt(creature->VelocityX * creature->VelocityX + creature->VelocityY * creature->VelocityY);
    if (speed > DELTA_NOTIONAL_ZERO)
        FramesToKeepUpdatingMovement = 2;
    else
        --FramesToKeepUpdatingMovement;
    C3DVector direction = creature->GetPos() + creature->PhysicsTC->GetFacingVector();
    CTCHeroSpecialMovement* special;
    if (ParentThing->PeekTC((ETCInterfaceType)0xdf, &special) && special->Sprinting
        && creature->GetMaxRunningSpeed(direction) + DELTA_NOTIONAL_ZERO < speed) {
        EExistingAnimStatus status = anim.MainAnimType == 2 ? STATUS_SAME : STATUS_REPLACE;
        CMovementSetup from(creature->GetMaxRunningSpeed(direction), 3, *PRunAnimation);
        CMovementSetup to(creature->GetMaxSprintingSpeed(direction), 2, *PSprintAnimation);
        SetUpMovementAnim(anim, status, speed, from, to);
        return;
    }
    EExistingAnimStatus status;
    if (!PJogAnimation.IsNull()) {
        if (creature->GetMaxJoggingSpeed(direction) + DELTA_NOTIONAL_ZERO < speed) {
            switch (anim.MainAnimType) {
            case 4: case 5: status = STATUS_REPLACE; break;
            case 3: status = STATUS_SAME; break;
            case 2: status = STATUS_NONE; break;
            default: status = STATUS_OTHER; break;
            }
            CMovementSetup to(creature->GetMaxRunningSpeed(direction), 3, *PRunAnimation);
            CMovementSetup from(creature->GetMaxJoggingSpeed(direction), 4, *PJogAnimation);
            SetUpMovementAnim(anim, status, speed, from, to);
        } else if (creature->GetMaxWalkingSpeed(direction) + DELTA_NOTIONAL_ZERO < speed) {
            switch (anim.MainAnimType) {
            case 5: status = STATUS_REPLACE; break;
            case 4: status = STATUS_SAME; break;
            case 2: case 3: status = STATUS_NONE; break;
            default: status = STATUS_OTHER; break;
            }
            CMovementSetup from(creature->GetMaxWalkingSpeed(direction), 5, *PWalkAnimation);
            CMovementSetup to(creature->GetMaxJoggingSpeed(direction), 4, *PJogAnimation);
            SetUpMovementAnim(anim, status, speed, from, to);
        } else {
            {
                long type = anim.MainAnimType;
                switch (type) {
                case 2: case 3: case 4: status = STATUS_NONE; break;
                case 5: status = STATUS_SAME; break;
                default: status = STATUS_OTHER; break;
                }
            }
            CMovementSetup from(*PStandAnimation);
            CMovementSetup to(creature->GetMaxWalkingSpeed(direction), 5, *PWalkAnimation);
            SetUpMovementAnim(anim, status, speed, from, to);
        }
    } else {
        if (creature->GetMaxWalkingSpeed(direction) + DELTA_NOTIONAL_ZERO < speed) {
            switch (anim.MainAnimType) {
            case 2: status = STATUS_NONE; break;
            case 3: status = STATUS_SAME; break;
            case 5: status = STATUS_REPLACE; break;
            default: status = STATUS_OTHER; break;
            }
            CMovementSetup from(creature->GetMaxWalkingSpeed(direction), 5, *PWalkAnimation);
            CMovementSetup to(creature->GetMaxRunningSpeed(direction), 3, *PRunAnimation);
            SetUpMovementAnim(anim, status, speed, from, to);
        } else {
            {
                long type = anim.MainAnimType;
                switch (type) {
                case 2: case 3: status = STATUS_NONE; break;
                case 5: status = STATUS_SAME; break;
                default: status = STATUS_OTHER; break;
                }
            }
            CMovementSetup from(*PStandAnimation);
            CMovementSetup to(creature->GetMaxWalkingSpeed(direction), 5, *PWalkAnimation);
            SetUpMovementAnim(anim, status, speed, from, to);
        }
    }
}