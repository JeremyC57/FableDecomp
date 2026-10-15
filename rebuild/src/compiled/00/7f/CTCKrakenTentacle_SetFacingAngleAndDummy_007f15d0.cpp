// CTCKrakenTentacle::SetFacingAngleAndDummy(float facingAngle, const CCharString& dummyName) -- store the facing angle and the dummy name, then move the tentacle back to its desired position (ReturnToDesiredPos(true))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F15D0, length 36 bytes.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, CTCKrakenTentacle::ReturnToDesiredPos(bool) @ 0x007F0F40.
// Layout (local model, no engine header): FacingAngle +0x44, DummyName +0x48 (PDB offsets).
// Shape: the float is copied as a dword through eax (loaded first, stored after the string argument push); `this` parked
// in esi for the final call.
#include "fable_string.h"

struct CTCKrakenTentacle {
    unsigned char _pad_0x00[0x44];
    float         FacingAngle;                     // +0x44
    CCharString   DummyName;                       // +0x48
    void ReturnToDesiredPos(bool immediately);     // 0x007F0F40
    void SetFacingAngleAndDummy(float facingAngle, const CCharString& dummyName);
};

void CTCKrakenTentacle::SetFacingAngleAndDummy(float facingAngle, const CCharString& dummyName)
{
    FacingAngle = facingAngle;
    DummyName = dummyName;
    ReturnToDesiredPos(true);
}