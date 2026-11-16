// CPlayerGui::AddAutoPickup(long defIndex) -- forward to the auto-pickup bar: `mov ecx,[ecx+2FCh] ; jmp 0x00647511`
// Leaf: ?AddAutoPickup@CPlayerGui@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437980 (11 bytes).
// NF39 (2026-11-16): moved off its T::Run stand-in onto the shared engine/CPlayerGui.h (opt-ins FABLE_RETAIL_CPLAYERGUI_MEMBERS for
//   PAutoPickups +0x2FC and FABLE_RETAIL_CPLAYERGUI_AUTOPICKUP, which names 0x00437980): Ego_r.pdb ?AddAutoPickup@CPlayerGui@@QAEXJ@Z (0x0044A630,
//   16 bytes, between OnHeroHit and AddHitByMelee) + FableWin.pdb; the callee is Ego_r.pdb ?AddObject@CAutoPickupBar@NPlayerGui@@QAEXJ@Z
//   (0x0068E281; catalog 0x00647511 NPlayerGui::CAutoPickupBar::AddObject). NPlayerGui::CAutoPickupBar has no shared header: its one member
//   used here is declared below. Preamble from the landed 0x004379E0 (the same MEMBERS opt-in).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI_AUTOPICKUP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"

namespace NPlayerGui {
class CAutoPickupBar {
public:
    void AddObject(long defIndex);                                      // 0x00647511
};
}

void CPlayerGui::AddAutoPickup(long defIndex)
{
    PAutoPickups->AddObject(defIndex);
}
