# NF170 notes (round 10; partial hand-back)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 27 entries: 11 done (q4267 q4268 q4278 q4632 q11742 q11755 q11757 q11759
q11782 q11813 q11814; q4632/q11757 one unit), 5 done-already (q4435 q4483 q4484 q5014 q5217), 1 rejected (q11675), 3 deferred
(q4978 q5012 q5013: 0x00807B70 held by NF167), 7 open with census (q4983, q6368 q6493 q6504 q6509 q6510, q11718).
Rows: 22 ported (recheck_final.log: 22 RELOCATION_MATCH + PASS, one vland run), 10 not-a-binder, 0 blocked, 58 locked rows open (not reached).
selfcheck.log: 0 problems.

## What changed
- GetSubDef<T> binders (q4267 / q4268 / q4278): 12 binders moved off local def / CDefPointer / CDefClassBase stand-ins (with fake same-TU
  GetSubDef bodies) onto engine/CDefClassBase_GetSubDef.h (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE) + the def's header; xref diff:
  only the GetSubDef pair changed in each. The three owners 0x006F0F60 / 0x006696B0 / 0x005ED4A0 defined strong explicit specializations
  (LNK2005 against every header includer, the nf135 case): moved to the shared body + nf86/nf88 anonymous-namespace address helper.
  Link proofs with LINK= cleared (link/): owner_new006ea2c0.log (HEAD owner + new binder: LNK2005), new_006f0f60_006ea2c0.log (exit 0),
  q4278/link.log (new owner + 9 binders: exit 0), ctl_owner_007355f0.log (HEAD owner + HEAD 0x007355F0: LNK2005, pre-existing).
- Transfer<T> self rows (q4632/q11757, q11755, q11759): the descriptive local enums EHeroSoulsPosition / EPostSavePosition / EKNAttackType
  replaced by the headers that define the PDB enums (engine/CQ_SunnyvaleMasterData.h EPlaceInHeroSouls / EGameflowPosition;
  engine/CFeatDef.h EFeatAttackType); leaf, TransferBinaryIn callee and TransferableComponent vtable respell with T (exe scan: only the self
  row references those callees). All their COMDATs are pick-any. Leaf lines now carry the full mangled name (were bare `Transfer`).
- Search-tool template owners (q11742 / q11782 / q11813 / q11814): re-authored on engine/CThingSearchTools.h template bodies (ALLTHINGSBODY_EGOD;
  BESTTHING_BODY + _NULLTEST + CTHING_SCANINCLUSION_ISALIVE; LEAFBODY + _CASTARG + ISALIVE) with pick-any scaffolding helpers. Callee
  respellings land on spellings already bound (PeekClassList / PeekTypeList IBE, STLport vector _M_insert_overflow).
- Self renames (RENAMES s1, 7 rows) move parent files; no landed EH funclet includes any of the 22 changed parents (grep of rebuild/src/compiled).

## Descriptive names
- No new made-up names. 0x005EA400 now reads the def's +0x10C as Ego_r CTavernGameDef::MoneyBagGraphic (was the descriptive ThirdBoxWidth).
- The local filter classes CThingFilter_ObjectHasProperty / CThingFilter_HasFreeActionPoint / CThingFilter_MarkerHasProperty in 0x00964230 /
  0x00964EF0 are copies of the header-less shapes their landed callers 0x00964360 / 0x00964CA0 already use (no header exists; names are
  the PDB's). Allowed by the narrow new-local rule: no header anywhere, a landed sibling uses the same shape, no base.

## For the coordinator (decisions / debt)
- Strong explicit GetSubDef specializations remain in rows outside my locks: 0x007355F0 (9 instances, incl. CCombatAbilityStrafeDef /
  CCombatAbilityUseProjectileWeaponDef), 0x005EA650 / 0x005EB590 / 0x005EB9E0 / 0x005EC270 (CTavernGameDef), 0x00757B30 (CCreatureDef).
  Each LNK2005s against its owner and every header includer (pre-existing; my binders add more includers of the same COMDATs). Fix: the
  NOINLINE header body in each (NF25b: 19 of 19 such ports matched first form). 0x005EA3C0 defines the real GetSubDef<CTavernGameDef>
  symbol from a FAKE template body (`out->Object = this`) as a pick-any COMDAT: the linker could select it. Same hazard to check elsewhere.
- Pick-any bodies differ between TUs: the binders 0x00964360 / 0x00913F30 instantiate BESTTHING_BODY without the _NULLTEST / ISALIVE nested
  opt-ins and 0x008DCB60 instantiates ALLTHINGSBODY without _EGOD, so their COMDAT copies of the owner instances differ from retail (the owner
  rows now emit the retail body). Not a naming split; the binders should define the same nested opt-ins (outside my locks).
- reloc_bindings.tsv is incomplete: q4983's wrong spelling ?..@CThingSearchTools_Calls@@QBE.. (4 rows) is not in it. q4435 / q4483 / q4484 /
  q5014 / q5217 / q11675 were closed or censused by compiling every REFS row (work/probe*.tsv), not from the table.
- Brief ambiguity (GetPItem family, not started): rows like N006a2e20::CBaseIntelligentPointer are namespaced local models of a class with a
  shared header; the advisor reading is to port them onto engine/CIntelligentPointer.h rather than respell in place. Needs your ruling
  before the family is worked.
- The FableAssignee_* GetPItem binders are 7 village-task IsCreatureAssignedToTask / IsThingInvolvedWithTask rows whose own signatures are
  stand-ins too (QAE_NJ vs both PDBs' UBE_NABVCThing@@); their port is a self re-author on engine/CVillageTask*.h.

## Header users
See hdr/README.md: three overlays (CCombatAbilityStrafeDef.h, CCombatAbilityUseProjectileWeaponDef.h, CTavernGameDef.h); none of their landed
includers defines the new macros. No FABLE_FORCEINLINE added; no shared-header member renamed. No Flags lines added.

## Rule slips
- 0x005EC0F0: a too-wide regex moved FABLE_RETAIL_CTCBASE to the top of the row; the graded draft matched, but I restored the original
  position and regraded (one extra grade).
- STATUS grades_used for the q4278 binders include one rebuild for the link test.
- Tests of the changed rows were left unchanged: they are standalone host models (they never link the row), several still spelling the old
  stand-in / descriptive names; all PASS. Rewriting them on the shared headers is left for a later pass.
