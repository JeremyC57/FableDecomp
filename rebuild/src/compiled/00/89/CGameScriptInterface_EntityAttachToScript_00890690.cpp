// CGameScriptInterface::EntityAttachToScript(const CScriptThing& entity, const CCharString& script_name) const -- a live thing's serialisation section name becomes the script name
// Leaf: ?EntityAttachToScript@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890690. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: entity.GetPThing() (CScriptThing slot 11, +0x2c), ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z, then the inline
//   ?SetSerialisationSectionName@CThing@@QAEXABVCCharString@@@Z (SerialisationSectionName +0x58 = name: ??4CCharString@@QAEAAV0@ABV0@@Z, retail
//   0x0099EFB0). Its CDebugManager::DoErrorMessage block (thing not alive) is debug-only (absent in retail).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"

void CGameScriptInterface::EntityAttachToScript(const CScriptThing& entity, const CCharString& script_name) const
{
    CThing* thing = entity.GetPThing();
    if (IS_THING_ALIVE(thing))
        thing->SetSerialisationSectionName(script_name);
}
