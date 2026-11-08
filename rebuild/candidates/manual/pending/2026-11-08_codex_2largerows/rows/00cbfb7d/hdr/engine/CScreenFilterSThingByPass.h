#ifndef FABLE_ENGINE_CSCREENFILTERSTHINGBYPASS_H
#define FABLE_ENGINE_CSCREENFILTERSTHINGBYPASS_H
// CScreenFilterSThingByPass: Ego_r.pdb struct (U-keyed: fable_nscript_forward.h, ScreenFilterFadeTo's
// vector<UCScreenFilterSThingByPass>), 16 bytes: CScriptThing thing +0x00, bool bypassSet +0x0C
// (ghidra_out/struct_layouts_egor.tsv). One thing the cutscene tint (GSI slot 675 ScreenFilterFadeTo) leaves untinted.
// Constructor ??0CScreenFilterSThingByPass@@QAE@AAVCScriptThing@@_N@Z (Ego_r.pdb + FableWin.pdb; the thing by non-const
// reference): inline, emitted out of line in the RunCutsceneMacro TU at 0x00CD2757 (`push [esp+8] ; call
// CScriptThing::CScriptThing(const&) 0x004ABE90 ; mov al,[esp+0Ch] ; mov [esi+0Ch],al ; ret 8`). The destructor
// (??1CScreenFilterSThingByPass@@QAE@XZ) is compiler-generated (the member CScriptThing's).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CScriptThing.h"
struct CScreenFilterSThingByPass {
    CScriptThing thing;                                          // +0x00
    bool bypassSet;                                              // +0x0C
    CScreenFilterSThingByPass(CScriptThing& t, bool bypass) : thing(t), bypassSet(bypass) { }
};
FABLE_STATIC_ASSERT(offsetof(CScreenFilterSThingByPass, bypassSet) == 0xc);
FABLE_STATIC_ASSERT(sizeof(CScreenFilterSThingByPass) == 0x10);
#endif
