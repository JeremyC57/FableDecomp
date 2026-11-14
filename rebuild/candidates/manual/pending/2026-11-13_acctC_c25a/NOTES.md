# agentC25 notes

## 0x007C0690 NThingShot::CTCShotDefinitionBase::Init (target-thing overload) -- RELOCATION_MATCH, test PASS, vland PASS (~45 grades)
Name: Ego_r/FableWin ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1PBV4@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z
+ header INIT opt-in. Extent ok (ret 0x20 @ 0x007C0ACD, 1088 bytes).
Decoded: PTargetThing = target; if shooter alive: PeekTC<CTCTargetingBase> && GetTargetedPos(pos,1) && (GetPlayer().IsFirstPersonTargeting()
|| (IsZTargeting() && IS_THING_ALIVE(GetPTargetLockedThing()) [called out of line 0x0040DCE0])) -> shoot; target alive && NTargeting::
IsThingVulnerableToMeleeStrikesFromThing -> pos = GetTargetingPositionOnThing(PMainGameComponent->World, *target, 1); type 2 && PC third-person
aiming -> pos = GetPos() + camera view dir * CTCTargetingPlayer::GetMaxTargetingDistance(); else shot facing pitched onto the shooter's
horizontal facing (Rotate about (f.Y,-f.X,0) by GFArcCos(|dir2D| clamped) * sign(dir.Z)), pos = physics position + dir * 20. Then Init(position overload).
Callee names: 0x00802390 = CTCTargetingPlayer::GetMaxTargetingDistance() const (Ego_r 0x007FD220 / FableWin 0x024CE410; landed row
DrawGetSeaLevel_00802390 is a placeholder struct, see NAME_FIXES.tsv); camera vslot 9 = CAIGameCameraBase::GetCameraManager(CGameCameraManager**) (result unused).
Header opt-ins: CTCTargetingPlayer.h FABLE_RETAIL_CTCTARGETINGPLAYER_MAXDISTANCE (decl; 38 asserts pinned); C3DVector.h ADD_NAMED_RESULT was tried
(no effect) and removed from the overlay.
What moved it (v1 1019/47 hunks -> v26 1053/11): GFArcCos's double param home (8-byte frame node) blocked facing from sharing the IP temp
slot: named `float angle = GFArcCos(cos_angle);` + explicit if/else-if clamp (ResolveFacingDirection 0x006AB820 idiom) fixed the frame (0x40);
named `facing_vector` call result copied into `direction` (retail copies GetFacingVector's temp from the frame and reads .Z through it) and a
named `pos` for GetTargetingPositionOnThing; facing test `facing.GetSquaredMagnitude() > DNZ^2` (SQUAREDMAGNITUDE_ZYX, Y then X loads);
GetMagnitude2D ONE_EXPRESSION opt-in (existing, fitted for Q9) for retail's Y-first magnitude.
Last two residues closed: `const CThing& target_thing = *PTargetThing;` named after the vulnerability test (the world argument was loaded
before the GetPItem call; with the named reference it loads after, as retail), and `GetParentThing().GetPhysicsTC()` for the fallback's
physics pointer (ParentThing in eax, not ecx). `*PTargetThing` = FableWin ??D?$CIntelligentPointer@$$CBVCThing@@ (DEREFERENCE opt-in, existing).
Control flow: a `have_target_pos` flag (the goto form grades identically). Uses the existing fitted MAGNITUDE2D_ONE_EXPRESSION GetMagnitude2D
text (the running-sum body loads X first; retail loads Y first here) -- coordinator: that opt-in now has two users.

## 0x007EF200 CTCCarriedActionUseExpression::OnActionUse(CThing&) -- RELOCATION_MATCH (/GS), test PASS, vland PASS (~25 grades)
Name: Ego_r ?OnActionUse@CTCCarriedActionUseExpression@@UAEXAAVCThing@@@Z (donor 0x007E8CE0) + vtable slot 28 (CTCActionUseBase::OnActionUse 0x007EAB90).
/GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9). Extent ok (ret 4 @ 0x007EF4DB).
Decoded: base OnActionUse; if (&user && !IsDead && !HasTC(TCI_BERSERK)) { target = 0; PeekTC<CTCTargetingBase> -> (OfType 2 && InRange 2 (vslot 38))
|| (OfType 1 && InRange 1) -> GetPTargetedThing; PeekTC<const CTCExpression> on the item (result unchecked: out slot uninitialised);
if HasDef: def.GameEventType -> if PeekPlayer().IsPlayerCharacter(): CGameEvent(type).SetPlayer(Number), PMainGameComponent->AddGameEvent;
else if def.ScriptName != "" -> QuestManager ActivateQuest(ScriptName [GetString 0x00415DD0], false, ScriptLoadResources), if PerformAfterScript ->
user.SetCurrentAction(CCreatureAction_PerformExpression(user, target, expression->GetPDef(), item.GetDefGlobalIndex(), -1,-1,-1)); else the same
action }. Then item GetTC<CTCInventoryItem> (0x1A) -> ReturnToInventory 0x005D8E50 (siblings call it "OnShowEnded": wrong, PDB ReturnToInventory).
New overlay headers: CTCCarriedActionUseBase.h, CTCCarriedActionUseExpression.h, CTCExpression.h (HasDef / PeekDef / GetPDef FableWin inlines);
opt-ins CPlayer.h FABLE_RETAIL_PLAYER_ISPLAYERCHARACTER (175 asserts pinned), CQuestManager.h FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST (39 pinned).
Uses FABLE_RETAIL_EXPRESSION_FRAME (+ CCREATUREACTIONBASE, _FULL_MEMBERS) for the 7-arg CCreatureAction_PerformExpression ctor (0x008430B0),
FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (implicit conversion calling GetString, not ??B 0x00415D70 -- q.py masks call targets!).
Open residue: (1) the two identical action copies: retail keeps both (register rotation differs: copy 1 edx/eax/ecx, copy 2 ecx/edx/eax);
ours cross-jumps the tails from `lea ecx,[esp+0x34] ; push edi` (retail `push edi ; lea ecx,[esp+0x38]`); GetParentThing().GetDefGlobalIndex()
reproduced retail's split (ParentThing loaded before the GetPDef copy, movzx after) -- 622 -> 673 bytes; (2) CGameEvent block register
rotation (retail edx type / al player / ecx &event, type stored after `push &event ; mov ecx,[PMainGameComponent]`); (3) quest manager PClass
load: ours before the argument pushes, retail after (same class of residue as Init's world argument); (4) targeting vslot38 vtable regs.
What closed 0x007EF200 (673/20 hunks -> RELOCATION_MATCH): the targeted-thing test written as if / else-if with the same
`target = targeting->GetPTargetedThing();` in both arms (VC7.1 cross-jumps them into the one retail call); the `||` condition gave other
allocation all the way down, which is also what made the two action statements merge. Earlier: GetParentThing().GetDefGlobalIndex(),
CDEFSTRING_FORCEINLINE_CONVERSION (GetString call target), named-action variants rejected (wrong temp lifetimes).

## 0x0072CF40 CTCSpecialAbilities::UseSpecialAbility(EHeroAbility) -- RELOCATION_MATCH, test PASS, vland PASS (5 grades)
Name: Ego_r ?UseSpecialAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z. Extent: 488 bytes of code (ret 4 @ 0x0072D125) + the 19-entry
jump table 0x0072D128..0x0072D173 = 564 (the oracle row as reserved). Draft from the disassembly graded 564v564/11 hunks at once; closed by
`bool success = false;` before the switch and `case HERO_ABILITY_RAISE_DEAD_SPELL: success = false; break;` (retail's duplicated
`xor bl,bl ; mov al,bl` epilogue is the threaded deed test). Names: SetupMultiStrike 0x0072AAF0 / IsAbilityAvailable 0x0072C430 (LABELS.tsv,
Ego_r name sets; descriptive inference by elimination, see there). Decision needed: deed constants 0x11 / 0x3D written as casts
(EOpinionDeedType.h gives EXPRESSION_BATTLE_CRY / RECEIVE_GIFT_FRIENDLY, which look like a different enum revision).
New header NEntityEvents_CEventPerformedSpecialAbility.h; opt-in CTCSpecialAbilities.h FABLE_RETAIL_CTCSPECIALABILITIES_USE.

## Not reached (context ran out after three matches)
0x009FDE80, 0x0074E860, 0x00735FD0, 0x007E15C0: not opened by C25 (C24's outline for 0x009FDE80 in prev_C24/NOTES.md).
0x006823D0 / 0x007BFE30: C24's drafts in prev_C24/differ/ unchanged (not regraded under the C25 overlay; the overlay additions do not
touch their headers' existing text). C24's matched 0x0058F9BC still grades RELOCATION_MATCH under the C25 overlay (checked).
Overlay hygiene: C3DVector.h ADD_NAMED_RESULT was tried and removed (no file in hdr/); every hdr/ change is listed in hdr/README.md.
