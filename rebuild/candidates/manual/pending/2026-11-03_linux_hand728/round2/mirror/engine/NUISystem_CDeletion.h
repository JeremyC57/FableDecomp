#ifndef FABLE_ENGINE_NUISYSTEM_CDELETION_H
#define FABLE_ENGINE_NUISYSTEM_CDELETION_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Layout: original Method+0 and actual four-byte STLport list+4.
class CDeletion { public:
enum EDeletionMethod { UI_DO_NOT_DELETE=0,UI_DELETE_THIS_INSTANCE=1,UI_DELETE_ALL_INSTANCES=2,UI_DELETE_THIS_INSTANCE_PER_PARENT=3 };
public: NUISystem::CDeletion::EDeletionMethod Method;
public: std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > AssociatedParents;
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CDeletion)==8);
#endif
