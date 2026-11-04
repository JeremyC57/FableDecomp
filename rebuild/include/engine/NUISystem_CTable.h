#ifndef FABLE_ENGINE_NUISYSTEM_CTABLE_H
#define FABLE_ENGINE_NUISYSTEM_CTABLE_H
// NUISystem::CTable (new, chunk 770): RTTI .?AVCTable@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0.
// Vtables 0x0124A224 (+0, slot 0 the ??_G 0x005540D0), 0x0124A1FC (+4, CObserver) and 0x0124A1F4 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: FableWin.pdb ??0CTable@NUISystem@@QAE@J@Z, ??0CTable@NUISystem@@QAE@ABV01@@Z, ??1CTable@NUISystem@@UAE@XZ,
//   ?GetDataFromDef@CTable@NUISystem@@QAEXXZ.
// Layout (retail ctor 0x005517E0 / dtor 0x00550D50; Ego_r names; STLport map 0xC): Sprites +0x15c,
//   PHorizontalSeparations +0x168, PVerticalSeparations +0x16c; sizeof 0x170 (as the generated engine/CTable.h, retail layout).
// Overrides are not declared (no row needs them).
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIDef.h"
#include "stlport/fable_map.h"
namespace NUISystem {
class CTable : public CChangingStateComponent {
public:
    CTable(long);
    CTable(const CTable&);
    virtual ~CTable();
    void GetDataFromDef();                                          // 0x00551340
protected:
    std::map<ETableSprites, CCountedPointer<CComponent> > Sprites;  // +0x15c
    std::vector<unsigned long>* PHorizontalSeparations;             // +0x168
    std::vector<unsigned long>* PVerticalSeparations;               // +0x16c
};
FABLE_STATIC_ASSERT(sizeof(CTable) == 0x170);
}
#endif
