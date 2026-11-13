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
#ifdef FABLE_RETAIL_CTABLE_CONSTRUCT
    // Retail opt-in (chunk 890, row 0x00551EA0 ConstructSpritesToDraw): the three virtuals CTable introduces after
    // CChangingStateComponent's last slot (141), in vtable order (rtti-vtables.tsv 0x0124A224: slot 142 0x00551EA0,
    // 143 0x005518E0, 144 0x00551BC0; the row calls 143 / 144 as `call [eax+23Ch]` / `call [eax+240h]`). Names, access and
    // signatures: Ego_r.pdb and FableWin.pdb ?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ,
    // ?ConstructHorizontalLine@CTable@NUISystem@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z and
    // ?ConstructVerticalLine@...@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z (protected virtual).
    // Parameter names unknown (descriptive).
protected:
    virtual void ConstructSpritesToDraw();                                                   // slot 142  0x00551EA0
    virtual void ConstructHorizontalLine(CCountedPointer<CComponent> sprite, CCountedPointer<CComponent> separation,
                                         C2DVector& position, unsigned long count, unsigned long separationOffset);   // 143  0x005518E0
    virtual void ConstructVerticalLine(CCountedPointer<CComponent> sprite, CCountedPointer<CComponent> separation,
                                       C2DVector& position, unsigned long count, unsigned long separationOffset);     // 144  0x00551BC0
#endif
#ifdef FABLE_RETAIL_CTABLE_DRAW
    // Retail opt-in (chunk 2006, row 0x00550DC0): the Draw override, CTable vtable 0x0124A224 slot 2 (the CUIInterface slot of the
    // primary vtable). Ego_r.pdb / FableWin.pdb ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z.
public:
    virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*);          // 0x00550DC0
#endif
protected:
    std::map<ETableSprites, CCountedPointer<CComponent> > Sprites;  // +0x15c
    std::vector<unsigned long>* PHorizontalSeparations;             // +0x168
    std::vector<unsigned long>* PVerticalSeparations;               // +0x16c
};
FABLE_STATIC_ASSERT(sizeof(CTable) == 0x170);
}
#endif
