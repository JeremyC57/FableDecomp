// std::map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent* >::map() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$map@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@U?$less@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@std@@V?$allocator@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@PAVCComponent@NUISystem@@@std@@@7@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00617140, 54 bytes; node 0x18.
// Identity (proven, agentFAM07): CTCInventoryMap::CategoriesList (Ego_r.pdb map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>): the landed
//   ~CTCInventoryMap 0x00606F80 calls this tree's _M_erase 0x00613AE0 and binds it with exactly these types; the ctor is the COMDAT
//   right before the map's destructor 0x00617180; node 0x18.
//   Was the placeholder CUnprovenContainer_00613ae0::CUnprovenContainer_00613ae0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CHeroLogBook.h (nested CHeroLogBookEntry::ECategory) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
namespace NUISystem { class CComponent; }
#include "stlport/fable_map.h"
#include "engine/CHeroLogBook.h"

template std::map<CHeroLogBook::CHeroLogBookEntry::ECategory, NUISystem::CComponent*>::map();
