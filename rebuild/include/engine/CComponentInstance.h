#ifndef FABLE_ENGINE_CCOMPONENTINSTANCE_H
#define FABLE_ENGINE_CCOMPONENTINSTANCE_H
#include "engine/CPSCBase.h"
#include "engine/CCountedPointer.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CComponentInstance:public CBaseClass {
public:CCountedPointer<CPSCBase> Component;bool Enabled;unsigned long InstanceName;
};
}
#endif
