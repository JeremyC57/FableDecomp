// CTCEmoteIcon::SetActiveTimer(float seconds) -- -1 seconds means "forever" (ActiveTimer = -1), 0 means "off" (ActiveTimer = 0), anything else is converted to a frame count through the world's static seconds-to-frames helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AA50, length 85 bytes.
// Callees: the CTCBase accessor @ 0x00686D60 (out of line: returns [g_pMainGameComponent 0x013B8A1C]+0x1c, `this` unused; the object is not identified -- the manifest's CViewBase::GetDisplayEngine is propagated, landed 0x007E1F70 calls it GetSubsystem),
// CWorldTime::SecondsToFrames(float) @ 0x00415FC1 (`fild g_framesPerSecond ; fmul [esp+4] ; __ftol ; ret 4`: a
// static `__stdcall` -- no `this` is loaded for it and nothing is popped after the call).
// Layout: PDB CTCEmoteIcon (48 bytes) over CTCBase (engine/CTCBase.h): ActiveTimer +0xc, EmoteText +0x10,
// EmoteIcon +0x14, EmoteScale +0x18, EmoteType +0x1c, EmotePriority +0x20, EmoteAlpha +0x24, Replaceable +0x25,
// Enabled +0x26, PrimitiveHandle +0x28.
// Shape: `fld [-1.0f] ; fld seconds ; fucompp ; test ah,44h ; jp` is `seconds == -1.0f` (the right operand is
// loaded first); each arm keeps its own `pop esi ; ret 4`. The static helper is reached through that accessor's
// object (`GetGameSubsystem1C()->SecondsToFrames(seconds)`): the getter call is kept for its side effects and its
// result discarded, as in the landed 0x006588DC. -1.0f is the pool constant @ 0x0122DEE0, 0.0f @ 0x0122DEDC.
#include "engine/CTCBase.h"

struct CWorldTime {
    static long __stdcall SecondsToFrames(float seconds);   // 0x00415FC1
};

struct CTCEmoteIcon : CTCBase {
    long          ActiveTimer;                     // +0x0c
    void*         EmoteText;                       // +0x10 CWideString
    long          EmoteIcon;                       // +0x14
    float         EmoteScale;                      // +0x18
    long          EmoteType;                       // +0x1c EEmoteType
    long          EmotePriority;                   // +0x20 EEmotePriority
    unsigned char EmoteAlpha;                      // +0x24
    bool          Replaceable;                     // +0x25
    bool          Enabled;                         // +0x26
    CWorldTime* GetGameSubsystem1C();              // 0x00686D60
    void SetActiveTimer(float seconds);
};

void CTCEmoteIcon::SetActiveTimer(float seconds)
{
    if (seconds == -1.0f)
        ActiveTimer = -1;
    else if (seconds == 0.0f)
        ActiveTimer = 0;
    else
        ActiveTimer = GetGameSubsystem1C()->SecondsToFrames(seconds);
}