# NF191 notes (round 10, returned game / creature / UI / input member names, second pass)

Handed back for context. Rule slip: queue order. After q11957 I skipped the open entries q11954 .. q12053 (dragon family, q11973, q11982, q11991, q11993, q12046, q12047, q12049, q12053) and finished later ones (q12054 q12055 q12098 q12108 q12126 q12149 q12153 q12205 q12206); the skipped ones are `open` with their HEAD census. Counts (from APPLIED.tsv / STATUS.tsv): 54 entries: 7 done, 8 done-already,
14 blocked, 1 deferred, 24 open (not started; each open line carries its HEAD census). 56 rows listed: 20 ported (RELOCATION_MATCH /
MATCH), 32 blocked, 4 deferred (held). 0 rejected. Final recheck: recheck_final.log, one vland run over the 20 ported rows.

## Census
- work/probe_all.tsv: every landed REFS row of every entry compiled once at HEAD (tools/probe.py, catalog flags), with the symbol it
  binds at each entry address; work/census.tsv classifies each (RIGHT / WRONG / ?) against the entry's claim. The open entries'
  APPLIED notes summarise their census (symbol counts); the row lists are in work/census.tsv.
- q4411's "RIGHT 465" counts the four CBase_0062e900 rows as right because q6405's queue line named that stand-in in its first
  column; they are wrong (STATUS).

## Self renames (RENAMES.tsv section 1; the row files move)
- 0x004944C0 CFrontendGameComponent::ClearTextInput -> ClearString (private; leaf ?ClearString@CFrontendGameComponent@@AAEXXZ).
  Test rewritten on the shared header (the explicit-instantiation friend idiom reaches the private member; it links against the row
  object). The coordinator moves the file to CFrontendGameComponent_ClearString_004944c0.cpp; the test does not include the row by
  path.
- 0x007AED30 CTCBoss::IncrementBossPhase -> IncrementStrikesSinceHitHero (the old leaf is 0x007AED20's own name).
- 0x00957170 leaf-only (reference form), 0x00A156D0 leaf-only (const reference), 0x0064BE45 leaf-only (NPlayerGui namespace).
- Every renamed address: no landed row references it except rows that already bind the new spelling (HEAD probe / exe scan).
- No EH funclet under rebuild/src/compiled/01/21 includes any changed row (grep of each file name).

## Callee spellings moved to existing PDB spellings (count does not grow)
- 0x008BD880 ??0CCreatureAction_RiseUp@@QAE@AAVCThingCreatureBase@@@Z and 0x006644F0
  ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z (0x00920960 port; both already bound by landed rows).
- 0x00857F30 reference ctor and 0x008574D0 ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x00957170 port; both
  already bound by landed rows).
- 0x00A7A8D0 ?Initialise@CNavQuadTree@@QAEXABVCNavigationAreaInit@@@Z (PDB, Ego_r 0x00BC1A80 + FableWin 0x03290030; no landed row;
  0x00A156D0 was its only binder: count stays 1). RENAMES section 2.

## Local models (three-condition rule / in place)
- 0x00739CC0 CCreatureAction_BlockBroken (quarantine header only), 0x007F05F0 CCombatAction_GenericStrikeResponseKnockdownRecover,
  0x009704B0 CCreatureAction_SummonerChargeSuccess: each row keeps its own class token (defined at HEAD) and now derives from the
  shared base its RTTI names (CCombatActionBase / CCombatAction_GenericStrikeResponseKnockdownRecoverBase); no defining header
  exists for any of the three. objdump: each object defines only its leaf.
- 0x00920960 CAIStateGroup_CreatureIdleUnderground (no header) on the shared CAIStateGroupBase, the shape of landed sibling
  0x00920DC0.
- 0x0064BE45 NPlayerGui::CDrawMeleeActionButtonEffects (no defining header; CPlayerGui.h forward-declares it) on the shared
  NPlayerGui::CDrawBase, the shape of landed sibling 0x00646580; the row's own token, moved into the namespace (objdump: only the
  leaf changes vs HEAD).
- The q4492 rows keep their other row-local models unchanged (pre-existing SHARED_HEADER_AVAILABLE warnings, not new).

## Pick-any check
- New COMDATs vs HEAD (objdump -t defined-symbol diff per row): 0x007BD80A the shared CIntelligentPointer<CThing> inlines (??1, ??C,
  Get) in place of the stand-in's; 0x00920960 GetCreature x2 (header inlines) and the implicit ??1CCreatureAction_RiseUp; 0x00957170
  ??_GCCreatureAction_BriarRoseShotFarInto@@UAEPAXI@Z. None has a reloc_bindings line or a landed Leaf owner. The ??_G is
  graded with q.py against the owner address 0x00957320: RELOCATION_MATCH (the landed row there defines a stand-in name, so no symbol
  clash). The row's ??_7CCreatureAction_BriarRoseShotFarInto@@6B@ is now the shared-header class's vtable (CCombatActionBase slots;
  the header declares only the slot-4 FinishAction override, not the slot-73 IsToFaceTarget 0x009571F0 override), where HEAD emitted a
  one-slot stand-in; the vtable has a table line only to this row (01294a2c, witness 00957170).
- No row emits a copy of IS_THING_ALIVE (declaration-only template).

## Binding diff (RENAMES section 2)
- tools/s2.py compiled each ported row at HEAD (pS/probe objects, else a fresh compile) and diffed every relocation pair (target,
  symbol) with reloc_identity.check against the hand-back object: 21 changed pairs, all listed in section 2; every new spelling is
  already bound at that address by landed rows, except 0x00A7A8D0 whose only referencing row (exe scan) is 0x00A156D0 itself.

## Flags lines
- None added.

## Descriptive names
- No new made-up name. The stand-in member name ScalarDeletingDestructor on 0x00739CC0 / 0x007F05F0 / 0x009704B0 is unchanged
  (landed descriptive stand-in for the compiler-emitted ??_G; the real ??_G names need the TU-emitter / scaffolding route).

## For the coordinator
- q4522: retail passes a converted temporary to IS_THING_ALIVE<CThing*> 0x0040DCE0 at 0x005840A6 / 0x0065B2C4 / 0x0065BFFA /
  0x0047DB73 while the variable stays in a register; the in-place template spelling differs (3 forms each). The whole-row port on
  engine/CTCVillage.h in the shape of 0x0065B142 is the next step; 0x0065B2C4's port would also bind ?Evict@CTCDummyVillager@@QAEXXZ
  at 0x0065F01D (now the stand-in ?Detach@CTCDummyVillager@@QAEXXZ; the self row there is already Evict).
- q5192: 0x00C48770's body is a compiler-generated copy constructor (it copy-constructs the pointer member through 0x00C16900),
  not operator=; the claimed name ??0CIntelligentPointer_00c16900 is itself an address-named stand-in. Identity / authoring item.
- q11957: 0x00871D30 is not CTCLook::UpdateLookTimer (it calls NCameraModes::CBase::GetObservedThing on this); candidate
  NCameraModes::CStringAndCageBase::GetZCoord in NAME_FIXES.tsv (both PDBs + adjacency to UpdateCageRadius 0x00871D50).
- q11942: the in-place access recipe for 0x006A4D60 / 0x007032F0 / 0x006A9EF0 is ready but waits for the 0x00833A70 port (no
  local CThingPhysical there); moving three of four would grow the spelling count.
- q6396: self row and its only binder 0x00861A80 (locked NF189) move together onto engine/NAnimationEntryBodge_CAnimEntryLite.h.
- New agent id NF192 seen in nflocks (0x007AB190).
- RELOC_CONFLICT in 0x007BDAC7 (??$IS_THING_ALIVE@PBVCThing@@ -> 0x00436380 call vs 0x007BC8FC) is pre-existing; that call's text
  is unchanged.
