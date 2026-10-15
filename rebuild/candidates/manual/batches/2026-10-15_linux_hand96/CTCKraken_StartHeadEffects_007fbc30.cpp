// CTCKraken::StartHeadEffects() -- create the head effect named "MEETS_WATER" (literal @ 0x01271D00) (CreateHeadEffect with a temporary CCharString)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBC30, length 44 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (length -1), CTCKraken::CreateHeadEffect(const CCharString&)
// @ 0x007FBB00, CCharString::~CCharString @ 0x0099EAE0.
// Shape: a named CCharString local in the `push ecx` frame slot built with the explicit -1 length, its address passed on,
// destroyed after the call; `this` parked in esi.
#include "fable_string.h"

struct CTCKraken {
    void CreateHeadEffect(const CCharString& effectName);   // 0x007FBB00
    void StartHeadEffects();
};

void CTCKraken::StartHeadEffects()
{
    CCharString effectName("MEETS_WATER", -1);   // literal @ 0x01271D00
    CreateHeadEffect(effectName);
}
