// CTCHeroSpecialMovement::~CTCHeroSpecialMovement() -- implicit destructor: release the PHeroSpecialMovementDef def pointer (count down, delete at zero, clear), then ~CTCBase
// Leaf: ??1CTCHeroSpecialMovement@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0B20, 35 bytes, thiscall; tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x007E0B00, slot 0 of vtable 0x0126FE3C (RTTI
//   .?AVCTCHeroSpecialMovement@@ : CTCBase : CBaseClassNonCopyable : CBaseClass; the vtable is stored by the constructor at
//   0x007E08FA); FableWin.pdb ??1CTCHeroSpecialMovement@@UAE@XZ (0x02772270, right after ??_G 0x02772230). It stores no vtable of
//   its own, so it is the compiler-generated destructor. Ego_r.pdb members (no STL container, so the offsets are retail's):
//   Sprinting +0x0C, Running +0x0D, SprintFramesRemaining +0x10, RunTimer +0x14, PHeroSpecialMovementDef
//   (CDefPointer<const CHeroSpecialMovementDef>) +0x18; sizeof 0x1C.
//   The catalog label "C3DAnimationBlendState::~C3DAnimationBlendState" is propagated and wrong. Byte-near-twin 0x006F1770 is
//   ~CCombatSequenceGeneric (its own ??_G 0x006F1750 / vtable 0x01261D74; Def at +0x28, tail ~CCombatSequenceBase): two symbols.
// Callees: the def's virtual slot 1 Delete() (CIVCountedPointeeBase, through the vtable), ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Shape: engine/CDefPointer.h's inline ~CDefPointer = Release(): `if (Object) { if (--count == 0) Delete(); Object = 0; }` --
//   the const-T form `dec dword ptr [ecx+4] ; jne` with the clearing store inside the non-null arm.
//   Retail's own constructor (??0CTCHeroSpecialMovement@@QAE@AAVCThing@@@Z; body not reconstructed here) emits the vtable,
//   hence ??_G and this out-of-line ??1.
// Layout: engine/CTCBase.h (retail opt-in) and engine/CDefPointer.h (retail def model); CTCHeroSpecialMovement has no shared header
//   (local class; engine/CHeroSpecialMovementDef.h is the flattened def layout and is not needed: only the pointer is touched).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> over CDefPointeeBase

class CHeroSpecialMovementDef;                              // RTTI : CDefClassBase (Ego_r 0x38 bytes)

class CTCHeroSpecialMovement : public CTCBase {             // vtable 0x0126FE3C
public:
    CTCHeroSpecialMovement(CThing& thing);                  // ??0CTCHeroSpecialMovement@@QAE@AAVCThing@@@Z
    virtual void OnCreate();
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    bool IsSprinting() const;
    bool IsRunning() const;
    void SetSprinting(bool sprinting);
    void SetRunning(bool running);
    bool IsAvailableForSprint() const;
    float GetSprintTimerPercentage() const;
    bool                                    Sprinting;               // +0x0C
    bool                                    Running;                 // +0x0D
    float                                   SprintFramesRemaining;   // +0x10
    long                                    RunTimer;                // +0x14
    CDefPointer<const CHeroSpecialMovementDef> PHeroSpecialMovementDef; // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroSpecialMovement) == 0x1c);

CTCHeroSpecialMovement::CTCHeroSpecialMovement(CThing& thing)
    : CTCBase(thing)
{
}