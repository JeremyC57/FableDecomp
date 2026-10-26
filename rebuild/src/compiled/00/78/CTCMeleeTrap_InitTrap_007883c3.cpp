// CTCMeleeTrap::InitTrap() -- reset PlayingContinuous / StrikeAnimation; a type-3 trap runs its own start routine (0x00788198), any other trap initialises its "DEFAULT" animation without looping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007883C3, length 67 bytes.
// Callees: the type-3 start routine @ 0x00788198 (__fastcall on the trap; manifest "CEngine::ChangeCameraAngle" propagated),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "DEFAULT" @ 0x01230554, length -1),
// CTCMeleeTrap::InitialiseAnimation(const CCharString&, bool) @ 0x0078809B, CCharString::~CCharString @ 0x0099EAE0.
// Layout (local model, no engine header): the trap type at +0x5c (an unnamed PDB word), PlayingContinuous +0x60 and
// StrikeAnimation +0x61 (PDB +0x6c/+0x6d: three retail containers before them are narrower). 
// Shape: frame pointer with `leave` while esi is used freely: `optimize("y",off)` + `("s",on)`; the type compare is
// scheduled before the two byte clears; the CCharString is a named local in the else block ([ebp-4]).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "fable_string.h"

struct CTCMeleeTrap {
    unsigned char _pad_0x00[0x5c];
    long          m_trapType;                      // +0x5c
    bool          PlayingContinuous;               // +0x60
    bool          StrikeAnimation;                 // +0x61
    void StartType3Trap();                         // 0x00788198
    void InitTrap();
private:
    void InitialiseAnimation(const CCharString&, bool);   // 0x0078809B
};

void CTCMeleeTrap::InitTrap()
{
    PlayingContinuous = false;
    StrikeAnimation = false;
    if (m_trapType == 3) {
        StartType3Trap();
    } else {
        CCharString animationName("DEFAULT", -1);
        InitialiseAnimation(animationName, false);
    }
}