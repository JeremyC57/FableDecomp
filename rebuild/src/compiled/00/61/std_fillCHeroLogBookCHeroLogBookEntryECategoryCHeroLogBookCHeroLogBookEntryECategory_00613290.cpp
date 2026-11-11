// std::fill(CHeroLogBook::CHeroLogBookEntry::ECategory* first, CHeroLogBook::CHeroLogBookEntry::ECategory* last, CHeroLogBook::CHeroLogBookEntry::ECategory const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@W4123@@std@@YIXPAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@0ABW4123@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00613290, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00618ad0 source (stdvectorCHeroLogBookCHeroLogBookEntryECategory_Mfillinsert_00618ad0.cpp) names CHeroLogBook::CHeroLogBookEntry::ECategory; binder ??$fill@PAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@W4123@@std@@YIXPAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@0ABW4123@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeroLogBook::CHeroLogBookEntry::ECategory.
#include "stlport/fable_containers.h"
class CHeroLogBook {
public:
class CHeroLogBookEntry {
public:
enum ECategory { };
};
};
template void std::fill<CHeroLogBook::CHeroLogBookEntry::ECategory*,CHeroLogBook::CHeroLogBookEntry::ECategory >(CHeroLogBook::CHeroLogBookEntry::ECategory*, CHeroLogBook::CHeroLogBookEntry::ECategory*, CHeroLogBook::CHeroLogBookEntry::ECategory const&);
