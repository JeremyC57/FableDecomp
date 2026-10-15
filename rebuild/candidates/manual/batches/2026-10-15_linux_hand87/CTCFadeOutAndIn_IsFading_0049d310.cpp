// CTCFadeOutAndIn::IsFading() const -- true while the component is fading in or fading out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D310, length 23 bytes.
// Callers: CWorld::UpdateRegionLoad @ 0x004A3740, CThing::IsAffectedByBulletTime @ 0x004C9920 (the result is used as a
// byte: `neg al ; sbb eax,eax ; inc eax`).
// Layout: PDB CTCFadeOutAndIn (40 bytes) over CTCBase (engine/CTCBase.h): FadingOut +0xc, FadingIn +0xd.
// Shape: `return FadingIn || FadingOut;` -- the short-circuit `||` materialises a full-register 0/1 (`xor eax,eax` /
// `mov eax,1` exits) even for a bool result; FadingIn (+0xd) is tested first.
#include "engine/CTCBase.h"

struct CTCFadeOutAndIn : CTCBase {
    bool  FadingOut;                               // +0x0c
    bool  FadingIn;                                // +0x0d
    float Alpha;                                   // +0x10
    long  FramesLeftForFade;                       // +0x14
    bool IsFading() const;
};

bool CTCFadeOutAndIn::IsFading() const
{
    return FadingIn || FadingOut;
}
