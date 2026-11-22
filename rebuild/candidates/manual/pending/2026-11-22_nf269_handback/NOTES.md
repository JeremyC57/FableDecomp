# NF269 notes (2026-11-22, round 28): entry 1 DONE (D5, the six Delete binders); entries 2-5 not started

Handed back after entry 1 (context: the chained reading was long). Six binder rows changed, all RELOCATION_MATCH + test PASS in one
vland --recheck-landed run with the overlay on, with the two self rows 0x00BDA090 / 0x00BDA0D0 as witnesses (recheck_final.log: 8
RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT). One header opt-in (hdr/README.md). Repo root (`ls -A`) before / after: identical.
No git, no sleep, no __asm / naked / _emit.

## D5: what moved and why it is right (two sources per call target)
- Call targets re-proven: the exe scan (callers.py) lists every E8 site of the queue trio; per-site xdis (evidence/callsites.txt, mnemonics
  only) shows each of the six rows calling 0x00BDA090 / 0x00BDA0D0 with `mov ecx,[GPLandscapeRenderer+0x6B0]` and one pushed member:
  0x00BEAA00 / 0x00BFDD40 / 0x00BEAAA0 / 0x00BEB4D0 -> 0x00BDA0D0; 0x00BFD480 -> 0x00BDA090 x2 + 0x00BDA0D0 x2; 0x00BFDE20 -> one each.
  The pushed members are the right buffer types by the PDB layouts: CEngineWaterBackgroundSubPatch IndexBuffer +0x08 / VertexBuffer +0x0C /
  Bridge*[4] (engine/CEngineWaterBackgroundSubPatch.h), CLandscapeLayerMesh IndexBuffer +0x34 / ForegroundVertexBuffer +0x38
  (engine/_quarantine/CLandscapeLayerMesh.h), CWaterPatchMesh VertexBuffer +0x10 (engine/CWaterPatchMesh.h); and the queue bodies select
  Index (+0x28) / Vertex (+0x1C) lists (nf267 evidence/identity.md). Names: DeleteIndexBuffer / DeleteVertexBuffer (public QAE), Ego_r
  009c25e0 / 009c2640 + FableWin 02cb7350 / 02cb7450, the spellings the self rows already define.
- 0x00BEAA00 / 0x00BFDD40: ported onto the retail branch of engine/CEngineLandscapeMemoryManager.h with the landed
  `_DELETEVERTEXBUFFER` declaration opt-in (the 0x00BEAE50 / 0x00BDA0D0 precedent); their row-local two-member manager model is gone.
  The header-less stand-in owner classes stay (their own names are NAME_FIXES, below); the freed member is typed CVertexBufferWin32*.
  objsec: every section byte- and relocation-equal to HEAD after the rename, except `.drectve` (+`/DEFAULTLIB:"libcp"`, expected once a
  row pulls stlport/fable_containers.h). DEF lists identical.
- 0x00BFD480 / 0x00BFDE20 (decision, see below): in-place respell of the manager model the rows already had (accepted rule). The retail
  branch cannot be used here because both rows also model CMovableResource locally (the branch includes engine/CMovableResource.h): HEAD
  debt, port queued. 0x00BFDE20's members +0x34 / +0x38 / +0x48 take the Ego_r names and types (HEAD had vertex / index the wrong way
  round: +0x34 goes to DeleteIndexBuffer). objsec ALL_EQ for both (5 sections each), DEF lists identical.
- 0x00BEAAA0 / 0x00BEB4D0 (July rows on candidates/outline_glow_compiled.h): the header's new opt-in replaces its stand-ins
  (DAT_01436ea8, LandscapeLayerList::InsertNode(int), a local two-member manager) with GPLandscapeRenderer + the retail manager branch and
  types +0x10 as the vertex buffer; the rows define the opt-in (and include the header first, so the overlay is taken). objsec ALL_EQ for
  both with the two renames. Tests rewritten on the shared headers (they link the rows: they defined DAT_01436ea8 and InsertNode), stub
  DeleteVertexBuffer / FreeVirtual and the renderer global over raw storage, PassPatterns kept. 0x00BEAAA0 got a Leaf line (HEAD had none),
  pinned from the HEAD object.
- The other four tests are self-contained host models (no extern; they define their own CLandscapeMemory / COwner / CLayerMesh /
  CEngineWaterBackgroundSubPatch / CMovableResource models and never link the row); unchanged. They still carry the stand-in names in
  their own models: the same debt as D6 (queue a rewrite on the shared headers).

## Census (objects, exe scan, table)
- 0x00BDA090: landed spellings 2 -> 1 (DeleteIndexBuffer: self, 0x00BE6090, 0x00BFD480, 0x00BFDE20). 0x00BDA0D0: 3 -> 1 (DeleteVertexBuffer:
  self, 0x00BE6090 and the six rows). Uncatalogued sites 0x00BE3807 / 0x00BE382B / 0x00BFD205 / 0x00BFD530 / 0x00BFD8C4 / 0x00BFDAD0 are in no
  landed row.
- 0x01436EA8: 0x00BEAAA0 / 0x00BEB4D0 move from `?DAT_01436ea8@@3PAEA` onto the PDB spelling; the stand-in keeps four July rows
  (0x00BE8920, 0x00BF4220, 0x00BF4130, 0x00BDC510: source level, each includes a candidates header declaring `extern fable_u8* DAT_01436ea8`
  (0x00BF4130's 00bf4130.hpp spells `std::uint8_t*`); only 0x00BE8920's use was read), so the table count stays 11 (witness moves,
  TABLE_CHANGES). No address grows.
- 0x00BD9B90: 0x00BEB4D0 already bound `?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z`; unchanged.
- New NAMES: none defined. Every object's DEF list equals HEAD's; the only new UND names (DeleteIndexBuffer / DeleteVertexBuffer /
  GPLandscapeRenderer) are each defined by one landed body (0x00BDA090 / 0x00BDA0D0 per the nf267 census; the global by no row).
  `??1CMovableResource@@QAE@XZ` (7-byte pick-any copy, no owner) is in 0x00BFD480 / 0x00BFDE20 at HEAD and after (pk.py, HEAD debt).
- Source grep: AppendFreeEdgeNode / FreeBlock are still DECLARED (never called, nothing emitted) in the local manager models of
  0x00BF9B40 / 0x00BF9B90 / 0x00BFDD70 (OnPostRelocation rows; not callers of 0x00BDA090 / 0x00BDA0D0 in the exe scan); left alone.
- Links (LINK unset, /NODEFAULTLIB /FORCE, tools/lnk.py): the six new objects together 0 duplicates; the six HEAD objects 0; control (one new
  object twice) 1 LNK4006.
- Vtables: 0x00BFD480 defines `??_7CEngineWaterBackgroundSubPatch@@6B@` / `??_7CMovableResource@@6B@`, 0x00BFDE20 `??_7CLandscapeLayerMesh@@6B@` /
  `??_7CMovableResource@@6B@` (local-model COMDATs, HEAD debt); vtcheck output identical HEAD v new (evidence/vtcheck_d5.txt: one stand-in
  slot each v retail's longer tables, unchanged). The other four objects emit none.

## Decisions for the coordinator
- Q1: 0x00BFD480 / 0x00BFDE20 use the in-place respell (row-local CMovableResource blocks the retail branch). Accept, and queue their port
  (CMovableResource.h + the retail manager branch, CEngineWaterBackgroundSubPatch.h for 0x00BFD480)?
- Q2: the candidates-header opt-in for 0x00BEAAA0 / 0x00BEB4D0 (a July header with two includers) rather than a full port: the rows are
  really CWaterPatchMesh's destructor pair (NAME_FIXES), and moving them onto engine/CWaterPatchMesh.h is a self rename with vtable-slot
  binders, out of D5's binder-only scope. Accept, and queue the CWaterPatchMesh unit (0x00BEAA00 ReleaseBuffers, 0x00BEAAA0 ~CWaterPatchMesh,
  0x00BEB4D0 ??_G) with the 0x00BFDD40 DeleteForegroundVertexBuffer split?
- Q3: 0x00BEAAA0 / 0x00BEB4D0, their two rewritten tests and the header overlay land together (the tests link the rows and define the opt-in;
  a HEAD test against a new row, or the reverse, fails to link). The other four rows are independent of each other and of the header.
- Q4: queue the four host-model tests above with D6 (rewrite on the shared headers).
- NAME_FIXES (not applied): six self names found while re-proving the binders (CWaterPatchMesh::ReleaseBuffers / ~ / ??_G,
  CLandscapeLayerMesh::DeleteForegroundVertexBuffer (a HEAD split with 0x00BF38F0's `?ClearBlock@CTextureOwner`), and the two virtual
  destructors 0x00BFDE20 / 0x00BFD480 whose self rows spell the non-virtual QAE while the table binds UAE).
- Catalog: STANDIN_LEAF warnings on 0x00BEAA00 / 0x00BFDD40 / 0x00BEAAA0 / 0x00BEB4D0 are those self names (HEAD state).

## Not started (APPLIED `open`)
Entry 2 (LocalDetailEngine 0x00BFAAE0 / 0x00BFB590, 0x00B684F0, the CEngineGlobal rows), entry 3 (D4 one definition in 0x00BDA150 /
0x00BD9CE0), entry 4 (D6 0x00BE6090's test), entry 5 (0x009B0AC0).

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none (no CompilerFlags entry on any of the six). Funclets / path includers of the six sources: none (grep of the six file names in
  rebuild/src, rebuild/tests, rebuild/include). Bases: orig/ and the six HEAD sources / two HEAD tests compared with the repo at hand-back:
  unchanged. NF268's overlay holds only engine/CCreatureAction_CastAssassinRush.h; no repo file names the new macro. Leaf lines: the four
  added / extended ones match the HEAD objects' defined symbols (objdump -t). Descriptive names: none new.
- Locks (rows.txt, NF269): the six rows and the witnesses 0x00BDA090 / 0x00BDA0D0 (the latter two had no lock dir; claimed new).
- Order: every compile ran after its lock was NF269, in a later call than a passing clang precheck (new and HEAD sources, new and HEAD
  tests, witnesses' sources and tests; pre.sh control with a deliberate error caught), with `timeout 900` on each launch line (grade
  batch, recheck); equiv_check under `timeout 3000`.
- Reading: the chained briefs NF267 back to NF215 and NF214's read-first list (NF92 brief / lessons / notes, NF44 brief, NF90 / NF89 / NF88
  lessons, ASM_RULES, EXTRA_RULES, COORDINATION), GOTCHAS.md every line (1-503), the nf267 landing README (D4-D6), the nf267 / nf265 /
  nf263 hand-backs (RULINGS D1-D3) and the L129 part of nf262's NOTES. The nf260 hand-back (0x009B0AC0 plan) was not read: entry 5 not started.
- Slips: one `rm -f $A/work/ctl_dup.obj` on a variable path (a single file I had just copied for the link control; not `-rf`, but not
  through tools/clean.py either). evidence/callsites.txt's 0x00BEAAA0 listing runs a few instructions past the function's end (xdis length).
