# Workflow wf15 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00bdf100 -- VERIFIED, author attempts 1

- What made it match: Using the shared headers instead of local models, with the correct nested class name and callee identity. Calling the real STLport push_back and _M_insert_overflow bodies at both sites let VC7.1 make its own inlining choice for each site, which reproduces retail. The hand-written __forceinline overflow copy is no longer needed.

## 005f6bd6 -- VERIFIED, author attempts 1

- What made it match: Three things made it match: (1) the shared headers and the PDB callee names; (2) the real same-TU bodies of IsAbilityAvailable 0x005EDF08 and GetNumAssignedReceivedAbilities 0x005EE146, expanded inline; (3) the real STLport std::find / std::__find bodies visible as noinline fastcall COMDATs, so the compiler knows &ability does not escape. Ability then stays in ebx and `last` is reused after the call. The last step was the received-order insert written as an `added` flag with breaks, with the outer duplicate loop doing `if (added) break;`.

## 00a60d40 -- VERIFIED, author attempts 1

- What made it match: Unchanged source. The overlay change was cosmetic and gives the same token stream when the CTOR flag is defined.

## 0091ddd0 -- VERIFIED, author attempts 3

- What made it match: The loop exit is spelled `if (AIAction_MoveToPos(...) == 2) continue; Terminate(); return;`, with return instead of break. The instructions are the same either way, but the block weights change: the CIntelligentPointer player now outranks the {frac, cos-Z temp} spill group, so the player takes esp+0x18 and the group takes 0x20, as in retail.

## 008db810 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The repair kept the byte match. Replacing the flat CModeBase + _base pad with CModeStandBase.h + CModeDragonHover gives the same offsets (Timer +0x80, PBeamWeaponEffect +0x84, LastExplodeFrame +0x8C, BeamActive +0x90).
- OPEN at hand-back: Overlay $W/hdr/engine/CJackDragonDef.h is stale and must be dropped: the repo header already has a landed FABLE_RETAIL_CJACKDRAGONDEF block (wf13, row 0x00951D60 IntoHover), and the overlay deletes those 38 lines and puts a different CJackDragonDef model in their place (`diff | grep -c '^<'` = 38, and hdr/README.md does not explain it). hdrcheck2 reports FABLE_RETAIL_CJACKDRAGONDEF=DIFF[126](cpp-err) for it. Landing it would overwrite the definition that the landed 0x00951D60 row depends on. The row does not need it: it grades RELOCATION_MATCH with only the NCameraModes_CTools.h overlay.
- OPEN at hand-back: The overlay $W/hdr/engine/CJackDragonDef.h is a stale copy that conflicts with the repo: rebuild/include/engine/CJackDragonDef.h already has its own FABLE_RETAIL_CJACKDRAGONDEF block (landed by wf13 for row 0x00951D60, commit 2b4e20ff, with Alloc/Transfer/Copy/GetSizeofClass and FABLE_STATIC_ASSERT(sizeof==0x78)). The overlay was built on the older header: it deletes that block and adds a second, different class under the same macro, so landing it as a file copy would undo wf13's header. Drop this overlay file and build the row against the repo version, whose member offsets are the same (FramesBetweenFireBeamUpdate +0x5C); then regrade and update hdr/README.md.

## 005fbaa0 -- VERIFIED, author attempts 2

- What made it match: Changing the structure of the aspect scale fixed both residues. (1) The two scales became an 8-byte local declared after width/height and the divisions. br is dead by then, so the local packs into br's frame slot: scale[1] lands at [esp+24h] and scale[0] stays on the x87 stack. A C2DVector scale(1,1) there already gave 634v634 with 2 hunks. (2) The X-line operand order in this function follows the number of loops VC7.1 saw earlier (period 4, found with probes). Writing the scale as `float scale[2]; for (int k = 0; k < 2; ++k) scale[k] = 1.0f;`, a 2-trip loop that is fully unrolled, gives retail's `fld [esi] ; fmul [ebp+2Ch]`.
Caveat for landing: this init loop compiles to no extra instructions. Its only effect is to steer that operand order. It is not proven original source, so the coordinator may treat it as compiler steering under GOTCHAS line 313 and should rule on it before landing.

## 007cd9a0 -- VERIFIED, author attempts 1

- What made it match: The original match is unchanged: the 4-argument CRGBColour constructor, and the kill loop spelled as in the dtor, with IS_THING_ALIVE and then CTCSpecialAbilities::FadeOrKillEffect. Neither repair changes the bytes. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf15/007cd9a0/CTCBurntEffect_FrameUpdate_007cd9a0.cpp

## 00b211b0 -- VERIFIED, author attempts 1

- What made it match: Two changes did it. First, the row now uses the shared CCardinalSpline.h with its original inline accessors, plus the STLport opt-ins FABLE_RETAIL_STLP_VECTOR_C, FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR and FABLE_RETAIL_CARRAY, defined in the row source. These make the vector copy constructor and operator= bodies visible while they are still called out of line, which fixed the ebx/ebp swap. Second, the count compare is written as `long prevCount = PreviousSpline.GetControlPointCount(); if (ThisSpline.GetControlPointCount() != prevCount)`. GFInterpolateArray is declared in namespace NParticleEngine. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf15/00b211b0/final.cpp. Test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf15/00b211b0/test.cpp.

## 007bf510 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Unchanged from the author's version. Renaming the enumerator does not change the bytes.
- OPEN at hand-back: Overlay $W/hdr/engine/CIntelligentPointer.h was made from an older copy of the repo header: `diff rebuild/include/engine/CIntelligentPointer.h $W/hdr/engine/CIntelligentPointer.h | grep -c '^<'` is 9. Landing it as it stands would delete the current FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR opt-in, both the `#include "fable_compiler.h"` guard at lines 4-6 and the FABLE_FORCEINLINE ctor block at lines 48-53, which belong to wf13 row 0x009341C0. hdr/README.md does not explain the removal. The overlay must be rebuilt as the current repo header plus only the FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED block. I checked that this rebuilt overlay, in $W/verify_mechanical/ovfix/engine/CIntelligentPointer.h, still grades RELOCATION_MATCH.
- OPEN at hand-back: The overlay $W/hdr/engine/CIntelligentPointer.h was built from an older copy of the header: compared with the current rebuild/include/engine/CIntelligentPointer.h it is missing the FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR block (the `#include "fable_compiler.h"` guard at lines 4-6 and the FABLE_FORCEINLINE ctor opt-in at lines 48-53 that row 0x009341C0 depends on). Copying the overlay as-is would delete that landed feature. It must be rebased onto the current header so that it only adds the FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED operator==.

## 00972cc0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Fixed all three verifier findings and the row still grades RELOCATION_MATCH. (1) The made-up local CCountedPointer_CCreatureActionBase / SCountBlock is gone. PAction is now the shared engine/CCountedPointer.h CCountedPointer<CCreatureActionBase> (with engine/CCreatureActionBase.h), with no opt-in needed. (2) IsStillRunnable is now the landed 0x00971B00 row's nested-if spelling, copied word for word. The comment names that landed file and says the earlier IS_THING_ALIVE one-expression spelling came from the parked 0x00975BB0 Update draft and gives the same bytes. (3) The creature/world steering locals were not needed: the plain call GetWorld().GetCombatManager().SetAttackerAsReadyToAttack(GetCreature(), true) gives the same bytes, because VC7.1 evaluates arguments right to left. The locals are removed, and the comment cites the retail bytes: [[edi+4]+0x18] is loaded into esi at 0x00972CDE..0x00972CE1, before `call 0x8fcfc0` at 0x00972CE6.
- OPEN at hand-back: The source open-codes `output.Types |= 0x20;` although both PDBs name that inline operation (Ego_r.pdb 0x0179B3F0 and FableWin.pdb 0x028923D0, `?SetAsPumpCombatActions@CBrainOutput@@QAEXXZ`), and the dossier itself says SetAsPumpCombatActions = Types|=0x20. The local CBrainOutput model should declare `void SetAsPumpCombatActions() { Types |= 0x20; }` and the row should call `output.SetAsPumpCombatActions()`, with a re-grade to confirm the bytes do not change.
- OPEN at hand-back: The inlined callee CAIStateGroup_JackOfBladesBase::IsStillRunnable (0x00971B00) is protected in both PDBs: Ego_r.pdb egor_pdb_names.tsv 0x00996980 and FableWin.pdb _fablewin_api.tsv 0x0179970D / 0x02BE5100 both spell it `?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@IAE_NXZ`. The landed row rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp has `// Leaf: ...@@QAE_NXZ` (public) and declares it under `public`, and DOSSIER.md section 5 wrongly says 'PDB says public QAE'. The author copied that landed body but handed back no NAME_FIXES entry and no NOTES line correcting the landed leaf.
- OPEN at hand-back: The local CNavigationInput model declares `long MovementType`, but Ego_r.pdb (struct_layouts_egor.tsv CNavigationInput +16) types it `EMovementType`, and a shared enum exists in rebuild/include/engine/fable_ai_action_enums.h. Use EMovementType (zero-initialised as AI_MOVEMENT_NULL), or at least add the `// +0x10 EMovementType` comment that the landed sibling 0x0093DDE0 carries.

