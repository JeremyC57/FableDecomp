// NCreatureMode::CModeDragonHover::~CModeDragonHover() -- implicit destructor: PTarget, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeDragonHover@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E31A0, 34 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is 0x008E3870 inside the scalar deleting destructor 0x008E3860 of
//   NCreatureMode::CModeDragonFireBeam (slot 0 of vtable 0x01285900; RTTI .?AVCModeDragonFireBeam@NCreatureMode@@ :
//   CModeDragonHover : CModeStandBase : CTCCreatureModeManager::CModeBase). That ??_G destroys FireBeam's own PBeamWeaponEffect
//   (+0x84) inline and then calls this body, so it is the base-class destructor; it stores no vtable, so it is the implicit
//   one. FableWin.pdb ??1CModeDragonHover@NCreatureMode@@UAE@XZ (0x024BF860, right after ??_G 0x024BF820; Ego_r inlines it). Members
//   (Ego_r.pdb, no STL container): Filter (CAnimFilterOfGroup) +0x68, PTarget (CIntelligentPointer<CThing>) +0x74,
//   UseHeadTurning +0x7C; sizeof 0x80; FireBeam adds Timer +0x80, PBeamWeaponEffect +0x84, LastExplodeFrame +0x8C, BeamActive
//   +0x90 (0x94). CModeDragonHover's own ??_G 0x00741C10 (vtable 0x0126845C) inlines its destructor, so this out-of-line copy is
//   the only ??1CModeDragonHover symbol in retail. The catalog label "CThingNoiseDef::~CThingNoiseDef" is propagated and wrong.
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10, ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@ 0x0122FFCC,
//   ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: the derived class's ??_G expands ~CModeDragonFireBeam and calls this base dtor out of line; retail's own CModeDragonFireBeam
//   constructor (??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@@Z, body not reconstructed here) emits that vtable / ??_G.
// Layout: engine/CModeStandBase.h retail opt-in (hdr/ overlay, chunk 710); the other classes have no shared header (local models).
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (retail opt-in)

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CAnimateThing;
class CAnimationEntry;
class C3DVector;
enum EMovementType;                                         // MS forward enum (no enumerator used here)

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CAnimFilterBase {                                     // vtable 0x0122FFCC (RTTI, no base)
public:
    CAnimFilterBase() { }                                   // ??0CAnimFilterBase@@QAE@XZ
    virtual ~CAnimFilterBase() { }                          // inline: its vtable store survives in every implicit owner dtor
    virtual bool operator()(const CAnimationEntry& anim) const = 0;   // slot 1
    virtual CAnimFilterBase* Clone() const = 0;             // slot 2
};

class CAnimFilterOfGroup : public CAnimFilterBase {         // vtable 0x01267568 (RTTI : CAnimFilterBase); Ego_r 12 bytes
public:
    CAnimFilterOfGroup(const CCharString& group);           // ??0CAnimFilterOfGroup@@QAE@ABVCCharString@@@Z
    virtual bool operator()(const CAnimationEntry& anim) const;       // 0x0073E9A0
    virtual CAnimFilterBase* Clone() const;                 // 0x0073E970
    CCharString   Group;                                    // +0x04
    unsigned long GroupCRC;                                 // +0x08
};

class CTCCreatureModeManager : public CTCBase {             // vtable 0x01268854; new(0x24) in Alloc 0x004D3547
public:
    class CModeBase {                                       // vtable 0x012673CC (12 slots; RTTI no base)
    public:
        CModeBase(CThing& thing);                           // 0x0073EAC0 ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z
        virtual ~CModeBase();                               // slot 0
        virtual unsigned long GetType() const = 0;          // slot 1 (_purecall)
        virtual void AnimUpdate(CAnimateThing& animateThing);   // slot 2 0x00739E40
        virtual void FrameUpdate();                         // slot 3 0x0073AC50
        virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4 0x00739E50
        virtual void OnActivate();                          // slot 5 0x0073AAC0
        virtual void OnDeactivate();                        // slot 6 0x00739E60 (Active = false)
        virtual float GetMovementSpeed(const C3DVector& from, const C3DVector& to, EMovementType type) const;   // slot 7 0x00739E70
        virtual bool IsIdleMode() const;                    // slot 8 0x00739E80
        virtual bool IsRemoveOnDeactivate() const;          // slot 9 0x00739E90
        virtual bool IsHoldWeaponWithBothHands() const;     // slot 10 0x0073A9F0
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;   // slot 11 0x00739EA0
        CThing*            ParentThing;                     // +0x04
        CAnimFilterOfGroup WeaponBasedFilter;               // +0x08
        long               FramesBeforeRemovingSelf;        // +0x14
        bool               Active;                          // +0x18
    };
    unsigned char PModes_[0x4];                             // std::list<CCountedPointer<CModeBase> > PModes +0x0C
    long          TimeLeftForCurrentMode;                   // +0x10
    long          TimeForNextIdleMode;                      // +0x14
    unsigned char AvailableIdleModes_[0xc];                 // std::vector<NCreatureMode::EMode> AvailableIdleModes +0x18
};

#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CModeStandBase.h"                          // CModeStandBase : CModeBase (retail opt-in, hdr/ overlay chunk 710)

namespace NCreatureMode {
class CModeDragonHover : public CModeStandBase {           // vtable 0x0126845C
public:
    CModeDragonHover(CThing& thing);                        // ??0CModeDragonHover@NCreatureMode@@QAE@AAVCThing@@@Z
    CModeDragonHover(CThing& thing, CThing& target);        // ??0CModeDragonHover@NCreatureMode@@QAE@AAVCThing@@0@Z
    virtual unsigned long GetType() const;
    virtual void AnimUpdate(CAnimateThing& animateThing);
    virtual void FrameUpdate();
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    void SetHeadTurning(bool useHeadTurning);
    CAnimFilterOfGroup          Filter;                     // +0x68
    CIntelligentPointer<CThing> PTarget;                    // +0x74
    bool                        UseHeadTurning;             // +0x7C
};
FABLE_STATIC_ASSERT(sizeof(CModeDragonHover) == 0x80);

class CModeDragonFireBeam : public CModeDragonHover {      // vtable 0x01285900
public:
    CModeDragonFireBeam(CThing& thing);                     // ??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@@Z
    CModeDragonFireBeam(CThing& thing, CThing& target, float duration);
    virtual unsigned long GetType() const;
    long                        Timer;                      // +0x80
    CIntelligentPointer<CThing> PBeamWeaponEffect;          // +0x84
    long                        LastExplodeFrame;           // +0x8C
    bool                        BeamActive;                 // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CModeDragonFireBeam) == 0x94);

CModeDragonFireBeam::CModeDragonFireBeam(CThing& thing)
    : CModeDragonHover(thing)
{
}
}