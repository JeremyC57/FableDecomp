# NF183 notes (round 10, globals and free functions) -- partial hand-back (context)

Entries handled in queue order q4131 .. q11766-in-family (q4131..q5122, q6198/q6199, plus the duplicate queue lines of the same
addresses: q4442, q4962, q11701, q11703, q11766). q6243 onward are `open` (not started) in APPLIED.tsv, except the duplicates above.

## Rows changed (rows/)
- 0x00416268, 0x0041627F (q4131): extern `g_Name` -> `GVersion` (forward `class CVersion;`), the CLoader stand-in's first parameter
  `const char*` -> `const CVersion&` in place (each stand-in callee 0x009F4CF0 / 0x009F5E10 has this row as its only binder: exe scan).
- 0x00CB28F0 (q4175): `_GetReadingString` was a file `static` (local symbol `__GetReadingString`); now an external definition, binding and
  DEFINING ?_GetReadingString@@3P6GIPAUHIMC__@@IPAGPAH2PAI@ZA. No other landed object defines it (grep). Coordinator: this is a new
  strong data definition in the row's object (the SDK TU's own global), not a COMDAT.
- 0x00B2C4E0 (q4200): local CPixelFormat / CPixelFormatInfo / g_PixelFormatInfo -> engine/CPixelFormat.h; the made-up HasAlpha() is
  `GetNoABits() > 0`. 0x0129BA4C is GD3DFormats[0].AlphaBits (GD3DFormats = 0x0129BA40), so the entry's address is base + 0xC.
- 0x0099D470, 0x0099D520, 0x0099E260 (q4243): g_ModuleLanguage / g_LanguageID -> gApplicationLanguage (0x0099E260 defines it).
- 0x004017B0 (q4245): SConfigDetectionInfo stand-in -> engine/ConfigDetection.h (fields mapped by offset to the Ego_r TPI members:
  cf_MaximumResolution +0x04, cf_UseFixedFunction +0x14, cf_SafeMode +0x58, cf_UseAlternateRenderTargetClear +0x70 ... cf_GenericFlag1
  +0xBC); defines pConfig. Test rewritten on the header.
- 0x012297F0 (q4256): BUILDING_BOUNDRY's atexit thunk off the AutoTinyThunk stand-in onto fable_string.h (qualified ~CCharString call);
  self leaf unchanged (placeholder family, its `_$E` form needs the TU that defines the static). Test rewritten on fable_string.h.
- 0x00AB4700 (q4288/q4442): ResolveMouseWindowOwner stand-in -> GFGetSystemManager()->GetApplicationWinHandle() (engine/CSystemManager.h
  _MOUSE opt-in, as 0x00AB5030). Test rewritten on the header (mock GFGetSystemManager over 0x180 bytes of storage).
- 0x00B49AA0, 0x00B49A40, 0x00B49FE0, 0x00B47980 (q4502/q4505): local CDynamicObjectHeap models -> engine/CDynamicObjectHeap.h with the
  NF183 opt-in FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE; heaps respelled to the PDB LOCAL_HEAP_CEngineLightingManager__* statics.
  0x00B47980 also moves 0x01437450 (not an entry) onto LOCAL_HEAP_CEngineLightingManager__ManagerShadowedSpotLight, the spelling its
  landed initializer 0x01224780 defines (narrows that split; Ego_d.pdb public + Ego_r/FableWin initializer names).
- 0x0044B800, 0x0057F78F, 0x00614450, 0x006C7B00, 0x006C7C30, 0x00CB7A00 (q4628): stand-in ctor CCharString(unsigned __int64) ->
  free __fastcall GFUHUGEToCharString (declaration placed after each row's existing local CCharString model; transformer work/../tools/huge.py).
  Their tests are standalone replicas and are unchanged.
- 0x0092B800 (q5049/q11703): g_InvSqrtMantissaTable -> GFastOneOverSqrtTable (unsigned long[128], C3DVector.h's spelling).

## Pick-any check (EXTRA_RULES 2026-11-20)
- New COMDATs per symdiff (tools/symdiff.sh: HEAD vs hand-back object, defined external symbols): 0x00B2C4E0 +GetNoABits@CPixelFormat,
  0x00AB4700 +GetApplicationWinHandle@CSystemManager (header inlines; reloc_bindings maps neither to an address). All other changed rows:
  no new defined symbol except the data definitions named above (0x00CB28F0 _GetReadingString, 0x0099E260 gApplicationLanguage,
  0x004017B0 pConfig: each replaces the row's own old definition of the same object). Checked with objdump that the other REFS objects
  (0x00CB36E0, 0x00CB4340, 0x0099DCB0, 0x0099D470, 0x0099D520) leave these symbols undefined, and a source grep finds no other definition.
  _GetReadingString is a NEW strong data definition (the row used a file static before): coordinator decision.
- HELD by the rule: 0x0054ED90 CText::GetDataFromDef. Its port onto NUISystem_CText.h / CSystemManager.h / CFontManager.h grades
  RELOCATION_MATCH (work/blocked/0054ed90.cpp, opt-in draft work/blocked/hdr/NUISystem_CText.h) and binds GFGetSystemManager and the PDB
  CFontManager::GetFont, but it newly defines ??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z (72 bytes, ShareData inlined), which
  reloc_bindings maps to 0x00419463 whose owner row (leaf ?AssignFrom@CCPtr@@) is 23 bytes calling ShareData: different body.
  ??1?$CCountedPointer@VCFontBank@@@@QAE@XZ (0x004057A0) and ?GetString@CDefString@@QBE?AVCCharString@@XZ (0x00415D70) copies are
  byte-identical (tools/cmp2.py). Retail inlines operator= in this function, so a declaration-only opt-in cannot match. The landed twin
  0x00541FD0 already defines the same copy (HEAD debt; verified: its object defines the 72-byte ??4 COMDAT, byte-identical to the draft's). DECISION NEEDED: accept as the twin's debt, or hold.

## Decisions for the coordinator
- q4422 0x013B8384: the claim PRenderManager is superseded by Ego_d ?GPRenderManager@@3PAVCRenderManager@@A (global scope; GFInitialise
  stores CDisplayManager::PRenderManager there, beside GPInputManager / GPDisplayManager). No landed row binds it; 11 rows (6 on
  PRenderManager) would move as one family and GFInitialise 0x004022B0 is blocked/port. Replace the queue claim? (REJECTED / NAME_FIXES)
- q4573 / q4578: Ego_d NEngineInternal::GPWeatherRenderer / GPSurfaceCompositionManager replace the descriptive claims; no landed row
  binds them, each needs its whole REFS family moved (q4573 includes 0x00B24690 whose callee 0x00B50C60 is unidentified).
- q4574 / q4576 / q4580: the address is already on the GP* spelling; remaining stand-in binders (declaration grep): 0x00B33010 (q4574),
  0x00B63010 + 0x00B77E20 (q4576), 0x00BCB800 (q4580): each a local stand-in struct needing a header port (STATUS blocked).
- q6513 (not started, noted from the triage): Ego_d spells ?NullString@CScriptThing@@1VCCharString@@A (protected static); the landed
  rows / engine/CScriptThing.h use @@2 (public). An access change on a shared-header member = header claim; one source only.
- 0x008ED590 (q4996): candidates/object_family_compiled.h:21 declares GFROR13 with an unsigned long return (?GFROR13@@YIKK@Z): header item.
- 0x004022B0 (q4288/q4422): porting GFInitialise onto engine/CSystemManager.h needs a name for retail's +0x7C counted pointer (stored into
  PSoundManager), absent from Ego_r's CSystemManager: header decision.
- Labels: ghidra_out/labels_landing_2026-11-11cl.tsv keeps `?g_NormaliseEpsilon@@3MB` alive for 0x0129BA3C (first-match order).

## Census method
- REFS rows probed by compiling the HEAD row and reading binds.py pairs (tools/probe.sh; results work/probe/<target>.tsv) for q4175,
  q4200, q4243, q4245, q4256, q4288, q4422, q4433, q4502, q4505, q4573, q4628, q4629, q4916, q4996 (all 180), part of q4574/q4577/q4580.
  Large entries without a full probe say so in APPLIED (declaration grep + reloc_bindings).
- reloc_bindings.tsv is incomplete (AutoTiny* families absent), see LESSONS.

## Descriptive names
- None introduced. All new spellings are PDB names (Ego_d.pdb publics / Ego_r / FableWin).

## Flags lines
- None added.

## Funclets
- No changed row is included by an EH funclet row (grep of rebuild/src/compiled/01/2* for the changed files' names: none).

## Rule slips
- One Bash call exited 2 (a grep over a missing pattern list); every other call exited 0.
- A background full-REFS probe was killed mid-run (pkill) to free the compiler; no file outside my directory was touched.
- Several entries' edits (q4175 / q4200 / q4243 / q4245 / q4256) were prepared while that probe ran and graded together in one batch,
  rather than strictly one entry at a time.
- Census statements first rested on reloc_bindings + identifier grep; after the advisor review, q4644 was probed, the q4576 / q4580
  binder lists were recounted from declaration lines (the identifier counts included comments), and q4575 / q4577 / q4579 / q4581 / q4735
  carry a "not probed" caveat.
