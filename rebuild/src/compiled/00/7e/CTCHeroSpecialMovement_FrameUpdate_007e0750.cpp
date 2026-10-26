// CTCHeroSpecialMovement::FrameUpdate() -- per-frame hero run/sprint state: with the run option on, holding run (action 31) sets Running and running long enough starts a sprint; otherwise the sprint button (action 30) sprints directly; a sprint drains the sprint bar and stops when it is empty, otherwise the bar refills
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0750, 385 bytes (+ int3 padding), `ret` (virtual, no arguments).
//   Ego_r.pdb: `?FrameUpdate@CTCHeroSpecialMovement@@UAEXXZ`.
// Callees: CThing::GetPlayer @ 0x004C7A10 and its const twin @ 0x004C7A30, CPlayer::GetPlayerNumber @ 0x00487510,
//   CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) @ 0x00446B10,
//   IsAvailableForHeroRun(CThing*) @ 0x0085F560 and IsAvailableForHeroSprint(CThing*) @ 0x0085F470 (`__fastcall`),
//   __ftol2 @ 0x00BFEA70.
// Data: the run-option byte @ 0x013756EA, g_FramesPerSecond @ 0x01375550 (long); 0x0122DED8 is 1.0f, 0x0122DEDC 0.0f.
// Shape: SetRunning / SetSprinting (retail 0x007E06B0 / 0x007E0670, same TU) are defined inline in the class, so VC7.1
//   expands them here and threads their flag tests into the surrounding branches (`if (on) { if (!flag &&
//   IsAvailable()) flag = true; } else if (flag) flag = false;`); the second and third player reads go through the const
//   GetPlayer on a const thing reference. The refill clamp really is `if (max > remaining) remaining = max`
//   (fld max ; fcom [remaining] ; test ah,41h ; jne skip). Both clamps are free inline helpers on a `float&` (the
//   member is stored and re-read around them); the sprint-end test is `if (empty || !available) SetSprinting(false)`.
// Layout (Ego_r.pdb, matches retail): CTCBase 0x0c, Sprinting +0x0c, Running +0x0d, SprintFramesRemaining +0x10,
//   RunTimer +0x14, PHeroSpecialMovementDef +0x18; CHeroSpecialMovementDef TotalTimeToSprintSeconds +0x28,
//   TotalTimeToFullyRestoreSprintBarSeconds +0x30, NoSecondsToRunBeforeSprintStarts +0x34; CPlayer +0x0c is the game
//   player interface, +0x218 a control-lock byte.
#include "engine/CTCBase.h"

class CGamePlayerInterface { public:
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber);   // 0x00446B10
};
class CPlayer { public:
    unsigned char _pad000[0x0c];
    CGamePlayerInterface* m_pInterface;                 // +0x0c
    unsigned char _pad010[0x218 - 0x10];
    bool m_controlsLocked;                              // +0x218
    long GetPlayerNumber() const;                       // 0x00487510
};
class CThing { public:
    CPlayer* GetPlayer();                               // 0x004C7A10
    const CPlayer* GetPlayer() const;                   // 0x004C7A30
};
bool __fastcall IsAvailableForHeroRun(CThing* thing);       // 0x0085F560
bool __fastcall IsAvailableForHeroSprint(CThing* thing);    // 0x0085F470

class CHeroSpecialMovementDef { public:
    unsigned char _pad00[0x28];
    float TotalTimeToSprintSeconds;                     // +0x28
    float MinimumNumberOfSecondsLeftToBeAllowedToSprint;   // +0x2c
    float TotalTimeToFullyRestoreSprintBarSeconds;      // +0x30
    float NoSecondsToRunBeforeSprintStarts;             // +0x34
};
extern long g_FramesPerSecond;                          // 0x01375550
extern bool g_bHeroRunOption;                           // 0x013756EA

// In-place float clamps through a reference: the referenced member is stored and re-read from memory
// (`fstp [esi+10h] ; fld [esi+10h]`, `fcom [esi+10h]`) where a direct member test keeps it on the x87 stack.
inline void ClampLow(float& value, float lo) { if (value < lo) value = lo; }
inline void Raise(float& value, float hi) { if (hi > value) value = hi; }

class CTCHeroSpecialMovement : public CTCBase { public:
    bool Sprinting;                                     // +0x0c
    bool Running;                                       // +0x0d
    float SprintFramesRemaining;                        // +0x10
    long RunTimer;                                      // +0x14
    const CHeroSpecialMovementDef* PHeroSpecialMovementDef;   // +0x18 CDefPointer<const CHeroSpecialMovementDef>

    const CThing& GetThing() const { return *ParentThing; }
    void SetSprinting(bool sprinting)                   // 0x007E0670
    {
        if (sprinting) {
            if (!Sprinting && IsAvailableForHeroSprint(ParentThing))
                Sprinting = true;
        } else if (Sprinting)
            Sprinting = false;
    }
    void SetRunning(bool running)                       // 0x007E06B0
    {
        if (running) {
            if (!Running && IsAvailableForHeroRun(ParentThing))
                Running = true;
        } else if (Running)
            Running = false;
    }
    void FrameUpdate();
};

void CTCHeroSpecialMovement::FrameUpdate()
{
    CGamePlayerInterface* input = ParentThing->GetPlayer()->m_pInterface;
    if (g_bHeroRunOption) {
        SetRunning(input->IsGameActionEventInQueue(0x1f, 2, GetThing().GetPlayer()->GetPlayerNumber())
                   && !GetThing().GetPlayer()->m_controlsLocked && IsAvailableForHeroRun(ParentThing));
        if (Running) {
            if (!Sprinting) {
                if (++RunTimer > (long)(g_FramesPerSecond * PHeroSpecialMovementDef->NoSecondsToRunBeforeSprintStarts))
                    SetSprinting(true);
            } else
                RunTimer = 0;
        } else
            SetSprinting(false);
    } else {
        SetSprinting(input->IsGameActionEventInQueue(0x1e, 2, GetThing().GetPlayer()->GetPlayerNumber())
                     && IsAvailableForHeroSprint(ParentThing));
    }
    if (Sprinting) {
        SprintFramesRemaining -= 1.0f;
        ClampLow(SprintFramesRemaining, 0.0f);
        if (SprintFramesRemaining == 0.0f || !IsAvailableForHeroSprint(ParentThing))
            SetSprinting(false);
        if (Sprinting)
            return;
    }
    SprintFramesRemaining += PHeroSpecialMovementDef->TotalTimeToFullyRestoreSprintBarSeconds
                             / PHeroSpecialMovementDef->TotalTimeToSprintSeconds;
    Raise(SprintFramesRemaining, g_FramesPerSecond * PHeroSpecialMovementDef->TotalTimeToSprintSeconds);
}