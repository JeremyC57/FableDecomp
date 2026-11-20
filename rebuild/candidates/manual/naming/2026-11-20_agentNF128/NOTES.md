# NF128 notes (round 10, PRIORITY slice 7 third remainder + q6394 / q6395 from NF123)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries (8): 3 done (q6394, q6395, q6454), 0 done-already, 0 rejected, 0 blocked, 0 deferred, 5 open (q6453 / q6455 partial: 3 more binders;
  q6408, q4802, q4747 not started).
- Rows: 6 ported (0x0073AAC0, 0x008621A0, 0x008D6AC0, 0x008D8CE0, 0x008E9A30, 0x008EA500: all RELOCATION_MATCH + test PASS in one
  recheck_final.log run, 0 HARD / RELOC_CONFLICT), 2 not-a-binder (0x0073A680, 0x008EACB0), 0 blocked, 26 not reached (STATUS `none`).
- Header opt-ins (4, all new macros, see hdr/README.md): NCREATUREMODE_CMODESTRAFE_ONACTIVATE, CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE,
  CTHINGCREATUREBASE_GETDEFHOVERSPEEDS, NCREATUREMODE_CMODESITBASE_ANIMUPDATE.
- Locks: every changed row is in rows.txt (no extra claims). Rows read-only for the census: the 12 NF126 / NF124 rows in tools/bindchk.sh runs
  (compiled from the tree, not changed).

## q6394 / q6395 (0x008E8FF0 / 0x008E9080) and 0x008E9A30
- Claims are in the evidence column (the correct_symbol column holds the old spelling; NF123 NOTES). Both in Ego_r + FableWin:
  ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z (0x0090F4A0 / 0x02AD6AC0) and
  ?GetAnimDistanceAndIndexFromAnimName@NCreatureMode@@YI_NABVCThingCreatureBase@@ABVCCharString@@AAJAAM@Z (0x0090F530 / 0x02AD6BA0).
  engine/NCreatureMode_CModeStrafe.h (PASS24) already declares both.
- 0x008E9A30 (the only binder of both, and a q6454 binder) was ported, not respelled in place: the row's base was the stand-in
  BaseModeNative, so the q6454 callee could only move with a port. It now uses engine/NCreatureMode_CModeStrafe.h (new overlay opt-in
  ..._CMODESTRAFE_ONACTIVATE), the shared nested CModeBase (VIRTUALS), engine/CThing.h (GETTC_EXPANDED), engine/CThingPlayerCreature.h
  (MOVEMENTSPEEDS), engine/CTCAnimationComplex.h, engine/CAnimateThing.h (LOOK_HEAD_ANIM + UPDATEANIMATION, the 0x00665860 define set)
  and engine/CAnimFilterOfGroup.h (new overlay opt-in ..._SETCURRENTGROUPNAME_INLINE). The quarantine CThing include under the
  `#define CThing CThingDonor` renaming macro is gone. RELOCATION_MATCH in 2 forms: the inline call on a temporary
  (`SetCurrentGroupName(CCharString("STAND"))`) gave DIFFER(300v297) (ctor return reused as the reference, two null tests); a named local
  `CCharString group("STAND")` in its own scope matches (one shared null test, as retail).
- Self symbol: ?OnActivate@CModeStrafe@@QAEXXZ -> ?OnActivate@CModeStrafe@NCreatureMode@@UAEXXZ. It gains the namespace as well as
  QAE -> UAE; the catalog name (NCreatureMode::CModeStrafe::OnActivate) is unchanged. Both PDBs agree (Ego_r 0x0090FFC0, FableWin 0x02AD8350);
  the exe scan finds no landed referencer (virtual, vtable 0x01268588 slot 5). The build entry's Module / file name
  (CModeStrafe_OnActivate_008e9a30.cpp) may follow the namespace, like NF126's 0x008E7290 case.
- Collateral callee respells in 0x008E9A30 (each spelling already bound to its address; coll.py OK; the spelling sets shrink):
  0x006A9A00 ?InitialiseMovementSpeeds@ThingNative@@QAEXXZ -> ?InitialiseMovementSpeeds@CThingPlayerCreature@@QAEXXZ (the PDB spelling);
  0x0129AAF4 ?EmptyNarrowString@@3PADA -> ?NullChar@CCharString@@1DB (fable_string.h's NullChar; an existing 20-spelling split).
- Funclets: no row under rebuild/src/compiled/01/21 includes CModeStrafe_OnActivate_008e9a30.cpp.
- Test: the landed standalone test (unchanged) PASSes.

## q6454 (0x0073AAC0): family complete
- Last three binders ported: 0x008E9A30 (above), 0x008621A0, 0x008D8CE0. tools/bindchk.sh compiled the other 12 REFS rows from the tree: all bind
  ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ. So all 15 REFS rows end on UAE, and the self row moves QAE -> UAE in this hand-back.
- 0x0073AAC0 self row: `virtual` added in place on the row's existing local nested CModeBase model (which already had `virtual void v00()`, so no
  layout change). In-place respelling inside a local model the landed row already had; no new model. RELOCATION_MATCH in one form. Its other
  local models (CThing, CTCCarrying, CTCWeapon, CAnimFilterOfGroup with the stand-in SetGroup spelling of 0x0073DFF0) are HEAD's, unchanged.
- 0x008621A0 (CModeStandBase::OnActivate, also a q6453 binder): onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + _ACTIVATE) over the
  shared nested CModeBase. The HEAD helper stand-ins took their PDB spellings: 0x00861E60 LookupStandAnim -> NCreatureMode::
  GetRandomAnimationFromCreature(name, const CThingCreatureBase&, const CAnimFilterBase&, unsigned long*) (Ego_r 0x00816810 / FableWin 0x0283D4B0;
  the self row's own leaf; slot 4 is GetAnimFilter, the 4th argument CWorld::GetWorldSeed); 0x00861950 GetAnimSpeed -> NCreatureMode::
  GetMovementSpeedFromAnimation(long, const CThingCreatureBase&, const CMeshDataBank&) (the self leaf; the bank is CWorld::GetMeshBank, NF126's
  FLAT_GETMESHBANK). Own leaf unchanged (?OnActivate@CModeStandBase@@UAEXXZ). RELOCATION_MATCH in one form.
- 0x008D8CE0 (CModeBlock::OnActivate; NF126's blocker): ported with FABLE_RETAIL_CTHING_ORIGINAL_VTABLE (the 0x00665860 define set) so
  GetSoundPair is the real CThingCreatureBase virtual; CModeBlock keeps a row-local class (no shared header anywhere under rebuild/include; HEAD
  already defined it; its base is now the shared nested CModeBase; PCreature is CThingCreatureBase* per Ego_r). RELOCATION_MATCH in one form.

## q6453 / q6455 (0x0073A680): partial
- Ported this round: 0x008621A0 (above), 0x008EA500, 0x008D6AC0. Left (6, not reached): 0x00862EC0, 0x008D7630, 0x008DC900, 0x008DCB60,
  0x008E7B90, 0x008E7CF0 (NF126 census stands; APPLIED has the per-row stand-ins).
- 0x008EA500 (CModeHoverBase::InitialiseMovementSpeeds): the row-local CModeHoverBase now has the landed sibling 0x008EACB0's class shape (same
  members, virtuals and base; the two nested CRadialArea / CHoverState constructors are left out, unused here) over the shared nested CModeBase. Local-model rule check: no `class CModeHoverBase` anywhere under rebuild/include; the landed sibling
  0x008EACB0 uses the same shape; its base comes from engine/CTCCreatureModeManager.h. Own leaf QAE -> AAE (private in both PDBs: Ego_r
  0x00910A50, FableWin 0x02AD11D0); its only exe referencer 0x008EACB0 already binds AAE (bindchk).
- 0x008EA500 callee 0x006620E0 / 0x006620F0 / 0x00662100: the descriptive stand-ins ?GetHoverSpeed{Slow,Medium,Fast}@CThingCreatureBase@@QBEMXZ
  became the PDB spellings ?GetDefHover{Slow,Medium,Fast}Speed@CThingCreatureBase@@QBEMXZ (Ego_r 0x0053CF60/70/80, FableWin 0x01D9AF70/A0/D0:
  consecutive 16-byte bodies as retail; body = creature def +0xE0 then the hover-speed float).
  Position check: Ego_r order GetDazedTimeSeconds / GetDefHoverSlowSpeed / Medium / Fast / MakeNoise, and the catalog has 0x00662110 =
  CThingCreatureBase::MakeNoise right after 0x00662100, so the three names are not shifted. Retail reads def +0xA4 / +0xA8 / +0xAC where Ego_r
  has MaxHoverSpeedSlow / Medium / Fast at +0xA8 / +0xAC / +0xB0: the retail creature-def layout is -4 vs Ego_r there. New PDB spelling replacing a stand-in on a callee:
  each address keeps 2 spellings (the binder's and the July self row's free fastcall GetAirResistance stand-in), so the count does not grow.
  The self rows are not mine: NAME_FIXES.tsv has all three (catalog label CTCPhysicsBase::GetAirResistance is a propagated label).
- 0x008D6AC0 (CModeSitBase::AnimUpdate): onto engine/NCreatureMode_CModeSitFloor.h (new ANIMUPDATE opt-in) with the landed sibling 0x008D6580's
  define set. Two forms: the first read ParentThing late (444v446); a named `CThing& thing = ParentThing;` before SetMainAnim matches.
  FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR keeps the temporary's destructor call on 0x00693EF0's self leaf ??1CCreatureActionBase@@UAE@XZ
  (the virtual header dtor would have created the new spelling ??1CCreatureAction_PlayAnimation@@UAE@XZ). Own leaf QAE -> UAE (slot-2 override,
  Ego_r 0x00817E40 / FableWin 0x02841E30); no exe referencer.

## Split check
- tools/fin.py (pdiff HEAD vs hand-back + coll.py) on every changed row: every new callee pair is a spelling already bound to that address or the
  callee's self leaf, except the three GetDefHover* pairs above (accepted rule: PDB spelling replacing a stand-in, count unchanged).
- Self leaf changes (RENAMES s1, 4 rows): 0x008E9A30 (no exe referencer), 0x0073AAC0 (15 referencers, all on UAE), 0x008EA500 (1 referencer,
  already AAE), 0x008D6AC0 (no exe referencer). Each new leaf is in both PDBs.

## Descriptive names
None new. The three GetDefHover* spellings replace descriptive ones with PDB names.

## Flags lines
None added; every changed row grades with its build entry's CompilerFlags (none).

## Funclets
No row under rebuild/src/compiled/01/21 includes any of the six changed files (grep of each file name).

## Tests
All six landed tests unchanged (standalone models); vland PASS for all.

## Tools (tools/)
NF126's helpers repointed to this directory; fin.py gained ALLOW_NEW (targets whose new PDB spelling is accepted); new tools/bindchk.sh
(compile tree rows and print the symbol each binds at one target).

## Decisions for the coordinator
- One definition (GOTCHAS nf80 / nf86 / nf88): rows/008e9a30.cpp includes the new inline CAnimFilterOfGroup::SetCurrentGroupName
  (FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE), and its object emits the external COMDAT
  ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z (objdump), while the landed owner 0x0073DFF0 defines that symbol strongly
  (row-local model, not a header). A full link of the two would give LNK2005 (nf80). Not linked here. The fix is the nf86 / nf88 pick-any
  scaffolding on 0x0073DFF0 (owner takes the header inline body's address; `// Fidelity: scaffolding, not retail`), a row outside my locks.
  The same header's INLINE_CTOR opt-in (0x00847DF0 expanding the ctor whose out-of-line copy is 0x0073E8F0) is the precedent shape.
  Land 0x008E9A30 with that fix, or hold it.
- The 0x0073AAC0 self rename (QAE -> UAE) relies on NF126's landing (batches/2026-11-20_landing_nf126) going in whole: 12 of its 15 referencers
  were checked in the working tree, and 0x008D6760 is one NF126 asked to "confirm or hold". If any NF126 OnActivate row is held,
  rows/0073aac0.cpp waits with it.
- 0x008E9A30's symbol gains the namespace (CModeStrafe@@ -> CModeStrafe@NCreatureMode@@) and its build entry Module / file name
  (CModeStrafe_OnActivate_008e9a30.cpp) may follow, as NF126's 0x008E7290.
- 0x006620E0 / 0x006620F0 / 0x00662100: respell the July self rows to CThingCreatureBase::GetDefHover{Slow,Medium,Fast}Speed (NAME_FIXES) to close
  the existing 2-spelling split.
- 0x0073DFF0: 0x0073AAC0 (unchanged local model) still binds the stand-in ?SetGroup@CAnimFilterOfGroup@@...; the self leaf is
  ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z (an existing split; vland RELOC_NAME warning).

## Rule slips
- docs/pipeline/GOTCHAS.md (134 KB), EXTRA_RULES.md and ASM_RULES.md were not read in full; I read the brief's required NF92 / NF44 / NF88-90
  files and grepped GOTCHAS for the one-definition lines (nf80 / nf86 / nf88) at hand-back.
- fin.py was once run with a placeholder NOTE (`x`) for 0x008D6AC0; the line in rows/008d6ac0.cpp was replaced by the real Naming comment
  before the vland runs (no grade depended on it).
- Funclet greps that found nothing ended a Bash call; all calls still exited 0.
