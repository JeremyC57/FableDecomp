// std::list<CQuestManager::CBoast>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel, counting the nodes
// Leaf: ?size@?$list@VCBoast@CQuestManager@@V?$allocator@VCBoast@CQuestManager@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B87D0, 24 bytes, thiscall, plain `ret`; no callees.
// Identity (NF394, T3 nf391 q10): the only retail reference is `call 0x004B87D0` at 0x0056B157 inside CTCBoastUI::Activate 0x0056AD40 (landed,
//   Leaf ?Activate@CTCBoastUI@@QAEXXZ; the table already binds this spelling to 0x004B87D0 with that witness). FableWin's
//   ?Activate@CTCBoastUI@@QAEXXZ (tc_boast_ui.obj) calls exactly one list size, ?size@?$list@VCBoast@CQuestManager@@V?$allocator@VCBoast@
//   CQuestManager@@@std@@@std@@QBEIXZ (0x0239C528 / 0x0239D4CA); Ego_r.pdb types CQuestManager::Boasts / AcceptedBoasts / TempBoasts as
//   list<CQuestManager::CBoast>. STLport spells list<T, allocator<T> >::size() const, size_t, exactly that symbol.
// NF394: was the made-up `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated), shared
//   with 0x006CD240 / 0x00DA0ED0 (NoDuplicates duplicates at link). Model: the 0x005143E0 form (NF391) -- the shared stlport/fable_containers.h
//   list with STLport's own distance(begin(), end()) (the 0x00ADB570 form, NF39); explicit instantiation emits the member as its own pick-any
//   COMDAT, as retail kept every used inline member. The element type from the shared engine/CQuestManager.h (nested CBoast).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "fable_string.h"
#include "engine/CQuestManager.h"
#include "stlport/fable_map.h"

template unsigned int std::list<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::size() const;
