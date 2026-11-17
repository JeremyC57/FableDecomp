// std::_Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float>, _Select1st<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> >, less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, allocator<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@U?$_Select1st@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@@4@U?$less@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@4@V?$allocator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567AF0, 191 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, float> tree; Ego_r.pdb members of this type:
//   CTCHeroOnlineScoreboard::LocalScoreboard. Landed callers of this address: 0x00567BB0
//   std::_Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity,std::pair<CTCHe; 0x00568230
//   std::_Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, std::pair<CTCH.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `_unattributed::Insert_CRetailTree_00567af0`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroOnlineScoreboard.h"
namespace std {
template _Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float>, _Select1st<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> >, less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, allocator<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> > >::iterator _Rb_tree<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float>, _Select1st<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> >, less<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity >, allocator<pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity const, float>&, _Rb_tree_node_base*);
}
