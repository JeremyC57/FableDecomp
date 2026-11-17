// std::map::operator[](const key& k) -- (map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, float>) STLport subscript: lower_bound, insert (k, 0.0f) when absent, return the mapped best score
// Leaf: ??A?$map@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@MU?$less@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@std@@V?$allocator@U?$pair@$$CBW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@M@std@@@4@@std@@QAEAAMABW4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005692B0, 115 bytes, thiscall, `ret 4`, returns &(*it).second.
// Identity: the manifest's "std::map<int,int>::insert" is propagated. Ego_r.pdb names
//   ??A?$map@W4EOnlineScoreboardActivity@CTCHeroOnlineScoreboard@@MU?$less@... (the scoreboard's LocalScoreboard map);
//   the landed CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0 and CreateStringFromMyScores 0x0055F480
//   declare `float& operator[](const EOnlineScoreboardActivity&)` @ 0x005692B0.
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) 0x00568230 (the iterator argument built in its slot,
//   the iterator result returned through the dead `k` argument slot).
// Shape: as the map<unsigned long, unsigned long> subscript 0x00553E60: STLport 4.6 map::operator[] verbatim with
//   `_M_lower_bound` inline (the enum key compares signed: `jl` / `jge`); `__default_constructed((float*)0)` is the
//   late `mov [esp+1Ch],0` store of 0.0f after the argument pushes, which needs insert_unique's same-TU out-of-line
//   stand-in body that reads the value.
// NF61 (round 7, q10572): moved onto stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[] body, PASS22 __default_constructed,
//   FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY: the visible out-of-line hinted
//   insert_unique that reads the value, which schedules the pair temporary's late second-word store as retail -- the 0x00553E60 /
//   0x00A4A670 pattern); the local std::map model and its same-TU insert_unique stand-in are gone. The call binds the tree's
//   insert_unique(iterator, const value_type&) NF58 landed; the self leaf is the 4-argument STLport spelling. The landed caller 0x0055F3B0 already binds that spelling; 0x00560720 binds a 2-argument stand-in (NAME_FIXES); no landed caller bound the old 3-argument self spelling. The enum comes from engine/CTCHeroOnlineScoreboard.h (was a local class model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#include "engine/CTCHeroOnlineScoreboard.h"
namespace std {
template float& map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, float>::operator[](const CTCHeroOnlineScoreboard::EOnlineScoreboardActivity&);
}
