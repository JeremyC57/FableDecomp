// std::__uninitialized_copy(CHeroLogBook::CHeroLogBookEntry::ECategory* first, CHeroLogBook::CHeroLogBookEntry::ECategory* last, CHeroLogBook::CHeroLogBookEntry::ECategory* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAW4123@@std@@YIPAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAW4123@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00612770, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00618ad0 source (stdvectorCHeroLogBookCHeroLogBookEntryECategory_Mfillinsert_00618ad0.cpp) names CHeroLogBook::CHeroLogBookEntry::ECategory; binder ??$__uninitialized_copy@PAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAW4123@@std@@YIPAW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAW4123@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeroLogBook::CHeroLogBookEntry::ECategory.
#include "stlport/fable_containers.h"
class CHeroLogBook {
public:
class CHeroLogBookEntry {
public:
enum ECategory { };
};
};
template CHeroLogBook::CHeroLogBookEntry::ECategory* std::__uninitialized_copy<CHeroLogBook::CHeroLogBookEntry::ECategory*,CHeroLogBook::CHeroLogBookEntry::ECategory* >(CHeroLogBook::CHeroLogBookEntry::ECategory*, CHeroLogBook::CHeroLogBookEntry::ECategory*, CHeroLogBook::CHeroLogBookEntry::ECategory*, std::__false_type const&);
