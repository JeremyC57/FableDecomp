// std::random_shuffle<CThingBuilding**,RandFunc>(CThingBuilding** first, CThingBuilding** last, RandFunc& rand) -- STLport random_shuffle of the region population's houses with the world's float random generator: for each i after first, swap *i with first[rand(i - first + 1)]
// Leaf: ??$random_shuffle@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513210, 88 bytes (padded to 96), `ret 4` (`__fastcall`, a /Gr algorithm TU:
//   first in ecx, last in edx, the generator by reference on the stack).
// Callers: 0x00505407 (once, on the house list that is then sorted by CIsHouseLargerThan and
//   read through PeekPSimDefinition) in CWorldMap::PopulateRegionWithVillagers (the functor is built on the
//   world's random seed, `[0x013B86A0] + 0x1613C`). Byte-identical twin of 0x00513270 (the CThingAICreature** instance).
//   Catalog label "PopulateRegionWithVillagers_FloatRandom" is descriptive.
// Callees: GFFloatRandom(float, unsigned long*) 0x004978A0 (`__fastcall`: the float pushed, the seed pointer in ecx;
//   PDB ?GFFloatRandom@@YIMMPAK@Z), __ftol2 0x00BFEA70.
// Shape: STLport `if (first == last) return; for (i = first + 1; i != last; ++i) iter_swap(i, first + rand((i - first)
//   + 1));` with __iter_swap (`T tmp = *a; *a = *b; *b = tmp;`) and the functor's `(long)GFFloatRandom((float)n, Seed)`
//   inlined (n homed in the spare frame word for `fild`). The Ego_r name set holds the TU's functor as an
//   anonymous-namespace `RandFunc` (`_Random_shuffle<CThingAICreature**, RandFunc@?A0x37019565, int>`).
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CThingBuilding;
class CThingAICreature;

namespace {
struct RandFunc {
    unsigned long* Seed;                            // +0x00 the world's random seed
    long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); }
};
}

namespace std {

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 i1, ForwardIter2 i2, T*)
{
    T tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 i1, ForwardIter2 i2)
{
    __iter_swap(i1, i2, i1);
}

template <class RandomAccessIter, class RandomNumberGenerator>
void __fastcall random_shuffle(RandomAccessIter first, RandomAccessIter last, RandomNumberGenerator& rand)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        iter_swap(i, first + rand((i - first) + 1));
}

}

template void __fastcall std::random_shuffle<CThingBuilding**, RandFunc>(CThingBuilding** first, CThingBuilding** last, RandFunc& rand);