# NF66 notes (round 7: NF62's 18 unfinished game-side ports)

Conventions
- grades_used in STATUS.tsv counts every q.py compile of the row by NF66 (work/grades.log), including the one baseline regrade of
  the landed file for the binding table (work/base_<addr>.xref). Earlier agents' grades are named in the note, not added.
- Ported rows: rows/<addr>.cpp (tests unchanged: all five tests are standalone behaviour tests). recheck_final.log is one vland
  --recheck-landed run over all five (5 RELOCATION_MATCH + PASS). No EH funclet under rebuild/src/compiled/01/21 includes any of them.
- RENAMES.tsv section 2 is generated from base_<addr>.xref vs last_<addr>.xref (every relocation whose symbol changed).

Results (rows.txt: 18 rows)
- Ported (6): 0x0053AD07, 0x0053822B, 0x0053B321 (the CList group, one define block from the landed same-TU rows 0x005379F5 /
  0x0053A0A1 / 0x0053BEAC), 0x006666A0, 0x004B11A0, 0x00694430. Plus one EXTRA binder row outside rows.txt: 0x0053BF20 (found by
  callers.py while verifying q10462 / q10472; unlocked; ported with the 0x0053AD07 recipe).
- Blocked after real attempts (4): 0x0075A530 (12 grades, blocked/header), 0x004FE630 (7, blocked/port), 0x00596CCE / 0x00597006
  (2 each, blocked/port on the prerequisite port of 0x005955AB, outside this slice).
- Blocked, not attempted beyond one probe (8): 0x004091C0, 0x00416953, 0x0041B550, 0x00549B20, 0x0057770A, 0x005778CA, 0x00599E3F,
  0x006D53C0. STATUS names the remaining gaps (for 0x004091C0 the full list of members / opt-ins the port needs).
- Entries (22): done 5 (q10445, q10462, q10472, nf41:e81, nf41:e47; each verified with callers.py on the target and a probe or
  source check of every other landed binder), blocked 17.
- recheck_final.log: one vland --recheck-landed run over the 7 changed rows: 7 RELOCATION_MATCH + PASS; 4 RELOC_NAME warnings, all
  about labels of other addresses (0x0053C91C, 0x0042ABCA, 0x00693250, 0x00662450: NAME_FIXES.tsv / below).

Coordinator decisions / header changes proposed
- 0x0053BF20 (extra binder row, not in rows.txt): ported with the 0x0053AD07 recipe; it also binds 0x0053CFC2 under the q10461
  spelling (NF56's entry, outside this slice: q10461 may now be done). CListArrow cast in 0x0053B321: Ego_r.pdb
  ??$GFDebugPDynamicCast@PAVCComponent@NUISystem@@PAVCListArrow@2@@@ (the component-to-arrow cast exists) and the CListArrow vtable
  slot 150 SetMaxToDraw (landed 0x0054C330) are the two sources.
- Locks held by NF66 at hand-back: all 18 rows of rows.txt plus 0x0053BF20 (the coordinator may release the blocked ones).
- 0x0075A530: retail's bounding-box X pair (Min first, 0x0075A588) needs one extra by-value C3DVector return between
  GetBoundingBox().GetCenter() and the local (proved by work/attempts/0075a530_helper_probe.cpp: a file-local by-value helper gives
  Min-first, 609v611). No PDB member provides it (only CPhysicsMeshObject::GetBoundingBoxCentre and C3DBoundingBox::GetCenter). Proposal:
  allow a descriptive inline `C3DVector CPhysicsMesh::GetBoundingBoxCentre() const { return GetBoundingBox().GetCenter(); }` under an
  opt-in FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXCENTRE_BYVALUE (the landed RM row uses exactly that wrapper locally), then regrade with
  the landed row's Transform shape.
- 0x00596CCE / 0x00597006: retail compiles CFrontEndManager::GetProfileNames 0x005955AB (and ~vector<CWideString>, the
  CCountedPointer<CList> bodies) in this TU; the landed rows fake it with a stand-in body over a made-up global. Proposal: port
  0x005955AB onto the shared headers, then expose its body as a visible noinline opt-in in CFrontEndManager.h
  (e.g. FABLE_RETAIL_CFRONTENDMANAGER_GETPROFILENAMES_BODY) for these rows.
- 0x004FE630: the text test is solved (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE + FABLE_RETAIL_CBASICSTRING_COMPARE_BODY keep
  PStringData in esi across both out-of-line operator== calls). This may also help the coordinator's 0x006D41F0 (same callee).
  What is left is register allocation only (retail: i in ebp, `sections` reloaded from its slot; ours the reverse).
- 0x0042ABCA: the shared NUISystem_CDeletion.h / STLport list gives `??1?$_List_base@PAVCComponent@NUISystem@@..@@QAE@XZ` for the
  CDeletion list destructor, while the landed self row 0x0042ABCA is `??1?$list@PAVCComponent@NUISystem@@..@@QAE@XZ` (vland
  RELOC_NAME warning on 0x0053B321). Which spelling is right is not proven here; flagged, not in NAME_FIXES.
- 0x006666A0 self rename (QAE_NXZ -> UAE_NXZ): its two landed base callers 0x006AD9D0 (CThingPlayerCreature::WorldUpdate) and
  0x00834760 (CThingAICreature::WorldUpdate) declare it on local models; they are outside this slice (NAME_FIXES.tsv).

Descriptive names
- 0x00662450 SetPItemToUnsheatheAfterCutscene (no PDB / FSE / Ego_d name) and the anonymous-namespace GetPActionAnimSet 0x00693250
  (Ego_r name, TU-specific hash): NAME_FIXES.tsv.
- CTCCreatureHitNotification has no shared header: 0x006666A0 keeps a one-member local model (Ego_r ?UpdateHitNotification...).
- NAME_FIXES 0x0053C91C / 0x0053C8F8 are STLport find forwarders named descriptive-by-library (no PDB has STLport); evidence is the
  call shape at the ported callers.

Rule slips
- 0x004FE630 grade 3 and 0x0053822B grade 2, 0x0053B321 grade 2, 0x006666A0 grade 2, 0x004B11A0 grade 2, 0x00694430 grade 3 were SRC_FAIL compiles
  (missing typedef / declaration / define); counted in grades_used.
- 0x004B11A0 grade 3 was RELOCATION_MATCH but bound the non-const CVectorMap LowerBound spelling at the const 0x004365B0; caught by
  the base/last binding diff and fixed in grade 4 (const PeekTC). No other slip found.
- One Bash call exited 1 (an early `diff` of two NF62 drafts without a trailing `; true`); no file was affected.
- Budget: the slice brief allows 15 grades per row; the rows marked 'not attempted beyond one probe' got only NF62's probe regraded
  under this overlay. They are blocked with that reason, not with a claim that the port is impossible.
