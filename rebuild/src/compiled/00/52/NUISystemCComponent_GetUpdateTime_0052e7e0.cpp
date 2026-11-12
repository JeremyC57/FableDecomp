// NUISystem::CComponent::GetUpdateTime() const -- the base component has no update time (returns 0.0f, the pooled literal at 0x0122DEDC); vtable 0x0124608C slot 54
// Leaf: ?GetUpdateTime@CComponent@NUISystem@@UBEMXZ
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
float CComponent::GetUpdateTime() const { return 0.0f; }
}
