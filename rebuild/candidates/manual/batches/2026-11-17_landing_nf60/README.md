# 2026-11-17 landing nf60: name-fix round 7, NF60 (159 rows: `pGlobalScriptDef` binders)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

Ego_r.pdb and FableWin.pdb make the global at 0x0143E90C `CDefPointer<const CScriptDef> pGlobalScriptDef` (they carry its dynamic
initializer and atexit destructor), and `engine/fable_script_def_global.h` declares it so. NF57 settled the name (q10429); about
154 landed quest-script rows still bound it as a plain pointer, `?pGlobalScriptDef@@3PAVCScriptDef@@A`. NF60 took queue entries
q10541 / q10542: the 205 landed rows that mention `pGlobalScriptDef` without including that header. Brief and records:
`naming/2026-11-17_agentNF60/`.

## Rows (205, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 159: 151 bound the plain-pointer spelling, 4 the stand-in `?g_ScriptDef@@3PAVCScriptDef@@A`, and 4 already bound the right spelling but carried their own CScriptDef / CDefPointer models (now deleted) |
| not-a-binder | 43: 42 already compile to the exact CDefPointer symbol (each original compiled), 0x01224CA5 has no reference |
| blocked/port | 3 (unchanged; still bind the plain pointer) |

- Each port includes `engine/fable_script_def_global.h` and uses pGlobalScriptDef through CDefPointer. NF60 checked every port's
  relocation set: it is unchanged apart from the 0x0143E90C binding, except in two rows. There, 0x00DCD1D0 and 0x00E0A510 now bind
  0x00411570 as the PDB's protected static `?Compare@?$CBasicString@D@@KIJPBD0@Z`, not the free stand-in `?Compare@@YIJPBD0@Z`.
- Local def models used invented member names; NF60 mapped them onto `engine/CScriptDef.h` names by retail offset, and each such
  row lists old name -> header name in a comment.
- Small source changes that keep the bytes: 0x00CBEEB0 is now `pGlobalScriptDef.Reset(0)`. 0x00EC1CD0 / 0x00E1E960 reorder their
  `rand()` expressions. 0x00E6AAC0 / 0x00EDC720 use an inline `CTimer::operator=`. 0x00DA6D50 / 0x00DA7830's local `GiveHeroMorality`
  takes `float`, as the header and PDB say.
- Many local STLport vector, CWideString, CConversation and CHeroSoulsRoundDef models were replaced by the shared headers too.

**Blocked rows:**
- **0x00E12580 (7 grades):** on the shared string and vector headers, VC7.1 calls the CDefPointer accessor out of line at one site.
  A trial `__forceinline` overlay did not fix it.
- **0x00EDA8C0 (2 grades):** retail expands `vector<CCharString>::push_back` at one site and calls it at two others; the shared
  STLport vector has no per-site control.
- **0x00EE2A80 (4 grades):** needs a `__forceinline` `CBasicString<char>::operator==(const char*)` opt-in in the root `fable_string.h`
  (a coordinator header change; the same need as NF52's 0x006D41F0).

## Entries (APPLIED.tsv)
- **q10541: blocked.** 155 binders respelled; the 3 rows above are left.
- **q10542: blocked.** The row 0x01224C99 (pGlobalScriptDef's `_$E1`) is locked by NF59. NF60's private trial
  (`work/try_01224c99.cpp`, a definition `CDefPointer<const CScriptDef> pGlobalScriptDef;` as 0x0122AE7E has) grades RELOCATION_MATCH
  on leaf `_$E1`. It can land as the self rename `` `dynamic initializer for 'pGlobalScriptDef'' `` (module `_global`) once that lock
  is free (queued).

## Checks
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 159 changed rows graded from the repo after landing: 159 RELOCATION_MATCH, every
  test PASS. NF60's own final run agrees. No test changed.
- `naming/rebind.py` removed 76 binding lines and added 76. No landed binding of the plain-pointer spelling is left in
  reloc_bindings.tsv. bindings_multi_check is at its baseline (`NullCoord`).
- No header changed (`hdr/` is empty; `naming/2026-11-17_agentNF60/hdr/README.md` lists the existing opt-ins the rows use).

## Name fixes queued (`land1117fu`, 5)
- 0x00411570 `?Compare@?$CBasicString@D@@KIJPBD0@Z`: other callers still bind the free stand-in (NF60 NAME_FIXES.tsv).
- From NF60's NOTES.md "Gaps seen" (coordinator lines): the 0x0143E8F8 global, which rows bind as `g_pGameScriptInterface`,
  `g_pScriptTimerManager` or `pDSTGame`; 0x00E39540's `CRetailVector<long>::resize` for 0x004428F0 (catalog: `std::vector<long>::resize`);
  0x00EDC720's address-suffixed `WatchForAcolyteDeath_00edc980` / `WatchForAcolyteHit_00edcb80` (catalog:
  `NScript::CV_TempleOfLightScript::WatchForAcolyte*`); 0x00E24090's missing `// Leaf:` line.
- q10542 (0x01224C99) stays in the queue as it was.

## Rule slips (NF60 NOTES.md)
- An empty grep pattern built a 140 MB file in its work directory; deleted at once. A later attempt to delete the tool's saved copy
  outside its directory was denied by the permission system; nothing outside changed.
- One read-only Bash call exited 1; two foreground `until ...; sleep` waits.
- A helper ran its main loop when imported and overwrote 7 rows; they were redone and regraded.
