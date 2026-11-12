#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWDAMAGE_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWDAMAGE_H
// NPlayerGui::CDrawDamage (new, chunk 2043): Ego_r.pdb CDrawDamage, 56 bytes (struct_layouts_egor.tsv): the CDrawBase part (PDB 44,
// retail 0x28: its handle vector is STLport's 12 bytes) and DamageValues, list<NPlayerGui::CDrawDamage::SDamageValue> (PDB +44,
// retail +0x28: `mov eax,[esi+28h] ; cmp [eax],eax`, Draw 0x0064ED5D; the landed Update 0x00644CE2 / AddDamageChange 0x006472B2).
// SDamageValue: Ego_r.pdb Duration (long) +0, Change (float) +4, Hero (bool) +8, 12 bytes; FableWin.pdb
// ??0SDamageValue@CDrawDamage@NPlayerGui@@QAE@JM_N@Z (inline constructor). Members: Ego_r.pdb / FableWin.pdb
// ??0CDrawDamage@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@..@std@@@Z (0x0064D3DA),
// ?Update@CDrawDamage@NPlayerGui@@QAEXXZ (0x00644CE2), ?AddDamageChange@CDrawDamage@NPlayerGui@@QAEXM_N@Z (0x006472B2),
// ?Draw@CDrawDamage@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (0x0064ED4F). The vtable 0x0125A5A4 holds CDrawBase's
// two virtuals only (0x00644627, 0x00442760).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawDamage : public CDrawBase {
public:
    class SDamageValue {
    public:
        SDamageValue(long duration, float change, bool hero) : Duration(duration), Change(change), Hero(hero) { }
        long Duration;                                                   // +0x00
        float Change;                                                    // +0x04
        bool Hero;                                                       // +0x08
    };
    void Update();                                                       // 0x00644CE2
    void AddDamageChange(float change, bool hero);                       // 0x006472B2
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064ED4F
protected:
    std::list<SDamageValue> DamageValues;                                // +0x28
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawDamage) == 0x2c);
#endif
