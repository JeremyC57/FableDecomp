// std::swap(KnickKnack& left, KnickKnack& right) -- exchange two spot-the-addition knick-knack records through a by-value temporary
// Leaf: ??$swap@UKnickKnack@@@std@@YIXAAUKnickKnack@@0@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D9A00, 81 bytes (BOUNDS_FIX), __fastcall (left in ecx, right in edx), plain `ret`. No direct
//   E8/E9 callers and no data reference (an unreferenced COMDAT, as its byte twins 0x00806220 / 0x008068B0 over pair<C3DVector,long>).
// Identity: FableWin.pdb ??$iter_swap@PAUKnickKnack@@PAU1@@std@@YIXPAUKnickKnack@@0@Z (0x027565F0) and
//   ??$swap@UKnickKnack@@@std@@YIXAAUKnickKnack@@0@Z (0x027568A0); KnickKnack is Ego_r.pdb's 16-byte {C3DVector Pos; CThingObject*
//   Thing}, the same words as pair<C3DVector,long> (which is why the four bodies are identical: the vector words through the frame
//   by C3DVector's copy constructor, the pointer in eax). This TU is CTCTavernGameSpotTheAddition's (vector<KnickKnack> helpers
//   0x007D96F0..0x007D9FC0 and the landed random_shuffle<KnickKnack*> 0x007D9B40, whose shuffle swaps knick-knacks). Which of the
//   two addresses is iter_swap and which swap is INFERRED from the sibling TU's order (iter_swap 0x00806220 before swap 0x008068B0):
//   0x007D97E0 = iter_swap, 0x007D9A00 = swap. The manifest label CNearestBuildingWithProperty (a 24-byte class) does not fit.
#include "engine/C3DVector.h"
class CThingObject;
struct KnickKnack {                                     // 16 bytes (Ego_r.pdb KnickKnack)
    C3DVector     Pos;                                  // +0x00
    CThingObject* Thing;                                // +0x0c
};
namespace std {
template <class T> inline void swap(T& left, T& right) { T temporary = left; left = right; right = temporary; }
template <class I1, class I2> inline void iter_swap(I1 left, I2 right) { std::swap(*left, *right); }
}
template void std::swap<KnickKnack>(KnickKnack&, KnickKnack&);