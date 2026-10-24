// CTCTrapBase::SetTransitionParams(float fadeInTime, float fadeOutTime) -- (re)open the trap's opener fader with the given fade times towards opaque black
// Leaf: SetTransitionParams
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787C35, 45 bytes, __fastcall `ret 8` (unaligned start: the size-optimised
//   trap TU packs its functions without padding).
// Identity: Ego_r.pdb `?SetTransitionParams@CTCTrapBase@@IAEXMM@Z`; called from 0x00787D45 and 0x007884DA (the landed
//   CTCProjectileTrap::TriggerTrap 0x00787CA5 names it too).
// Callees: CFadeInFadeOutBase::Open(float, float, CRGBColour) @ 0x00434840 (landed; `ret 0Ch`).
// Layout: Ego_r.pdb CTCTrapBase with the retail 12-byte vectors (HitThings, ImmuneThings, TriggerThings each 4 bytes
//   below the PDB): TrapOpener (CFadeInFadeOutBase) at +0x38 (PDB +0x44). engine/CTCTrapBase.h carries the PDB offsets,
//   so the class is a local model here. CRGBColour is B, G, R, A in memory (as the landed 0x00434840 source).
// Shape: `TrapOpener.Open(fadeInTime, fadeOutTime, CRGBColour(0, 0, 0, 255));` -- the colour is built in place in its
//   argument slot (`push ecx ; mov eax,esp`, stores R +2, G +1, B +0 from one zero register, A +3) before the two float
//   slots are filled; the call is not a tail call (three argument words against the caller's two). Size TU:
//   `#pragma optimize("s", on)` gives the x87 float copies into the pre-reserved slots (no frame pointer here, unlike the
//   trap TU's `y`-off bodies); CRGBColour needs its user copy constructor to be built in its argument slot.
#pragma optimize("s", on)
class CThing;

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};

struct CFadeInFadeOutBase {                              // 0x1c bytes
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);   // 0x00434840
};

class CTCTrapBase {
public:
    virtual void v00();
    CThing*            ParentThing;                      // +0x04 (CTCBase)
    unsigned char      _pad08[0x38 - 0x08];              // CTCBase flags, HitThings +0x0c, ImmuneThings +0x18,
                                                         // PObstruction +0x24, TriggerThings +0x2c (retail sizes)
    CFadeInFadeOutBase TrapOpener;                       // +0x38 (PDB +0x44)

    void SetTransitionParams(float fadeInTime, float fadeOutTime);
};

void CTCTrapBase::SetTransitionParams(float fadeInTime, float fadeOutTime)
{
    TrapOpener.Open(fadeInTime, fadeOutTime, CRGBColour(0, 0, 0, 255));
}