// CPlayerGui::RemovePerceivingThing(CThing& thing) -- forward to the perceiver display: `mov ecx,[ecx+1B8h] ; jmp 0x0064639c`
// Leaf: ?RemovePerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437900 (11 bytes).
// NF39 (2026-11-16): moved off its T::Run stand-in onto the shared engine/CPlayerGui.h (opt-ins FABLE_RETAIL_CPLAYERGUI_MEMBERS for
//   PDrawPerceivers +0x1B8 and FABLE_RETAIL_CPLAYERGUI_PERCEIVERS, which names 0x00437900): Ego_r.pdb 0x0044A5A0 ?RemovePerceivingThing@CPlayerGui@@QAEXAAVCThing@@@Z
//   (Ego_r donor order 0x0044A590 / 0x0044A5A0 = retail 0x004378F0 / 0x00437900) + FableWin.pdb; the callee is Ego_r.pdb
//   ?RemovePerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z (retail 0x0064639C). NPlayerGui::CDrawPerceivers has no shared header: its
//   member used here is declared below. Preamble from the landed 0x004379E0 (the same MEMBERS opt-in).
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
#define FABLE_RETAIL_CPLAYERGUI_PERCEIVERS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"

namespace NPlayerGui {
class CDrawPerceivers {
public:
    void RemovePerceivingThing(CThing& thing);                              // 0x0064639C
};
}

void CPlayerGui::RemovePerceivingThing(CThing& thing)
{
    PDrawPerceivers->RemovePerceivingThing(thing);
}
