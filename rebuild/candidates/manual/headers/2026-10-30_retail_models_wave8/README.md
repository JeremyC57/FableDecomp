# Retail class-model migration, wave 8 (2026-10-30): 1,384 landed rows onto shared retail headers, bytes unchanged

Brief BRIEF.md; per-row ROWS.tsv; REVERTED.tsv (94 family edits, each from its own pre-image); HEADERS.md; NEXT.md (wave 9
counts and plan; the CScriptThing remainder by shape); LESSONS.md; NAME_FIXES.tsv (57); bind_add.tsv (68 respelled lines,
same targets); logs/ (recheck1..5.log, mcheck and apply logs); scripts/.

Headers added: `engine/CActiveEntityScriptBase.h` (: CSpawnedFuncBase, 0x58 bytes, vtable 0x012C3594 without RTTI, stored
by the ctor 0x00CE1110; slots 5 OnPredicateFail 0x00CE1090 and 6 OnInterrupted 0x00CE10A0; members PEntityScript +0x34,
Flags +0x3C, EntityUID +0x40, Entity +0x48; implicit dtor; both PDBs agree) and `engine/CActiveEntityScript.h`
(template <class T>, 0x60 bytes, Func `void (T::*)(CScriptThing)` +0x58, PParentScript +0x5C, both ctors inline, slot 4
SuspendableProcess). T for all 120 SuspendableProcess rows was recovered from the single site that stores each RTTI-less
vtable, the inlined ctor in CEntityScriptBinding<T>::CreateActiveEntityScript, and confirmed against both PDBs.
Headers changed: `engine/CEntityScriptBinding.h` Func is `void (T::*)(CScriptThing)` (Ego_r pmf, FableWin `P823@AEXV1@@Z`;
all 6 includers identical); comment-only notes in CTimer.h (SetTime/Set are `operator=(long)`, Ego_r `??4`, no PDB has
SetTime) and CScriptGameResourceObjectMovieBase.h (the per-site inlining finding).
Rows per family (a row can carry several): CScriptThing 399 + 89 (non-template PImp first) + 9 (local CBase -> CBaseClass.h),
CPlayerManager 383 (methods-only stand-ins onto the header), CTimer 185, CEntityScriptBase second pass 173 (namespace
placement, derived base words), SuspendableProcess 120 (shared template body + explicit instantiation; rows declare
`class __single_inheritance T;` so the member pointer stays 4 bytes), binding rows 85, predicates 85, MovieBase scaffolding
removed 14 (VC7.1 reproduces retail's per-site mix by itself with the implicit dtor and a plain inline release in 15 of 24
rows), master-data T respellings 6, the 5 out-of-line CLevelScriptBase<T> ctor rows via explicit instantiation (closes wave
7's unproven item).
Agent recheck 1,384/1,384 PASS (1,364 RELOCATION_MATCH, 20 MATCH); mcheck 1,150 SAME + 234 RELOC_RENAMED (180 leaf
spellings now PDB-exact, 47 callee respellings with the same target, 7 `$L` renumberings); payload_check 0 stale.
Coordinator: clean cherry-pick onto 7a56ee86e; independent spot regrade 186/186 PASS (157 edited rows across all 12 families + 29 unedited includers of the changed headers; coordinator_spot_recheck.log);
multi-check 0. 57 NAME_FIXES rows queued for naming wave 11 (prefix mig8: 55 entity classes to nest in their NScript script
classes, CQuestInfoElementTimer nested in CDrawQuestInfo by reference, 00efbe20 RegisterFilter -> SetPredicate; unresolved:
00dd8ba0 CTortureDoorCourtyard has two PDB owners, 00dcf4c0 CVtable_012DB4B8 is invented).
Reverted 94: CScriptThing 30 (std vector/list arity, IsEqualTo argument types, PImp_Data uses, CGSI conversions),
CEntityScriptBase 23, CPlayerManager 19 (a const-only stand-in whose non-const call sites would bind the QAE GetMainPlayer
0x00449970 instead of retail's QBE 0x00449960: LESSONS 6, read every RELOC_RENAMED), predicates 14, others 8.
Open: MovieBase single-site rows that call 0x006E7AB0 still expand fully (0x00DF2D30 385 v 349); a shared
engine/CCountedPointer.h is the next lever (local copies 3,987 -> 4,290: migrated rows need a minimal one); CGSI 220 not
reached; 35 namespace CreateActiveEntityScript rows keep an older body shape.
Remaining local models: CScriptThing 428, CEntityScriptBase 293, CPlayerManager 104, CScriptBase 88, CIsThingAlive 154,
CPredicateBase 142, CIsThingConscious 27, CTimer 16, MovieBase 85, CGameScriptInterface 220.
