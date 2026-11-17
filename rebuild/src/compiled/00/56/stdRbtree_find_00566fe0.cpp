// std::_Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<const EOnlineScoreboardActivity, vector<pair<float, CWideString> > >, _Select1st, less, allocator>::find(const key_type&) -- STLport 4.6 find: _M_find inlined -- lower-bound walk from the root, end() unless the found key is not greater; iterator returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566FE0, 71 bytes.
// NF50 (round 7, nf41 e185): the instantiation is proven: its only landed caller CTCHeroOnlineScoreboard::DisplayScoreboardGUI
// 0x00563920 already binds this STLport symbol over the shared engine/CTCHeroOnlineScoreboard.h member map (FableWin.pdb /
// Ego_r.pdb CTCHeroOnlineScoreboard map<EOnlineScoreboardActivity, vector<pair<float, CWideString> > >; the key is the enum,
// compared signed). Was the placeholder CUnprovenKey/Value/KeyOfValue_00566fe0 instantiation over a private STLport copy; now the
// explicit instantiation of the shared stlport/fable_map.h find (descriptive-by-library).
// Leaf: ?find@?$_Rb_tree@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@U?$_Select1st@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@4@U?$less@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@4@V?$allocator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@V?$vector@U?$pair@MVCWideString@@@std@@V?$allocator@U?$pair@MVCWideString@@@std@@@2@@std@@@std@@@2@@2@ABW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroOnlineScoreboard.h"
namespace std {
template _Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > >, _Select1st<pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > > >, less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, allocator<pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > > > >::iterator _Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > >, _Select1st<pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > > >, less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, allocator<pair<const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, vector<pair<float, CWideString> > > > >::find(const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity&);
}
