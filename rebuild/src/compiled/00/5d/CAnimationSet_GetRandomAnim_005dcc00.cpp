// CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter, long index_to_ignore) const
//   -- walk the run of animations whose key equals the first one named `name`; of those the filter accepts, pick one at random
//   (GFRandom on `seed`) among the ones whose BankIndex is not index_to_ignore; if there is none, return the (last) accepted one
//   with that BankIndex, else Anims.end()->second.
// Leaf: ?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z
// Flags: none (TU default /O2 /Oy /W3; no /EHsc: retail has no EH frame around the local vector)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DCC00, 264 bytes, __thiscall `ret 10h`. Ego_r.pdb / FableWin.pdb publics give the self spelling.
// Callees: vector<const pair<unsigned long, CAnimationEntry>*>::reserve 0x005E15E0 and ::_M_insert_overflow 0x005E1500 (both
//   out of line, STLport stl/_vector.c bodies visible in the TU: their pick-any copies are byte- and relocation-equal to retail),
//   CAnimationSet::FindFirstAnimWithName(const CCharString&) const 0x005DC020, the filter's virtual operator() (slot 0),
//   GFROR13 0x00497890 (inside the inline GFRandom), free 0x00BFEA14 (the vector's storage).
// Source shape: Ego_d.pdb (FableLib animation_set.cpp lines 1003..1034; locals anim_to_ignore_iterator, found_anims, index,
//   start_it, `it` in the for scope, the pointer anim_entry in its body) and FableWin.exe's /Od body of the same function, whose
//   code order is the statement order below: ASSERT(AnimExists(name, filter)) (FableWin calls 0x0180C460 -> 0x01FA1400 =
//   ?AnimExists@CAnimationSet, FableWin publics 0002:8463360; line 1004, compiled out in release), the vector,
//   anim_to_ignore_iterator constructed from Anims.end() after it (EH state 4 follows the vector's 3), reserve(5), start_it, the
//   loop, the empty case with its ASSERT, then index = GFRandom(size, &seed) and found_anims[index]->second.
// Visible-body opt-ins: STLP_VECTOR_OVERFLOW_OUTOFLINE and STLP_VECTOR_RESERVE (+ _ALLOCATE_AND_COPY / _C, the landed 0x0077E580
//   owner set): with the bodies visible c2 knows neither call captures `it` or the vector, so it keeps `it` in edi (homed over the
//   dead `name` slot) and _M_finish in esi across the loop, as retail does.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterBase.h"
#include "engine/CAnimationSet.h"
#include "engine/GFRandom.h"
#include "fable_assert.h"

const CAnimationEntry& CAnimationSet::GetRandomAnim(const CCharString& name, unsigned long& seed, const CAnimFilterBase& filter,
                                                    long index_to_ignore) const
{
    FABLE_RETAIL_ASSERT(AnimExists(name, filter));

    std::vector<TAnimMap::const_iterator> found_anims;
    TAnimMap::const_iterator anim_to_ignore_iterator = Anims.end();
    found_anims.reserve(5);

    TAnimMap::const_iterator start_it = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = start_it; it != Anims.end() && it->first == start_it->first; ++it)
    {
        const CAnimationEntry* anim_entry = &it->second;
        if (filter(*anim_entry))
        {
            if (anim_entry->BankIndex == index_to_ignore)
                anim_to_ignore_iterator = it;
            else
                found_anims.push_back(it);
        }
    }

    if (found_anims.empty())
    {
        FABLE_RETAIL_ASSERT(anim_to_ignore_iterator != Anims.end());
        return anim_to_ignore_iterator->second;
    }

    long index = GFRandom(found_anims.size(), &seed);
    return found_anims[index]->second;
}
