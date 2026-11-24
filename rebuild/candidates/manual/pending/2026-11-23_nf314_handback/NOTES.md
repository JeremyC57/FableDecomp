# NF314 notes (2026-11-23): NF309's continuation, hand-back 1 (88 entries)

## Summary
- **25 rows changed, all landing-ready** (`rows.txt`).
  - vland `--recheck-landed` with the hdr/ overlay: **WINS 25**, all RELOCATION_MATCH PASS, no DIFFER / LINK_FAIL / SRC_FAIL
    (`recheck_final.log`; payload work/rc1).
  - One pre-existing warning: 0x004FF8B0 `RELOC_CONFLICT ?RemoveMap@CEngineManager@@QAEXJ@Z -> 0x006403D0 here, 0x00640430 in landed
    0x004FEDE0`. HEAD 0x004FF8B0 binds the same symbol (H object read), and the callee line is untouched here (Q7).
- **One shared-header change**, an add-only `#ifdef` opt-in under ruling NF309 Q2: `hdr/engine/C3DPrimitive2.h`
  (`evidence/hdr_C3DPrimitive2.diff`; the HEAD copy is in orig/).
  - `FABLE_RETAIL_CDEVICEINDEPENDENT_ADDTRIANGLELIST_PRIVATE`:
    - a global `class C3DPrimitive2;` forward declaration;
    - in N3DPrimitive::CDeviceIndependent: `friend class ::C3DPrimitive2; private: AddTriangleList(...); public:`;
    - `#else` keeps the old public line.
  - equiv_check over all 1,443 includers (transitive, plus relative-include rows): **0 differing**
    (`evidence/equiv_e1_new.tsv`).
  - Planted control (a typedef token in the LOADER block, outside the new `#ifdef`): **9 flagged**, every LOADER user
    (`evidence/equiv_e1_control.tsv`).
  - A second control, the header without the friend, fails clang with "AddTriangleList is a private member" at both calls in 0x00A91640
    (`evidence/pre_e1_control_nofriend.log`). So the friend is what the retail access needs.
  - The header has no unpinned assert, so no assert moved.
- **TABLE_CHANGES.tsv: 26 lines.**
  - 15 delete. One is a table-only stale line (entry 33).
  - 3 replace (targets with no PDB line: entries 1, 36 and 86).
  - 8 rewitness. In each, other landed rows still bind the old form, and the new witness's HEAD object was compiled and read.
  - Two of the rewitnesses repair stale witnesses: entry 72 (onto 0x009224F0) and entry 77's UAE line (onto 0x00908B50).
- **Mid-run landings:** nf313 (06:20) and nf312 (06:25) landed while this batch ran.
  - Re-checked: every orig/ copy and the header still equal HEAD; every TABLE_CHANGES old line is still present with the expected witness;
    the delete audit re-run is unchanged apart from my own moves; the final stay-off / T3 screen of rows, targets and witnesses is clean.
  - Another agent (NF315) holds locks on 0x0092B3E0 / 0x009621F0. Neither is a row of mine.
- **Locks:** NF314 keeps the 25 changed rows plus 0x00DC0300 (the held entry-169 measurement). Every other lock this run took is released.
- **orig/:** HEAD copies of every changed row, its test and the header (85 files). All still equal HEAD at hand-back.
  orig/tools_*.nf309 are the tools as received.

## Entries (STATUS.tsv: all 88)
- done 7: 1 4 33 36 78 79 86.
- done-partial 14: 44 56 58 62 63 64 65 67 69 71 72 77 87 88.
- skipped-held 39.
  - 14 are the CPersistTraits family (19-32).
  - Most of the others: a binder or target is an NF312 / NF313 entry or witness, a T3 row, or a standing held row.
  - Entry 2: the held DepopulateLevel respell.
  - Entry 59: a CPersistTraits family row.
  - Entry 61: the CModeStandBase unit.
  - Entry 76: a vtable decision.
- skipped-port 28: 18 by census, 10 opened.

## Order of work
1. Entry 1 (the header unit).
2. The held entries re-screened against NF312 / NF313. Entries 4 / 36 / 44 / 33 were taken; their binders were T3-mentioned only for
   other targets (checked line by line).
3. The 33 open entries, opened one by one with silook.
4. The advisor's exe-based delete audit, which added 0x004FF8B0 and 0x00958000 and turned eight deletes into rewitnesses.
5. The held entry 2, measured under `evidence/held/`.

## Units (detail per entry in STATUS.tsv, per row in APPLIED.tsv)
- **Respells onto the PDB name**, each with the donor FableWin caller as the second source (tools/rcalls.py):
  - 0x00699090: Extra -> PushCurrentManualMode.
  - 0x0093A080: -> private RemoveMessageEventsAdded.
  - 0x009574D0 / 0x00958000: FinishUpdate -> Terminate.
  - 0x009C1050: -> ResizeWindowToBackBufferSize.
  - 0x009BFB30: -> private const GetClosestDepthBufferFormat.
  - 0x0056A980: DetachAll -> OnNoBoastsRemain.
  - 0x006F9640 / 0x00AE85B0: Lookup(name, long*) -> FindSymbol(name, long&) const.
  - 0x008DBB30: GetAnimateThing -> GetAnimation.
- **Pointer to reference / const** (NF309 unit-1 shape):
  - 0x00449E60: CGameJoystickManager::Get.
  - 0x004FF440 / 0x004FF8B0: CTBaseSingleton<CEngineManager>::Get.
  - 0x005B37F7 / 0x005CD767: GetAnim, which also renames GetAnimation.
  - 0x00750470: CTCAnimationComplex::GetAnimation.
  - 0x00477964: CParentDefClassBase::operator=.
  - 0x00733690: GetAnimFromName const.
  - 0x006EC4F0: GetCombatTypeDef const.
- **virtual on a local model** (NF299 Q3 / NF311 Q5-Q6). No ??_7 / ??_R appears and the vftable count is unchanged.
  - 0x007A7BF0: ~CCreatureAction_UnsheatheItemFromInventory.
  - 0x0098ABF0: ~CVertexShaderConstantLayout, on a member model with the pad shrunk by 4.
  - 0x008EC260: CCombatActionBase::OnAnimationLoaded made protected virtual. The row's own override flips QAE -> UAE, the PDB spelling
    (RENAMES section 1). A full `// Leaf:` line was added, and the payload leaf follows it.
- **Access + friend** (NF305 Q5): 0x00692B30 / 0x006655A0, CThing::GetWorld -> protected `const CWorld& PeekWorld() const`.
  - 0x00692B30 is a CCreatureActionBase member (SetMoveToPos inlined) reaching another object's protected member. Its local CThing gets
    `friend class CCreatureActionBase;`.
  - The donor has a protected CCreatureActionBase::PeekWorld wrapper that calls CThing::PeekWorld (Q5).
- **Reverted after measuring:**
  - 0x004E1437: virtual ~CTCBase emits ??_7CTCKraken and stores the vptr, DIFFER 94v88 (Q4).
  - 0x00717AA0: the re-scope renames the implicit base dtor pick-any onto a non-PDB spelling (NF307 D1).
  - 0x00740340: see entry 61 / Q3.
  - All three are kept in work/reverted/.
- **Not edited (vtable):** 0x008C52C0 emits ??_7CCreatureAction_WaspQueenDie. A virtual FinishAction on its base would change that
  table (Q4).

## Proof
- **objsec** (tools/proof2.py, exact old=new pairs): ALL_EQ for all 25 rows, every non-debug section and relocation included. There are
  no UNMAPPED symbols and the table counts match H = N (evidence/proof_e1, n1, n2, n4, n7). The held row is proved in
  evidence/held/proof_held.txt.
- **Links** (evidence/link_e1_*, b1_*, b2_*, b3_*, b4_*): the HEAD binder set plus the HEAD owner, against the new set plus the owner.
  - Nothing HEAD resolved is left unresolved, and there are no duplicates.
  - The new spellings that stay unresolved point at owners that define stand-ins (NF302 Q2; NAME_FIXES):
    - 0x0070B460 (DrawGetEnvironment);
    - 0x00666D10 (CThingCreatureBase_Methods);
    - 0x005CC091 (CAggregate_005cc091).
- **Delete audit** (`evidence/delete_audit.txt`, re-run after the mid-run landings nf312 / nf313; tools/delaudit3.py + hitctx.py),
  per the advisor. A widened name-only pass for entries 44 / 64 / 69 / 72 is in `evidence/delete_audit_wide.txt`. It turned entry 72's
  delete into a rewitness: 0x009224F0 has `typedef long EAIActionReturn` and a public AIAction_DoNothing.
  - For every delete / replace / rewitness line, every landed exe caller of the target (calls and absolute references) was grepped for
    the old declaration form, and each hit was placed in its enclosing class.
  - Rows with a same-class hit were either moved (0x004FF8B0, 0x00958000) or the line became a rewitness onto a compiled witness:
    - 007F90B0, 008CD820, 0040F7E0, 006E0730, 00732950, 00663510, 00908B50, 009224F0.
  - For the two vtable-slot lines (FinishAction / OnAnimationLoaded UAE), tools/clsmember.py scanned every landed CCombatActionBase model.
    - OnAnimationLoaded UAE: 4 public-virtual models; rewitnessed to 0x00908B50.
    - FinishAction UAE: 15 public-virtual models, but none of 3 compiled candidates binds it, so entry 76's line is left unchanged.
- **Name census** (`evidence/name_census.txt`, namecensus.py plus a manual supplement for ??1 / ??4): no landed row defines a same-named
  body with a different body.
- **Two sources** for every name: both PDB name sets (tools/pdbq.sh: egor=1 fablewin=1 for every new spelling). For name changes, also
  the donor caller, or the self Leaf or the PDB line witness.

## Prechecks
- Every compiled source and test was prechecked in an earlier call, after its lock (work/pre_*.log; tests before their locks).
- 0x006F9640: clang fails identically on HEAD and new (`field has incomplete type 'CCharString'`, line 26), so `cl /Zs` counts under NF305
  Q2. ZS_OK on both (work/zs_h006f.log, work/zs_n006f.log).
- 0x00859EF0 (owner, HEAD only): clang fails on a shared-header FABLE_STATIC_ASSERT typedef (D1); `cl /Zs` ZS_OK (work/zs_h00859.log).
- 0x005B37F7's test: clang fails only in VC7.1 `<functional>` (D1); `cl /Zs` ZS_OK (work/zs_t005b.log).

## Tools (tools/)
- New:
  - wrep.py: exact-count replacements, orig -> rows.
  - delaudit3.py: the exe-based delete audit.
  - hitctx.py: the enclosing class of each audit hit, with stay-off / T3 / lock state.
  - clsmember.py: every landed / header model of Class::member, with its access and virtual-ness.
  - grheld.sh: gr.sh reading evidence/held/.
  - lkb1.sh / lkb2.sh / lkb4.sh: the link batches.
- Modified: mkstayoff.py, retargeted to NF312 / NF313 (the original is in orig/tools_mkstayoff.py.nf309). so_check.py is unchanged; its
  orig copy is in orig/.

## Slips (own)
- I claimed the self lock 0x00A90900 for entry 1 before I knew whether I would compile it. It was compiled (HEAD, for the link) and then
  released.
- In one bash call I ran mkpay.py after a `cd /home/user/FableDecomp`. recheck.py only reads the repo, and its output went to work/rc1.
  Nothing was written into the repo; the repo include tree was checked after.
- My first delete plan rested on the table-derived tracker spellings. The advisor's exe-based audit found 20+ further binders and
  changed eight deletes into rewitnesses plus two extra moves. That is the lesson in LESSONS.md.

## Questions
- **Q1 (entry 2 / NF309 169):** the target 0x00DC0830 is now an NF312 witness: its KillAllThingsInVector line is NF312 entry 85.
  - The DepopulateLevel respell of 0x00DC0300 is measured and ready (RM H / RM N, objsec ALL_EQ).
  - Files: evidence/held/00dc0300.cpp, evidence/held/TABLE_LINES_held.tsv (a replace: the target has no PDB line).
  - Land it once NF312 releases 0x00DC0830? NF314 keeps the 0x00DC0300 lock until then.
- **Q2 (entry 76):** FinishAction UAE on 0x00859A20.
  - The line's witness 0x0085C120 binds MAE, so the line is stale.
  - 15 landed models declare the member public virtual, but none of 0x0085C0B0 / 0x00924180 / 0x008CF460 / 0x008B75E0 binds it.
  - Should the coordinator's split-gate scan rewitness or delete that line?
- **Q3 (entry 61):** a T1 family unit: the self 0x00861E00 to `CModeStandBase(CThing&)` plus its 23 PAV binders
  (evidence/e61_pav_binders.txt), each `: CModeStandBase(*thing)`. The next batch, or T3?
- **Q4 (vtable decisions):**
  - 0x004E1437: making ~CTCBase virtual gives ~CTCKraken its UAE spelling, but adds the vptr store and ??_7CTCKraken. Retail stores no
    vptr there, so the local model needs another shape.
  - 0x008C52C0: making CCombatActionBase::FinishAction virtual changes the emitted ??_7CCreatureAction_WaspQueenDie.
  - Both are handed back, not attempted further.
- **Q5 (0x00692B30):** the friend lands under NF305 Q5. Should the row instead model the donor's protected inline
  CCreatureActionBase::PeekWorld wrapper (FableWin ?PeekWorld@CCreatureActionBase@@IBEABVCWorld@@XZ, which calls CThing::PeekWorld)?
  Retail inlines it.
- **Q6 (follow-up units the audit found):** rewitnessed lines whose remaining binders follow the same respell pattern. Queue them for the
  next batch?
  - GetAnimation PBV: 0x0041E6D3 0x007306C0 0x00730DD0 0x007502D0 0x00752640 0x007F90B0; 0x00730C00 is T3.
  - CSymbolMap::Lookup: 0x0040F7E0 0x0077F650 0x006E0730; 0x009AD410 is T3.
  - CTCAnimationComplex::GetAnimation pointer: 0x00732950 0x008E0D20 0x008EA9F0.
  - GetAnimFromName pointer: 9 rows.
  - The UnsheatheItem dtor: 0x008BAC40 0x008CD820 0x008CDA30.
- **Q7 (RELOC_CONFLICT on 0x004FF8B0, pre-existing):** 0x004FF8B0 CWorldMap::RemoveLevel binds RemoveMap to 0x006403D0 (correct), while
  landed 0x004FEDE0 CWorldMap::UnloadLevel binds the same RemoveMap symbol to 0x00640430.
  - Evidence that 0x00640430 is SetMapAsUnloaded:
    - the donor UnloadLevel calls SetMapAsUnloaded, and the donor RemoveLevel calls RemoveMap;
    - in Ego_r, RemoveMap / SetMapAsLoaded / SetMapAsUnloaded sit in address order (0x005A5870 / 58A0 / 58D0);
    - the catalog names 0x00640430 SetMapAsUnloaded.
  - So the fix belongs in 0x004FEDE0, queued in NAME_FIXES.
  - If the coordinator holds 0x004FF8B0 under the RELOC_CONFLICT ruling, entry 64's delete line must become a rewitness onto
    0x004FF8B0 (its HEAD object binds the pointer form).

