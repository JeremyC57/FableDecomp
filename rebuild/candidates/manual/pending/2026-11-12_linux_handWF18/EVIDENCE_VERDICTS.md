# Workflow wf18 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 008a7a70 -- VERIFIED, author attempts 1

- What made it match: PDB single-argument type-deduced GetTC/PeekTC spelling with NAVIGABLE define keeps byte match

## 00724630 -- VERIFIED, author attempts 3

- What made it match: Unchanged from attempt 3: `operator delete(PPhysicsMeshInfo)` replaces the delete-expression, plus /EHsc. The repair edits do not change the bytes.

## 009ce5e0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The source is unchanged apart from comments, so the earlier match still holds.
- OPEN at hand-back: NAME_FIXES.tsv row 1 is keyed by address 009CC750, which is CBankFileEntryUpdateData::Equals itself, but its 'correct name' column is 'CBankStateBlock::CreateCRC (vtable slot 2 callee)': that is neither a mangled symbol nor the name of 0x009CC750, so a coordinator applying the file by address would rename Equals to CreateCRC. Rewrite the row to give the actual fix: in the landed Equals model, the CBankStateBlock slot-2 virtual `GetCRC() const` should become `CreateCRC()` (non-const UAEKXZ). Sources: FableWin ?CreateCRC@CWADBankStateBlock@@UAEKXZ 0x017CDB39, and CWADBankStateBlock vtable 0x0129CFD0 slot 2 = 0x00A3A2D0. Alternatively, move this note out of NAME_FIXES and into NOTES.md.
- OPEN at hand-back: The source carries an inline body for CBankCreatorException(const CCharString&) (`: ErrorText(errorText) { }`), and retail expands it in place at 0x009CE62B-0x009CE63E (default ctor 0x0099E4B0 on +0x00, copy ctor 0x0099EC30 on +0x04). No comment says which retail bytes show that body was visible or inlined, or that no landed out-of-line row exists to copy (PDB name ??0CBankCreatorException@@QAE@ABVCCharString@@@Z, FableWin 0x01817946). Add a one-line justification, as was done for EntryExists and the CBankFileEntryUpdateData ctor.

## 00627400 -- VERIFIED, author attempts 1

- What made it match: The shared-header STLport list opt-ins (out-of-line ~list with visible body; noinline visible clear) replace the local specialisations and produce the same bytes.

## 007eb6a0 -- VERIFIED, author attempts 1

- What made it match: Moving the row onto the shared headers. CPlayer.h has to come before CThingPlayerCreature.h, otherwise the inline GetPPlayerCharacter is compiled as a call. PostDeed's thing argument is spelled &GetParentThing(); reading the ParentThing member directly swaps eax and edx through the rest of the function.

## 00959f80 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The verifier's fix only renames the enum values, so the code the compiler emits is unchanged. It still grades RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1. vt.py reports PASS for the test. addmatch.py rewrote payload.json and oracle.tsv from /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf18/00959f80/final.cpp.
- OPEN at hand-back: Overlay $W/hdr/engine/C3DVector.h is stale against the current repo header. `diff rebuild/include/engine/C3DVector.h $W/hdr/engine/C3DVector.h` shows 7 removed lines: the whole `#if defined(FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX)` Normalise branch, which the landed row rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesBase_RushToPos_009735d0.cpp uses, and the following `#elif` turned back into `#if`. hdr/README.md does not explain this rewrite, so the overlay must be rebased on the current header so that it only adds the FABLE_RETAIL_C3DVECTOR_CROSSWITHUP block.

## 0094f180 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged and still grades RELOCATION_MATCH. Only the overlay was cleaned up, so every header change sits behind its FABLE_RETAIL_* opt-in. Payload and oracle were rewritten with addmatch.py.

## 00ac53f0 -- VERIFIED, author attempts 1

- What made it match: Four changes. (1) Case values taken from the index table: 8 = Bump and 9 = Reflection. The DOSSIER had this off by one; the old draft was right. Writing the case blocks in retail's code order (Transparent, Diffuse, Bump, Reflection, Illumination) gave the `and esp,-8` frame. (2) GetExtendedProperties() goes into a named local. (3) The row uses the shared headers through overlay opt-ins. (4) The decisive change: CalculateMapFlags' real body (landed row 0x00AC5260, defined earlier in the same retail TU) sits in the row file under `#pragma auto_inline(off)`. That fixed the register choice and the exit-block layout. The source is /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf18/00ac53f0/00ac53f0.cpp and the test is /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf18/00ac53f0/test_00ac53f0.cpp. vt.py printed PASS, and addmatch.py wrote payload.json and oracle.tsv. NAME_FIXES.tsv records that 0x00ABFDC0 is C3DMeshFileMaterialExtChunk::GetSelfIllumination, not CWorld::DrawGetEnvironment.

## 007e6450 -- VERIFIED, author attempts 1

- What made it match: The verbatim landed OutputsFiring body (int i / i++) produces the same bytes, so the match holds. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf18/007e6450/007e6450.cpp, test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf18/007e6450/t.cpp

## 00959bf0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: All three verifier findings are fixed and the row still grades the same. (1) The local class definition is gone. The row now includes engine/CAIStateGroup_HoverWander.h with a new overlay opt-in, FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER. Under it the header declares the real base (CAIStateGroup_HoverMovementBase), a public virtual Update (Ego_r ?Update@CAIStateGroup_HoverWander@@UAEXXZ) and a sizeof 0x1c assert. Without the opt-in the header is the same as before, so the GetName sibling is not affected. (2) I removed the wrong GetNearbyWanderPosition name note from NOTES.md and wrote down that the landed header spelling (QBE, reference out) is correct. No NAME_FIXES entry existed for it. (3) Slot 20 IsCombatStateGroup is now public in the VSLOTS overlay. I checked egor_pdb_names: ?IsCombatStateGroup@CAIStateGroup_HoverWander@@UBE_NXZ. hdr/README.md is updated. addmatch.py rewrote payload.json and oracle.tsv from $W/final.cpp.
- OPEN at hand-back: The source writes enum values as raw casts and literals, although the PDB enumerators exist in rebuild/include/engine/EAIAction.h and EMovementType.h. Replace `(EAIActionRepeat)1` with ACTION_REPEAT_UNTIL_FINISHED, `(EAIActionRepeat)2` with ACTION_REPEAT_CALLBACK, `!= 2` with AI_ACTION_FINISHED, `== 4` with AI_ACTION_IN_PROGRESS and `+ 8` with AI_MOVEMENT_HOVER_SLOW. NOTES.md says the row was ported onto the 'LookAtPos enums', but the source does not use them.
- OPEN at hand-back: The overlay class in hdr/engine/CAIStateGroup_HoverWander.h declares only Update. FableWin.pdb (_fablewin_api.tsv 0x01811FCD) also proves a public override ?IsCombatStateGroup@CAIStateGroup_HoverWander@@UBE_NXZ (vtable 0x0128C5D4 slot 20 = 0x00900C13), and the header omits it.
- OPEN at hand-back: The VSLOTS overlay puts IsCombatStateGroup on CAIStateGroup_EnemyCreatureBase. Neither Ego_r nor FableWin has a symbol for that function on EnemyCreatureBase, CAIStateGroupBase or HoverMovementBase; only the 15 derived classes have it. The overlay should mark this as an inferred placement, there to put slot 22 in the right place. Its README also says 'only GetCombatMinDistance proven in Ego_r', which is stale: Ego_r proves Init, IsRunnableLeaf, OnActivateLeaf, CleanupLeaf, GetCombatProximity, GetCombatMinDistance and IsRequireDirectAwarenessOfThing, all with the declared MAE/MBE access and const.

