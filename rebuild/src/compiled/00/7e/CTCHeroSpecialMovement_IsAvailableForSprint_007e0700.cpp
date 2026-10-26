// CTCHeroSpecialMovement::IsAvailableForSprint() const -- sprinting is allowed while a sprint still has frames left, or when enough frames have recharged for a new one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0700, 69 bytes (+11 int3 padding).
// Globals: frames-per-second long @ 0x01375550 (15), the sprint-enable byte @ 0x013B86C5, 0.0f @ 0x0122DEDC.
//
// Shape: the minimum frames (`fild fps ; fmul def+2Ch`) are computed into the one-word float frame BEFORE the global
// flag test (a named float local initialised first); `fld ; fcomp ; fnstsw ; test ah,41h ; je` is the ordered `>`
// (true path), `jne` its negation; the two exits share `mov al,1` / `xor al,al` tails.
// Layout: Sprinting +0x0C, SprintFramesRemaining +0x10, PHeroSpecialMovementDef +0x18 (PDB = retail for this class);
// the def's +0x2C float is the sprint recharge time in seconds (named descriptively).

extern long g_FramesPerSecond;                 // 0x01375550
extern bool g_SprintEnabled;                   // 0x013B86C5

class CHeroSpecialMovementDef { public:
    unsigned char _pad_0x00[0x2C];
    float         MinSprintRechargeSeconds;    // +0x2C
};

struct CTCHeroSpecialMovement_Retail {
    void*                    __vftable;        // +0x00
    void*                    ParentThing;      // +0x04 CTCBase
    unsigned char            _pad_0x08[0x4];   // +0x08 CTCBase flags
    bool                     Sprinting;        // +0x0C
    bool                     Running;          // +0x0D
    float                    SprintFramesRemaining;    // +0x10
    long                     RunTimer;         // +0x14
    CHeroSpecialMovementDef* PHeroSpecialMovementDef;  // +0x18 CDefPointer::Object

    bool IsAvailableForSprint() const;
};

bool CTCHeroSpecialMovement_Retail::IsAvailableForSprint() const
{
    float minimumFrames = g_FramesPerSecond * PHeroSpecialMovementDef->MinSprintRechargeSeconds;
    if (!g_SprintEnabled)
        return false;
    if (Sprinting && SprintFramesRemaining > 0.0f)
        return true;
    if (SprintFramesRemaining > minimumFrames)
        return true;
    return false;
}