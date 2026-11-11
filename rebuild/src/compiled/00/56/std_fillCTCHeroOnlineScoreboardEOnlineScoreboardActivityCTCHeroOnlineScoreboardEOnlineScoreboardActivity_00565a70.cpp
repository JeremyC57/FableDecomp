// std::fill(CTCHeroOnlineScoreboard::EOnlineScoreboardActivity* first, CTCHeroOnlineScoreboard::EOnlineScoreboardActivity* last, CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@W412@@std@@YIXPAW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@0ABW412@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00565A70, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 005686c0 std::vector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, std::allocator<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>>::_M_fill_insert; caller 005686c0 std::vector<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, std::allocator<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity>>::_M_fill_insert; binder ??$fill@PAW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@W412@@std@@YIXPAW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@0ABW412@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCHeroOnlineScoreboard::EOnlineScoreboardActivity.
#include "stlport/fable_containers.h"
class CTCHeroOnlineScoreboard {
public:
enum EOnlineScoreboardActivity { };
};
template void std::fill<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity*,CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >(CTCHeroOnlineScoreboard::EOnlineScoreboardActivity*, CTCHeroOnlineScoreboard::EOnlineScoreboardActivity*, CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const&);
