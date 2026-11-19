// CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const -- from the first entry stored under the name's CRC, the entry_num-th one (counting only entries the filter accepts) of that equal-key run
// Leaf: ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
// Retail: Fable.exe @ 0x005DC340, 66 bytes, `ret 0Ch`. Ego_r.pdb animation_set.obj #65 / FableWin.pdb #192 (0x01FA17C0); callee
//   CAnimationSet::FindFirstAnimWithName 0x005DC020 (landed), the filter's virtual operator() (slot 0).
// Shape: FableWin /Od 0x01FA17C0 -- start_it = FindFirstAnimWithName(name); a for loop over it from start_it while
//   `it != Anims.end() && it->first == start_it->first`, returning it->second when the filter accepts it and entry_num is 0, else
//   counting entry_num down; Ego_d locals start_it / it (it in the loop scope), parameter entry_num.
// Unreachable: the loop never runs out in a valid call (the debug build asserts GetNoAnims(name, filter) > entry_num first), and the
//   end of the function is marked unreachable. Evidence: retail tests neither `it != end` nor the key (0x005DC360..0x005DC376 is an
//   endless loop) yet keeps `this` in edi across the call (`mov edi,ecx` 0x005DC349, never read): the end() test was there when
//   registers were assigned and was deleted afterwards. Ego_r.pdb's VC10 release body (0x0063BD40) drops the same two tests; the FableWin
//   /Od body has the tests and no code after the loop (an `__assume(0)` emits none at /Od). Graded: the same loop falling off the end
//   keeps both tests (DIFFER(..v66)); a loop without the tests loses the dead edi (DIFFER(56v66)); with `__assume(0)` after the loop:
//   RELOCATION_MATCH. Same unreachable-marker form as the landed CPersistContext switch defaults (engine/CPersistContext.h,
//   `#ifdef _MSC_VER __assume(0); #endif`).
// The debug-only ASSERT(GetNoAnims(name, filter) > entry_num) at the top is not modelled (no release bytes; the dead-arm form leaves the
//   code unchanged).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include "fable_string.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"

const CAnimationEntry& CAnimationSet::GetAnim(const CCharString& name, long entry_num, const CAnimFilterBase& filter) const
{
    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = start_it; it != Anims.end() && it->first == start_it->first; ++it)
    {
        if (filter(it->second))
        {
            if (entry_num == 0)
                return it->second;
            entry_num--;
        }
    }
#ifdef _MSC_VER
    __assume(0);
#elif defined(__GNUC__)
    __builtin_unreachable();   // portable twin of the MSVC unreachable marker (no MSVC bytes)
#endif
}