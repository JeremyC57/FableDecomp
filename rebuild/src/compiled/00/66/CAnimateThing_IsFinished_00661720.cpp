// CAnimateThing::IsFinished() const -- the animation counts as finished when the queue says so or it was explicitly marked finished
// Leaf: ?IsFinished@CAnimateThing@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661720, length 32 bytes.
// Callees: CAnimateThing::IsAnimationFinished() const @ 0x0070C0E0 (manifest label; Ego_r
// ?IsAnimationFinished@CAnimateThing@@ABE_NXZ).
// Label correction: the manifest's "CTCWife::IsInLoveWithHero" is propagated (the CTCWife body is the landed 0x006CF900).
// `this` is the CAnimateThing that 0x0070C0E0 reads, and the same test is inlined at 0x006659FC, next to a call of
// SetAsFinished (0x0070D580) followed by `or byte ptr [edi+94h],1`: bit 0 of the byte at +0x94 is the "marked finished"
// flag. No code or data references this copy (an out-of-line instance of an inline accessor); the name is descriptive.
// Shape: `a || b` with a bool result gives the separate `xor eax,eax` / `mov eax,1` tails; the flag is a one-bit
// bitfield (`test byte ptr [esi+94h],1`).
// NF59 (round 7, queue q10507): the class is engine/CAnimateThing.h (NF59 opt-ins FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED: the
//   private const ?IsAnimationFinished@CAnimateThing@@ABE_NXZ, Ego_r.pdb 0x005752F0 / FableWin.pdb 0x01E96330; and _ISFINISHED: this
//   descriptive accessor; Interruptable is the header's bit at +0x94); the local model declared IsAnimationFinished public (QBE).
#define FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED
#define FABLE_RETAIL_CANIMATETHING_ISFINISHED
#include "engine/CAnimateThing.h"

bool CAnimateThing::IsFinished() const
{
    return IsAnimationFinished() || Interruptable;
}
