// CTCEmoteIcon::SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha) -- ignore a request of lower priority (a larger number); a positive icon takes over the emote (icon, scale, priority, type 1, alpha) and starts its timer, any other icon clears it (priority 8, icon 0, timer off)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AAB0, length 81 bytes.
// Callees: CTCEmoteIcon::SetActiveTimer(float) @ 0x0071AA50 (row of this chunk).
// Layout: PDB CTCEmoteIcon (48 bytes) over CTCBase (engine/CTCBase.h): ActiveTimer +0xc, EmoteText +0x10,
// EmoteIcon +0x14, EmoteScale +0x18, EmoteType +0x1c, EmotePriority +0x20, EmoteAlpha +0x24.
// Shape: five stack arguments (`ret 14h`); `cmp priority,[ecx+20h] ; jg` is the signed early return; the float
// scale is copied as a dword; `this` stays in ecx for the SetActiveTimer call (no store can clobber it), and
// the clearing arm pushes the 0.0f literal as `push 0`. The calls are not tail jumps (the argument counts differ).
#include "engine/CTCBase.h"

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
    void SetActiveTimer(float seconds);            // 0x0071AA50
    void SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha);
};

void CTCEmoteIcon::SetEmoteIcon(long priority, long icon, float seconds, float scale, unsigned char alpha)
{
    if (priority > EmotePriority)
        return;
    if (icon > 0) {
        EmoteIcon = icon;
        EmoteScale = scale;
        EmotePriority = priority;
        EmoteType = 1;
        EmoteAlpha = alpha;
        SetActiveTimer(seconds);
    } else {
        EmotePriority = 8;
        EmoteIcon = 0;
        SetActiveTimer(0.0f);
    }
}