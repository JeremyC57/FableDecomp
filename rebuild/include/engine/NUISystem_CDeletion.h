#ifndef FABLE_ENGINE_NUISYSTEM_CDELETION_H
#define FABLE_ENGINE_NUISYSTEM_CDELETION_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Layout: original Method+0 and actual four-byte STLport list+4.
class CDeletion { public:
#ifdef FABLE_RETAIL_CDELETION
// Retail opt-in (chunk 735, rows 0x005729D8 / 0x00573792 CTCInventoryExperience::AddStatToList(PC)): the default and copy
// constructors are out of line in retail (0x0042BED4 ??0CDeletion@NUISystem@@QAE@XZ and 0x0042CD84
// ??0CDeletion@NUISystem@@QAE@ABV01@@Z, landed bindings; FableWin.pdb names both); the destructor stays implicit (retail
// expands it as the AssociatedParents list destructor 0x0042ABCA).
CDeletion();
CDeletion(const CDeletion&);
#endif
enum EDeletionMethod { UI_DO_NOT_DELETE=0,UI_DELETE_THIS_INSTANCE=1,UI_DELETE_ALL_INSTANCES=2,UI_DELETE_THIS_INSTANCE_PER_PARENT=3 };
public: NUISystem::CDeletion::EDeletionMethod Method;
public: std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > AssociatedParents;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CDeletion)==8);
#endif
