// NCreatureMode::CModeScorpionKingTailPlunge::~CModeScorpionKingTailPlunge() -- implicit destructor: PDustTrailEmitter, PTailEmitter, PTarget, the Filter (Group string + CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeScorpionKingTailPlunge@NCreatureMode@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E37C0, 59 bytes (+ int3 padding to 0x008E3800), thiscall; tail `jmp ~CModeStandBase`
//   0x0073D860.
// Identity (proven, two sources): the only caller 0x008E37A3 is the scalar deleting destructor 0x008E37A0, slot 0 of vtable
//   0x01285860 (RTTI .?AVCModeScorpionKingTailPlunge@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase);
//   FableWin.pdb ??1CModeScorpionKingTailPlunge@NCreatureMode@@UAE@XZ (0x02853510, right after ??_G 0x028534D0). It stores no
//   vtable of its own: the compiler-generated destructor. Ego_r.pdb members (offsets are retail's): RecoveryCount +0x68,
//   DustEffectCounter +0x6C, SpikeTime +0x70, SpikeDecisionTime +0x74, Filter +0x78, PCreature +0x84, bRecovering +0x88,
//   TargetPos +0x89, TailPos +0x95 (C3DVector, 1-aligned), PTarget +0xA1, PTailEmitter +0xA9, PDustTrailEmitter +0xB1 (odd
//   offsets: the CIntelligentPointer type is 1-packed); sizeof 0xBC. The catalog label
//   "CAIStateGroup_CarryCrateAndFollow::~CAIStateGroup_CarryCrateAndFollow" is propagated and wrong. Byte-near-twin 0x008E3720 is
//   ~CModeJackOfBladesFlash (own ??_G 0x008E3700, vtable 0x012857A8): two symbols.
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (x3), ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@ 0x0122FFCC,
//   ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: as ~CModeJackOfBladesFlash (members in reverse order, inline ~CAnimFilterBase store, base dtor as the tail jump); the PDB
//   constructor ??0CModeScorpionKingTailPlunge@NCreatureMode@@QAE@AAVCThing@@@Z is defined for emission (body not reconstructed).
// Layout: engine/CModeStandBase.h retail opt-in (chunk 710); `#pragma pack(1)` only on C3DVector (retail C3DVector is 1-aligned) and
//   on CBaseIntelligentPointer / CIntelligentPointer<T> (members at +0xA1/+0xA9/+0xB1 while the PDB class size stays 0xBC),
//   GOTCHAS chunks 660/703; no shared header for the other classes (local models, every member at its PDB offset).
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (retail opt-in)

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CAnimateThing;
class CAnimationEntry;
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)
enum EMovementType;                                         // MS forward enum (no enumerator used here)

#pragma pack(push, 1)                                       // CIntelligentPointer members sit at odd offsets (+0xA1/+0xA9/+0xB1)
class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
#pragma pack(pop)

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

class CThingCreatureBase;

namespace NCreatureMode {
class CModeScorpionKingTailPlunge : public CModeStandBase { // vtable 0x01285860 (RTTI : CModeStandBase)
public:
    CModeScorpionKingTailPlunge(CThing& thing);             // ??0CModeScorpionKingTailPlunge@NCreatureMode@@QAE@AAVCThing@@@Z
    virtual unsigned long GetType() const;                  // slot 1
    virtual void FrameUpdate();                             // slot 3
    virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4
    virtual void OnActivate();                              // slot 5
    virtual void OnDeactivate();                            // slot 6
    void SetTarget(CThingCreatureBase& target);
private:
    void Spike();
    void Recover();
public:
    long                                    RecoveryCount;      // +0x68
    long                                    DustEffectCounter;  // +0x6C
    long                                    SpikeTime;          // +0x70
    long                                    SpikeDecisionTime;  // +0x74
    CAnimFilterOfGroup                      Filter;             // +0x78
    CThingCreatureBase*                     PCreature;          // +0x84
    bool                                    bRecovering;        // +0x88
    C3DVector                               TargetPos;          // +0x89
    C3DVector                               TailPos;            // +0x95
    CIntelligentPointer<CThingCreatureBase> PTarget;            // +0xA1
    CIntelligentPointer<CThing>             PTailEmitter;       // +0xA9
    CIntelligentPointer<CThing>             PDustTrailEmitter;  // +0xB1
};
FABLE_STATIC_ASSERT(sizeof(CModeScorpionKingTailPlunge) == 0xBC);

CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())          // emission only: retail body not reconstructed (group name unknown here)
{
}
}