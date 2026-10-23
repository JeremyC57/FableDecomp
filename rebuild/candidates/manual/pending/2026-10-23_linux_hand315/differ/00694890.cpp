// Grade: DIFFER(1v1140) -- parked unreached (skeleton only): aligned-stack frame (and esp,-8) animation update with x87 timing (ftol 0xBFEA70), anim-entry counted pointers and a cut-scene event map walk; not reached beyond decode.
// CCreatureActionBase::UpdateActionAnimation() -- PARK SKELETON (decoded only)
// Leaf: UpdateActionAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00694890, length 1152 bytes.
// Decode: aligned-stack frame (and esp,-8) animation update with x87 timing (ftol 0xBFEA70), anim-entry counted pointers and a cut-scene event map walk; not reached beyond decode.
#pragma optimize("s", on)
struct CCreatureActionBase {
    unsigned char _pad[0x100];
    void UpdateActionAnimation();
};

void CCreatureActionBase::UpdateActionAnimation()
{
}
