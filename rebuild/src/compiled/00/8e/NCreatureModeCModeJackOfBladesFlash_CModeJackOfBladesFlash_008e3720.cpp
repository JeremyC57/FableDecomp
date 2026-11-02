// NCreatureMode::CModeJackOfBladesFlash::~CModeJackOfBladesFlash() -- implicit destructor: PSpotLight, PEmitter, PTarget, the Filter (Group string + CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeJackOfBladesFlash@NCreatureMode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E3720, 59 bytes (+ int3 padding to 0x008E3760), thiscall; tail `jmp ~CModeStandBase`
//   0x0073D860.
// Identity (proven, two sources): the only caller 0x008E3703 is the scalar deleting destructor 0x008E3700, slot 0 of vtable
//   0x012857A8 (RTTI .?AVCModeJackOfBladesFlash@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase);
//   FableWin.pdb ??1CModeJackOfBladesFlash@NCreatureMode@@UAE@XZ (0x028496A0, right after ??_E 0x02849660; Ego_r inlines it).
//   It stores no vtable of its own, so it is the compiler-generated destructor. Ego_r.pdb members (no STL container below
//   CModeStandBase, so the offsets are retail's): Timer +0x68, BurnCounter +0x6C, Filter (CAnimFilterOfGroup) +0x70,
//   PCreature +0x7C, PTarget (CIntelligentPointer<CThingCreatureBase>) +0x80, PEmitter +0x88, PSpotLight +0x90; sizeof 0x98 --
//   exactly the offsets the body destroys. The catalog label "CAIStateGroup_CarryCrateAndFollow::~CAIStateGroup_CarryCrateAndFollow"
//   is propagated and wrong. Byte-near-twin 0x008E37C0 is ~CModeScorpionKingTailPlunge (its own ??_G 0x008E37A0, vtable
//   0x01285860, odd member offsets): two symbols.
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (x3, called directly on the members), ??1CCharString@@QAE@XZ 0x0099EAE0
//   (Filter.Group), ??_7CAnimFilterBase@@6B@ 0x0122FFCC (the inline ~CAnimFilterBase's store), ??1CModeStandBase@@UAE@XZ
//   0x0073D860 (tail).
// Shape: members in reverse declaration order; CAnimFilterOfGroup's implicit dtor destroys Group then runs the inline
//   ~CAnimFilterBase (its vtable store survives); the base dtor is the tail jump. Retail's own constructor
//   (??0CModeJackOfBladesFlash@NCreatureMode@@QAE@AAVCThing@@@Z, body not reconstructed here) is defined so that the TU emits
//   the vtable, the deleting dtor and this out-of-line ??1, as retail's TU does.
// Layout: engine/CModeStandBase.h retail opt-in (FABLE_RETAIL_CMODESTANDBASE, chunk 710); the classes below have no shared header
//   (local models with every member at its PDB offset; the model is chunk 710's 0x008E31A0 ~CModeDragonHover).
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

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes; 4-aligned members here, no pack needed)
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
class CModeJackOfBladesFlash : public CModeStandBase {     // vtable 0x012857A8 (RTTI : CModeStandBase)
public:
    CModeJackOfBladesFlash(CThing& thing);                  // ??0CModeJackOfBladesFlash@NCreatureMode@@QAE@AAVCThing@@@Z
    virtual unsigned long GetType() const;                  // slot 1
    virtual void FrameUpdate();                             // slot 3
    virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4
    virtual void OnActivate();                              // slot 5
    virtual void OnDeactivate();                            // slot 6
    void SetTarget(CThingCreatureBase& target);
private:
    void ExtinguishFlames();
    void Flash();
public:
    long                                    Timer;          // +0x68
    long                                    BurnCounter;    // +0x6C
    CAnimFilterOfGroup                      Filter;         // +0x70
    CThingCreatureBase*                     PCreature;      // +0x7C
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0x80
    CIntelligentPointer<CThing>             PEmitter;       // +0x88
    CIntelligentPointer<CThing>             PSpotLight;     // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CModeJackOfBladesFlash) == 0x98);

CModeJackOfBladesFlash::CModeJackOfBladesFlash(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())          // emission only: retail body not reconstructed (group name unknown here)
{
}
}