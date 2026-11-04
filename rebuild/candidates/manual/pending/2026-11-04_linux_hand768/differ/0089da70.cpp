// CGameScriptInterface::EntitySetAlpha(const CScriptThing& entity, float alpha, bool includeCarried) const -- set the live entity's graphic alpha (alpha clamped to [0, 1], scaled to a byte) and, when asked, the alpha of every live thing it carries
// Grade: DIFFER(314v314) -- loop registers: retail keeps the carried-things vector start in eax and the element in ecx (`mov edx,ecx`), ours start in ecx and element in eax; same length.
// Leaf: ?EntitySetAlpha@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089DA70, 312 bytes (+ int3 padding), thiscall `ret 0Ch`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C slot 510 (engine/CGameScriptInterface.h); Ego_r.pdb ?EntitySetAlpha@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z.
// Callees (PDB spellings): CScriptThing slots 75 IsAlive / 11 GetPThing, __ftol2 0x00BFEA70, NGraphicAppearance::CTCGraphicAppearance::
//   SetAlpha(unsigned char) 0x004BFAC0, CVectorMap<ETCInterfaceType,CTCBase*>::Find 0x0040F020 (inside CThing::GetTC<CTCCarrying>,
//   interface 0x46: UsingComponents word 2 bit 6), CTCCarrying::GetAllCarriedThings(std::vector<CThing*>&) const 0x006A2220 (Ego_r.pdb;
//   landed bindings spell it GetCarriedThings / GetAllItems, NAME_FIXES), free 0x00BFEA14 (the vector's storage).
//   Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x01230014 = 255.0f.
// Model: the retail CThing / CScriptThing / CGameScriptInterface headers (as the landed CameraUseCameraPoint 0x0089E190) and the
//   STLport vector; the two declarations are header opt-ins (CTCCARRYING_ALLCARRIED, CTCGRAPHICAPPEARANCE_SETALPHA).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

void CGameScriptInterface::EntitySetAlpha(const CScriptThing& entity, float alpha, bool includeCarried) const
{
    if (!entity.IsAlive())
        return;
    CThing* thing = entity.GetPThing();
    NGraphicAppearance::CTCGraphicAppearance* appearance = thing->GetTCGraphicAppearance();
    if (!appearance)
        return;
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    appearance->SetAlpha((unsigned char)(alpha * 255.0f));
    CTCCarrying* carrying;
    if (includeCarried && thing->GetTC(&carrying)) {
        std::vector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned long i = 0; i < carried.size(); ++i) {
            CThing* item = carried[i];
            if (item && !item->IsDead() && item->GetTCGraphicAppearance())
                item->GetTCGraphicAppearance()->SetAlpha((unsigned char)(alpha * 255.0f));
        }
    }
}
