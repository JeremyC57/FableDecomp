// std::map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[](const NUISystem::ETableSprites& __k) -- the counted pointer stored under the sprite key, inserting an empty one when the key is missing
// Leaf: ??A?$map@W4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@U?$less@W4ETableSprites@NUISystem@@@std@@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@5@@std@@QAEAAV?$CCountedPointer@VCComponent@NUISystem@@@@ABW4ETableSprites@NUISystem@@@Z
// Retail: Fable.exe @ 0x00554030, 150 bytes, thiscall `ret 4`.
// Identity: called four times by NUISystem::CTable::GetDataFromDef 0x00551340 (landed) on CTable::Sprites (+0x15C,
//   engine/NUISystem_CTable.h: std::map<ETableSprites, CCountedPointer<CComponent> >); signed key compare (`jl`), the value at node +0x14.
// Callees: _Rb_tree<...>::insert_unique(iterator, const value_type&) 0x00553C50 (landed), the count block's deleter, operator delete.
// Shape: STLport 4.6 map::operator[] from the shared stlport/fable_map.h, with the same opt-ins as the landed map<unsigned long,
//   unsigned long>::operator[] 0x00553E60 of this TU region (the visible out-of-line hinted insert_unique, ORIGINAL_INSERT_BODY): the
//   pair temporary's zero stores land after the argument pushes and its CCountedPointer release tests the decrement's flags, as retail.
//   The parked local model was the residue. The enum and class come from the caller's shared headers (0x00551340's include set).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NUISystem_CTable.h"

namespace std {
template CCountedPointer<NUISystem::CComponent>& map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[](const NUISystem::ETableSprites&);
}
