# Workflow wf16 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 007e4e30 -- VERIFIED, author attempts 1

- What made it match: Renaming the enum value changes no code, so the earlier match still holds (/GS, opt-in FABLE_RETAIL_CTCBETTING_FRAMEUPDATE)

## 007e8590 -- VERIFIED, author attempts 1

- What made it match: Unchanged from the author's version: shared headers with the opt-ins, GetScriptData, the block-scoped nav name, and the GetParentThing() reference accessor. The repair's GetType virtual only fixes the vtable model and leaves the code bytes the same.

## 00922010 -- VERIFIED, author attempts 1

- What made it match: The row uses only the shared headers: CTCAIScratchpad (ScriptingStateGroup / TargetName), GetCreature().PeekPos(), GFRandom, CIntelligentPointer, CThingFilter_IsNot / HasScriptName, CFilter_Bind3, and the same-TU noinline GetPBestThing / GetAllThings bodies. Each search block has a named CCharString copy and a filter built from it. The group is read into a local `EScriptingStateGroups group` for the 8/9 if, which gives retail's re-test `cmp eax,8` and lets mode 8 fall through. The last residue was the late Nearest Pos-copy stores; it went away once FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE was left undefined. With CThingCompare_Nearest::operator() visible, the caller knows the comparator temporaries are not captured. NAME_FIXES.tsv records that the landed IsRunnable 0x00921630 uses a made-up local CTCWalkToMarker{Mode, MarkerName}; the real names are CTCAIScratchpad{ScriptingStateGroup, TargetName}. Files are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf16/00922010/: 00922010.cpp, test_00922010.cpp, NOTES.md and NAME_FIXES.tsv.

## 00af5090 -- VERIFIED, author attempts 2

- What made it match: The "zero-register" residue was really a control-flow mistake in the model. fg.py hides it because it normalises jump targets. In retail, when the `PEngine && !handle.IsNull()` test fails it jumps to 0xAF5256 (the ++group), so particle->ClearParentHandle(group) belongs inside that if, after the inner GetPrimID loop. Moving the call there gave RELOCATION_MATCH. The strict-headers regrade is also RELOCATION_MATCH, the vt.py test passes, and addmatch.py wrote the row. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf16/00af5090/final.cpp (also saved as differ/00af5090.cpp). Test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf16/00af5090/test.cpp

## 00eb2140 -- VERIFIED, author attempts 1

- What made it match: I moved the row onto engine/CV_MurderTwistFinalScript.h, following the landed CMurderTwistAssassin::Main row (TRetVoid _Fail_Val, the SetPredicate(CIsThingAlive(Me)) temporary, the out-of-line ScriptedThingBase assignment opt-in). I fixed the callee names to IsValid, DoRemindingHeroToKillAssassin, DoAskingHeroToSpeakToBossAgain, DoCongratulatingHero and AttackHero. I also added a reconstructed dead conditional, `bool dead=false; if (dead && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {}`, placed exactly before the first SheriffState wait loop. It is marked Inferred in the source. Placed anywhere else, it swaps registers in the Guards[ID] computation.

## 009f3b00 -- VERIFIED, author attempts 1

- What made it match: Three changes together: (1) the default boxes are C2DBoxI::Initialise(0,0,...) calls, so both inline GetDimensions descs in the both-null arm stay alive in one expression and the frame becomes 0x9C; (2) the real Lock body (0x009F33E0) sits in the same file out of line under #pragma auto_inline(off), plus a named CPixelFormat format = GetPixelFormat() local; (3) the row copy is the indexed loop memcpy(to + y*Pitch, from + y*sourcePitch, rowBytes).

## 005c71aa -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The code is unchanged from the matched row: the 0x005C6B26 sibling form plus FABLE_RETAIL_STLP_VECTOR. This repair changed only the comment and the hand-back files.
- OPEN at hand-back: The row writes its own out-of-line definitions of CDefClassBase::GetSubDef<T> and CDefinitionManager::GetDef<T> under #pragma auto_inline(off). The shared header rebuild/include/engine/CDefinitionManager.h (lines 156-190) already has the same bodies behind FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY and FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY. I swapped the local block for those two defines and the row still grades RELOCATION_MATCH (see $W/verify_evidence/hdrbodies.cpp and ATTEMPTS.log), so the copy is not needed for the bytes. It also risks a redefinition if another TU turns on the header switch. Replace the local template bodies with the two header defines, and point the comment at them.

## 009735d0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The match is unchanged from the original work: the row was ported onto the shared headers, the string args were passed as implicit CCharString temporaries, and Normalise was written with parenthesised squares in Z,Y,X order. The repair changed only the CastAssassinRush overlay layout (pack(1)) and the README text, and the row bytes did not change.
- OPEN at hand-back: Constructor signature disagrees with both PDBs: the overlay $W/hdr/engine/CCreatureAction_CastAssassinRush.h declares CCreatureAction_CastAssassinRush(CThingCreatureBase*, const C3DVector&, float, bool, long, CCountedPointer<CCreatureActionBase>), and the row calls it with `&GetCreature()`. Ego_r (egor_pdb_names.tsv 00905670) and FableWin (_fablewin_api.tsv 017c5939/029f92d0) both spell it ??0CCreatureAction_CastAssassinRush@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@M_NJV?$CCountedPointer@VCCreatureActionBase@@@@@Z. The first parameter is a CThingCreatureBase& (reference), so the declaration should take `CThingCreatureBase&` and the call should pass `GetCreature()`. A reference and a pointer pass the same bytes.
- OPEN at hand-back: The landed callee row 0x008CE1B0 (rebuild/src/compiled/00/8c/CCreatureActionCastAssassinRush_CCreatureActionCastAssassinRush_008ce1b0.cpp) has the wrong // Leaf: `...@@QAE@PAVCThingCreatureBase@@...UCCountedPointer_CCreatureActionBase@@@Z` (a pointer, and a stand-in struct type), against the PDB `AAVCThingCreatureBase` / `V?$CCountedPointer@VCCreatureActionBase@@@@`. DOSSIER.md section 3 repeats the wrong PAV spelling. NOTES.md does not hand this back as a NAME_FIXES entry with its two sources.

## 00803d60 -- VERIFIED, author attempts 1

- What made it match: Used the sibling's TU prelude and the real callee identities. C3DVector offset=pos-eye (3D, Z unused) fixed the stack slot layout. The range is GFFastSqrt(C2DVector(offset).GetSquaredMagnitude()) with C2DVECTOR_SQUAREDMAGNITUDE_YX and FROM3D (retail squares Y first). A named float fov, IS_THING_ALIVE(hit), and a named CIntelligentPointer<CThing> for the new target.

## 006d8f10 -- DIFFER, author attempts 3

- What made it match: not matched: Same as attempts 1 and 2: only where ebp is saved differs. Retail pushes ebp at 0x6D9064, just before the first StatIncreasePerHitType transfer, and pops it at 0x6D91A3, inside the last vector transfer between `sub eax,2` and `mov edx,edi`. Our build pushes ebp in the prologue and pops it in both duplicated epilogues. That makes the body 1 byte longer and puts the float traits slot at [esp+0x1c] where retail has [esp+0x18]. Every other instruction is identical. The best source is unchanged at /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf16/006d8f10/differ/006d8f10.cpp (line 2 holds the residue).

