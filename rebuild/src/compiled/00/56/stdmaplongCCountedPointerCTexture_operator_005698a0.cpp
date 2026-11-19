// std::map<long, CCountedPointer<CTexture> >::operator[](const long& __k) -- the cached texture stored under the id, inserting an empty counted pointer when the id is missing
// Leaf: ??A?$map@JV?$CCountedPointer@VCTexture@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCTexture@@@@@std@@@3@@std@@QAEAAV?$CCountedPointer@VCTexture@@@@ABJ@Z
// Retail: Fable.exe @ 0x005698A0, 150 bytes, thiscall `ret 4`.
// Identity: only caller CTCHeroOnlineScoreboard::GetCachedImage 0x0055FC90 (call at 0x0055FE24) on CTCHeroOnlineScoreboard::PictureCache
//   (+0x10, engine/CTCHeroOnlineScoreboard.h, Ego_r.pdb: map<long, CCountedPointer<CTexture> >); FableWin.pdb names the instantiation
//   (0x022E97D0, the same decorated symbol). The catalog's made-up _global::RBTree_SearchOrCreate is this STLport member (NAME_FIXES.tsv).
// Callees: _Rb_tree<...>::insert_unique(iterator, const value_type&) 0x00568AD0, the count block's deleter, operator delete.
// Shape: the shared stlport/fable_map.h map::operator[] with the opt-ins of its byte-twin family 0x00554030 / 0x00553E60 (visible
//   out-of-line hinted insert_unique, ORIGINAL_INSERT_BODY); the owner class from its shared header.
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/CTCHeroOnlineScoreboard.h"

namespace std {
template CCountedPointer<CTexture>& map<long, CCountedPointer<CTexture> >::operator[](const long&);
}