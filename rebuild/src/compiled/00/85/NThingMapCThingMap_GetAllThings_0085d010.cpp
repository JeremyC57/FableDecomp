// NThingMap::CThingMap::GetAllThings(set<CTCMapwho*>& out) -- inserts every mapwho component held in any cell of any mapwho level into the set
// Leaf: ?GetAllThings@CThingMap@NThingMap@@QAEXAAV?$set@PAVCTCMapwho@@U?$less@PAVCTCMapwho@@@std@@V?$allocator@PAVCTCMapwho@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D010, length 138 bytes (`ret 4`, + int3 padding).
// Callees: the set's tree insert_unique(const value_type&) @ 0x0085E140 (NF58; pair<iterator,bool> by hidden pointer).
// Globals: NThingMap::MaxLayers @ 0x013BA848 (?MaxLayers@NThingMap@@3JA, as the landed Validate 0x0085CDE0).
// Shape: three nested loops (level `jl`, cell `jb` with size re-read, node walk); the insert result is an unused
// pair<iterator,bool> temporary; `this` and the level counter live in the frame.
// NF61 (round 7, q10582): moved onto the shared header engine/NThingMap_CThingMap.h (FABLE_RETAIL_MAP_VALIDATE variant: the
//   Ego_r.pdb CThingMap / CThingMapCell / CMapwhoNode records and the declared GetAllThings(std::set<CTCMapwho*>&)) and the
//   stlport/fable_map.h set (inline set::insert forwarding to the tree's insert_unique). The local CThingMap / CTCMapwhoSet /
//   vector models are gone; the call binds NF58's _Rb_tree<CTCMapwho*,...>::insert_unique 0x0085E140 and the self leaf is the
//   FableWin.pdb spelling ?GetAllThings@CThingMap@NThingMap@@QAEXAAV?$set@PAVCTCMapwho@@...@Z (was QAEXPAUCTCMapwhoSet@2@@Z).
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_C2DBOXI
#include "engine/NThingMap_CThingMap.h"
void NThingMap::CThingMap::GetAllThings(std::set<CTCMapwho *,std::less<CTCMapwho *>,std::allocator<CTCMapwho *> > & out)
{
    for (long level = 0; level < MaxLayers; ++level) {
        for (unsigned long i = 0; i < Cells[level].size(); ++i) {
            for (CMapwhoNode* node = Cells[level][i].PFirstNode; node; node = node->PNext)
                out.insert(node->PMapwho);
        }
    }
}
