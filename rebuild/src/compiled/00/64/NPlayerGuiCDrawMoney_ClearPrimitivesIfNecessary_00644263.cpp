// NPlayerGui::CDrawMoney::ClearPrimitivesIfNecessary() -- when the money display was not drawn this frame, clear the player GUI money bar's primitive handles
// Leaf: ?ClearPrimitivesIfNecessary@CDrawMoney@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00644263, 25 bytes, thiscall, plain `ret` (the bar call is a tail jump).
// Identity: slot 0 of vtable 0x0125A544 (RTTI .?AVCDrawMoney@NPlayerGui@@); Ego_r.pdb ?ClearPrimitivesIfNecessary@CDrawMoney@NPlayerGui@@UAEXXZ
//   (0x006871B1) and FableWin.pdb (0x020E2DB7). The bar is CPlayerGui::PMoney (CCountedPointer<NPlayerGui::CStatBar>, retail +0x2CC),
//   whose slot 1 is CStatBar::ClearPrimitiveHandles. Twin of the landed CDrawCrime row 0x0064431B (PCrime +0x2EC).
// Extent: the catalog row ran on into the same member of CDrawRenown (0x0064427C, PRenown +0x2D4, vtable 0x0125A550) and
//   CDrawMorality (0x00644295, PMorality +0x2DC, vtable 0x0125A55C); see SPLITS.tsv.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_FRIEND_STAT_DRAWERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_DTOR
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CStatBar.h"

namespace NPlayerGui {
class CDrawMoney : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo&);                       // 0x006441F6
    virtual void ClearPrimitivesIfNecessary();                           // 0x00644263
};
}

void NPlayerGui::CDrawMoney::ClearPrimitivesIfNecessary()
{
    if (!Drawing && !PlayerGui->PMoney.IsNull())
        PlayerGui->PMoney->ClearPrimitiveHandles();
}