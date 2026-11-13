#ifndef FABLE_ENGINE_NUISYSTEM_CDELETION_H
#define FABLE_ENGINE_NUISYSTEM_CDELETION_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Layout: original Method+0 and actual four-byte STLport list+4.
class CDeletion { public:
#ifdef FABLE_RETAIL_CDELETION_INLINE
// Retail opt-in (chunk 4007, row 0x0061B610 CTCInventoryQuests::ConstructQuestList): the two user-declared constructors
// (FableWin.pdb ??0CDeletion@NUISystem@@QAE@XZ / ??0CDeletion@NUISystem@@QAE@ABV01@@Z; retail COMDATs 0x0042BED4 / 0x0042CD84)
// with their bodies in the class, so a caller may expand them: the row expands both at 0x0061DB42..0x0061DB85.
CDeletion() { }
CDeletion(const CDeletion& other) : Method(other.Method), AssociatedParents(other.AssociatedParents) { }
#elif defined(FABLE_RETAIL_CDELETION)
// Only for small functions: above VC7.1 c2's 10000-IL-tuple limit (CManager::Action 0x0041E6D3) a user-declared ctor makes every
// CDeletion local escape, so block locals stop sharing a frame slot; there keep the implicit members (single-row 886, session 3).
// Retail opt-in (chunk 735, rows 0x005729D8 / 0x00573792 CTCInventoryExperience::AddStatToList(PC)): the default and copy
// constructors are out of line in retail (0x0042BED4 ??0CDeletion@NUISystem@@QAE@XZ and 0x0042CD84
// ??0CDeletion@NUISystem@@QAE@ABV01@@Z, landed bindings; FableWin.pdb names both); the destructor stays implicit (retail
// expands it as the AssociatedParents list destructor 0x0042ABCA).
#ifdef FABLE_RETAIL_CDELETION_BODIES
// Nested opt-in (chunk 892, row 0x005A887D CTCInventoryTrade::RefreshBuyPossibilityPC): the same two out-of-line constructors, with
// their bodies visible in the TU and kept out of line (retail calls 0x0042BED4 at 0x005A89F5 and 0x0042CD84 at 0x005A8A23). VC7.1
// then knows neither keeps the local's address (with the list<CComponent*> bodies, FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE /
// _DTOR_OUTOFLINE), drops the alias edge between `deletion.Method = ..` and the in-place iterator argument store, and schedules
// the Method store last before the insert call (0x005A8A0D); declaration-only constructors pin it ahead of that store.
#ifdef _MSC_VER
#define FABLE_CDELETION_NOINLINE __declspec(noinline)
#else
#define FABLE_CDELETION_NOINLINE
#endif
FABLE_CDELETION_NOINLINE CDeletion() { }
FABLE_CDELETION_NOINLINE CDeletion(const CDeletion& other) : Method(other.Method), AssociatedParents(other.AssociatedParents) { }
#else
CDeletion();
CDeletion(const CDeletion&);
#endif
#endif
enum EDeletionMethod { UI_DO_NOT_DELETE=0,UI_DELETE_THIS_INSTANCE=1,UI_DELETE_ALL_INSTANCES=2,UI_DELETE_THIS_INSTANCE_PER_PARENT=3 };
public: NUISystem::CDeletion::EDeletionMethod Method;
public: std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > AssociatedParents;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CDeletion)==8);
#endif
