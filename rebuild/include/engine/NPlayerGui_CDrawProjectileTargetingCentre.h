#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWPROJECTILETARGETINGCENTRE_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWPROJECTILETARGETINGCENTRE_H
// NPlayerGui::CDrawProjectileTargetingCentre (new, NF239 2026-11-22): Ego_d.pdb TPI 0x3E197 (field list 0x3E196): one public base,
// NPlayerGui::CDrawBase at offset 0, and no data members of its own (methods: two constructors, Draw, the implicit destructor and
// vector deleting destructor); Ego_d sizeof 48 = CDrawBase's (retail 0x28). Retail RTTI .?AVCDrawProjectileTargetingCentre@NPlayerGui@@.
// Only what the CPlayerGui accessors need (CPlayerGui::IsDrawingTargetingCentre 0x00437640 reads the inherited Drawing +0x18).
#include "engine/NPlayerGui_CDrawBase.h"
#ifdef FABLE_RETAIL_DRAWPROJECTILECENTRE_PASS30
class CEnginePrimitivePrepareInfo;
#endif
namespace NPlayerGui {
class CDrawProjectileTargetingCentre : public CDrawBase {
public:
#ifdef FABLE_RETAIL_DRAWPROJECTILECENTRE_PASS30
    // FableWin and Ego_r both declare Draw with this parameter.
    void Draw(const CEnginePrimitivePrepareInfo& info);
#endif
};
}
#endif
