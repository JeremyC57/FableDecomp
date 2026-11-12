// CHitParameters::SetThingHitting(CThing& thing) -- remember the thing doing the hitting (PThingHitting)
// Leaf: ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008685F0, 71 bytes, thiscall, `ret 4`.
// NF28 (2026-11-12, queue land1011ei): was landed as unattributed::AssignTarget008685f0 (CClass_008685f0 stand-in, one of 27 bodies of
//   this shape). Identity: Ego_r.pdb 0x0083E4C0 / FableWin.pdb 0x02810420 ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z;
//   engine/CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_CTOR already declares it at 0x008685F0 (landed callers 0x007CCE20, 0x007C4430);
//   the landed callers 0x00838BC0 GetMeleeHitParameters / 0x00838EC0 GetShotHitParameters call it on a CHitParameters right before
//   SetWeapon 0x00868CE0 (same TU, same opening shape: the CIntelligentPointer<CThing> temporary of the T* assignment). Layout:
//   PThingHitting +0x50 (Ego_r +0x58; retail 12-byte DamageMap / Augmentations), PWeapon +0x58.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CHitParameters.h"

void CHitParameters::SetThingHitting(CThing& thing)
{
    PThingHitting = &thing;
}
