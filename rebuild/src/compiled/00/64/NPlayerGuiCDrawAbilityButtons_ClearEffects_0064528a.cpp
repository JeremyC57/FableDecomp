// NPlayerGui::CDrawAbilityButtons::ClearEffects() -- clear the ability-button drawer's particle effects through the static CDrawBase::ClearEffects(vector&)
// Leaf: ?ClearEffects@CDrawAbilityButtons@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064528A, `add ecx,48h ; jmp 0x006445BA`.
// Name: Ego_r.pdb 0x00687E5E + FableWin.pdb 0x020F6BFE ?ClearEffects@CDrawAbilityButtons@NPlayerGui@@QAEXXZ (public, non-virtual);
//   the landed callers 0x00438230 / 0x00439070 (CPlayerGui) already bind this spelling at this address. Was the placeholder
//   ?GetTarget_0064528a@CClass_0064528a@@UAEPAVCBaseObject@@XZ (NF177, q11775).
// Layout: engine/NPlayerGui_CDrawAbilityButtons.h (FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23, sizeof 84): Effects +0x48.
// Callees: NPlayerGui::CDrawBase::ClearEffects 0x006445BA (static __fastcall, the vector in ecx).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#include "fable_compiler.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"

void NPlayerGui::CDrawAbilityButtons::ClearEffects()
{
    CDrawBase::ClearEffects(Effects);
}
