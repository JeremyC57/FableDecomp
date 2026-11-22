# NF258 notes (2026-11-22, round 22): L37 done except one blocked binder; L129 not started

Handed back after L37 (the brief: "hand back after it if large"; advisor: leave L129 for a fresh pass). 7 rows changed (0x006250B0,
0x00AD9B20, 0x00ADA240, 0x00ADCE10, 0x009D4810, 0x009D3CA0, 0x00ADC310): all RELOCATION_MATCH + test PASS in one `vland --recheck-landed`
run (recheck_final.log: 7 / 7, 0 RELOC_CONFLICT; the WARN lines are SHARED_HEADER_AVAILABLE for local models the rows already had at
HEAD, RELOC_NAME label notes, and STANDIN_LEAF on the two `_Construct` rows, whose catalog names RENAMES section 1 corrects). One binder
blocked: 0x009B0AC0 (3 forms). No header change (no hdr/ / orig/). No new CompilerFlags. No new split: every touched address ends on the
same or fewer spellings (census below). Link of the 7 new objects + HEAD 0x00535470 / 0x00AD9770 / 0x00AD9980 / 0x009CF3A0: 0 LNK2005 /
LNK4006 (control with a duplicated strong object: 1 LNK4006, so the check is live; a duplicated all-COMDAT object gives 0, so the
control must duplicate a row whose leaf is a strong symbol). Repo root (`ls -la`) before / during / after: identical. No git, no
foreground sleep; every compile ran under an outer `timeout 900` (hk.sh / gr.sh / fin.sh / scall.sh patched) after its lock was NF258 and
its clang precheck passed.

## L37 (0x00535470, vector<unsigned long> copy constructor)
| row | before (binds at 0x00535470) | after | forms |
|---|---|---|---|
| 0x006250B0 CUIStateDef copy ctor | `??0CULongVector@@QAE@ABU0@@Z` (local struct) | member `std::vector<unsigned long>` from stlport/fable_containers.h, COPY_DECL_ONLY; nothing else changes | 1 |
| 0x00AD9B20 CalcCurrentBlockSegments | `??0CGroupArray@@QAE@ABU0@@Z` | `CArray<unsigned long>` (engine/CArray.h; Ego_r CVertexGroupBlock::Groups) inside the HEAD-local CVertexGroupBlock model, COPY_DECL_ONLY | 1 |
| 0x00ADA240 Process | same (2 sites) | same | 1 |
| 0x00ADCE10 list<CVertexGroupBlock>::push_back | `??0?$vector@K@std@@QAE@ABV01@@Z` (one-parameter local std model) | full header port (CVertexGroupBlock.h MEMBERS, CArray.h, fable_map.h MAP_COPY_OUTOFLINE, header list): self leaf one-parameter -> two-parameter list (the table spelling, witness 0x00AD9F10); 0x00A96E40 `??0?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$less@K@2@@std@@...` -> `??0?$map@KJ...@QAE@ABV01@@Z` || 2 (+ TREE_DTOR_DECL_ONLY) |
| 0x009D4810 map::operator[] | `??0?$vector@K@std@@QAE@ABV01@@Z` + a same-TU auto_inline(off) copy body | header port, the 0x009AC5D0 recipe, mapped type `CArray<unsigned long>`; COPY_OUTOFLINE (visible body) | 1 |
| 0x009D3CA0 (July, no Leaf) | `?copy_00535470@CArrayUL@@QAEXPBU1@@Z` (no table line) | `_Construct<pair<const CCharString, CArray<unsigned long> >, ...>` explicit instantiation, the 0x007AE0C0 recipe (CONSTRUCT_FASTCALL); 0x0099EC30 `?copy@CCharString@@QAEXPBV1@@Z` -> `??0CCharString@@QAE@ABV0@@Z` | 1 |
| 0x00ADC310 (July, no Leaf) | `?ctor@CCharString@@QAEXPAV1@@Z` | `_Construct<CVertexGroupBlock, CVertexGroupBlock>` explicit instantiation on CVertexGroupBlock.h; 0x00A96E40 `?ctor@CArrayUL@@QAEXPAU1@@Z` -> `??0?$map@KJ...@QAE@ABV01@@Z` || 2 (+ TREE_DTOR_DECL_ONLY) |
| 0x009B0AC0 AddDefClass | `??0?$CRetailVector@K@@QAE@ABU0@@Z` | **blocked** (below) | 3 |
- Every changed row's relocation list v its HEAD object (sc.sh diff) changes only the lines named above; everything else is identical.
- **0x009B0AC0 blocked/port**, DIFFER(407v404) in all three forms: (1) `std::vector<unsigned long>` + `Instances[0]`; (2) + the existing
  FABLE_RETAIL_STLP_VECTOR_INDEX_START (operator[] on _M_start); (3) `CArray<unsigned long> GlobalIndices` (the Ego_r member and type;
  CArray's default operator[] is `this->_M_start[i]`). All three emit the same code: after `mov ecx,[eax+8] ; mov edi,[ecx]` a dead
  `add eax, 8` (the inline operator[]'s `this`), and esi / edi swap roles for `this` / the index across the whole function. HEAD reads
  `Instances._M_start[0]` with no inline call, which the public header cannot spell (`_M_start` is protected). Next step (queued): port
  the row onto engine/CDefinitionManager.h (FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF's CDefClassInfo with GlobalIndices), as its
  landed siblings 0x009AD390 / 0x009ADE10 / 0x009ADC90 use (`info->GlobalIndices.begin()[i]`); draft work/drafts/009b0ac0_f3.cpp (form 3).
- **0x009D4810's mapped type.** HEAD modelled `map<CCharString, vector<unsigned long> >` (3-parameter local map); both PDBs spell this
  function `??A?$map@VCCharString@@V?$CArray@K@@...` and its only referencer 0x009CF3A0 (CBankFile::WriteSymbolHeaderFile) already binds
  that spelling, so the self leaf moved to it (2 -> 1). Its callees 0x009D2BA0 (`_M_lower_bound`) / 0x009D4380 (hinted `insert_unique`)
  now bind the CArray `_Rb_tree` spellings: count-neutral (2 -> 2 each; 0x009D4810 is their only binder, the self rows keep their own).
  0x009D4380 / 0x009D40B0's own leaves spell the `vector<unsigned long>` mapped type (wrong per both PDBs): NAME_FIXES, queued.
  The copy constructor stays a call with its body visible (COPY_OUTOFLINE), replacing the HEAD same-TU stand-in body: the pick-any
  `??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z` copy is EQ 0x00535470 (95 = 95; HEAD's one-parameter stand-in copy was NEQ 112 v 112).
- **0x00ADC310 / 0x009D3CA0 names.** Both July rows are unreferenced in Fable.exe (no E8/E9 site, no absolute reference) and carried
  the same propagated Dinkumware label `_Cons_val<allocator<pair<CCharString const, CArray<unsigned long> > >, ...>`. 0x009D3CA0 is that
  pair (CCharString +0, CArray<unsigned long> +4; the map of 0x009D4810); 0x00ADC310 copies +0 with vector<unsigned long> and +0xC with
  map<unsigned long,long>, which is CVertexGroupBlock (Ego_r; both PDBs have its construct helper; no pair keyed by a 12-byte vector exists
  in Ego_r; its TU-mates are the CVertexGroupBlock helpers). Both become STLport `_Construct<T, T>` explicit instantiations, the
  0x007AE0C0 (NF87) precedent: descriptive-by-library, `__fastcall` via the existing FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL. Their self
  symbols change with no referencer, so no split; catalog renames in RENAMES section 1. nf256's pair<const vector, map> reading: REJECTED.
- **Census** (compiled HEAD objects of the 9 locked rows + nf254's 26-referencer census + table): 0x00535470 7 spellings -> 2 (the
  STLport one + `??0?$CRetailVector@K@@QAE@ABU0@@Z` of the blocked 0x009B0AC0); 0x009D4810 2 -> 1; 0x00ADCE10 2 -> 1; 0x00A96E40 loses
  `?ctor@CArrayUL@@QAEXPAU1@@Z` (only 0x00ADC310 defined it: source grep) and 0x00ADCE10's 3-parameter `_Rb_tree` use (the spelling
  stays: witness 0x00ADC330), so at least one fewer; 0x0099EC30 neutral (`?copy@CCharString@@QAEXPBV1@@Z` stays, witness 0x00518B60);
  0x009D2BA0 / 0x009D4380 neutral; 0x009D3CA0 / 0x00ADC310 1 -> 1 (no referencers). Table: TABLE_CHANGES.tsv (5 deletes, 2 replaces,
  3 keeps).
- **Pick-any copies** (pk.py with owners.tsv + the table; HEAD v new): no copy that was EQ at HEAD stopped matching. Same-as-HEAD NEQ
  debt: 0x006250B0 `??0CParentDefClassBase@@QAE@ABV0@@Z` (55 v 55), 0x00ADA240 `erase<bool>`, 0x009D4810 `??MCCharString` /
  `less<CCharString>` (cmpobj SAME v HEAD). New copies EQ: 0x009D4810 vector copy (above) and `??M?$CBasicString@D@@` 0x00429950, 0x00ADCE10
  `_List_iterator_base` 0x0041C780, `operator new` 0x00405C10 everywhere.
- **The map<unsigned long,long> `_M_erase` / `clear` copies (resolved).** Form 1 of 0x00ADCE10 and 0x00ADC310 newly defined
  `_M_erase` (51 B v owner 0x004507A1) and `clear` (51 B v 0x00451046), NEQ: the header CVertexGroupBlock's implicit copy constructor makes
  VC7.1 emit the member destructors (unreferenced in the object) and through ~_Rb_tree those two (byte-SAME as the copies landed
  0x00AD9770 / 0x00AD9980 define at HEAD, also NEQ there). Form 2 of each adds the existing NF178b opt-in
  FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY (its comment describes exactly this: ~_Rb_tree instantiated from the map members and emitted as
  pick-any COMDATs nothing calls): both rows stay RELOCATION_MATCH with the same leaf relocations, and neither object defines `_M_erase` /
  `clear` any more (pk.py: only `operator new` / `_List_iterator_base`, EQ). Left: the unowned `??1?$map@KJ...@QAE@XZ` COMDAT (no table line,
  no retail owner known, nothing in the object calls it) now calls ~_Rb_tree instead of expanding it, so it differs from 0x00AD9770's
  copy (cmpobj DIFF; 13 other common COMDATs SAME); its UND `??1?$_Rb_tree@KU?$pair@$$CBKJ...` is defined by 0x00AD9770 / 0x00AD9980
  (pick-any), so the build resolves it. No pick-any rule applies to an unowned copy (nf254 L114). These are the handed-back forms.
- Vtables: only 0x006250B0 emits any (`??_7CParentDefClassBase@@6B@` v 0x01230CBC, `??_7CUIStateDef@@6B@` v 0x0125871C): vtcheck on the
  new and HEAD objects is identical (1-slot stand-ins, NOLINE, 0 conflicts): HEAD debt, unchanged.
- Defined-symbol diffs v HEAD (work/sym_pH_* / sym_pS_*): only the leaf (renamed for 0x00ADCE10 / 0x009D4810 / 0x009D3CA0 / 0x00ADC310),
  shared-header inline / pick-any COMDATs, and the dropped local-model members (CRetailVector / CGroupArray / one-parameter std models).

## L129 (not started)
nf256's proof (`?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A` = 0x01436EA8) and plan stand. Today's census
(work/census0.txt): 18 table spellings, 91 absolute referencers. No row touched.

## Decisions for the coordinator
- D1: none left for the `_M_erase` / `clear` copies (TREE_DTOR_DECL_ONLY, above); say if the unowned `??1?$map@KJ` variant should be mirrored
  (0x00AD9770 / 0x00AD9980 would then need the same opt-in, outside this pass).
- D2: L37 lands as 7 of 8 with 0x009B0AC0 blocked (each move is count-reducing on its own; 0x00535470 ends on 2 spellings, not 1), or
  wait for the 0x009B0AC0 port onto CDefinitionManager.h.
- D3: catalog renames of 0x009D3CA0 / 0x00ADC310 to the STLport `_Construct` names (RENAMES section 1). 0x009D4810 / 0x00ADCE10 keep the
  catalog's module spelling (`std::map` / `std::list<CVertexGroupBlock>`), per the NF92 STL rule: leaf-only.

## Flags / funclets / tests / descriptive names / locks / slips
- Flags: none of the 7 rows has CompilerFlags and none needs one (the `__fastcall` of the two `_Construct` rows comes from the existing
  header opt-in, as 0x007AE0C0).
- Funclets: no file under src/compiled, tests or include names any of the 7 source basenames (grep).
- Tests: all 7 keep their HEAD tests (standalone host models that do not link the row; PASS). Rewriting them on the shared headers is
  owed (0x00ADC310's models the wrong pair). clang cannot parse VC7.1's own <cstdio> + <vector> (a two-line control file gives the same 2
  errors), so the 0x00AD9B20 / 0x00ADA240 tests' clang "errors" are that incompatibility, not a missing include; cl compiled them.
- Descriptive names: the two `_Construct` names and the STL template spellings are descriptive-by-library. No new made-up name.
- Locks (rows.txt, NF258): 00535470 006250b0 009b0ac0 009d4810 00adce10 00ad9b20 00ada240 00adc310 009d3ca0 00ad9770 00ad9980
  009cf3a0 (7 changed, 0x009B0AC0 blocked, the rest controls). Release with the landing.
- Reading: the chained briefs, NF214's read-first list (NF92, NF44, NF90 / NF89 / NF88 lessons, ASM_RULES) and the nf254 / nf256 hand-backs
  were read in full; GOTCHAS.md (144 KB) and EXTRA_RULES.md (60 KB) were read only in the sections that apply here (pick-any,
  in-place respelling, nf205 / nf212, STLport spellings, size pragmas), by grep.
- Competing copies under the newly used names: HEAD 0x009CF3A0 defines no `??A?$map@VCCharString@@V?$CArray@K@@...` copy (declares it);
  HEAD 0x00AD9770 / 0x00AD9980 define `_Construct<CVertexGroupBlock,...>` only as the cdecl `YA` form (a different name from 0x00ADC310's
  `YI` leaf). Not checked (not compiled): 0x00AD9F10 (list push_back binder) and the /Gr TU-mates 0x00ADC330 / 0x00ADC190 / 0x00ADCB90,
  which could define a copy of the list push_back or of the `YI` `_Construct`.
- Slips: (a) two Bash calls exited non-zero (a trailing `kill -0` on a finished job; an early `wc` over a mistyped NF89 / NF88 path).
  (b) Judgment call: the 0x00AD9B20 / 0x00ADA240 HEAD tests went into the vland recheck although their clang precheck printed errors; the
  errors are clang's failure on VC7.1's own <cstdio> + <vector> (reproduced with a two-line control file), not a missing include, and cl
  compiled both tests (PASS). Otherwise every compile ran only after its lock was NF258 and its clang precheck passed, under an outer
  `timeout 900`.
