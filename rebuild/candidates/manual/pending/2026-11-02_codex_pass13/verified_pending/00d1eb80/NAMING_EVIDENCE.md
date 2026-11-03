# 00d1eb80 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 18. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00d1eb80/NAMING_EVIDENCE.md

# 00d1eb80 naming, layout and extent

Both original PDBs identify the exact private virtual leaf ?Main@CGuardToHelpDefend@CQ_BreakSiegeScript@NScript@@EAEXXZ. Ego_r complete TPI1165e/1169c gives the complete CQ_BreakSiegeScript and nested CGuardToHelpDefend, with protected ParentClass reference+14 and PMasterData pointer+18, private GateGuard+1c, FollowingHero+1d, TimesSpokenTo+20, nested size24. Owner flags BanditSiegeStart and EndCutsceneDone are donor61/68 translated through the proven retail CLevelScriptBase to native4d/54. Header and full original evidence were established for00d1fd30; copied original evidence00d1eb80_types.json records the full class context.

Native factory00d1ea10 allocates24, copies owner Game+40 to Game+4, copies Me+8, stores vtable012cafa4 and both parent/data fields. Its slot1 at012cafa8 is00d1eb80. Init00d1eae0 independently clears guard state and TimesSpokenTo. Final ret00d1f508 followed by7 CC bytes and separate factory00d1f510 proves assigned2441-byte span. grade.strict_extract and objdump/symbols in attempt16 confirm the exact unique Leaf without padded extent adjustment.

Original function-local records00d1eb80_function_locals.json identify expression:CCharString, seh_me:CScriptGameResourceObjectScriptedThingBase, ThePrisoner:CScriptThing, _Fail_Val:TRetVoid and six independent HLStyleConversationIndex:long scopes. FableWin original donor confirms conscious predicate (not alive), case0/1/2/default switch, the physical-hit || (any-ability && !healing-ability) expression and the final constant frame loop. Native Thing slots3/21/27/29/41/42 independently identify GetDataString, MsgIsHitBy, MsgIsTalkedToBy, MsgExpressionPerformedTo and the two ability checks. The script's message text is "TEXT_QST_018_" + Me.GetDataString() + suffix; original prefix concatenation overload0099f690 and suffix0099f600 are both explicit/proven.

The exact default animation flag is bool NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations at01375748, supported by original PUBLIC32 and bool data records in evidence/00e25f70_helpers.json and independently by this native PlayAnimation argument load. The complete shared CQ_SunnyvaleMasterData model gives original public BreakSiegeNoAssistance at116; this row clears it after successful follow animation/conversation. The original EHeroAbility healing enumerator is14, independently matching the native argument.

106 selected COFF relocations are independently checked at their exact native operand offsets in RELOCATIONS.json, including all terminated string payloads, predicate/resource vtables, direct calls and the typed runtime bool. The19 virtual bindings separately read game and Thing vtables, preserving receiver identity. Every selected operand matches; no unresolved/folded binding is silently accepted.

The original signed-character Compare body and both string-equality layers are documented in evidence/00d1eb80_COMPARE_REVIEW.json and original helper types in00d1eb80_helper_types.json. Making Compare available while retaining its call permits native common string-data reuse. The final selection16 uses ordinary inline equality bodies, a plain Compare definition and /Ob1. The new header opt-in preserves all existing/default paths. Stronger annotation probes12..14 remain in immutable history, but no stronger annotation is active in the final source.
