# NF126 notes (round 10, PRIORITY slice 7 second remainder: the 6 entries NF120 / NF124 left open)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries: 0 done, 0 done-already, 0 rejected, 0 blocked, 0 deferred, 6 open (q6453 / q6455 / q6454 / q4802 partial; q6408 / q4747 not started).
- Rows: 19 ported (all RELOCATION_MATCH + test PASS in one recheck_final.log run), 3 not-a-binder (0x0073A680, 0x0075B8D0, 0x008EACB0),
  3 blocked (0x00B21DD0, 0x00B21E90, 0x008D8CE0), 28 not reached (STATUS `none`, 0 grades).
- Claims: the three claim symbols this round acted on are in both PDB name sets (egor / fablewin_pdb_names.tsv): nested GetWorld QBE (Ego_r
  0x00751030 / FableWin 0x024C31E0), nested OnActivate UAE (0x00751510 / 0x024C17D0), vector<C3DVector>::operator= (0x006E01E0 / 0x02056BD0).
  q6408 / q4747 rely on NF120 / NF124's verification (not re-checked).
- Locks claimed this round beyond rows.txt: 00862ec0 008621a0 008d6ac0 008dc600 008d9480 008d89c0 008d6760 008e7290 008de6a0 008dbb30 008d9e30
  008da2e0 008e7390 008dc6c0 008ddd70 00b21090 00b21160 00b21dd0 00b21e90 00b22100 00b22310 00b22530 00bcd850 00bcd8f0 00bcd950 (all free).

## Split check (exe scan, nftools/refs.py via tools/xr.sh; coll.py on every changed pair)
- Every new callee pair is a spelling already bound to that address in reloc_bindings.tsv or the address's self leaf (coll.py `OK`). No new
  spelling is introduced; the old spellings each row dropped are listed in RENAMES section 2.
- Self leaf changes (RENAMES section 1, 11 rows): each new leaf is in both PDBs; the exe scan finds no landed referencer, except
  0x008DBB30 (referenced by 0x008DC6C0 only): both are in this hand-back (a 2-row family), and 0x008DC6C0 now binds 0x008DBB30's new UAE leaf.
- Leaf-only self renames of rows outside my entries' self addresses (coordinator rule from NF124): 0x008DC600, 0x008D9480, 0x008D89C0, 0x008DE6A0,
  0x008D9E30, 0x008DBB30, 0x008DC6C0, 0x008DA2E0, 0x008E7390 (all QAE -> UAE, PDB virtual overrides). 0x008E7290 and 0x008D6760 change symbol
  more than the leaf: 0x008E7290 was the July global ?OnActivate@CModeDragonBase@@QAEXXZ (now NCreatureMode::, UAE, on engine/CModeDragonBase.h);
  0x008D6760 was the extern "C" forwarder _CModeExplosiveForcePushed_OnActivate (now ?OnActivate@CModeExplosiveForcePushed@NCreatureMode@@UAEXXZ).
  The catalog names (module / name) of 10 are unchanged. 0x008E7290: build_candidates.ps1 Module is `CModeDragonBase` (file
  CModeDragonBase_OnActivate_008e7290.cpp) while the oracle name is NCreatureMode::CModeDragonBase::OnActivate; RENAMES s1 gives module
  NCreatureMode::CModeDragonBase, so the coordinator may rename the build entry module (and the file, if the naming convention follows the module).

## q6454 (0x0073AAC0)
- Transformer tools/mb.py (proved by hand on 0x008DC600, then applied): delete the row's local `struct CModeBase`, include
  engine/CTCCreatureModeManager.h with FABLE_RETAIL_CMODEBASE_VIRTUALS (plus engine/ETCInterfaceType.h in place of a local empty enum), derive
  from CTCCreatureModeManager::CModeBase, ParentThing `->` becomes `.` (CThing&), the derived slot functions declared `virtual`.
- 10 binders ported (STATUS). 0x008DA2E0's local CModeStandBase now binds ?OnActivate@CModeStandBase@@UAEXXZ (0x008621A0; already bound).
- Left: 0x008D8CE0 blocked (STATUS); 0x008621A0 (its own leaf has 8 bound spellings; local CThingCreatureBase not derived from CThing) and
  0x008E9A30 (CThingDonor quarantine-CThing include under a renaming macro, BaseModeNative / ThingNative stand-ins; engine/NCreatureMode_CModeStrafe.h
  PASS24 has only AnimUpdate: needs OnActivate / FrameUpdate opt-ins) not reached.

## q6453 / q6455 (0x0073A680)
- 0x008DDD70: its local NCreatureMode::CModeBase (member m_pThing) replaced by the shared nested CModeBase; its local CModeStandBase moved out of
  namespace NCreatureMode (the real class is global, RTTI .?AVCModeStandBase@@), so 0x008621A0 binds ?OnActivate@CModeStandBase@@UAEXXZ.
- 0x008E9850: the stand-in CGlobalComponents (GetAnimBank, +0x3c) is the world: engine/CWorld.h + overlay opt-in FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK
  (3 grades: SRC ok/DIFFER 249v251 with the direct member, match with the inline getter).
- Census of the 9 left (no grade spent):
  - 0x008E7B90, 0x008E7CF0, 0x008D7630: ThingNative / ModeBriar / ModeIceTroll / WorldNative stand-ins and, in two of them, the quarantine CThing
    included under a renaming macro. Port onto CThing.h / CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE) / CWorld.h first.
  - 0x00862EC0: GetGlobalComponents()->GetGameState() is a virtual slot 4 (+0x10) of the world object; no PDB CWorld member of that name. Needs
    the CWorld slot identified (retail polymorphic CWorld) before the respell.
  - 0x008DC900: GetDisplayEngine()->GetWorldA/B stand-in on the same address.
  - 0x008DCB60: CModeJackOfBladesFlash modelled with a 0x68 pad and no CModeBase base, next to a local `namespace std { struct pair }` and a local
    CVectorMap specialisation, which clash with the STLport header that CTCCreatureModeManager.h pulls in. CVectorMap / std::pair must move first.
  - 0x008621A0, 0x008D6AC0, 0x008EA500: local CThingCreatureBase not derived from CThing (ParentThing is used as CThingCreatureBase*). Move
    CThingCreatureBase (engine/CThingCreatureBase.h) first; 0x008EA500 also needs its slot-12 GetAnimPrefix declared on CModeHoverBase.

## q4802 (0x0075B8D0)
- 0x00B20FD0 (NF124's blocked draft): the exact 0x00B211B0 recipe form (`long prevCount = Prev...; if (This... != prevCount)`) gave the same
  DIFFER(177v177) (`cmp ebx,eax`); the mirrored compare `if (prevCount != ThisSpline.GetControlPointCount())` matches.
- Transformer tools/cs.py (local CArray<T> + CCardinalSpline -> engine/CCardinalSpline.h with the 0x00B211B0 recipe defines) for 0x00B21160,
  0x00B21090 (CONTROLPOINT opt-in), 0x00B22310 (Get/SetTension, SetControlPoints), 0x00B22530, 0x00B22100 (APPENDCONTROLPOINT opt-in for
  DeleteAllControlPoints). In passing 0x00B22310 / 0x00B22530 / 0x00B22100 now bind the self-leaf STLport spellings of 0x006F0810 (vector copy),
  0x0045BC09 (vector<unsigned long>::operator=) and 0x0075C040 (vector<C3DVector>::resize) instead of local ??0/??4/?resize@?$CArray spellings.
- Blocked: 0x00B21DD0 (copy-construct by zero + assign), 0x00B21E90 (STLport erase's __copy expanded inline in retail). Both need a header
  decision (STATUS). Drafts in work/.
- Not reached: 0x00A6CB20 (CPhysicsMesh, local CArray), 0x00762E10 (CPointVector), 0x00B20B60 (CKeyVector; vland RELOC_NAME shows 0x00B20B60
  bound by 0x00B22310 as ??4CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@ while the catalog says CSplineData_00b20b60::CopyFrom: a
  candidate catalog fix, one source only, not in NAME_FIXES), 0x00764E90 / 0x00BCD850 / 0x00BCD8F0 / 0x00BCD950 (CElement12 STL algorithms).

## q6408 / q4747
Not started; NF120's / NF124's census stands.

## Descriptive names
None new.

## Flags lines
None added. Every ported row grades with its build entry's CompilerFlags (none).

## Funclets
No EH funclet row includes a changed row (grep of rebuild/src/compiled/01/21/*Unwind* for the 19 file names: none). The only possible file move is 0x008E7290
(module gains NCreatureMode::, above); it has no funclets.

## Tests
Unchanged: every ported row's test is a standalone model (vland PASS for all 19).

## Tools (tools/)
mb.py (CModeBase transformer), cs.py (CCardinalSpline transformer), fin.py (pdiff + coll + Leaf fix + RENAMES + STATUS; refuses on a new
spelling or an unpaired self-leaf change), xr.sh (exe-scan referencers), pdb.sh, defs.sh, pre.sh (cpp precheck, wired into g.sh).

## Final checks
- recheck_final.log: one vland --recheck-landed run over the 19 rows/ rows: 19 RELOCATION_MATCH, every test PASS; 0 HARD / RELOC_CONFLICT.
- WARNs: SHARED_HEADER_AVAILABLE on local models the landed rows already had (kept in place); 6 RELOC_NAME on callee bindings the rows had at
  HEAD (pdiff shows none of them changed).
- selfcheck.log: 0 problems.

## Rule slips
- The first 0x008DC600 lock was taken with `mkdir -p` (would not have refused an existing lock). The directory's mtime shows it was created by
  that call (free); every later claim used claim.sh (plain mkdir).
- Seven grades were SRC_FAIL from gaps in my own transformers (0x008DBB30 / 0x008E7390 pointer arguments, 0x008DA2E0 downcast, 0x008DC6C0
  declspec base, 0x008E7290 July model, 0x00B22310 dropped GFInterpolateArray declaration, 0x00B22100 dropped C3DVector include); STATUS
  grades_used comes from work/grades.log and counts them.
- 0x00B21DD0 / 0x00B21E90 / 0x008D8CE0 are marked blocked after 1 form each (not 3): each failure is structural (a shared-header construct no
  row spelling reaches), stated in STATUS.

## Decisions for the coordinator
- 0x008D6760: the row now defines a local NCreatureMode::CModeExplosiveForcePushed (no shared header; the landed siblings 0x008D6820 /
  0x008D6780 model it the same way; RTTI: CModeExplosiveForcePushed : CTCCreatureModeManager::CModeBase) where HEAD had no class at all (an
  extern "C" forwarder). selfcheck passes (no shared header for the class); the brief's "base-list local models: never add one" is not defined in
  the nf120 / nf121 landing READMEs. Please confirm or hold this row.
- Two header decisions block q4802 rows: (1) a CArray / vector copy construction that zeroes and assigns (0x00B21DD0); (2) an inline STLport
  erase / __copy for CCardinalSpline::DeleteAllControlPoints (0x00B21E90; retail expands it at 0x00B220B6).
- 0x00B20B60: one-source candidate catalog fix (bound by 0x00B22310 as ??4CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@...; catalog
  CSplineData_00b20b60::CopyFrom). Not in NAME_FIXES.
