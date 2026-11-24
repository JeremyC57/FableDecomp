# NF380: split_pdb_multi research (89 targets)

Agent NF380 for Account A. Work dir: `$S/w22/agentNF380`. Nothing was written into the repo and no git command was run.

## Ruling needed #1: retail Fable.exe has no ICF
Several GOTCHAS rules assume the retail linker folded identical bodies onto one address. Those are GOTCHAS 302 (NF30, `ret 8`
bodies "ICF-folded") and 458 (landing nf104: a "linker-folded address" such as 0x004046B0 may carry several names). The retail
binary does not support that assumption. The evidence is in `evidence/noicf.tsv`:
- **Linker settings.** The retail debug directory is a CodeView RSDS record that points to
  `...\Fable1_5Release\Fable\Build\Fable.pdb`, from linker 7.10. That means a /DEBUG link, and VC7.1's /DEBUG default is
  /OPT:NOREF,NOICF.
- **Identical template bodies at separate addresses.** Template instances are always COMDAT, so ICF would have folded them
  whatever /Gy was. Yet identical ones (call targets included) sit at two retail addresses each: the CIntelligentPointer<const
  CThing> ctor at 0091f990 / 00654d58, std::fill at 00a52080 / 00a945d0 and at 00a96350 / 00a95b00, and the 0x004046B0 STLport
  ~_List_base family. These carry the proof.
- **Non-template pairs agree.** 00a629d0 / 00a62ac0, 00a62960 / 00a629a0 and 0041a3d0 / 0041a3f0 point the same way, but are
  weaker alone: a non-COMDAT function survives ICF without /Gy.
- **Trivial bodies are not merged.** The catalog holds 717 `ret` bodies, 263 `ret 4`, 77 `ret 8` and 28 `mov eax,ecx; ret`.
- **The nf104 exemplar has twins.** 0x004046B0 has more than 40 identical twins at separate addresses (`evidence/twins.tsv`).

**Consequence:** each retail address carries exactly one defining function name. The only true multi-name address among the 89 is
0x00BFEA30, where `__chkstk` and `__alloca_probe` are two public labels on one CRT asm routine (`FOLDED.tsv`). Every other split
is a wrong spelling somewhere. Usually that is a binder whose model inlines a wrapper or names a sibling class, or a self row
named after its identical twin.

The owner or coordinator should:
- rule on retiring the "folded" reading of GOTCHAS 302 / 458;
- treat a `split_pdb_multi` row as "find the one right name", never as "keep both".

b1 and b2 do not depend on this ruling. Each rests on retail RTTI, FableWin callers and the table witnesses, so both hold even if
the ruling is declined.

## Verdicts (RULINGS.tsv, all 89)
In the vocabulary the brief asked for: **folded 1, respell 56, unknown 32**. The sub-tags are below.

| verdict | count | meaning |
|---|---|---|
| respell | 44 | one right name proven (≥2 sources); the wrong spelling is named |
| respell(wrapper) | 7 | right name proven; the binder inlines a trivial wrapper, so the fix is a model change |
| respell(T3) | 3 | T3_PORT_QUEUE target: researched read-only, not changed |
| respell(binder identity) | 1 | 0x00810FC0 = GetPHitBy; binder 0x00733020's own identity is doubtful (QUEUE q6) |
| respell(held) | 1 | 0x0052C7E0: binder 005367f3 -> CChangingStateComponent::Update, but the target is on BRIEF_NF_BATCH's held list (NF281 D4) |
| unknown(wrapper) | 23 | wrapper pairs (~CCountedPointer / ReleaseData, CArray<T> / vector<T>, wrapper-class copy) where reading cannot decide which one is out of line |
| unknown | 3 | long / unsigned long instance pairs (0050e500, 00a326d0, 00a335e0): object measurement needed |
| unknown(T3) | 6 | T3 targets, read-only |
| alias (folded) | 1 | 0x00BFEA30 `__chkstk` / `__alloca_probe` |

**Evidence ranking:**
1. FableWin.exe donor callers. FableWin is a debug build with no ICF, and its ILT jmp thunks resolve every call.
2. Retail RTTI base lists.
3. Retail identical-body twins.
4. Ego_r / FableWin object order.
5. The retail body read against the Ego_r layout.

Ego_r is folded and inlined, so it never breaks a tie on its own. FableWin / Ego_r use Dinkumware while retail uses STLport
(NF301), so the `_Sort_heap` selves are respelled to the STLport `sort_heap` the binders already use.

## Batches (land_scripted run dirs)

### b1: 13 binder rows, 9 targets, all ACCEPT
Rows: 00902873, 006f5530, 0044c146, 00a986b0, 00a98740, 00548350, 00549230, 008e9110, 008e9080, 009acab0, 009b0200, 00af4240,
00918640.

Gates:
- **vland:** RELOCATION_MATCH PASS for HEAD and new at one LANDVERIFY_WORK path.
- **objsec:** ALL_EQ with one rename per row. 00918640 has 23 pick-any renames under the rule `VCThingGameObject@@ ->
  VCThing@@` (`tools/objsec_vec.py`).
- **Pick-any checkers:** SAME. 00918640 loses one HEAD NEQ ADDR_CONFLICT at 0x00788FFC and gains nothing.
- **Reach flips:** 0.
- **Link:** 0 new duplicates.
  - The new names resolve, except GetNonLoopingDuration and WriteUBYTE. Those were unresolved under their old names as well,
    because the owner rows 0x00A23CE0 / 0x00993E50 define stand-ins (NF302 Q2; NAME_FIXES).
  - The one LNK4006 on 009b0200 (CDefClassBase::InternalSaveBinary) was there at HEAD too.
- **apply_table --dry:** 0 misses, no new multi-target symbol.

Table changes: TABLE_CHANGES has 1 replace and 7 deletes. HAND_TABLE_CHANGES has 1 rewitness:
`??0CShoveHaPennyRegionDef` at 0x0044C0C0 moves to witness 0044c301, which still binds it (compiled, UND, xr).

### b2: 2 self rows, ACCEPT
- **0x00A629D0:** `~CPackedUIntArray` -> `?Clear@CPackedUIntArray@@QAEXXZ`.
  - The identical twin 0x00A62AC0 is the destructor.
  - Table witness 009cd740 (FableWin caller CBankFile::PackRuntimeData) binds Clear here.
  - Ego_r order is Clear < ~.
  - `engine/CPackedUIntArray.h` already declares Clear at 0x00A629D0.
- **0x00E0F1D0:** `EndMission` -> `?WatchForTermination@CQ_WaspBossScript@NScript@@QAEXXZ`.
  - CQ_WaspBossScript::Main 0x00E0EA40 is the table witness and FableWin / Ego_r caller.
  - EndMission is its own body at 0x00E13310.

Gates:
- **vland** (one LANDVERIFY_WORK path): HEAD PASS. New PASS three ways:
  - r2n, with the catalog leaf;
  - r2m, with the new `// Leaf:`;
  - r2t, with the test respelled the same way (`tests/`; the method is renamed in the test's local class).

  The 00a629d0 test still prints DTOR_OK, because the catalog PassPattern names that token. The r2t objects are the same as the
  r2n objects (objsec ALL_EQ, renames=0).
- **objsec:** HEAD v new ALL_EQ with 1 rename each.
- **Pick-any checkers:** only HEAD lines are removed (an EQ at 0x00A62AC0 and a NEQ at 0x00E13310). Nothing new, 0 reach flips.
- **Census by compile.** Every landed row that names the old or new spelling was compiled and linked /FORCE /MAP with the row,
  HEAD v new:
  - CPackedUIntArray: 00a62ac0 009ce180 0066c000 0066c150 009cd850 009cd740 009cc530 009ccc00;
  - CQ_WaspBossScript: 00e13310 00e12580 00e137b0 00e0ea40.

  Duplicates drop from 4 to 2. Clear and WatchForTermination now resolve, and ~CPackedUIntArray / EndMission now resolve to their
  own rows. Nothing is lost. The 2 duplicates left are 0066c000 / 0066c150, which also define ~CPackedUIntArray at HEAD (QUEUE q9).
- **Table:** it already binds both new names at these addresses, so TABLE_CHANGES is empty. The dry run shows 0 misses.

Both new names are in the Ego_r and FableWin name sets (`evidence/pdbnames_b2.txt`).

**Stay-off checks for b2:** no NF378 / NF379 row, no T3 line, no Account B claim or pending row, no Codex RESERVED.tsv line, and
not on BRIEF_NF_BATCH's held list (`naming/briefs/BRIEF_NF_BATCH.md`). The one Codex hit is pass26_open/00baa870
BINDING_EVIDENCE.json "00a629d0". It is an instruction address in that row's probe notes, not a reservation.

## Hand steps for the coordinator
1. Land b1:
   - apply its TABLE_CHANGES.tsv;
   - then apply HAND_TABLE_CHANGES.tsv (the rewitness).
2. Land b2. It has no table change. Copy its `tests/` too, for example with `apply_rows.py <b2>/tests`. Then fix the catalog labels, which vland reports as WARN STANDIN_LEAF until they change:
   - 0x00A629D0: `CPackedUIntArray::~CPackedUIntArray` -> `CPackedUIntArray::Clear`;
   - 0x00E0F1D0: `NScript::CQ_WaspBossScript::EndMission` -> `NScript::CQ_WaspBossScript::WatchForTermination`.

   Renaming the source / test files is optional; the current names are `CPackedUIntArray_CPackedUIntArray_00a629d0*` and
   `NScriptCQWaspBossScript_EndMission_00e0f1d0*`.
3. b1 and b2 are independent and can land in either order.

## Not changed, and why
- **0x004382C0 (ForceClearGui).** Account B's pending CPlayerGui.h headers (acctB_c2009, c2017, c2039, c2041, r2041, parked)
  already model 0x004382C0 as ForceClearGui. That agrees with this ruling and is left to their work.
- **0x00533CC0 (AddChildDynamically).**
  - Evidence: vftable 0x0124608C slots 59 / 60 / 61 are AddChild 0x00533B50, this row, and AddChildFront 0x00533BA0. Ego_r
    ui_component.obj order is the same. GOTCHAS 303 (NF30) says the same.
  - Why not changed: the rename moves the AddChild definition that binders of T3 nf294 (0x00533B50) resolve against, so it goes
    with that unit.
- **Binder 0042ded5 (0x0041A3D0 GetHDMusicDir).** It is in Account B pending.
- **Binders 006314c0 / 00558770 / 006bab80 / 00733020.** Each needs an identity check first (QUEUE q3, q6).
- **Owner stand-ins.** 0x00A23CE0 (GetNonLoopingDuration), 0x00993E50 (WriteUBYTE), 0x00AF0710 (NF284 AutoTinyVirtualObject) and
  0x00CBD510 are T3 or owner ports; their names are recorded in NAME_FIXES.
- **The 0x0044C0C0 family.** b1 fixed only the 0044c146 pilot. The full family is QUEUE q1: about 78 generated Def-ctor rows,
  with candidates listed in `evidence/shovehapenny_family.txt`. That list is a text census and must be filtered by binding.

## Hand-back files
- **RULINGS.tsv:** all 89 targets.
- **FOLDED.tsv:** 0x00BFEA30 only.
- **STATUS.tsv:** 89 targets. accept 11 (b1 9, b2 2), queued 41, unknown 26, skipped-T3 9, held-ruling 1, folded 1.
- **NAME_FIXES.tsv:** 56 respell rulings, with the right symbol, wrong spellings, and fixed_by.
- **QUEUE_PROPOSALS.tsv:** q1–q9. q9 is new from the b2 link: 0x0066C000 and 0x0066C150 also define ~CPackedUIntArray.
- **RULINGS.tsv action column:** updated for the rows b1 / b2 changed and for the two self renames left alone.
- **evidence/:** noicf, triage, callers, elsewhere, simret, twins, objsec, pk_gate, link, census and pdbnames files.
- **tools/:** every helper, including the NF377 copies retargeted to NF380.

## Measurements
- **Compiles:** about 57 of the 100 budget.
  - runpair r1: 26.
  - r2 / r2m / r2t: 8.
  - Context head-only compiles c1 / c4 / c5: 21.
  - Census c2 / c3: 2.
- **Locks:** NF380 holds the 15 changed rows until landing:
  - b1: 00902873 006f5530 0044c146 00a986b0 00a98740 00548350 00549230 008e9110 008e9080 009acab0 009b0200 00af4240 00918640;
  - b2: 00a629d0 00e0f1d0.

  Every context lock is released.
