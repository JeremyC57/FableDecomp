# 2026-11-17 landing nf62: name-fix round 7, NF62 (30 rows: game-side ports of blocked rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF62 took the 49 rows below 0x00800000 that earlier round-7 agents had left `blocked/port`: rows whose wrong binding comes from a
local model of a class that has a shared header. It had a 15-grade budget per row. It ported 30. Of the 19 left, 3 had real attempts
and 16 got one scripted header probe each; the 18 that are not the coordinator's went to NF66. Brief, list and records:
`naming/2026-11-17_agentNF62/`.

## Rows (49; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 30 |
| blocked | 19: 3 after real attempts, 14 scripted header probes, 2 NF51 drafts regraded |

**Ported:** 0x004CA010, 0x004E9550, 0x004E9642, 0x004FE2A0, 0x00500230, 0x00506E30, 0x00523860, the CTCInventory CloseInventory
group (0x00587847, 0x00589D30, 0x0059E2C7, 0x0059F304, 0x005FCE60), 0x0063A540, 0x00691AE0, 0x0069AD10, 0x006A5950, 0x006A9590,
0x006C3620, 0x006C7370, 0x006DCB20, 0x00713510, the CMeshMorphSet pair 0x00722270 / 0x007222B0, 0x0072E130, 0x0073A8E0, 0x00747330,
0x00799900, 0x007CDFE0, 0x007CEB00 and 0x007E7C90.
- Among the self renames: `CTCInventoryMap::CloseInventory` 0x005FCE60 (was `vfunc_31`),
  `NTCScriptedControl::CActionPerformExpression::Update` 0x00713510 (was `vfunc_1`), and `_M_insert` of
  `map<long, CComboMultiplierData>` 0x006DCB20 (was `CUIBinaryTree::Insert`). NF58 had left that row
  blocked at DIFFER 227 v 198.

**Blocked after real attempts:**
- 0x004FE630 (8 grades, DIFFER 461 v 440): retail keeps the name's string data in a register across the out-of-line `operator==`
  calls.
- 0x006D41F0 (4 grades, DIFFER 307 v 295): needs an inline split of `CBasicString<char>::operator==` (coordinator: fable_string.h).
- 0x0075A530 (7 grades, DIFFER 610 v 611): every binding matches. One byte is off because the bounding-box X pair is added
  Max-first, where retail adds it Min-first.

## Entries (APPLIED.tsv, 46): 21 done, 25 blocked
- **Done:** e67, nf41:e11 / e30 / e31 / e33 / e99 / e174 / e200, nf44:5, nf44:57, the 0x006DCB20 entry, and q10405, q10432, q10433,
  q10454, q10455, q10457, q10458, q10464, q10465, q10466.
- **Blocked:** 23 have binders left in this slice; 2 only have binders outside it (for example nf41:e106 IsPerformingAnyAction:
  0x00961880 / 0x009627C0, with NF67).

## Rows and checks
- **Section 1: 17 self renames (8 file moves). Section 2: 119 respelled bindings.**
- **Coordinator correction:** 0x006DCB20's module was abbreviated (`std::_Rb_tree<long,std::pair<long const,CComboMultiplierData>,...>`).
  It now uses the instance's catalog spelling from its sibling row 0x006DCDD0 (`insert_unique`).
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 30 changed rows graded from the repo after landing: 30 RELOCATION_MATCH, every test
  PASS. NF62's own final run agrees.
- **Checks:** `naming/rebind.py` removed 103 binding lines and added 55; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (46 opt-ins in 40 files; lines added only, default path unchanged)
Details: `naming/2026-11-17_agentNF62/hdr/README.md`.
- The first opt-in in a root header: `fable_resource_directories.h` (`FABLE_RETAIL_NRESOURCEDIRECTORYNAMES_BONECONFIGDIR`, the out-of-line
  `NResourceDirectoryNames::GetBoneConfigDir` 0x0041A290). It is a five-line `#ifdef` block that adds a declaration only.
- `CTCPhysicsRigidBody.h` `SETPHYSICSMESHBANKINDEX` serves only the blocked 0x0075A530 draft.
- `CCreatureAction_PerformExpression` `INLINE_DTOR` removes the declared destructor under the macro, so it becomes implicit. The
  macro name is misleading (queued: rename and regrade).

## Coordinator decisions requested
- 0x004FE2A0 stays QAE: engine/CThingManager.h declares the member public; Ego_r.pdb says protected (IAE).
- 0x00691AE0 reads the raw `UInputEventData` union.
- 0x007E7C90 types Handles as `CArray`; Ego_r.pdb has a vector.
- 0x004F97B3: map copy constructor vs tree copy constructor naming.
- 0x00597006 / 0x00596CCE need NF51's CFrontEndManager.h opt-ins, then a ruling on whether a row may see same-TU callee bodies.
- 0x006C7370: the PDBs name all 40 vector transfers `TransferVector`, none `Transfer`.

## Name fixes queued (`land1117fu`, 17)
From NF62's NAME_FIXES.tsv, among them 0x0069A8B0 PushCombatCameraMode (identified from the body), 0x006994F0
IsObservedThingInMelee (ABE), the `__copy` / `TransferVector` / `_M_find` instances 0x006C9B70 / 0x006C9EE0 / 0x006C31B0 and
0x0072DD60 AddThingToOwnerTC.

## Rule slips (NF62 `slips.txt`)
- A relative log path briefly wrote into the repo's ignored `work/` directory. NF62 moved it out; a leftover `work/pay_fix.json` was
  moved to the coordinator's scratch directory at landing. Nothing was committed.
- 0x006A9590's lock was claimed late; two Bash calls exited 1; one `git log` was denied by the permission system.
- A double-quoted note argument ran its backtick text as commands (not found, no effect); the note was rewritten.
