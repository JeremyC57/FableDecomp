# NF260 notes (2026-11-22, round 23): L129 units 1 + 2 done (0x01436EA8 19 -> 17 spellings: the PDB one + 16 stand-ins); the other L129 units handed back as a plan

Handed back after two finished L129 units (brief: "hand back after any finished unit"). 54 rows changed, all RELOCATION_MATCH + test PASS
(recheck_final.log: one `vland --recheck-landed` run over all 54, about 40 s; three earlier 18-row payloads gave the same; 0 RELOC_CONFLICT; the WARN lines are SHARED_HEADER_AVAILABLE for local models the rows had at HEAD, RELOC_NAME label notes on
CEngineLandscapeMemoryManager callees and STANDIN_LEAF on the rows' own HEAD leaves, none about 0x01436EA8). One header gets one add-only opt-in
(hdr/, orig/, hdr/README.md; equiv_check over 1,462 includer rows: 0 differ; pin_asserts: 0 pinned). No new split, no new CompilerFlags
(0x00B40260 /d2inlT1267 and 0x00B40BD0 /d2inlT576 keep their catalog flags and were graded with them). Repo root (`ls -A`) before /
after: identical. No git, no foreground sleep; every compile ran after its lock was NF260 and its clang precheck passed, under an outer
`timeout 900` (see Slips for the two exceptions).

## L129 census (0x01436EA8, `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`)
- Address re-checked (two sources, tools/gdata.py): Ego_r.pdb S_GDATA32 GPResourceManager 3:0007bdf4, GPLightingManager +4, GPCamera +8,
  GPEngine +0xC, GPLandscapeRenderer 3:0007be04 (+0x10); FableWin.pdb 4:0008807c .. 4:0008808c, same order and spacing; the table binds
  the first four at 0x01436E98 / 9C / A0 / A4. Spelling as nf256.
- Referencers: 91 landed rows hold an absolute reference in retail (tools/census.py), plus 9 rows whose source names `g_pLandscapeRenderer`
  (100 rows, all locked NF260). Every one was compiled from its HEAD source (work/hc/, work/hobj/; 98: 0x00BF8030 / 0x00BF84C0 fail the
  clang precheck on clang-only C++ semantics and were read from source), and grouped by the symbol each object names at 0x01436EA8
  (relocations of the leaf, plus UND names): 19 spellings at HEAD (the table's 18 + `?g_obj_01436ea8@@3PAUCGlobal@@A` of 0x00BE5C30, which has
  no table line).

| spelling at HEAD | rows | unit |
|---|---|---|
| `?g_pLandscapeRenderer@@3PAVCEngineLandscapeRenderer@@A` | 45 + 0x00BF5D30 (its Leaf line is the bare prefix `??_G`, which the census tool resolved to `??_GCMovableResource`; with the full leaf `??_GCEngineLandscapePatch@@UAEPAXI@Z` it binds the old spelling at +0x4E) + 0x00BF7F70 (UND only: an in-class operator delete; its leaf does not reference it) | **1: done** |
| `?g_LandscapeRenderer_01436EA8@@3PAVCEngineLandscapeRenderer@@A` | 7 (0x00B26360 0x00B40260 0x00B40BD0 0x00BE8E60 0x00BEA110 0x00BFBD80 0x00BFE900) | **2: done** |
| `?GPLandscapeRenderer@NEngineInternal@@...` (right) | 4 (0x00B412A0 0x00B6FA40 0x00BEAE50 0x00BF7BC0) | - |
| 16 stand-in-pointee spellings | 27 rows + 0x00BF8030 / 0x00BF84C0 by source (NAME_FIXES line) | planned (below): untouched rows of units not started, not a carry |
| none | 13: 8 rows that mention the old name only in comments (0x00BDDA50 0x00BDE020 0x00BF6490 0x00BF6750 0x00BF6A30 0x00BF6AC0 0x00BF6BF0 0x00BF71D0) and 5 July `candidates/*_compiled.h` rows without a Leaf line (0x00BE6F70 0x00BE7D70 0x00BF3B60 0x00BF4570 0x00BFE050) | not-a-binder |

## Units 1 and 2 (done): 54 rows onto the PDB spelling
- Both old spellings already had the right type (`CEngineLandscapeRenderer*`, class key V), so each move is a name change only.
- Two routes (STATUS.tsv notes):
  - **header route (42 rows):** the row includes engine/CEngineLandscapeRenderer.h itself, so it defines the new add-only
    `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` before its first #include and its row-local extern line goes;
  - **local route (12 rows):** 0x00B42A90 / 0x00B42B10 / 0x00B42D10 / 0x00BDE390 / 0x00BE5D20 / 0x00BE5D80 / 0x00BF7DF0 / 0x00BF7F70 model
    CEngineLandscapeRenderer locally (HEAD debt, not touched: porting them onto the header is a separate job), and 0x00B26360 (CEngine::Initialise:
    an address-ordered block of row-local `namespace NEngineInternal { extern ... }` lines) and 0x00BE8E60 / 0x00BFBD80 / 0x00BFE900 (one-line
    stand-in models with the extern inside) keep a row-local extern, now `namespace NEngineInternal { extern CEngineLandscapeRenderer*
    GPLandscapeRenderer; }` in place: the landed precedent of 0x00B412A0 / 0x00BEAE50 / 0x00BF7BC0 / 0x00B6FA40.
  - The identifier tokens become `NEngineInternal::GPLandscapeRenderer`. Comment lines are not edited: older history comments ("NF29: ... is
    g_pLandscapeRenderer (descriptive ...)") still name the old spelling (24 rows); the declaration comments that called it descriptive are gone.
- Proved by hand on three rows first (0x00BF6260 header route, plain use; 0x00BF5D30 header route, in-class use; 0x00BF7F70 local route, a
  leaf that does not reference it), then the scripted rest (tools/u1x.py, tools/u2x.py).
- Accepted by symbol, not by grade: for 52 rows the HEAD-v-new relocation list of the leaf differs in exactly one line (old -> new symbol
  at 0x01436EA8, same count); 0x00BF7F70's leaf has no reference (UND only), and 0x00BF5D30's bare `??_G` Leaf makes the logged list
  another function's, so it was checked separately with the full leaf (+0x4E old -> new). For all 54 the defined symbols are equal and
  the UND list differs only in that name (work/gr_p1.log, gr_u1.log, gr_u2.log). Over all sections (objdump -r, the 54 objects v their HEAD objects with the old name substituted) the relocation lists are
  identical and the .text bytes are identical, so the 16 objects that emit vtables emit the same vtables as at HEAD (vtcheck results
  unchanged from HEAD). Pick-any copies (pk.py, owners.tsv): the new and HEAD tables are identical line for line (232 copies; the NEQ ones
  are HEAD debt: 0x00BDE390 `??1CDataInputStream`, 0x00B26360 two GetDef copies, 0x00B40260 / 0x00B40BD0 PeekThemeId / PeekThemeBlend,
  0x00BE8E60 `??0CPatchTesselationEdgeStrip` / `??1CBaseClassNonCopyable`).
- Census after: 0x01436EA8 19 -> 17 spellings; the two old symbols are bound nowhere else (one table line each, deleted: TABLE_CHANGES.tsv).
  No other file in rebuild/src, rebuild/include or rebuild/tests names `g_LandscapeRenderer_01436EA8` / `g_pLandscapeRenderer` in code apart
  from the tests below; no file includes any of the 54 sources (`../src/compiled/<basename>` grep: 0 hits), so no funclet or folded row moves.
- Tests: 0x00B40260's test links the row object and defined the old global: rewritten to define `NEngineInternal::GPLandscapeRenderer`
  (rows/00b40260_test.cpp, one line; PassPattern kept). The other tests that name an old spelling (0x00BE5D80, 0x00BEAB40, 0x00BF7F70:
  `static` locals; 0x00BE8E60, 0x00BFBD80, 0x00BFE900: full inline models) are self-contained and do not link the row: unchanged, PASS.
- Link (54 new objects, LINK= cleared, /FORCE /NODEFAULTLIB): 1 LNK4006, `??1CPatchTesselationEdgeStrip@@UAE@XZ` defined strongly by both
  0x00BE8E60 (its local model) and 0x00BFC410 (the owner); the 54 HEAD objects give the same line, so it is HEAD debt (queued below). Control
  (a copy of 0x00B40260's object, a strong leaf): +1 LNK4006, so the check is live.

## Plan for the 16 stand-in spellings left (29 rows, none changed)
Each is a port, not a rename: the stand-in pointee type must become CEngineLandscapeRenderer and its fields the header's members
(MemoryManager +0x6B0, Settings +0x608 via GetSettings(), the themes via PeekTheme). Units, each moving every row of one spelling:
1. `NativeLandscapeRenderer` (0x00BFBE40, 0x00BFBF40, 0x00BFC040: the three CEdgeStipVertexArray<T>::Resize; their sibling 0x00BFC140 is
   already on g_pLandscapeRenderer -> now GPLandscapeRenderer, so it is the recipe).
2. The `+0x6B0` memory-manager stand-ins, one unit per spelling: `GB` 0x00BF36A0 / 0x00BF9920 and 0x00BF36E0 (operator new rows; recipe
   0x00BF6260), `CEngineGlobal` 0x00BE61B0 / 0x00BE79D0 / 0x00BF8030 / 0x00BF84C0, `LocalDetailEngine` 0x00BFAAE0 / 0x00BFB590,
   `CEngineGlobal_01436ea8` 0x00B3B1A0 / 0x00B3B4A0, then the singles 0x00BFB210, 0x00BDE860, 0x00BDFA10, 0x00BE3660 (spelled as CEngine:
   wrong class), 0x00BE5C30, 0x00BE6090, 0x00B684F0, 0x00BEAA30, 0x00B428E0.
3. `DAT_01436ea8` (fable_u8*, 6 rows on July candidates headers: +0x618 / +0x6B0 byte offsets): a port onto the header's GetSettings /
   MemoryManager, header route; 4 of the 6 bind it only as UND at HEAD, so check what their leaves really emit first.
4. The 5 July rows that name no symbol at 0x01436EA8 need a re-port before they can bind anything (closer to authoring than naming).
The header route (the new opt-in) serves every unit whose row includes the header.

## Queued findings (not changed)
- 0x00BF5D30's `// Leaf: ??_G` is a bare prefix that resolves to `??_GCMovableResource@@UAEPAXI@Z` in its object; the row's function is
  `??_GCEngineLandscapePatch@@UAEPAXI@Z` (NAME_FIXES line). Its grade and test pass either way; the Leaf line fix is a one-line follow-up.
- 22 RELOC_NAME label notes on CEngineLandscapeMemoryManager callees (e.g. 0x00BD9AB0 bound as RebindBlockOwner / SetMovableResourceHandler)
  are HEAD state, unchanged here.

## Decisions for the coordinator
- D1: the header opt-in v the row-local `namespace NEngineInternal { extern ... }` form. I used the header opt-in where the row already
  includes engine/CEngineLandscapeRenderer.h (42 rows; nf256's plan), row-local in place elsewhere (12). If you prefer one form, either is
  symbol-identical (the three proof rows covered both); the row-local form needs no header change.
- D2: the `??1CPatchTesselationEdgeStrip@@UAE@XZ` duplicate strong definition (0x00BE8E60's local model v owner 0x00BFC410, LNK4006 at HEAD):
  queue it (the nf86 / nf88 inline / pick-any fix on 0x00BE8E60's model), not part of this naming pass.

## Entry 2 (0x009B0AC0 CDefinitionManager::AddDefClass): open, not attempted (handed back early after the L129 units)
- No form compiled. What the next pass needs (read from the row, the header and the table):
  - The row is a full local model (CCharString, CDefinitionManager, CDefClassInfo with CRetailVector<unsigned long>, a CDefClassInfoMap with
    forceinline Add / Sort, CDefPointer, CPersistContext). Its landed siblings (0x009AD390 / 0x009ADE10 / 0x009ADC90) sit on
    engine/CDefinitionManager.h (CCHARSTRING / CDEFSTRINGTABLE / CSYMBOLMAP / DEF_CLASS_BASE / CDEFINITIONMANAGER_INSTANTIATEDEF ...), whose
    CDefClassInfo is the Ego_r record (Name, AllocFunc, CArray<unsigned long> GlobalIndices). The header already declares
    `AddDefClass` (FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS).
  - AddDefClass also needs the manager's `DefClassInfo` map typed (CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo>, retail +0x78,
    compare +0xC, unsorted flag +0xD). The header has it only as bytes in the existing line `unsigned char NameCRCToIndexMap[16],DefClassInfo[16];`,
    so a new opt-in splits that line (`#ifdef ... unsigned char NameCRCToIndexMap[16]; CVectorMap<...> DefClassInfo; #else <the line> #endif`)
    with an equiv_check proof, as the brief allows.
  - The sort instance at 0x009B9490 has only stand-in spellings in the table, so its STLport spelling needs proving first.
  - Count-reducing once ported: 0x00535470 2 -> 1; 0x009B9170 `?push_back@CDefClassInfoMap@@QAEXABUSDefClassInfoPair@@@Z` (witness 0x009B0AC0)
    -> the STLport `?push_back@?$vector@U?$pair@KVCDefClassInfo@CDefinitionManager@@@std@@...` (witness 0x009B9290); 0x009B9490
    `?SortDefClassInfo@@YIXPAUSDefClassInfoPair@@0UCKeyPairCompareLess@@@Z` (witness 0x009B0AC0; also `?GuiVarSetBool@@YIXPAXHE@Z` /
    `?Callee@@YIXPAX0E@Z` from 0x009B94F0 / 0x009B9510) -> the proven sort instance; the class-info lookup 0x009AD2E0 ->
    `?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z` (witness 0x009AD390).
- Locks 0x009B0AC0 / 0x009AD390 are NF260 (claimed, nothing compiled): release with the landing or hand them to the next pass.

## Flags / funclets / tests / descriptive names / locks / slips
- Flags: none new. Funclets: none (include grep). Tests: one rewritten (0x00B40260), the rest unchanged.
- Descriptive names: none added; two descriptive spellings (`g_pLandscapeRenderer`, `g_LandscapeRenderer_01436EA8`) removed from code.
- Locks (rows.txt, NF260): 102 rows: the 100 L129 family rows (54 changed, the rest unchanged) and 0x009B0AC0 / 0x009AD390 (entry 2, not
  compiled). Release with the landing.
- Reading: the chained briefs (NF258 back to NF215, NF234 / NF232 / NF230 included), NF214's read-first list (NF92, NF44, NF90 / NF89 / NF88 lessons, ASM_RULES, EXTRA_RULES) and the nf258 /
  nf256 hand-backs and READMEs were read in full; GOTCHAS.md (144 KB) was read by grep (its newest ~60 lines and every line on data globals,
  NEngineInternal and pick-any), not end to end.
- Slips: (a) the 54 rows' HEAD tests were clang-prechecked a few seconds AFTER the first three 18-row vland payloads were launched (the row
  sources and the one rewritten test had been prechecked before; every test then prechecked clean; recheck_final.log is the later single run). (b) 0x00BDE390's source fails the clang precheck on a
  clang-only rule (a virtual destructor in a class with only a placement operator delete), the same on HEAD; `clang -E` of it is clean, so it
  was graded: noted as a judgment call outside the brief's VC7.1-header-control exception.
