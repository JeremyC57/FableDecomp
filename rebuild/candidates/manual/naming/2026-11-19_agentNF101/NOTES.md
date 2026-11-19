# NF101 notes (round 9, NF98 leftovers: q11260 .. q11286)

ENTRIES.tsv holds 8 entries (the brief says 11); the counts below come from the file.

## Counts (from STATUS.tsv / APPLIED.tsv / grades.log)
- Entries 8: done 4 (q11260, q11274, q11277, q11286), done-already 1 (q11271), blocked 3 (q11280, q11281, q11283: port),
  rejected 0, deferred 0, open 0.
- Rows 16 in STATUS.tsv: ported 9 (0078b070, 00848500, 008c64f0, 0055fc90, 00b9dc60, 00518130, 005185f0, 0063fae0, 00670d50),
  not-a-binder 6 (00703210, 005da750, 009f9e00, 00beb660, 0049c6f0, 00436500), blocked 1 (0078954d).
- Header opt-ins 4 (hdr/README.md): CTCWEAPON_GETMISSILELOADCARRYSLOT, CCREATUREACTION_HEROLOADRANGEDWEAPON_CLASS,
  CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD, CTHINGCREATUREDEF_VALIDATE.
- Grades 24 (grades.log): 0078b070 8, 008c64f0 3, 00b9dc60 2, 00518130 2, 005185f0 2, 0078954d 2, 00670d50 2, 00848500 1,
  0055fc90 1, 0063fae0 1. No row over 15; the CThingSearchTools family used 7.
- recheck_final.log: all 9 ported rows RELOCATION_MATCH + PASS (WINS: 9). Warnings: RELOC_NAME on 0x005DA920 (the stand-in
  self label, NAME_FIXES.tsv) and on two 0x0055FC90 callees; SHARED_HEADER_AVAILABLE on 0x0055FC90's pre-existing local models
  (not new: the row was respelled in place, two lines; porting its whole local model is outside q11274).

## Descriptive names
- `GetAllTCDsInRoughArea` body in hdr/engine/CThingSearchTools.h (opt-in TCDSINROUGHAREA_EGOD): the local names (`loop_thing`,
  `pmapwho`, the cell / map loop locals) are descriptive, copied from the shared TCs EGOD body it mirrors; no PDB locals for this instance.
- 0x0078B070: local `object_def` (descriptive).
- 0x008C64F0: `start_handedness` local inside the `// Inferred:` scope (descriptive).
- No made-up function, global, class or member names were added. Member names Cupboard / ItemDefIndex / CupboardAnimIndex
  (0x00848500) and PRangedWeapon / PAmmo (0x0078B070) are Ego_r.pdb spellings.

## Entries
- q11260 (0x00703210 ?Create@CThingObject@@SIPAV1@...): done. The self row already defines it (not-a-binder). Binders 0x0078B070,
  0x00848500 and 0x008C64F0 ported onto shared headers (each RELOCATION_MATCH PASS; RENAMES s1 has the two QAE -> UAE leaf respells).
  0x0078B070 also binds 0x005DA920 by its PDB spelling ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ through the new CTCWeapon opt-in;
  the self row 0x005DA920 and binder 0x00789FF0 still spell stand-ins (NAME_FIXES.tsv).
- q11271 (0x005DA750): done-already. Self row defines the QBE spelling; its four landed callers bind it (cs.py).
- q11274 (0x009F9E00 GetSurfaceLevel): done. 0x0055FC90 respelled in place (declaration + call site).
- q11277 (0x00BEB660 GetMaxDrawDistance): done. 0x00B9DC60 ported onto the retail CEngineInternalPrimitiveStaticMesh chain
  (self leaf now ?IsInsideFrustum@CEngineInternalPrimitiveStaticMesh@@UAE_NABVCEngineCamera@@_N@Z).
- q11280 / q11283 (0x0049C6F0 GetAllThingMapsInArea): blocked: port. The 3 WORKLIST binders (0x00518130, 0x005185F0, 0x0063FAE0) are
  ported as explicit instantiations of the shared CThingSearchTools bodies. cs.py + a declaration grep leave 57 rows that still declare
  a stand-in for 0x0049C6F0 (work/cs_0049c6f0_rows_left.txt, includes 0x0078954D): beyond the 40-grade family budget.
- q11281 (0x00436500 GetMapwhoArea): blocked: port. Rows left 0x0078954D (DIFFER 640v354, STATUS.tsv has the exact out-of-line
  callees; draft work/0078954d_blocked.cpp) and 0x0078971D (not graded this round). Needs first: a CThingSearchTools.h body
  variant with GetMapwhoArea / GetCell / AreaMaps erase / CIntelligentPointer ctor / push_back out of line, and the PDB spellings of
  0x00436830 / 0x00436A80 / 0x004AD520 settled.
- q11286 (CValidateDefinitionInit::World): done. 0x00670D50 ported with the 0x00680050 recipe (validation_class.World,
  world.PeekMeshBank()), replacing the local Resources / MeshBank model.

## Needs header claim
- CThingSearchTools.h TCSINROUGHAREA_BODY copy (used by landed 0x0050A650) spells the search statics and LayerInfo with stand-in names
  (SearchCount_013b89a8 / SearchWork_013b89ac / SearchID_013b89b0 / LayerInfo_0137dad8); the EGOD bodies already use the PDB names
  (NAME_FIXES.tsv line 2). Unblocks one-name-per-address for 0x013B89A8 / 0x013B89AC / 0x013B89B0 / 0x0137DAD8.
- 0x005DA920 self row (CTCWeapon::GetWeaponDefLongAt94) and 0x00789FF0 (GetAmmoCarrySlot) respell to ?GetMissileLoadCarrySlot@CTCWeapon@@QBEJXZ
  (NAME_FIXES.tsv line 1).

## Funclets
- No EH funclet in rebuild/src/compiled includes or names any of the 9 changed rows (grep of the row addresses); nothing to regrade.
  Self renames (0x0078B070, 0x00848500, 0x00B9DC60, 0x00518130, 0x005185F0) move only their own files.

## Rule slips
- Two early orientation Bash calls exited non-zero (a `wc` over a mistyped LESSONS path, and `rdis.py 0x30` with a hex length).
- Several early orientation calls (reads only) skipped the mandated `export S...; source env.sh; cd` prefix.
- g.py passed a stray `none` flag on the first grade of 0x0078B070 (tool bug, fixed in tools/g.py); cost one grade.
