// CTCHaste::GetActionSpeedMultiplier() const -- 0 while the haste is paused (PauseFrames >= 0), else the global override when it is positive, else the haste's own multiplier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFFB0, length 44 bytes.
// Globals: the float 0.0f pool @ 0x0122DEDC, the action-speed override float @ 0x013BAF18 (debug/console override, <= 0 = off).
// Shape: `test eax,eax ; jl` on +0x14 is a signed `>= 0` test of a long; `fld g ; fcomp 0.0 ; test ah,41h ; jne` is
// `g > 0.0f` with the global re-loaded for the return (no CSE across the compare); the fall-through `fld [ecx+0Ch]` is the member.
// Local model: no engine header exists for CTCHaste (members after CTCBase: Multiplier +0xc, ToTerminate +0x10, PauseFrames +0x14).
struct CThing;
extern float g_ActionSpeedMultiplierOverride;              // 0x013BAF18

struct CTCHaste {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned char _pad08[4];                                // +0x08 CTCBase flags
    float Multiplier;                                       // +0x0c
    bool ToTerminate;                                       // +0x10
    long PauseFrames;                                       // +0x14 (-1 = running)
    float GetActionSpeedMultiplier() const;
};

float CTCHaste::GetActionSpeedMultiplier() const
{
    if (PauseFrames >= 0)
        return 0.0f;
    if (g_ActionSpeedMultiplierOverride > 0.0f)
        return g_ActionSpeedMultiplierOverride;
    return Multiplier;
}
