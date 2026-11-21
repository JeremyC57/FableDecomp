# NF214 notes (round 10: UnpackNames family, CRetailVector overflow rows; single agent)

Counts: see STATUS.tsv / APPLIED.tsv. Final recheck: recheck_final.log (one vland run over every changed row).

## Unit 1: q12343 (0x00A2ED10 C3DMeshStats::UnpackNames family) -- done
- Census (HEAD objects in work/head/, binds.py pairs): self 0x00A2ED10 defined `?UnpackNames@C3DMeshStats@@QAEXAAUCCharStringVector@@...`;
  0x00A2F0F0 / 0x00A2F120 (no Leaf lines) defined QAE CCharStringVector stand-in leaves and bound the QAE UnpackNames stand-in;
  0x00A2F150 (bare Leaf) defined a QBE CCharStringVector leaf and bound the QBE UnpackNames stand-in; 0x00A2F170 already bound the PDB
  ABE spelling. Fable.exe scan (work/fam_refs.tsv): 0x00A2F0F0 <- 0x00A35B30, 0x00A2F120 <- 0x00A35AF0, 0x00A2F150 <- 0x00A35B50,
  0x00A2F170 <- 0x00A35B10. The three July wrappers bind unrelated stand-ins (?h@N@@QAEXHH@Z, ?h_00a2f120@N@@QAEXHH@Z,
  ?Build@Inner@@QAE_NPAXJH@Z), never the callers' old self leaves.
- Moves (one hand-back): self leaf -> PDB ABE vector form; the three callers ported onto engine/C3DMeshStats.h (new opt-in
  FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES) + fable_string.h, their own leaves -> the PDB QBE vector forms (both PDBs), their
  UnpackNames calls -> ABE. Result at 0x00A2ED10: 4 of 4 referencers bind the self leaf (3 spellings -> 1).
- **Coordinator item (count-neutral self renames, not blocking):** the callers' self renames leave each caller address at 2 spellings,
  as before (old stand-in self leaf + the wrapper's stand-in -> PDB self leaf + the same wrapper stand-in); gate.py's CREATED needs
  before <= 1, so it does not fire. No landed row binds an old caller leaf. The wrappers 0x00A35B30 / 0x00A35AF0 / 0x00A35B50 are
  outside rows.txt and were not claimed or changed; their PDB spellings are in NAME_FIXES.tsv (C3DMeshObject::GetAllDummyObjectNames /
  GetAllHelperPointNames / GetDummyObjectNamesStartingWith). Porting them would cascade into their callers (0x004BDB40 already binds
  the PDB GetAllDummyObjectNames@C3DMeshObject; 0x004BDA30 / 0x004BDD70 bind CStdVector stand-ins).
- Self row models: moved CCharString (fable_string.h, FABLE_RETAIL_CCHARSTRING), CCRC (engine/CCRC.h), CMatrix3x4 (engine/CMatrix3x4.h);
  dropped the CCharStringVector wrapper (nf212 advisor decision). Not movable (recorded):
  - the local C3DMeshStats model: engine/C3DMeshStats.h types HelperPoints / DummyObjects only under
    FABLE_RETAIL_C3DMESHSTATS_INITIALISEFROMFILEDATA, which includes engine/CPreAllocVectorMap.h and engine/C3DHelperPoint.h; those
    exist only in rebuild/candidates/manual/pending/2026-11-08_linux_hand868/hdr/engine/. With the default header the maps are byte
    arrays and the row would need a cast. Needs those two headers promoted first (then the maps' PDB QBE bound searches).
  - CHelperPointMap / CDummyObjectMap / SHelperPointEntry / SDummyObjectEntry stand-ins and their same-TU strong bound-search bodies
    (HEAD debt: the bodies differ from owners 0x00A316B0 / 0x00A31700 / 0x00A31790 / 0x00A317E0, DIFFER 60v65 / 60v65 / 61v66 / 61v66;
    landed siblings 0x00A2E920 (QAE) and 0x00A2EA60 (QBE) bind the same stand-in spellings declaration-only). Real fix: the PDB
    `?LowerBound@?$CPreAllocVectorMap@KVC3DHelperPoint@@...@@QBE...` spellings with the owners' bodies (nf96a same-TU form).
  - The local GetNoHelperPoints / GetNoDummyObjects FABLE_FORCEINLINE helpers (HEAD; made const). Made-up helper names on
    C3DMeshStats (the PDB has GetNoHelperPoints only on C3DMeshObject / CPhysicsMesh*): descriptive, unchanged from HEAD.
- `mutable` (stand-in device) on the two local map members: the const UnpackNames keeps the HEAD non-const stand-in symbols. The
  const-method form also graded RELOCATION_MATCH but newly defined ?LowerBound@CDummyObjectMap@@QBE... (bound by 0x00A2EA60, table
  line at 0x00A31790) with a body differing from its owner: a pick-any hold, so it was dropped (advisor).
- Section 2 also records 0x00A2ED10's "" literal -> ?NullChar@CCharString@@1DB (0x0129AAF4; retail loads that address at
  0x00A2ED8E / 0x00A2EDD9 / 0x00A2EE24). 0x0129AAF4 already has 1DB / 2DB / stand-in spellings; 1DB is bound by 0x00AE8D50, so nothing grows.

## Pick-any checks, unit 1 (final objects)
- 0x00A2ED10: IAE _M_insert_overflow v owner 0x00433530 (fresh object): 316 = 316, bytes / relocs True; push_back v 0x0044BFF0: 59 = 59,
  True / True. New ?GetAsCharArray@CCharString@@QBEPBDXZ copy (table line 0x00403A10): q.py RELOCATION_MATCH against 0x00403A10; the
  owner row defines the stand-in free function ?CCharString_operator_char_const@@YIPBDPBVCCharString@@@Z (no LNK2005) and its one
  relocation names ?g_EmptyCharString_0129AAF4@@3PADA, the same retail address as the copy's ?NullChar@CCharString@@1DB (nf205).
  Other new symbols (?GetLength@?$CBasicString@D@@QBEKXZ, ?PeekData@?$CBasicString@D@@QBEPBDXZ) are shared-header inlines with no table
  line and no landed Leaf; the QBE GetNo* helpers are local-model inline COMDATs nothing binds. The four QAE map stand-ins: cmpbody
  HEAD v new equal (debt unchanged). Removed: the in-object "" literal and ?GetText@CCharString@@QBEPBDXZ (local model).
- 0x00A2F0F0 / 0x00A2F120 / 0x00A2F150: defined-symbol diff HEAD v new = the own leaf only.

## Unit 2: q6333 / q6342 (0x00485DBC, the two CRetailVector rows) -- done
- Rows 0x007E4B60 CTCCreditsUI::Activate and 0x006116E0 CTCInventoryMap::OpenInventory: NF213's ports (its work/*_f1_sizebody.cpp:
  CRetailVector stand-in -> std::vector<NUISystem::CComponent*>, OVERFLOW_OUTOFLINE + COPY_TRIVIAL_OUTOFLINE + FILL_N_FASTCALL + the landed
  NF213 OVERFLOW_ / PUSHBACK_ NUISYSTEM_CCOMPONENT_SIZE_BODY opt-ins) plus one new opt-in, FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY
  (NF213 option d, next form as the brief gives it): fill_n<NUISystem::CComponent**> visible and noinline in its own optimize("s", on) block.
  First form on each row: RELOCATION_MATCH (q.py, then vland work/b2.log RELOCATION_MATCH + PASS with /d2inlS12).
- Pick-any (final objects, fresh owner objects in work/owner/): overflow v 0x00485DBC 137 = 137 bytes / relocs True (the owner's copy
  calls the same ??$fill_n@PAPAVCComponent@NUISystem@@... symbol, external there, in-object COMDAT-any here: nf212 refinement, all three
  conditions: same name, COMDAT any (selection 2), equal to its own owner); fill_n copy: q.py MATCH against 0x0048514C (23 bytes; the owner
  row's own symbol is the unsigned long instance, so no LNK2005); push_back v 0x00486024 49 = 49 True / True; placement new ??2@YAPAXIPAX@Z
  (table -> 0x00405C10) q.py MATCH. The other new symbols (size, _M_set, allocate, deallocate, max, _Construct, __destroy*, _M_clear,
  std::__true_type ctor) are STLport inlines with no table line and no landed Leaf. Removed: the CRetailVector stand-in push_back /
  _M_insert_overflow (strong, same-TU) and the global ::__true_type ctor.
- Size-pragma control: with the two pragma lines removed both row bodies are byte-identical and still RELOCATION_MATCH (0x007E4B60 165 = 165,
  0x006116E0 535 = 535); only the fill_n COMDAT changes (30 -> 23). hdr/README.md.
- **LNK2005 (debt, report; accepted pattern nf181 / nf184):** landed 0x0047E196 CTCInGameMenu::SaveGame defines the same
  ??$fill_n@PAPAVCComponent@NUISystem@@IPAV12@@std@@...@Z as a STRONG explicit specialization (plain .text section, not COMDAT; its row
  sets optimize("y", on) / ("s", on) / auto_inline(off)); its 23 bytes are identical to the new pick-any copy (cmpbody 23 = 23, bytes /
  relocs True; work/owner/other_0047e196.obj). Linked with either new row this gives one LNK2005 with identical bytes. Queue: make
  0x0047E196's copy `inline` (pick-any, nf140) or move it onto the new opt-in. No landed row other than 0x0047E196 defines the symbol
  (source grep).
- **Gate note for the coordinator:** RENAMES section 2 has 0x00A2ED10's "" literal -> ?NullChar@CCharString@@1DB (0x0129AAF4). 0x0129AAF4 is
  referenced by a very large number of rows, so a gate scan of it can explode (GOTCHAS landing nf120); the old side was an in-object
  literal with no table line, and 0x00AE8D50 already binds the 1DB spelling, so the set cannot grow: prove it from reloc_bindings.
- In-object call targets (pick-any COMDATs under the owners' own spellings, nf212 refinement): 0x007E4B60 / 0x006116E0 push_back ->
  _M_insert_overflow -> fill_n; 0x00A2ED10 push_back -> _M_insert_overflow, and its four differing QAE map stand-ins (strong, HEAD debt).
- Inline budget: S11 leaves allocator::allocate out of line in the overflow copy (135 v 137) on 0x007E4B60, so /d2inlS12 is the smallest N
  (as NF213 found for 0x005414A0).
- Census: with NF213's three moves, 11 of 11 REFS referencers of 0x00485DBC bind the IAE self leaf; the CRetailVector spelling
  (bound only by these two rows) is gone. No split created; the address narrows to one spelling.

## Flags lines
- 0x007E4B60 and 0x006116E0: `// Flags: /d2inlS12` + `// Inline-budget:` lines (from NF213's drafts; HEAD had none). The catalog
  CompilerFlags of both must carry `/d2inlS12` (vland b2 ran with it in the payload).

## Header opt-ins (hdr/README.md)
- engine/C3DMeshStats.h: FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES (declarations, 5 PDB spellings listed there).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY (size-pragma scaffolding, conditions met).

## Locks
- Only rows in rows.txt touched; no new claims. 0x00A2F170 (REFS binder, already right) and the July wrappers 0x00A35AF0 / 0x00A35B30 /
  0x00A35B50 were not claimed or changed.

## Debt noticed (not changed)
- 0x00A2F170 C3DMeshStats::GetHelperPointNamesStartingWith still models C3DMeshStats / CCharString locally (it can move onto the new
  UNPACKNAMES opt-in, leaf unchanged); 0x00A35B10 models C3DMeshStats locally too.
- 0x00A2E920 / 0x00A2EA60 / 0x00A2E8C0 bind the CHelperPointMap / CDummyObjectMap stand-ins (QAE / QBE) of 0x00A316B0.. 0x00A317E0, whose
  landed self rows carry bare `// Leaf: LowerBound` / `UpperBound` lines; 0x00A2ED10 still defines differing strong stand-in copies.
- 0x00A35AF0 and 0x00A35B30 define the same stand-in symbol ?fn_00a35b30@@YIXPAUBL@@HH@Z (one spelling, two addresses).

## Descriptive names
- None new (GetNoHelperPoints / GetNoDummyObjects on the local C3DMeshStats model are HEAD's).

## Funclets
- None: no row or test under rebuild/src/compiled or rebuild/tests includes any of the six changed files.

## Casts
- No stand-in type cast to a real one.

## Rule slips
- One control compile ran on an unchanged overlay (a scratch-overlay rewrite failed its own assertion and wrote nothing, so the grade
  was the declaration-only form again: DIFFER 158v165); rerun correctly. Logged in work/grades.log. No other slips: no git, no writes
  under the repo, one compile at a time inside timeout, only rows.txt rows touched.
