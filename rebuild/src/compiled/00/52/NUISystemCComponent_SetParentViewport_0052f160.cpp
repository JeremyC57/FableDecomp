// NUISystem::CComponent::SetParentViewport(NUISystem::CComponent*) -- store the parent viewport (+0xdc); vtable 0x0124608C slot 90
// Leaf: ?SetParentViewport@CComponent@NUISystem@@UAEXPAV12@@Z
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::SetParentViewport(CComponent* pParentViewport) { PParentViewport = pParentViewport; }
}
