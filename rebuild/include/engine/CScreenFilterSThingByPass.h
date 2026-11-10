#ifndef FABLE_ENGINE_CSCREENFILTERSTHINGBYPASS_H
#define FABLE_ENGINE_CSCREENFILTERSTHINGBYPASS_H
// New header (chunk 1010, row 0x008A9880 CGameScriptInterface::ScreenFilterFadeTo). Ego_r.pdb struct CScreenFilterSThingByPass,
// 16 bytes: CScriptThing thing +0x0, bool bypassSet +0xC (struct_layouts_egor.tsv; the U key of the PDB's vector argument);
// Ego_r.pdb ??0CScreenFilterSThingByPass@@QAE@AAVCScriptThing@@_N@Z and ??1CScreenFilterSThingByPass@@QAE@XZ. Retail reads the
// two members at +0x0 / +0xC with a 0x10 stride (0x008A98C0..C3). The includer supplies engine/CScriptThing.h first.
struct CScreenFilterSThingByPass {
    CScreenFilterSThingByPass(CScriptThing& thing_, bool bypass_set) : thing(thing_), bypassSet(bypass_set) { }
    CScriptThing thing;                                                  // +0x00
    bool bypassSet;                                                      // +0x0c
};
#endif
