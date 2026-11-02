// NCreatureMode::CModeJackOfBladesFloat::~CModeJackOfBladesFloat() -- implicit destructor: PEmitter2, PEmitter, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeJackOfBladesFloat@NCreatureMode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740420, 42 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x00740400, slot 0 of vtable 0x01267D10
//   (RTTI .?AVCModeJackOfBladesFloat@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase; the vtable is stored by the
//   constructor at 0x0074035B); FableWin.pdb ??1CModeJackOfBladesFloat@NCreatureMode@@UAE@XZ (0x024BAEF0). It stores no vtable of its own, so it is
//   the compiler-generated destructor. Members (Ego_r.pdb, no STL container, so the offsets are retail's): Filter
//   (CAnimFilterOfGroup) +0x68, PEmitter (CIntelligentPointer<CThing>) +0x74, PEmitter2 (CIntelligentPointer<CThing>) +0x7C; sizeof 0x84.
//   The catalog label "_Dest_val<std::allocator<CMacro>,CMacro>" is propagated and wrong. Byte-near-twins 0x00741390 (~CModeScreamerAggressive) / 0x008AC4D0 (~CModeScreamerSuperAttack) are
//   other classes' implicit destructors (each named from its own ??_G / vtable; three symbols).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (twice), ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@
//   0x0122FFCC (the inline ~CAnimFilterBase store), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline `virtual ~CAnimFilterBase() {}`
//   expand here. Defining retail's own constructor (??0CModeJackOfBladesFloat@NCreatureMode@@QAE@AAVCThing@@@Z; body not reconstructed here, only its
//   member-init order) emits the vtable, hence ??_G and this out-of-line ??1 -- no emission helper.
// Layout: engine/CModeStandBase.h retail opt-in (hdr/ overlay, chunk 710); CModeBase / CTCCreatureModeManager / CAnimFilter* /
//   CIntelligentPointer have no shared header (local models; CTCCreatureModeManager's STL members as opaque storage at their offsets).
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

namespace NCreatureMode {
class CModeJackOfBladesFloat : public CModeStandBase {                         // vtable 0x01267D10
public:
    CModeJackOfBladesFloat(CThing& thing);
    virtual unsigned long GetType() const;
    CAnimFilterOfGroup                  Filter;                     // +0x68
    CIntelligentPointer<CThing>        PEmitter;              // +0x74
    CIntelligentPointer<CThing>        PEmitter2;             // +0x7C
};
FABLE_STATIC_ASSERT(sizeof(CModeJackOfBladesFloat) == 0x84);

CModeJackOfBladesFloat::CModeJackOfBladesFloat(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())
{
}
}