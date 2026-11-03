#ifndef FABLE_ENGINE_NUISYSTEM_CYESNO_H
#define FABLE_ENGINE_NUISYSTEM_CYESNO_H
#ifndef FABLE_RETAIL_CYESNO
#error "NUISystem_CYesNo.h: define FABLE_RETAIL_CYESNO (retail model only)"
#endif
// NUISystem::CYesNo (chunk 735, row 0x00577AC0): RTTI .?AVCYesNo@NUISystem@@ (rtti_bases.tsv: primary base
// NUISystem::CChangingStateComponent at 0, CObserver at +4, CActionParam at +24). Only the base relation is modelled (the
// yes/no dialog component CTCInventoryExperience::pYesNo points at); its own members are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CYesNo : public CChangingStateComponent {
};
}
#endif
