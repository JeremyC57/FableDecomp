# hdr/ overlay -- chunk 876 (row 0x00CBFB7D RunCutsceneMacro, parked)

Every change is an additive `#ifdef FABLE_RETAIL_*` block (or a new header). Without the macro the default path is the
same as the shared header in rebuild/include. Only differ/00cbfb7d.cpp defines these macros. Nothing landed depends on
them, so fold them into the shared headers only when the row lands.

| Header | Opt-in | What it adds | Retail evidence |
|---|---|---|---|
| fable_string.h (root) | FABLE_RETAIL_CBASICSTRING_TEXT_EQ_BODY | Bodies of CBasicString<T>::Compare (0x00411570) and operator==(const T*) (0x004115A0) visible out of class | Calls stay out of line, but VC knows they write no memory: after `weaponState == "MELEE"` fails it tests "RANGED" without re-testing PStringData (0x00CC38ED) |
| fable_string.h (root) | FABLE_RETAIL_CCHARSTRING_GETLENGTH_EXPANDED | `GetLengthExpanded()` / `StartsWithExpanded(prefix)`: the bodies of GetLength / StartsWith, `__forceinline` under `_MSC_VER` (plain inline elsewhere). Inline-only twins, no symbol | Inline-budget device (session 4): VC7.1's caller-size cap leaves GetLength an out-of-line call (0x00403A00) at 524 sites of RunCutsceneMacro, but retail expands it at 0x00CC0040 (`cue`) and inside StartsWith("$") at 0x00CC03EA. A `__forceinline` wrapper that only calls GetLength stays capped, so the twin carries the body. Fixes exactly those two sites |
| fable_string.h (root) | FABLE_RETAIL_CCHARSTRING_APPEND_CHAR | Declaration `const CCharString& operator+=(char)` | ??YCCharString@@QAEABV0@D@Z, Ego_r.pdb + FableWin.pdb, called at 0x0099F180 |
| fable_string.h (root) | FABLE_RETAIL_CCHARSTRING_APPEND_COUNT | Declaration `void Append(char, unsigned long)` | ?Append@CCharString@@QAEXDK@Z, Ego_r.pdb + FableWin.pdb, called at 0x0099F3B0 (SetLightScene) |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE | `__forceinline` on C3DVector(float,float,float) and the copy ctor | No out-of-line copy in Fable.exe (reloc_bindings) or in the Ego_r name set; retail expands all 30 sites in this 74 KB function |
| engine/CRGBColour.h | FABLE_RETAIL_CRGBCOLOUR_CTOR_FORCEINLINE | `__forceinline` on CRGBColour(r,g,b,a) and the copy ctor | No out-of-line copy in Fable.exe or Ego_r (FableWin 0x018BCBE0 is the debug COMDAT); retail stores bytes in place (0x00CD0C57..0x00CD0C6D) |
| engine/CDefinitionManager.h | FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_BYNAME_BODY | Template body of GetDef-by-name in the TU | Retail emits GetDef<CCutsceneDef> beside its only caller (0x00CD2A71, landed). With the body in view the CDefPointer local stays unexposed: `ebx` caches the def (0x00CBFCD8) and the release has no store of 0 (0x00CD1F4C) |
| engine/CGameScriptInterfaceBase.h | FABLE_RETAIL_GSIBASE_ENDLETTERBOX_PUBLIC | Slot 378 EndLetterBox declared public on the base | `Fullscreen` calls slot 378 through the CGameScriptInterfaceBase* pDSTGame (0x00CD06CD `call [eax+5E8h]`). Access only: no layout, slot or mangling change |
| engine/CSubtitleRenderer.h | FABLE_RETAIL_CSUBTITLERENDERER_CUTSCENE | SetAsInCutscene declaration and inline SetCutsceneAsSkippable | ?SetAsInCutscene@CSubtitleRenderer@@QAEX_N@Z at 0x0049319C (landed). The skippable store is in place (`mov byte ptr [eax+0F8h],1`) |
| engine/CScreenFilterSThingByPass.h | (new header) | Ego_r.pdb struct, 16 bytes: CScriptThing at +0, bool at +0xC; the (CScriptThing&, bool) ctor | Ctor emitted out of line in this TU at 0x00CD2757; layout from ghidra_out/struct_layouts_egor.tsv |

Not kept: an opt-in in stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_FIND_OUTOFLINE) was dropped. The row never defined it,
so the overlay was a no-op.
