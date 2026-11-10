#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMCCOUNTEDPOINTER_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMCCOUNTEDPOINTER_H
#ifndef FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER
#error "engine/NUISystem_CActionParamCCountedPointer.h is a retail model: define FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER"
#endif
// Retail model (new, chunk 1002b, rows 0x005C3E91 / 0x005C47D5 CTCInventoryWeapons::FinalConstructWeaponItemEntries(PC)):
// NUISystem::CActionParamCCountedPointer<T>, the counted-pointer UI action parameter (UI_ACTION_PARAM_TYPE_COUNTED_POINTER = 9).
// Ego_r.pdb ??0?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@QAE@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// (retail 0x004854DF: ActionParamType 9 at +4, vptr 0x01236794, Value copy-constructed at +8; called out of line by the rows, so
// declared here only) and ??1?$CActionParamCCountedPointer@VCComponent@NUISystem@@@NUISystem@@UAE@XZ (the only virtual).
// Layout struct_layouts_egor.tsv CActionParamCCountedPointer<NUISystem::CComponent>: Value +8, sizeof 0x10 (operator new(0x10)).
#include "engine/NUISystem_CComponent.h"
#include "engine/CCountedPointer.h"
namespace NUISystem {
template <class T>
class CActionParamCCountedPointer : public CActionParam {
public:
    CActionParamCCountedPointer(CCountedPointer<T>& value);          // CComponent: 0x004854DF
    virtual ~CActionParamCCountedPointer();
    CCountedPointer<T> Value;                                         // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>) == 0x10);
#endif
