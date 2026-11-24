# NF267 notes (2026-11-22, round 27): entry 1 DONE (the CEngineLandscapeMemoryManager Delete trio family); entry 2 not started

Handed back after entry 1, as the brief says. Seven rows ported as one unit, two controls regraded; every row RELOCATION_MATCH + PASS
in one vland --recheck-landed run with the overlay on (recheck_final.log, 9 rows). Repo root (`ls -A`) before / after: identical.
No git, no sleep, no __asm / naked / _emit.

## The unit (RULINGS D1-D3)
- 0x00BDA150 ProcessDisposalList / 0x00BD9CE0 FreeDisposalList: in place (D1). The three same-TU destroy-now helpers lose the public
  names: `DeleteIndexBuffer` / `DeleteVertexBuffer` / `DeleteTexture` (QAE) -> `DeleteIndexBufferBlockWhileBusy` /
  `DeleteVertexBufferBlockWhileBusy` / `DeleteTextureBlockWhileBusy` (private, AAE), declarations wrapped in `private:` / `public:` where
  they stood (no reordering). Proof: each object's non-debug sections are byte- and relocation-equal to HEAD's after the three renames
  (tools/objsec.py: ALL_EQ, 50 sections each). Leaves unchanged. Identity / Shape comments corrected; both carry a `// Fidelity:
  same-TU strong definitions` line. Section-3-style list of these DEF renames: evidence/helper_inplace_renames.tsv (kept out of
  RENAMES.tsv because no relocation binds them).
- After the recheck, one comment line of rows/00bd9ce0.cpp was corrected (its HEAD claim that the TU reproduces retail 0x00BD9CC0 byte for
  byte is false: 75 v 28 bytes). Same line count; gcc cpp output md5-identical to the graded file (work/00bd9ce0_graded.cpp).
- 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110: self renames onto the public names (RENAMES s1), ported onto the header (D3: the 0x00BDA110 draft
  form, the retail branch + one declaration opt-in each), tests rewritten on the header with the old PassPatterns. The HEAD rows'
  renamed-include local models of the manager, its bases and std::list are gone.
- 0x00BE5C30 / 0x00BE6090: NF265's held drafts, regraded today (RENAMES s2: 7 callee lines; 0x00BE5C30's leaf -> ClearTextures).
- Identities re-proven from a second angle: evidence/identity.md (Ego_d field-list access, retail callers of each trio, bodies, sizes).
  The no-friend leg was dropped: the Ego_d type dump shows no LF_FRIENDCLS anywhere, so it cannot show one here.

## Census (from objects and the exe scan)
- Per address (landed spellings): 0x00BDA110 3 -> 1; 0x00BDA090 3 -> 2 (AppendFreeEdgeNode left in 0x00BFD480 / 0x00BFDE20);
  0x00BDA0D0 4 -> 3 (FreeBlock in 0x00BEAA00 / 0x00BFDD40 / 0x00BFD480 / 0x00BFDE20, LandscapeLayerList::InsertNode in 0x00BEAAA0 /
  0x00BEB4D0); 0x00B677A0 2 -> 2 (0x00BE6090 moves from the Native stand-in to the PDB `AAJ` spelling; the self row still spells `PAJ`,
  NAME_FIXES; the self row 0x00B677A0 has no Leaf line and defines `QAEXPAJ@Z`; 0x00BE6090 is its only caller, callers.py); 0x01436EA8 table 12 -> 11 (`?gRenderer@Native@@3PAURenderer@1@A` loses its only binder; the untabled `?g_obj_01436ea8`
  also goes). No address grows; no new split.
- Per new NAME (rebuild/src + rebuild/tests + rebuild/include grep, then the objects): `?DeleteIndexBuffer` / `?DeleteVertexBuffer` /
  `?DeleteTexture@CEngineLandscapeMemoryManager@@QAE...` are defined only by 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 (queue bodies) after
  the unit; at HEAD 0x00BDA150 and 0x00BD9CE0 defined them with destroy-now bodies. The BlockWhileBusy names are defined only by
  0x00BDA150 and 0x00BD9CE0 after the unit (at HEAD by the three queue rows); the two copies are identical (tools/cmpobj.py SAME).
  New pick-any COMDATs of the header port (list<T*>::push_back / insert / _M_create_node / allocate for the three pointer types) have no
  table line, no catalog entry (demangled grep of auto-re-candidates.tsv for list<CTexture / CIndexBuffer / CVertexBufferWin32*>: none)
  and no other landed definer in source; `end` and the iterator ctor are SAME as 0x00BDA150's copies.
  `?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ`: defined by 0x00BE5C30 only, bound by nobody (NF265).
- Links (LINK unset, /NODEFAULTLIB /FORCE, tools/lnk.py): HEAD 0x00BDA150 + HEAD 0x00BD9CE0 = 4 duplicates (FreeVirtual + the three helper
  names); the seven new objects together = the same 4 (FreeVirtual + the three BlockWhileBusy names), no Delete<X> duplicate. Control:
  HEAD 0x00BDA150 + NF265's 0x00BDA110 draft object reproduces NF265's LNK4006 on DeleteTexture.
- 0x00BE6090: both emitted vtables (`??_7Patch@Native@@6B@`, `??_7Movable@Native@@6B@`) vtcheck-identical to HEAD. Other objects emit none.
- pk.py: no new copy with a retail owner differs; FreeVirtual's TU-mate copies are 68 v owner 0x00BD9B90's 97 bytes in both rows,
  at HEAD and after (debt, unchanged).

## Decisions for the coordinator
- Landing: the seven rows (0x00BDA150, 0x00BD9CE0, 0x00BDA090, 0x00BDA0D0, 0x00BDA110, 0x00BE5C30, 0x00BE6090) and the three overlay
  headers land together or not at all: a mixed HEAD / new set puts two bodies under one Delete<X> or BlockWhileBusy name again.
- D4 (the HEAD duplicate, renamed): 0x00BDA150 and 0x00BD9CE0 each define FreeVirtual and the three helpers strongly (non-inline
  same-TU members). That was 4 duplicates at HEAD and is 4 after (same functions, the helpers now under their private names); FreeVirtual
  is also defined by its owner row 0x00BD9B90. The helper copies are not retail's out-of-line bodies (72 / 72 / 75 bytes v retail 0x00BD9C00 /
  0x00BD9C20 / 0x00BD9CC0's 29 / 29 / 28 bytes, read from the disassembly to each `ret 4`: retail calls FreeVirtual out of line there, the TU-mate copies inline it). This is HEAD
  debt carried under new names (4 duplicates before, 4 after; the pick-any alternative was not measured). Accept it as carried debt, or queue a later pass that makes the TU-mate copies one definition (e.g. the
  nf86 / nf88 pick-any scaffolding, which must leave ProcessDisposalList / FreeDisposalList's bytes unchanged)?
- D5: the six other binders of 0x00BDA090 / 0x00BDA0D0 keep their stand-ins (AppendFreeEdgeNode, FreeBlock, LandscapeLayerList::InsertNode),
  as RULINGS D1 allowed; NAME_FIXES has them. Several are L129 rows (0x00BEB4D0's DAT_01436ea8, 0x00BEAAA0 ...), so they fit the next
  L129 unit.
- D6: 0x00BE6090's landed test is a self-contained host model carrying the old Native:: stand-ins (`Native::LandscapeMemory::Delete<X>`,
  `Native::Renderer`, `gRenderer`); it does not link the row, so it still PASSes. Rewrite it on the shared headers in a later pass?
- Catalog: RENAMES s1 renames the catalog names of 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 (vland prints WARN STANDIN_LEAF until then)
  and files `..._Delete<X>BlockWhileBusy_<addr>.cpp` -> `..._Delete<X>_<addr>.cpp`. NAME_FIXES gives the BlockWhileBusy names for the
  uncatalogued 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (VB / IB for the first two from Ego_r order only).

## Not started (APPLIED `open`)
LocalDetailEngine 0x00BFAAE0 / 0x00BFB590, 0x00B684F0, 0x00BEB4D0's DAT_01436ea8, the CEngineGlobal rows, 0x009B0AC0 (nf260 plan).

## Flags / funclets / descriptive names / locks
- Flags: none (no row needs a CompilerFlags entry). Funclets / path includers of the seven rows: none (grep of rebuild/src, tests,
  include). Descriptive names: none new.
- Locks (rows.txt, NF267): 0x00BDA150, 0x00BD9CE0, 0x00BDA090, 0x00BDA0D0, 0x00BDA110, 0x00BE5C30, 0x00BE6090, 0x00BE5C60, 0x00BEAE50.

## Order, reading, slips
- Every compile ran after its lock was NF267, in a later call than a passing clang precheck, under an outer `timeout 900`. Clang-only
  precheck failures reproduced by two-line VC7.1-header controls and graded: 0x00BDA150's test (`<list>` / `<cwchar>`), 0x00BE6090's
  HEAD test (`assert`), 0x00BEAE50's test (`<cwchar>`). pre.sh control (a deliberate syntax error) and the equiv_check planted-token control
  were run live.
- Two-source names checked first-hand this session: ghidra_out/egor_pdb_names.tsv + fablewin_pdb_names.tsv list all six Delete names
  (public QAE / private AAE), `?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ` and `?UnregisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJ@Z`;
  Ego_d.pdb types agree on the six (evidence/identity.md).
- Reading: the chained briefs NF265 back to NF215, NF214's read-first list (NF92 brief / lessons / notes, NF44 brief, NF90 / NF89 / NF88
  lessons, ASM_RULES, EXTRA_RULES, COORDINATION) and GOTCHAS.md every line (1-503), the NF265 hand-back (RULINGS, NOTES, LESSONS).
  The nf263 / nf262 / nf260 landing READMEs were not reread (entry 2 not started).
- Slip: the final recheck was first launched before the unchanged HEAD sources / tests of 0x00BD9CE0 / 0x00BE6090 / 0x00BE5C60 /
  0x00BEAE50 had been prechecked in this session. I stopped it by PID (one cl.exe of 0x00BDA150 had started; killed by killmine.sh, which
  matches this agent's folder), prechecked them with controls, and relaunched in a later call. While stopping it I once used
  `pkill -P <pid>` (match by parent PID, not by name); every other stop was by explicit PID.
