// CAnimateThing::IsAnimationFinished() const -- finished when there is no current animation, its first entry has no frames left, or the "finished" flag is set
// Leaf: ?IsAnimationFinished@CAnimateThing@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C0E0, length 31 bytes (+ int3 padding).
// Callees: none.
// Layout: the current animation's entry vector at +0x04 (first) / +0x08 (last), its element starting with a
// signed short count; the state flags byte at +0x51 (bit 1 = finished).
// Shape: the three tests jump to the shared `mov eax,1` tail with `xor eax,eax` in the middle: a bool returned as one `||` expression
// materialises 0/1 in eax (an if / return-true form gives `mov al,1`; NF57 lesson). The element is reached through the vector's iterator object
// (`mov edx,eax` re-derives `*begin()`), the shape of the landed CRetailVector bodies.
// Identity (NF59, round 7, queue q10507): Ego_r.pdb 0x005752F0 / FableWin.pdb 0x01E96330 ?IsAnimationFinished@CAnimateThing@@ABE_NXZ (private
//   const, bool); retail placement = FableWin order GetMeshBank / IsAnimationFinished / AnimsCanBeIgnored = 0x0070C0D0 / 0x0070C0E0 /
//   0x0070C100 (NF55, REJECTED nf41 e49). Re-authored on engine/CAnimateThing.h (NF59 opt-in FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED):
//   the reads are CurrentAnim.MainAnimSet (+0x00) Anims (vector<short> +0x04) and its Finished bit (+0x51 bit 1, engine/CAnimSet.h; CAnimSet
//   befriends CAnimateThing). The landed row defined a public long-returning member on a local CAnimateThing / CRetailVector model.
#define FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED
#include "engine/CAnimateThing.h"

bool CAnimateThing::IsAnimationFinished() const
{
    const CAnimSet& set = CurrentAnim.MainAnimSet;
    return set.Anims.empty() || *set.Anims.begin() <= 0 || set.Finished;
}
