// NUISystem::CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>&) -- the base component ignores the action (empty virtual, `ret 4`); vtable 0x0124608C slot 80
// Leaf: ?AddActionOnLeftUnclicked@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::AddActionOnLeftUnclicked(CCountedPointer<CAction>&) {}
}
