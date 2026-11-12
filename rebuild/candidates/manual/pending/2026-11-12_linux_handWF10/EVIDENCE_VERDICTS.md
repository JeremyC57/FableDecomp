# Workflow wf10 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00808e40 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The match did not change; the repair only fixes the member type. The full CThingPlayerCreature to CThing base chain had to be visible (defines FABLE_RETAIL_CTHINGPHYSICAL/CTHINGGAMEOBJECT/CTHINGCREATUREBASE/CTHINGPLAYERCREATURE) so that *PlayerCreature converts to const CThing& for PeekTC and NTargeting.
- OPEN at hand-back: The made-up global name `g_bPlayerMeleeTargetingCheck` for bool 0x013B85F4 does not say what the flag is. `python3 $S/w23/xref.py 013b85f4` shows a second reader, 0x004085F4, in CUserProfileManager::ResetAssignedInputs 0x004085F0. That function tests the byte and then loads the def named by the retail string at 0x0122E1A8, "FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING". So the flag is the PC 'left click is king' control-scheme setting, which pending candidate 004085f0 already calls `g_bControlSchemeLeftClickIsKing`. The rule of one name per address, as accurate as the evidence allows, means the global should be renamed to match.
- OPEN at hand-back: NOTES.md and DOSSIER.md both say the global 0x013B85F4 is read only by this function ('only this function reads it'). This is false: the global is also read at 0x004085F4 (ResetAssignedInputs). The source header comment also calls it 'the melee check global', and that wording should be corrected along with the name.

## 0070ce10 -- VERIFIED, author attempts 1

- What made it match: The fill constructor is the COMDAT at 0x004516E7. Its body has to be visible to the compiler but not inlined. VC7.1 then knows `speeds` does not escape, so it keeps _M_start in ebp through the calls and the destructor, and it puts the empty allocator temporary on the `distance` argument slot (frame 0x10). Three other things were needed: UpdatePercentageCompleteTo is only `Last = Current; Current = p;`, with the wrap/clamp written in the body (the UpdateTimePassed idiom); GetMovementDistance is MovementVector.GetMagnitude() in the retail C3DAnimationInfo branch; and the speeds are indexed as plain speeds[i].

## 00949380 -- VERIFIED, author attempts 1

- What made it match: Renaming the callee does not change the bytes because the call target is a masked relocation. The original match still holds: shared headers, the GetThankingPhrase inline accessor overlay, and the IsStopping() ? false : true tail.

## 006e3580 -- VERIFIED, author attempts 2

- What made it match: Unchanged from the author's pass 2: CBasicString::Compare (0x00411570) and the std::__find body (0x004B8FF0) are compiled out of line in the row's source file under auto_inline(off). The row uses plain /O2 and the hdr/ overlay.

## 00966af0 -- VERIFIED, author attempts 1

- What made it match: The match is unchanged from the author: the pick runs inside while(true){ if(!bars.empty()){...; return true;} return false; }, so VC7.1 inlines ~list only at the first return outside the loop and calls 0x00792240 at the returns inside it. The repairs were comment and overlay hygiene only.

## 006992a0 -- VERIFIED, author attempts 1

- What made it match: I moved the row onto the shared headers. These opt-ins were set: CThing GETTC_EXPANDED, CTHINGSEARCHTOOLS, CIENGINE, the inline CEnginePrimitiveLine constructor (ONHIT + CENGINEPRIMITIVE_ID_CTOR), the CYLINDER branch of NCameraModes::CBase, and the reference form of CEnginePrimitivePrepareInfo. I also copied each debug point into a local pair. Two body-visibility facts then closed it (the Q32 lesson). First, the GetAllThings template body is visible to the compiler (FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY), so the list is kept in the dead info argument slot and the frame becomes 0x64. Second, CThingFilter_HasTC::operator() stays out of line (FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE). Retail's GetAllThings at 0x004ACD70 calls 0x006A80A0, which puts the filter store ahead of the call setup.

## 008f0eb0 -- VERIFIED, author attempts 1

- What made it match: Three changes on top of the shared headers. One loop sums both CurrentPos.X and CurrentPos.Y, and each is then divided by (float)Cards.size(). The named CCharString("TAVERN_GAME_CARD_DEAL", -1) sits in its own {} block, so it is destroyed before the inlined Move. There is an explicit `return;` after SetState(0x1a) in the state-0x19 branch, which gives retail's separate release of def through ebp. The real GetCardDef and Move bodies in the TU keep def in ebp and let the def slot be reused for the 53 - i counter. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf10/008f0eb0/CTCTavernGameCardBase_CardsTurning_008f0eb0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf10/008f0eb0/test_008f0eb0.cpp

## 00929340 -- VERIFIED, author attempts 1

- What made it match: Five source changes: (1) the orientation is fetched first into a named reference, `const CRightHandedSet& rh = DiceGamePosition->GetParentThing().GetPhysicsTC()->GetRHSet();`, then the call gets `GetPos() + rh.GetForward()`; (2) ++NumberOfPlayers goes through Get/SetNumberOfPlayers, while --NumberOfPlayers is written directly; (3) the Unsheathe failure path is an early `Terminate(); return;` followed by `WasCarryingWeapon = false;`; (4) the entry test reads `CTCExclusiveUse* position = DiceGamePosition;` before `position->User.Get()`; (5) the row sits on the shared engine headers with the CCombatActionBase MELEE opt-in. CTCExclusiveUse, CTCDiceGame and CCreatureAction_SheatheWeapons are modelled locally because no engine header exists for them. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf10/00929340/src_00929340.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf10/00929340/test_00929340.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf10/00929340/payload.json

## 009f4390 -- VERIFIED, author attempts 1

- What made it match: Extents written as four scalar conditional expressions (`long destWidth = destRect ? destRect->BRX - destRect->TLX : GetWidth();` etc., source falls back to dest width/height). VC7.1 merges the paired tests, homes the widths in the dead destRect/sourceRect arg slots and gives ebp to sourceFormat; if/else spellings gave a width a register instead. Ported onto shared headers with overlay opt-ins.

## 00641c30 -- MATCH_WITH_OPEN_ISSUES, author attempts 3

- What made it match: Retail CTCLight.cpp defines DrawLight before OnAppearanceDraw. VC7.1 can see that DrawLight only copies its const C3DVector& argument, so it keeps the fallback pos in registers through the if/else and stores it once at the merge. Putting DrawLight's real body, rebuilt from the disassembly, out of line before OnAppearanceDraw reproduced that. The DrawLight body is not byte-verified yet.
- OPEN at hand-back: Stand-in body: the row defines a full body for CTCLight::DrawLight 0x006414A0, and that body does not byte-match retail. Compiled from the row source with the overlay, the leaf ?DrawLight@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z is 1612 bytes against retail's 1596. Against $W/drawlight.dis the mnemonic-sequence similarity is only 0.894, with prologue, FP and switch differences. NOTES.md admits it is 'NOT byte-verified'. The row's RELOCATION_MATCH depends on this body: without it VC7.1 cannot see that DrawLight's argument does not escape, and the match goes away. An unverified, non-matching reconstruction of another function would therefore be landed in the TU as if it were real.
- OPEN at hand-back: Reservation conflict: reserved_check.py reports that the row defines a body for reserved address 006414a0 (CTCLight::DrawLight, reserved by 2026-11-11_acctB). This row should not land a body for DrawLight while another lane holds that address.
- OPEN at hand-back: Overlay: hdr/engine/NGraphicAppearance_CTCGraphicAppearance.h removes one line of the repo header. Line 18 goes from `#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL` to `#if defined(FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL) || defined(FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PEEKLIGHTINGCHANNEL)`. hdr/README.md describes the PEEKLIGHTINGCHANNEL opt-in but does not explain this rewritten #if line. hdrcheck2 still shows default=same, so the default path is unchanged.
- OPEN at hand-back: The row carries a full 1596-byte body of CTCLight::DrawLight (retail 0x006414A0) that is not a landed row (no rebuild/src/compiled/00/64/*006414a0* file exists; manifest marks it unlanded) and was never byte-verified (ATTEMPTS.log '006414a0 grade a.cpp ERROR -- probe: how close is the reconstructed DrawLight'; NOTES: 'NOT byte-verified, no oracle'). Check 5 requires each carried callee body to name the landed row it copies, and this one copies none. The OnAppearanceDraw match depends on DrawLight not letting &pos escape, so the match rests on an unproven stand-in. DrawLight should be verified and landed as its own row first, or graded and landed together with this one.
- OPEN at hand-back: Landing this TU as-is would define CTCLight::DrawLight (0x006414A0) a second time once DrawLight is landed as its own row (duplicate symbol). It would also put unverified compiled code for that address into the build. Nothing in the payload or NOTES says how that will be reconciled at landing.
- OPEN at hand-back: The four overlay headers (hdr/engine/CEnginePrimitiveLight.h, CEnvironment.h, CGameTimeManager.h, NGraphicAppearance_CTCGraphicAppearance.h) exist only for the unverified DrawLight body. CEnvironment::PeekDiffuseColour reads `*reinterpret_cast<const CRGBFloatColour*>(&GameTickState[4])`, and the author admits the member index name is unproven. So these shared-header opt-ins would land on the strength of a row that has not been byte-checked.
- OPEN at hand-back: the repair agent did not keep the match (DIFFER(502v490), 3 hunks (differ/00641c30.cpp, no overlay, no DrawLight body))

