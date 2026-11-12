// NUISystem::CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>&) -- the base component ignores the action (empty virtual, `ret 4`); vtable 0x0124608C slot 73
// Leaf: ?AddActionOnRightClicked@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::AddActionOnRightClicked(CCountedPointer<CAction>&) {}
}
