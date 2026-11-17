// std::vector<bool>::push_back(const bool& x) -- STLport vector push_back of a bool vector (one byte per element): store x at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@_NV?$allocator@_N@std@@@std@@QAEXAB_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053CB4F, length 48 bytes, `ret 4` (thiscall on the vector, &x).
// Identity: vector<bool> (sweep3; the landed rows said vector<unsigned char>): this size-optimised TU group (resize 0x00467686, erase 0x004676BB,
//   _M_fill_insert 0x004676EF, _M_insert_overflow 0x00467831, push_back 0x0053CB4F) serves vector<bool> members only: its callers
//   hold Ego_r vector<bool> members (CTCSpecialAbilities::AbilityAvailability 0x0072C6E0, CVertexGroupBlockOptimiser::RemainingBlocks
//   0x00ADA240, CTCWeaponTrail::ActiveTrail, CTCTavernTable::SeatsCurrentlyUsed, NUISystem::CList::ScrollingChildrenVisibility via
//   push_back) and GFSerialiseVectorBinaryIn<bool> 0x004675F6 calls resize (already bound as ?resize@?$vector@_N@std@@...). The
//   fill is STLport's generic byte loop (x re-read each pass), while the char-type overloads fill/fill_n use memset (the real
//   vector<unsigned char> overflow 0x00485B6C calls fill 0x0047CE40); retail STLport stores one byte per bool.
// Callees: vector<bool>::_M_insert_overflow(pos, x, const __true_type&, 1, true) @ 0x00467831 (landed).
// Callers: NUISystem::CList 0x0053835E, 0x0053840F, 0x0053B092, 0x0053BEDE, 0x0053C2AB and 0x00CDC98C.
// Label: catalog "std::deque_push_back_or_allocate" is descriptive; the body is STLport vector::push_back over a 1-byte
//   POD.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the placement copy is null tested (`test eax,eax`),
//   the __true_type temporary sits in the high byte of the `x` argument slot (`lea edx,[ebp+0Bh]`). Size-optimised
//   frame-pointer TU (`push ebp ; mov ebp,esp`, unaligned entry: `optimize("y", off)` + `("s", on)`), as the landed
//   CUIStateMap push_back 0x0053CFC2 of the same CList TU.
// Name fix (NF56, round 7, queue q10462): the local `namespace std { template <class T> class vector ... }` model replaced by the
//   shared STLport vector of stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY: the overflow 0x00467831 is its own
//   landed row), so the explicit instantiation is the PDB-shaped STLport spelling ?push_back@?$vector@_NV?$allocator@_N@std@@@std@@QAEXAB_N@Z
//   (the template default allocator in the mangled name; the local model's leaf was ?push_back@?$vector@_N@std@@QAEXAB_N@Z).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"

template void std::vector<bool>::push_back(const bool& x);
