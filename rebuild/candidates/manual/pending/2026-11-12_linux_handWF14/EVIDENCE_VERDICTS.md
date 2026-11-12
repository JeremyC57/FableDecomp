# Workflow wf14 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00959800 -- VERIFIED, author attempts 1

- What made it match: The repair kept the original recipe: shared headers with the ROUGHAREABODY noinline bodies. The only change is that GFRandom now comes from the shared header instead of a local copy. It is still a plain inline, so VC7.1 still expands the first call site and calls 0x004CDAE0 at the second.

## 004c9920 -- VERIFIED, author attempts 1

- What made it match: Moving onto the shared CThing, CThingManager, CPlayer, CThingPlayerCreature, CTCCarryable and CTCDParticleEmitter headers, with PeekTC used for every component lookup. Interface 0x4b is TCI_PARTICLE_EMITTER, read through the inline PeekPThingAttachedTo / IsAlwaysUpdating (the old draft had it as a 'follower'). Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/004c9920/final.cpp, test.cpp, payload.json, oracle.tsv, hdr/README.md, NOTES.md. NAME_FIXES.tsv: 0x00705170 should be ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ, not GetCarrier / GetCurrentSelectedEntryData.

## 00760e70 -- VERIFIED, author attempts 1

- What made it match: Unchanged from the original match. The repair only made the access private:, and the bytes still match.

## 006ac430 -- VERIFIED, author attempts 1

- What made it match: The template GetDef<CExpressionDef> body is visible to the compiler but kept out of line, through the header opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY. The other fixes that made the original match are unchanged: CDefIndex DummyObject and the GetTC<T>(T**) form.

## 0076c9a0 -- VERIFIED, author attempts 1

- What made it match: Used the same preamble as the landed same-TU CTCEnemy::CTCEnemy 0x0076C8A0 (shared headers, with the GetSubDef<T> and GetDef<T> bodies visible under auto_inline(off)), so VC7.1 keeps the enemy-def handle in ebp and reuses its slot for differs. Also used CFaction::GetName() returning CCharString by value (FableWin.pdb ?GetName@CFaction@@QBE?AVCCharString@@XZ 0x02444310; one source only). It is used both as the operator!= argument and in the ternary `PFaction ? PFaction->GetName() : CCharString("")`, with `bool differs = PFaction && GetSubDef && name != GetName()`.

## 00af5430 -- VERIFIED, author attempts 1

- What made it match: Unchanged from author (shared headers + CDecal inline accessors overlay, __copy specialization declared, FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER); repair touched only comment and test.

## 00b68da0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The guard in the real PDB inline, with a constant true argument, already gives retail `mov al,1 ; je ; xor al,al ; cmp al,[ecx+0CEh]`. So the operand-order opt-in was a stand-in for the missing guard and is no longer needed. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/00b68da0/final.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/00b68da0/test.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/00b68da0/hdr/engine/CEngineLightingManager.h
- OPEN at hand-back: Overlay hdr/engine/CEngineLightingManager.h is not repo-header-plus-added-lines: `diff rebuild/include/engine/CEngineLightingManager.h $W/hdr/engine/CEngineLightingManager.h | grep -c '^<'` is 1, because the default-path comment at line 153 ('setters with no out-of-line copy in retail ...') was rewritten in place. That is a comment, not a rewritten `#if` line, so the README note does not cover it. To fix it, keep the original line and add the correction as new lines, or as lines inside the FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD block.

## 00941690 -- VERIFIED, author attempts 1

- What made it match: Moving onto the shared headers made the board lookup match: the expanded GetTC result is never read, and VC7.1 still keeps the inline LowerBound loop while dropping the select and the store. Writing `ret = AIAction_PerformActionAndWait(CCreatureAction_PlayHopscotch(GetCreature()), false); if (ret != AI_ACTION_FINISHED)` puts the result in edi before the destructor call 0x693EF0. Comparing with `!= 2` inside the full expression instead makes a setne bool and caches 1 in ebx.

## 009c4440 -- VERIFIED, author attempts 1

- What made it match: The verifier was right. The explicit file.operator->() call was never needed: the natural file->SrcPath / file->DstPath spelling also grades RELOCATION_MATCH, so the source now uses it. The NOTES claim that the call fixed the spill order at 0x9C4454/0x9C4458 was wrong. The Shape comment now cites retail bytes for the other choices, and probes back both: a map-reference local hoists mov edi,[esp+0x10]; add edi,0x94 above ToCharString (DIFFER), and folding find into the if gives 615 v 609 bytes (DIFFER).

## 009f20b0 -- DIFFER, author attempts 3

- What made it match: Not matched. Proven with c2prio: loop 1 becomes byte-exact once const 0's priority (-13) beats heap's (-18). In the full function, loop 2's inner zeros and the tail MemoryAuditBuffer=0 pull const 0 down to -162. Notes are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/009f20b0/NOTES.md and the best source is /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf14/009f20b0/differ/009f20b0.cpp

