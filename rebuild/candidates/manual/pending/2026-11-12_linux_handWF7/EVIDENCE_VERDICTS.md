# Workflow wf7 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 008b0b60 -- VERIFIED, author attempts 1

- What made it match: Calling the inline CloneActions helper produces the same bytes as the two copy lines written out, so the earlier match holds.

## 009486d0 -- VERIFIED, author attempts 1

- What made it match: The retail IS_THING_ALIVE&lt;T&gt; template, together with the retail CIntelligentPointer&lt;T&gt; != int compare, gives the same bytes as the old local inline. The Pos/RHSet members keep the +0x64 offset of ActionPointIndex.

## 0094f480 -- VERIFIED, author attempts 1

- What made it match: Four source-level facts on top of the shared-header port. (a) Slot 29 (+0x74) of the targeting component is CTCTargetingBase::GetPBestEnemyCreatureForAttack() const, which returns CThingCreatureBase*; GetPTargetedThing is slot 33 (+0x84). Evidence: engine/CTCTargetingBase.h order, CTCTargetingAI / CTCTargetingPlayer vtables, and both PDBs carry the symbol. (b) One nested condition with a single `return false`. (c) The default C3DVector branch, so the one-line GetSquaredMagnitude2D loads Y first. (d) The creature is read through the inline CAIStateGroupBase::GetCreature() at every use, with no creature local. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf7/0094f480/0094f480.cpp. Notes, lessons, labels and name fixes are in the same directory.

## 005f6793 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Unchanged from the author: the real body of GetPrioritisedExpressionDefs (0x005EED8C) sits in the row file and FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE is set, so VC7.1 hoists size() out of the loop. The inner loop is a guarded do-while, which the retail bytes at 0x005F6862 require.
- OPEN at hand-back: NOTES.md says both wrong local declarations in landed OnInitialActivate 0x005F6B47 (CycleAssignedExpressions(long) and CycleAssignedAbilities(long)) were written to NAME_FIXES.tsv, but $W/NAME_FIXES.tsv has only the CycleAssignedExpressions line. The CycleAssignedAbilities(long) -> ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z fix (0x005F64DD; Ego_r.pdb 0x00585434 plus FableWin.pdb/the shared header line 76) is missing, and no other w22 hand-back carries it.

## 00847df0 -- VERIFIED, author attempts 1

- What made it match: Port onto the shared engine headers plus three source facts: (1) the animation entry is bound to a reference local (`const CAnimationEntry& anim = PCreature.Get()->GetRandomAnimFromNameWithFilter("LIGHT_LAMP", filter); SetAnimation(anim, -1.0f, 1.0f);`) so the "LIGHT_LAMP" temporary dies before SetAnimation's pushes; (2) the NCameraModes::CScriptModuleUseLamp ctor takes const CThing& and is called with `*PCreature, *PObjectToUse, GetTotalTimeWorldFrames()` (CIntelligentPointer operator*), which gives retail's lamp/creature/duration evaluation order with pushes afterwards; (3) CAnimFilterOfGroup has no user-declared destructor (an inline virtual dtor adds a derived-vptr store retail lacks) and its inline ctor is `Group = group; GroupCRC = group.GetCRC();` with CCharString::GetCRC inline. Guard is IS_THING_ALIVE written out then the const PeekTC(TCI_ON_ACTION_USE, &action_use). Real names used: IsPlayerCharacter, GetWorld().GetGameCamera(), GetCameraManager, GetTotalTimeWorldFrames.

## 0097d6f0 -- VERIFIED, author attempts 1

- What made it match: The repairs are layout-neutral, so the existing match held: an enum member is still 4 bytes, and NAME_FIXES.tsv is metadata only.

## 006ad3c0 -- VERIFIED, author attempts 1

- What made it match: Porting onto the shared headers with the dossier's callee identities got the 0x68 frame (GetCurrentRenderCamera returns the 0x38-byte CCamera). Two source facts closed the rest: (1) `const CTCPhysicsBase* physics = PhysicsTC;` read before `mode_manager->GetCurrentMode()`, matching retail's `mov ebp,[edi+60h]` ahead of the call; (2) `acceleration * scale` spelled in both arms (`it->second.first += acceleration * scale` / `insert(value_type(action, pair<C3DVector,long>(acceleration * scale, 1)))`) instead of one named vector. GetMaxAcceleration needs FABLE_RETAIL_CTHING_ORIGINAL_VTABLE to sit at slot 87 (+0x15c). Plain `direction.RotateXY(angle)` works; map find/insert stay out of line with the default fable_map.h. No made-up names.

## 007735d0 -- VERIFIED, author attempts 1

- What made it match: Two steps. (1) Moved the parked draft off its local stand-ins and its invented __forceinline helper onto the shared engine headers as a single body (PeekTC house test, the GetCurrentBuyPrice def prefix, GetShopKeeper() three times, header std::map const find, PeekPlayerManager().GetMainPlayer().GetPControlledCreature(), GetTC(TCI_HERO_STATS,&stats)); that alone still gave 430v448. (2) In the keeper block the opinion manager and the keeper are named references declared in evaluation order before the attitude call: `const COpinionReactionManager& manager = PeekWorld().PeekOpinionReactionManager(); const CThing& keeper = *GetShopKeeper(); attitude = manager.GetThingAttitudeToPlayer(keeper);`. The `const CThing& keeper` local is the lever: it moves this to ebp, ParentThing to edi, zero to ebx, the thing def to esi and swaps the two def slots. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf7/007735d0/007735d0.cpp (source), /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf7/007735d0/007735d0_test.cpp (test), payload.json / oracle.tsv / NOTES.md / LESSONS.md / STATUS.tsv / LABELS.tsv / NAME_FIXES.tsv in the same directory.

## 00773790 -- VERIFIED, author attempts 2

- What made it match: The source is unchanged. In the keeper block the reaction manager and the keeper are two named locals, which gives retail's register allocation. This pass only repaired the overlay header.

## 007fd650 -- VERIFIED, author attempts 1

- What made it match: Two things. (a) The shared-header port with the correct identities and the CEnginePrimitiveLine ctor body visible but out of line fixed the old frame residue (0x60 v 0xD8) by itself. (b) The last byte and the this-in-esi-vs-ebp register residue was the lift spelling: retail is `StartPos + C3DVector(0.0f, 0.0f, 2.0f)` via the inline C3DVector::operator+ (VC7.1 folds the `+ 0.0f` on X and Y; operator+'s this = &StartPos / &EndPos keeps those addresses in esi / edi), not the member-wise `C3DVector(StartPos.X, StartPos.Y, StartPos.Z + 2.0f)`.

