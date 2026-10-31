// CInputProcessJumpingAndRolling::ProcessRolling(const CInputEvent& event, CProcessedInput& processed) -- HEAD DRAFT (first
// Grade: DIFFER(191v1579) 425 diff lines (parked stub 13v1579/~465): head draft (stick vector 0x3C, length, held-button Y clear, per-frame roll arming at 0.8); direction projection, game-event adds and the action-7 branch not drafted (retry wave 2 sec2c)
// blocks of 1579 bytes): read the roll stick vector (game action 0x3C), its length, zero Y while the button is held, and once
// per frame arm the roll (state 5, counter 10) when the stick passes 0.8; the roll direction projection / game-event adds
// (0x006A418C..0x006A46E5) and the action-7 button branch (0x006A4533) are not drafted.
// Leaf: ProcessRolling
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A40D0, 1579 bytes, `ret 8`, /GS cookie 0x0139C8A8.
#include <math.h>
class CInputEvent;
class CProcessedInput;
struct C2DVector { float X, Y; };
class CWorld { public: static long __fastcall GetFrame(); };             // 0x0049D870
class CInputProcessGameBase {
public:
    bool IsEventButtonHeld(const CInputEvent& event);                     // 0x00687BD0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* out);   // 0x00687D00
    bool IsEventGameAction(long action, const CInputEvent& event);        // 0x00687CF0
    bool IsEventButtonPressed(const CInputEvent& event);                  // 0x00687A90
};
class CInputProcessJumpingAndRolling : public CInputProcessGameBase {
public:
    unsigned char _pad00[0x28];
    C2DVector RollDirection;                                              // +0x28
    long RollState;                                                       // +0x30
    long RollCounter;                                                     // +0x34
    unsigned char _pad38[0x4c - 0x38];
    bool RollArmed;                                                       // +0x4c
    long LastRollFrame;                                                   // +0x50
    void ProcessRolling(const CInputEvent& event, CProcessedInput& processed);
};
void CInputProcessJumpingAndRolling::ProcessRolling(const CInputEvent& event, CProcessedInput& processed)
{
    C2DVector stick;
    if (GetEventGameActionVector(0x3c, event, &stick)) {
        bool rolled = false;
        float magnitude = sqrtf(stick.Y * stick.Y + stick.X * stick.X);
        if (IsEventButtonHeld(event))
            stick.Y = 0.0f;
        long frame = CWorld::GetFrame();
        if (frame != LastRollFrame) {
            if (magnitude > 0.8f && !RollArmed) {
                RollArmed = true;
                if (RollCounter == 0) {
                    RollState = 5;
                    RollCounter = 10;
                }
            }
            if (!(magnitude > 0.8f))
                RollArmed = false;
        }
        (void)rolled;
        return;
    }
    if (IsEventGameAction(7, event) && IsEventButtonPressed(event))
        return;
}
