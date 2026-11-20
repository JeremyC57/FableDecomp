# 2026-11-20 landing nf130: the 0x0073DFF0 / 0x008E9A30 one-definition fix + PRIORITY slice 7 progress (3 rows)

Records: `naming/2026-11-20_agentNF130/` (selfcheck 0 problems). 8 entries: 3 done (q6394, q6395, q6454: their last binder 0x008E9A30, held
from nf128, lands here), 5 open, moved to NF134 when a slot frees (q6453 / q6455: 13 of 18 referencing rows on the claim; q6408, q4802, q4747).

## Rows (3): 3 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x0073DFF0 CAnimFilterOfGroup::SetCurrentGroupName: drops its local CCharString / CAnimFilterBase / CAnimFilterOfGroup / CCRC models for
  fable_string.h + engine/CAnimFilterOfGroup.h (nf128's SETCURRENTGROUPNAME_INLINE) and takes the inline body's address in an
  anonymous-namespace helper (`// Fidelity: scaffolding, not retail`, GOTCHAS nf86 / nf88), so it emits a pick-any COMDAT like every includer.
  Own leaf unchanged. 0x0129AAF4 moves from the "" literal to `?NullChar@CCharString@@1DB` (already bound there).
- Link test (wine link.exe, `LINK=` cleared, `/DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED`): the HEAD 0x0073DFF0 object + the 0x008E9A30
  object -> LNK2005 on `?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z`, LNK1169 (`logs/link_control.log`); the new
  0x0073DFF0 object + the same 0x008E9A30 object -> no LNK2005 (`logs/link_after.log`; only LNK4088 and unresolved externals).
- 0x008E9A30 NCreatureMode::CModeStrafe::OnActivate (NF128's row; module now namespaced, file moved): binds the q6394 / q6395 claims and
  the q6454 OnActivate claim.
- 0x00862EC0 CModeStandBase::AnimUpdate: off nine local models onto the shared headers; binds the nested GetWorld claim (q6453). The +0x13C
  virtual is slot 79 GetMaxWalkingSpeed (the old comment's GetMaxJoggingSpeed graded DIFFER).

## Headers (add-only opt-ins, default path token-identical; assert_names_check 0 mismatches)
CModeStandBase.h FABLE_RETAIL_CMODESTANDBASE_ANIMUPDATE; CThingCreatureBase.h FABLE_RETAIL_CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR (an inline
`PeekMovementVector()`: name from FableWin.pdb 0x01E46990 only, second source the Ego_r layout MovementVector +0x10C read in place; it emits no
retail symbol; recorded as a one-PDB inline name).

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0. 843 rows rechecked: 843 good / 0 bad. No-new-split gate 0. Table spelling sets:
0x006A9A00 / 0x0073A6F0 / 0x0073AAC0 2 -> 1, 0x0129AAF4 20 -> 19; none grew.
NAME_FIXES queued: 0x0073DFF0 is bound under stand-ins by 8 landed rows (SetGroup in 0x0073AAC0, 0x008D75D0, 0x008D87C0, 0x008D9220, 0x008D92C0,
0x008D9350, 0x008E9850; SetAndHash in 0x008D9530): an existing split, left as found. 0x008619C0 AddCombinationIdles keeps HEAD's global
spelling (both PDBs put it in an anonymous namespace another TU cannot name).
