# Codex pass 12 return -- read-only regrade: 50 of 50 claimed rows byte-exact on our lane (2026-11-03)

Outside Codex pass over the 100 rows reserved in `pending/2026-11-02_codex_pass12/RESERVED.tsv` (NA077-NA086). The return
(`FableDecomp_pass12_handoff`, SHA256SUMS verified clean) reports 47 RELOCATION_MATCH, 3 BYTE_MATCH_NAME_REVIEW (exact bytes, STLport
extension configuration unresolved: 0x0051FD80, 0x00520D00, 0x00B2D9F0) and 50 PARKED (`STATUS_codex.tsv`, `ROW_DETAILS_codex.md`,
`START_HERE_codex.md`, `LESSONS_codex.md`, `NAMING_REVIEW_OVERVIEW_codex.md`). **Nothing was landed, labelled or synced this round.**

## Regrade (`regrade_grades.tsv`)
Own oracle from our exe (`chunk_pool/oracle_from_exe.py`; scratch only, never committed: these rows are unlanded). Each row's header
context was rebuilt hash-checked from the return (`scripts/mat.py`: packaged base + the selected grade's own overlay; the selected
grade's manifest equals the row copy for all 50) and graded read-only with `FABLE_HDR_OVERLAY=<ctx> vland.py payload oracle`:
**50 of 50 RELOCATION_MATCH, every test PASS** (47 claimed + the 3 held rows). A second run graded each row on only its own include
closure (`verified_pending/<addr>/hdr`, the row's headers laid over our current tree): again 50 of 50 RELOCATION_MATCH PASS, so the
copies in `pending/2026-11-02_codex_pass12/verified_pending/` are sufficient to land without the zip. No test needed the
`static_assert` token rename (no test contains it). vland warnings: 53 RELOC_NAME (second spellings, see naming), 8 RELOC_ALIAS
(import thunks), 3 RELOC_CONFLICT (one symbol, two addresses; all three resolved below).

## Header needs (`row_header_needs.tsv`, per header `header_detail.tsv`, `hdrcheck2.txt`)
| Kind | Rows | |
|---|---:|---|
| A_none (no project header, or only headers identical to ours) | 4 | 0x00C01750 _deflate_slow, 0x00CA4160 vorbis_lpc_to_lsp, 0x00CB38E0 / 0x00CB48A0 ImeUi (engine/ImeUi.h = ours) |
| B_newonly | 2 | 0x00C9F8C0 mapping0_inverse, 0x00CA2BB0 floor1_encode (new engine/Vorbis.h, one version, no macros) |
| C0 additive only | 6 | GameCODA EAX/OpenAL rows: engine/EAX40.h gains 4 `extern const GUID EAXPROPERTYID_EAX40_FXSlot0..3` lines, nothing removed; 10 new single-version headers |
| C2 needs consolidation | 38 | every engine and NScript row |

The 38 C2 rows edit 139 distinct existing headers (674 row-header pairs; 36 headers exist in two to nine versions across rows:
stlport/fable_containers.h 9, C3DVector.h 7, fable_map.h 6, CArray.h 6, fable_string.h 5). `hdrcheck2.py` against our tree: 421
pairs default=same, 253 DIFF. Against the packaged base every `edit`/`edit_conflict` header keeps the default path except one, so
the DIFFs are not Codex rewriting defaults; they are:
- 252 pairs on headers Codex authored independently that pass 11 wave 2 / consolidation 2 have since landed in consolidated form
  (`both_new`: C3DVector.h, C2DCoordI.h, CArray.h, CVectorMap.h, CDefString.h, CPersistContext.h, CHeroLogBook.h, stlport/fable_containers.h,
  fable_map.h, fable_string.h ...): a second model of each class to be rebased onto ours.
- engine/CEngineComponent.h (0x00B3BE10): the **default path gains `#include "engine/CCharString.h"`** (owner-rule violation as packaged).
- 36 rows include `engine/CCharString.h`, the second CCharString model pass 11 already refused (beside fable_string.h).
- 420 pairs keep our default but add **135 per-feature macros** (FABLE_RETAIL_WASP 164 uses, NAVIGATION_UPDATE 156, CTHINGMANAGER_SAVE
  140, FACTIONS 124, SCRIPT_QUEST 116, GFMAIN 110, HERO_LOGBOOK 98, SHOP 96, ...); the class-named additions are only
  CDISKFILEWIN32, CIVCOUNTEDPOINTEEBASE, CRENDERSTATEMANAGER, CTCINVENTORY, CTHINGBASEDEF (and CCHARSTRING_DATA).
- 88 pairs compare vacuously (`same[0](cpp-err)`): CScriptBase.h, CLevelScriptBase.h, CDefPointer.h, CTimer.h ... default to `#error`;
  under their class macro (FABLE_RETAIL_NSCRIPT / DEF_CLASS_BASE) the existing branch is unchanged.
0x00821010 CMap::LoadBrushesFromFile packs the serialised CFileFormat* records with one push(1)/pop in the new engine/CFileFormatBrush.h
(odd offsets from both PDBs, `verified_pending/00821010/PACKING_EVIDENCE.md`): consistent with the 2026-11-03 rule if it sits in a
FABLE_RETAIL_CFILEFORMATBRUSH branch.

## Landing order (proposed)
1. **Wave A, now:** 0x00C01750, 0x00CA4160, 0x00C9F8C0 (+ engine/Vorbis.h). 0x00CA2BB0 waits on the per-TU static rule: its `@ilog@4`
   (floor1.c, 0x00CA1790) is already bound to 0x00C9FE70 by landed 0x00C9FEA0 (same shape as the retry1 `@ilog2@4` triplicates).
2. **Wave B, ImeUi** 0x00CB38E0 / 0x00CB48A0: no header work; respell the pass 11 descriptive ImeUi globals to the DXUT names (13
   second spellings) and decide how a file-static (`_g_dwCount`, `_g_uCandPageSize`, `_g_bCandList`: VC7.1 emits statics undecorated)
   binds against the `?g_...@@3` extern spellings the split pass 11 rows use.
3. **Wave C, GameCODA EAX/OpenAL** (6 rows): merge the 4 GUID lines into EAX40.h; one class, one model: Codex's CEAXListener / CEAXBuffer
   come from the SDK's EAXListener.obj (debug types, `evidence/00c5ffb0/SDK_COMPARISON.json` in the return), so pass 11's descriptive
   CEaxContext / CEaxSource rows (0x00C67F60, 0x00C68F90, 0x00C65A00, 0x00C68510) and their binders are respelled in the same commit;
   settle extern "C" `_EAXPROPERTYID_*` / `_DSPROPSETID_*` (DEFINE_GUID) against the landed C++ `?...@@3USGuid@@B` once (queue 595).
4. **Wave D, NScript** (17 rows 0x00CF9610 ... 0x00EF80A0): fold the script feature macros (WASP, HANGING_TREE, BODYGUARD, EXPOSE_MAYOR,
   SCRIPT_QUEST, PERSONAL_SCRIPT ...) into FABLE_RETAIL_NSCRIPT / per-class branches, rebase onto the landed CScriptThing / fable_string.h model.
5. **Wave E, engine** (21 rows incl. 0x00F29530 ~CScriptDef): after the header consolidation (one FABLE_RETAIL_<CLASS> per class on the
   current headers, CEngineComponent default restored, no engine/CCharString.h, includers rechecked). 0x00AC2BE0 is the lightest
   (5 headers, no CCharString). 0x0051FD80 / 0x00520D00 / 0x00B2D9F0 last, after the STLport decision below.

## Naming (`AUDIT.tsv`, `SECOND_SPELLINGS_cx12.tsv`)
Every leaf and relocation symbol of the 50 (974 entries) against the PDB name sets, ESTABLISHED_SPELLINGS.tsv and the queue's wrong
spellings: no established-spelling clash, no reintroduced wrong spelling, no hard reloc rejection. Leaves: 32 in the PDB sets, 8 in the
PDB sets with an older landed stand-in, 10 vendor C/GameCODA names (no PDB). The three RELOC_CONFLICTs:
- 0x0052CC50 is `??0CChangingStateComponent@NUISystem@@QAE@J@Z` (catalog + Codex NAME_FIXES); landed 0x0041B800 binds it as
  `??0CDraggableInto@NUISystem@@QAE@J@Z`, which belongs to 0x00558EC0 (0x0041D21B). Respell 0x0041B800.
- 0x0051AC70 destroys CMapDesc (`__destroy_aux<CStaticMapGenerationVisibiilityInfo::CMapDesc*>`); landed 0x0051C470 binds it as the
  `set<unsigned long>*` instantiation, which belongs to 0x00AE76E0 (0x00AE2E20). Respell 0x0051C470.
- `@ilog@4` at 0x00CA1790 vs 0x00C9FE70: two libvorbis per-TU statics; needs the per-TU static rule.

Second spellings: 252 retail addresses where a claimed row spells an already-bound address differently. 113 are queued with Codex's
spelling as the correct one (landing closes them); 55 are PDB spellings not yet queued (PRIORITY `cx12` at landing); 17 disagree with
the queue; 67 are vendor/data/descriptive names with no PDB entry. Queue disagreements worth deciding first:
- 0x00415DD0: Codex `?GetString@CDefString@@QBE` vs queue 540 / mig9 `??BCDefString` (operator CCharString). FableWin donor order
  (??B 0x018C20E0 then GetString 0x018C2120) matches retail 0x00415D70 then 0x00415DD0, and a landed binding already spells 0x00415DD0
  GetString: this settles mig9's duplicate-COMDAT question as two functions (0x00415D70 ??B, 0x00415DD0 GetString).
- 0x0143E8EC CScriptThing::NullString: Codex `@@1` (protected static, "raw Ego_r symbol stream 8 + TPI") vs queue 569 `@@2`. Settle with
  `cvdump -s Ego_r.pdb` (S_GDATA32) and the TPI member record on the owner's machine.
- 0x00CA1E00 render_point: Codex `@render_point@20` (fastcall, byte-exact caller) vs queue retry1 `_render_point@20`.
- 0x0143BDB0 `g_OpenAL` (CSalOpenAL) vs c675 `g_AudioContext`; 0x00C4D740 `CSalOpenAL::ALGetProcAddress` vs `CAudioContext::GetProcAddress`:
  GameCODA has no PDB; settle from the GameCODA objects if the owner has them.
- 0x009B9BA0 / 0x009B9E00 / 0x009BBBF0 / 0x009BA330: PDB spellings (SkipPastString, SkipPastNextItemIfIs, CheckUnknownSymbols,
  ReadAsStringUntilString) that confirm the open "likely" queue notes sec5b / n621.

Held rows (STLport extension configuration): STLport 4.6.1 `_tree.h` lines 457-466 declare `find` / `_M_find` as member templates
when `_STLP_MEMBER_TEMPLATES` is on and `_STLP_NO_EXTENSIONS` is off; stl_msvc.h only removes member templates for VC <= 1100 and
stl_user_config.h leaves `_STLP_NO_EXTENSIONS` commented out, so the stock VC7.1 build gives Codex's `??$_M_find@...` / `??$find@...`, and
our shared stlport/fable_map.h already models `_M_find` as a member template. Against it: 28 landed bindings (21 retail addresses,
26 binding rows, e.g. stdRbtree_Mfind_00756350.cpp) spell the non-template `?_M_find@`, and only 1 the template form. The donor PDBs are Dinkumware builds and
cannot decide. The bytes are identical either way, so the settler is a decision: one configuration for the whole build (stock
4.6 = member template unless the Lionhead stl_user_config.h turns up), then respell the losing side (the non-template landed rows or these 3 rows).

GFMain 0x00402510's eight descriptive names are all second spellings: the seven globals are already bound by landed rows
(StartupConsoleEnabled_0137548f = `g_bFrontEndFlag_137548F`, SaveSoundSymbols_0138dd3b = `g_bSaveBinarySoundSymbols`,
LoadSoundSymbols_013bc9f0 = `g_bLoadBinarySoundSymbols`, PrepareParticles_01375754 = `g_bParticlesEnabled`, RenderConfigurationFlag1/2 =
`g_FeatureA` / `g_FeatureF`, UseFixedFontFunction_013750ab = `g_FeatureG`) and 0x009D81E0 is landed as `ClearPendingFlag_009d81e0`.
None has two-source proof either way; one address, one name says GFMain uses the landed spellings until an Ego_r data-symbol dump
(`cvdump -s`, S_GDATA32/S_PUB32) names the globals and a donor-order match names the 17-byte flag clearer next to its setter 0x009D81C0.

Codex's 393 NAME_FIXES rows are kept unreviewed in `NAME_FIXES_codex.tsv` (`NAME_FIXES_CONFLICTS_codex.json`) for the naming wave.

## Parked (`parked_summary.tsv`)
50 rows, PLAN §3 buckets: 35 inline-vs-call (retail expands an ordinary inline the candidate calls, or the reverse), 5 stack-slot/frame,
4 schedule, 3 x87-order (0x00A1C010, 0x00A86110, 0x00DDAE40: same length), 2 incomplete-model, 1 block/arm-layout. Codex rejected its
own closer candidates that used stronger inline annotations. They return to the pool after the reservation (2026-11-30).

## Files
`regrade_grades.tsv`, `row_header_needs.tsv`, `header_detail.tsv`, `hdrcheck2.txt`, `AUDIT.tsv`, `SECOND_SPELLINGS_cx12.tsv`,
`parked_summary.tsv`, Codex's own records (`*_codex.*`), `scripts/` (materialize, grade, needs, audit; run with `python3 -I`, scratch
paths as arguments). Row copies: `pending/2026-11-02_codex_pass12/verified_pending/<addr>/` (source, test, naming evidence, selected
grade, `hdr/` overlay, `HEADER_STATES.tsv`); build a payload with `scripts/payload_from_verified_pending.py`.
