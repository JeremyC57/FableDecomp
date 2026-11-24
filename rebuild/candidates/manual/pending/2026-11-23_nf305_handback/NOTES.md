# NF305 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF302, hand-back 1

Base: the repo working tree as of today (orig/ copies cmp-equal to it at hand-back). I never wrote to it and ran no git. "Entry n" = the
n-th data line of ENTRIES.tsv (file line n+1).

## Summary (STATUS.tsv, 78 verdicts)
- Reached: entries 1-60 in order, plus 206, 211, 214, 217, 218, 220, 230, 234, 237, 246, 259, 263-268 and 274 pulled ahead (E2: the
  `tools/mdiff.py` QUAL shape -- stand-in and PDB spelling differ only in the access / const letters).
- done-partial 17, done-already 1, skipped-port 27, skipped-held 33 (every touched target still has some other stand-in binder that is a
  port or off-limits, so none is a full `done`). No rejects, no vtable change, no header change.
- 32 rows changed (rows.txt), one test changed (rows/00892f40_test.cpp). All in place, zero line delta except two owner rows that gain a
  full mangled `// Leaf:` line (0x00918880, 0x006D83B0).
- Proof: HEAD and new objects RELOCATION_MATCH; objsec ALL_EQ with only the listed renames, COMDAT sets SAME, emitted tables unchanged
  (evidence/proof_b1.txt, proof_b2.txt, cmpfn_b1/b2.txt); one link of all 45 objects (owners + binders), HEAD v new
  (evidence/link_b12.txt): nothing HEAD resolved is unresolved after, the same 4 equal-copy LNK4006 duplicates in both; vland
  `--recheck-landed` 32/32 RELOCATION_MATCH + test PASS (recheck_final.log; 0x00892F40 needed its test stub made const, run 2).

## What changed (APPLIED.tsv has every row)
- Access (`protected:` / `private:` on the member's own line, `public:` after it): AIAction_DoNothing, FindFreeGazePos (owner + 4),
  GetCamTools, ExtendData, InvalidateAll (+ `friend class CEngineInternalPrimitiveBase;`, the calling class, since the PDB member is
  private), Open, UpdateAbilitiesForStat / OnStatLevelChange (owners + 0x006D8B50), StopSound, Reset.
- const: ClipSphere, IsOpen, IsQuestActive, IsAbilityPotentiallyAvailable, IsActionListEmpty.
- Return type both PDBs give (R1): GetAnimFromName -> `const CAnimationEntry&`, GetCombatTypeDef -> `const CCombatTypeDef& ... const`;
  each use drops `->` / `*`. Byte-equal (objsec).
- Owner self fixes (NF302 Q5, access only): 0x00918880 FindFreeGazePos, 0x006D8170 UpdateAbilitiesForStat, 0x006D83B0 OnStatLevelChange.
  Their HEAD objects defined the public stand-in, which their binders resolved against. LAND TOGETHER: 0x00918880 with 0x009189C0 /
  0x00918900 / 0x00918AC0 / 0x00918B50; 0x006D8170 + 0x006D83B0 with 0x006D8B50 (and the matching table deletes). Alone, either side
  would turn a resolved reference into an unresolved one.

## Census (the table under-reports binders, again)
- For every respelled target: all E8/E9 call sites in the exe (evidence/xsites_b1.tsv), then the declaration each landed caller row uses
  (tools/callcensus.py + tools/memacc.py: class body, access label in force, const; work/cc_b1.txt). 14 of the 32 rows had no table line
  and were found this way. Rows with no local declaration use a shared header; memacc.py over every such header (CAIStateGroupBase,
  CAIStateGroup_Gaze (retail branch), NCameraModes_CBase, CEngineSceneGridCell, CFadeInFadeOutBase, CQuestManager, CTCDoor,
  CTCHeroExperience, CTCInventoryAbilities, CTCPhysicalShieldSpell, CTCScriptedControl, CTCTrapBase, CThingCreatureBase) finds the PDB
  form. Two were also compiled: 0x00918A40 (CAIStateGroup_Gaze.h) binds the PDB IAE FindFreeGazePos (unresolved at HEAD, resolved by
  the owner fix) and 0x00871FD0 binds the PDB IBE GetCamTools.
- Includers by path: none of the 32 changed sources is included by another landed source or test (grep of the basenames).
- Objects confirm it: 0x009224F0 declares AIAction_DoNothing public too but its object binds the `QAEJXZ` stand-in (another line), and
  0x006D8CA0's OnStatLevelChange is `CTCHeroExperience_Retail` (objsec showed it, so that draft was withdrawn).
- New-name census (evidence/namecensus.txt): no landed source defines any of the 17 Class::Member names except the owner rows; the two
  extra source hits are other overloads (ClipSphere(const C3DVector&, float) const in 0x00B39FB0, the filter GetAnimFromName 0x00662B10).

## Table (TABLE_CHANGES.tsv, 19 lines; apply_table.py --dry on a scratch copy: 19 hits, 0 misses, no new multi-target symbol)
- 16 delete: the PDB line exists and every binder of the stand-in moved (or, for ??0CBaseObject@@QAE@XZ, the witness 0x00EFC500 is
  stale: its HEAD object binds IAE; the census of the 941 call sites by declaration finds no landed source declaring the ctor public).
- 2 replace (no PDB line yet): ClipSphere, StopSound.
- 1 rewitness: ?GetCamTools@CBase@NCameraModes@@QBEABVCTools@2@XZ from 0x00872050 to 0x008728C0, which declares it public const (source
  read); 0x008728C0 is NF304-locked, so its object was not compiled.

## Owners that still define a stand-in (NAME_FIXES.tsv; NF302 Q2: binders unresolved before and after)
0x0048DA30 IsOpen (CFadeInFadeOutBase_Methods), 0x00730C00 Open (CTCDoor_Methods), 0x00666D10 GetCombatTypeDef
(CThingCreatureBase_Methods). 0x009A01C0 ExtendData has no landed row.

## Decisions / questions
1. Entry 15 (SetPDef 0x005BDB4D): 0x00582944's only fix moves its polymorphic local CTCInventoryBase into NInventory, which also moves the
   qualified OnCreate call (0x005C0121, an NF303 witness) onto a still-wrong non-virtual NInventory form. Port, or land the move with
   OnCreate staying unresolved before and after?
2. Entry 20: 0x00ED4830 fails clang outside the toolchain headers (deleted ~CStatueMasterChest cannot override), and the namespace fix
   changes the source, so D1 / NF300's extension do not cover it. May a VC7.1 `cl /Zs` stand in for the precheck of a CHANGED source
   when clang's only error is this MSVC-accepted construct?
3. Entry 35: 0x0054F8E0 needs native `wchar_t` (`_W`, both PDBs) where the TU has `unsigned short` (VC7.1 default /Zc:wchar_t-). Is that
   a flag decision or a port?
4. Rewitness of ?GetCamTools@CBase@NCameraModes@@QBEABVCTools@2@XZ to 0x008728C0 rests on a source read only (memacc: public const in
   its local CBase): the row is NF304-locked, so I did not compile it. Accept, or leave the line for NF304 / the landing to check?
5. 0x00B8FF70 gains `friend class CEngineInternalPrimitiveBase;` in its local CEngineSceneGridCell. The private PDB member is called from
   that class in retail, so the friendship is forced by access; it is not in the PDB name sets (friends are not mangled). OK under R1?

## Rule notes
- Locks: every compiled row locked before its compile; every landed test prechecked before its lock (pre_tests1..4, tests6; 0x00EFC500's test by
  `cl /Zs`, D1, zs_tests1.log); every source prechecked after its lock and before its grade (pre_srcH1..5, pre_rows1..2, tests5 for the
  changed test). Held now: exactly the 32 changed rows. Released: all owners, 0x00918A40, 0x00871FD0, 0x00582944, 0x00EFC500, 0x0083B610, 0x0099F0A0, 0x008D6DA0,
  0x008D7730, 0x009224F0, 0x006D8CA0, and 0x00ED4830 (released without a compile).
- Background: every grade, link and vland run in the background with `timeout 900` on its launch line, checked in a later call.
- No sleep, no rm (clean.py only), no git, nothing written under the repo, no `__asm` / `naked` / `_emit`, no header edit.
- No retail or donor bytes in hand-back files (hex-run grep over every hand-back file and folder: no hits); the vland oracles stay in
  work/pay1, work/pay2. Precheck, cl /Zs and grade logs are copied to evidence/.
- Off-limits set: tools/offl.py (NF303 / NF304 entries and witnesses, live locks, unexpired codex-p28 reservations, held rows,
  T3_PORT_QUEUE); NF299 / NF300 / NF302 dropped (landed). None of my 32 rows or the compiled owners is in it.
- Reading: prefix, standing brief, GOTCHAS.md (all), CLAUDE.md, NF302's hand-back and landing README in full. Not read: ASM_RULES.md (no
  asm), EXTRA_RULES.md and COORDINATION.md beyond what the brief quotes, the nf281..nf285 landing READMEs.
- tools/: NF302's set (paths to this folder; offl.py now NF303 / NF304 only), plus mdiff.py (mangled-difference triage), memacc.py,
  callcensus.py, ctoracc.py (declaration census), hb305.py / hb305b.py (exact edits), mkpay305.py (payload with rows/ sources AND tests),
  mkstatus305.py (STATUS generator), namecensus.py (NF305 list).
