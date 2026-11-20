// CGameScriptInterface::EntitySetMaxHealth(const CScriptThing& thing, float health, bool flag) const -- when the script thing is valid and its thing is physical (+0x6c bit 1), set its max health (CThingPhysical::SetMaxHealth @ 0x006A5260 with the float and the flag)
// Leaf: ?EntitySetMaxHealth@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E1C0, length 64 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c, CScriptThing::IsAlive), the thing fetched through slot 11 (+0x2c,
//   GetPThing), CThingPhysical::SetMaxHealth @ 0x006A5260. Shape: the two arguments re-read from the stack, `ret 0Ch`.
// NF171 (2026-11-20, q11767): ported onto the shared headers (engine/CGameScriptInterface.h NSCRIPT, engine/CScriptThing.h, the retail CThing /
//   CThingPhysical chain): 0x006A5260 binds Ego_r.pdb + FableWin.pdb ?SetMaxHealth@CThingPhysical@@QAEXM_N@Z (engine/CThingPhysical.h HEALTHMODIFIERS; the
//   landed self row; was ?SetMaxHealth@CThing_Retail@@QAEXM_N@Z on a local CThing stand-in). The physical test is CThing::GetTypeFlags() & 2
//   (FableWin.pdb ?GetTypeFlags@CThing@@QBEKXZ, inline; `test byte ptr [eax+6Ch],2`) followed by the CThing -> CThingPhysical downcast, as
//   the landed 0x0085FA00 / 0x00869020 spell it. The unused local CThing_Retail / CCharString / CWideString models of the old row are gone.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTHMODIFIERS
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetMaxHealth(const CScriptThing& thing, float health, bool flag) const
{
    if (thing.IsAlive()) {
        CThing* pthing = thing.GetPThing();
        if (pthing->GetTypeFlags() & 2)
            static_cast<CThingPhysical*>(pthing)->SetMaxHealth(health, flag);
    }
}
