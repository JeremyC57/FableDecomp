# NF55 notes (naming-fix queue round 7, game-side tail, second half: targets < 0x00800000)

## Result (counted from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Worklist: 70 rows. 51 ported, 2 not-a-binder, 17 blocked (15 blocked/port, 2 blocked/identity), 0 deferred, 0 carry.
- Plus 7 self rows of renamed targets, ported and re-authored on shared headers: 0x0065F17B, 0x0065F8CA, 0x006691C0,
  0x006A7390, 0x0072AC40, 0x007445D0, 0x007AED10. STATUS.tsv has 77 lines (70 worklist + 7 self).
- Entries: 39. 22 done, 14 blocked, 3 rejected (e221, e195, e49; REJECTED.tsv; NF54 spells rejected entries the same way).
- recheck_final.log: one vland `--recheck-landed` run over all 58 changed rows (rows/) with FABLE_HDR_OVERLAY=hdr: 58 PASS
  (56 RELOCATION_MATCH, 2 MATCH). No row's test changed; no EH funclet includes any changed row (grep of rebuild/src).
- RENAMES.tsv section 1: 16 self rows whose leaf changed; section 2: 94 relocation-binding changes. NAME_FIXES.tsv: 26 lines
  (callers / self rows outside this worklist that still carry a spelling NF55 proved wrong). grades_used in STATUS counts every
  grade of the row, including the one-off original (pO) grades taken for section 2 of 0x006A7390 / 0x0072AC40 / 0x007445D0 /
  0x007AED10.

## Entries worked after the resume point (the earlier ones are in STATUS notes)
- e49 (0x0070C0E0): claim IsMainAnimFinished REJECTED. 0x0070C0E0 is the private const IsAnimationFinished (Ego_r 0x005752F0 out
  of line; FableWin source order GetMeshBank / IsAnimationFinished / AnimsCanBeIgnored = retail 0x0070C0D0 / 0x0070C0E0 /
  0x0070C100). Binder 0x00661720 recorded not-a-binder; the QBE/ABE access and the self row's `long` return go to NAME_FIXES, with
  engine/CAnimateThing.h's UPDATEANIMATION opt-in (its IsMainAnimFinished declaration at 0x0070C0E0) and its row 0x00665860.
- e42 (0x00725C40 Move): local one-struct CTCPhysicsControlled replaced by engine/CTCPhysicsControlled.h : CTCPhysicsStandard :
  CTCPhysicsBase (two NF55 opt-ins). All seven direct callees now carry their Ego_r spellings; Move itself is the public virtual
  override (QAE -> UAE, RENAMES s1). 1 grade.
- e104 (0x00857B70): `CCombatComponent : CTCCombat` stand-in removed, flat-branch opt-in CTCCOMBAT_FLAT_RESETCOMBO.
- e65 (0x007445D0 GetFOV): self row re-authored (was `<unattributed>::GetScaledDefValue`), `PDef->FieldOfView / 360.0f` as the
  landed 0x00745120 spells the same body; binders 0x00665190 (existing local declaration respelled) and 0x00665260 (stand-in
  removed, flat-branch opt-in CTCPERCEIVETHING_FLAT_GETFOV).
  NOTE for the coordinator: 0x00745120 CTCPerceiveThing::UpdateLineOfSight also DEFINES CTCPerceiveThing::GetFOV() const in its own
  TU (to expand it). With 0x007445D0 now defining the same strong symbol, a whole-program link would see two definitions; rows are
  linked one at a time today, so nothing fails, but the convention should be checked when rows are linked together.
- nf44:9 (0x0074FB20 vector<pair<long,CCharString>>::erase): 0x008ED170 and 0x0074E6E0 ported onto engine/CTCContainer.h /
  CTCContainerRewardHero.h + STLport vectors (ERASE_RANGE_OUTOFLINE, OVERFLOW_OUTOFLINE) + stlport/fable_algorithm.h for_each; both
  1 grade. 0x0074E6E0's own leaf QAE -> IAE (RENAMES s1). AddItemToInventory (0x008ED090, IAE CThing&) and CreateNewObject
  (0x0074E560, AAE returning CThingObject*) self rows are outside the worklist and still define the old spellings: NAME_FIXES.
- e108 (0x0072AC40 FadeOrKillEffect(CThing*)): self row re-authored as the CTCSpecialAbilities static (was CTCPhysicalShieldSpell::
  FadeOutEffect); binders 0x007BD7CA / 0x007C6DA0 / 0x007C75E0 call it through engine/CTCSpecialAbilities.h FULL + TURNCOAT.
  0x007C6DA0 StopTurncoatingEffects made private (Ego_r AAE, as engine/CTCTurncoatSpell.h and the landed FrameUpdate 0x007C6E50
  already have it); 0x007C75E0 follows; OnKill 0x007C75A0 / FinalCleanup 0x007C7560 and 0x007BE0F9 are in NAME_FIXES.
- e99 (0x00798D90 AddNewCustomer): 0x00799900 blocked/port after 4 port grades. On engine/CTCShopKeeper.h the binding is EXACT,
  but DIFFER(351v351): the shared by-value CIntelligentPointer(T*) ctor loads the address-taken `customer` before the temporary's
  base ctor (built 0x007999B2 `mov edi,[esp+0Ch]`), retail after it (0x007999BB). Variants tried: explicit temporary, implicit
  conversion, ORIGINAL_SETPTR_PASS22, FORCEINLINE_CTOR. Only the landed stand-in's `T* const&` ctor reproduces it, and both PDBs
  give the ctor by value (Ego_r 0x0051BC00 / FableWin 0x01DA3880 ...QAE@PAVCThingCreatureBase@@@Z). Attempt and its header opt-in
  (CTCSHOPKEEPER_ACQUIRE) kept in work/attempts/ (not handed back in hdr/).
- nf44:10 / e132 (CTCBoss 0x007AED10 GetBossPhase / 0x007AED50 ResetStrikesSinceHitHero): self row 0x007AED10 re-authored (was a
  free `DrawGetEnvironment`); 0x009547D0 and 0x00954E30 ported off the MazeBoss stand-in onto engine/CTCBoss.h (0x009547D0 also uses
  SETPHASESTARTPOS and FABLE_RETAIL_C3DVECTOR_PACKED for PhaseStartPos +0x22). 0x0096C4F0 (interface 0xad = CTCBoss) blocked/port:
  its local CCharString / CDefPointer / CDefinitionManager models collide with CTCBoss.h's includes (C3857).
- e31 (0x007E8590 SetPOwner@CTCOwnedEntity): 0x0072DD60 ported (static_cast of the AddTC result to the shared class);
  0x0072E130 blocked/port (2 grades; whole TU of local CThing / IP / CCharString models).
- e152: 0x007F2DC0 not-a-binder: it binds the claimed GetAllThingsInRoughArea instance exactly except the per-TU ?A0x hash
  (EXTRA_RULES: anonymous-namespace leaves match by prefix; reloc_rename.py normalises the hash). Entry done.
- nf44:126 (GetPFirstThingInRoughArea<CThingFilter_HasScriptName,CThing>): 0x007FBC90 / 0x008C2240 / 0x0095B020 ported by
  declaring the local CThingSearchTools method as the const member template (0x0095B020 also takes the shared C2DBoxI instead of
  the CRectI stand-in). 0x007FCEF0's self row is a non-const template: NAME_FIXES.
- e62 (0x006A7390): the claim (ABVCThing, const CThing&) holds: Ego_r 0x005A32E0 + FableWin 0x01F93C90, the binder itself is Ego_r
  ??RCThingFilter_IsInTargetingRangeAndFOV@?A0x58ed6966@@QBE_NABVCThing@@@Z (it passes a const CThing&), and the landed
  GetAllThingsInRoughArea callers 0x00905D10 / 0x0080A550 already bind it. Self row and binder 0x008090E0 ported onto
  engine/CThingFilter_IsInFOV.h. (FableWin also places the const CThing* overload right after the CTargetPoint one; retail has a
  single body, so the reference overload is the one the callers bind.)

- e30 / e11 (IsEventGameAction@CInputProcessGameBase QBE (EGameAction, const CInputEvent&)): 0x0068B7C0 moved the declaration onto
  its existing local CInputProcessGameBase (1 grade); 0x00687FD0 and 0x006894C0 now derive their local classes from the shared
  retail engine/CInputProcessGameBase.h (INPUTPROCESS + ZTARGET; base members and the slot-0 placeholder dropped) - 1 port grade
  each; all calls use engine/EGameAction.h names (work/tega.py). 0x00691AE0 stays blocked/port (PhotoInput stand-in namespace with
  member-pointer vtable calls: a whole-row re-author onto CInputProcessPhotojournalCapture.h / CInputEvent.h / CManager.h).
- Late fixes after review: the CInputProcessGameBase opt-in macro renamed to FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISQUICKACCESSEVENTINQUEUE
  (0x0068E450 regraded); 0x008ED170's local CThingObject replaced by engine/CThingObject.h CREATE (regraded); NAME_FIXES extended
  with the 0x00725C40 callees (0x008E5670, 0x008E5270, 0x008E57D0, 0x008E5880, 0x007254B0, 0x00725B70), CTCTalk 0x006E28A0 /
  0x006E3B30, GetBoundingInfo 0x006AF640 and the GetCurrentPhase caller 0x00954410 (found by callers.py over every section-1 target).

## Edits to pre-existing local models (not new models)
Where a row already modelled a shared-header class locally and a full port was out of budget, only a declaration was respelled to
the PDB form: CTCPerceiveThing::GetFOV in 0x00665190; CThingSearchTools::GetPFirstThingInRoughArea (member template) in 0x007FBC90 /
0x008C2240 / 0x0095B020; CTCPhysicsBase::GetBoundingInfo (const, references) in 0x006A7390; CTCTurncoatSpell::StopTurncoatingEffects
access in 0x007C6DA0 / 0x007C75E0; IsEventGameAction moved from a derived local class onto the row's existing local
CInputProcessGameBase in 0x0068B7C0. No class was renamed and no new local class of a shared-header type was added; the stand-ins
that were removed are listed per row above and in STATUS.

## Blocked rows (all with the class/model that must be ported first; details in STATUS.tsv)
0x006666A0, 0x00691AE0, 0x00694430, 0x0069AD10, 0x006C7370, 0x006D53C0, 0x00713510,
0x0072E130, 0x00799900, 0x008909F0, 0x0090FAE0, 0x0094B8B0, 0x00961880, 0x009627C0, 0x0096C4F0 (blocked/port);
0x006D1250, 0x0075D8B0 (blocked/identity: the e221/e195 BINDING CONFLICT, REJECTED.tsv).
