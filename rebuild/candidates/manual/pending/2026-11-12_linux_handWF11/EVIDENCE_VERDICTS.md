# Workflow wf11 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 008da410 -- VERIFIED, author attempts 1

- What made it match: The row now sits on the shared headers. `CTCCombat* pcombat;` is declared uninitialised right before `GetParentThing().GetTC(&pcombat)`, and the setter called is the PDB inline SetIsReadyForMelee(true). Both effect names are passed as plain literals that convert implicitly to const CCharString&, not as explicit CCharString("...") temps.

## 0079cc80 -- VERIFIED, author attempts 1

- What made it match: The match comes from the earlier work and is unchanged: GetSubDef is written in both arms of the effect==0 branch and VC7.1 hoists it, and arm 2 passes the new effect straight into the CIntelligentPointer temporary. This repair adds only a comment, and the bytes still match.

## 00aabd50 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Unchanged from the author's version. Comment-only repair: descriptive-name markers, NAME_FIXES.tsv hand-back, and a faithful pitch-fader test.
- OPEN at hand-back: Faithfulness: the hand-hoisted pitch-fader loop (`if (pitchBits) { int bit = FindFirstSetBit(bits, 16); do {...} while (pitchBits); }`) is compiler-steering that the retail bytes do not require. The natural form that mirrors the gain loop (`while (pitchBits) { int bit = FindFirstSetBit(bits, 16); options.SetFlag(bit, true); pitchBits ^= 1 << bit; }`) also grades RELOCATION_MATCH ($W/verify_evidence/whileform.cpp; see $W/verify_evidence/ATTEMPTS.log). The row should use the plain while form, and its 'hoisted: kept as written' comment should be corrected.
- OPEN at hand-back: NAME_FIXES.tsv leaves out 0x00AAC870. It is landed as CCountedPointer<CUnprovenPointee_00aac870>::~CCountedPointer (rebuild/src/compiled/00/aa/DestvalstdallocatorCCountedPointer..._00aac870.cpp), but this row proves the pointee is CSoundEmitterLH::CActiveSound. Its only two callers are 0x00AABF30 (this row) and 0x00AAC1A7 (PlayAtmos), and both release the CCountedPointer<CActiveSound> temporary that 0x00AAC820 builds. The row uses that spelling, so one address now has two names.
- OPEN at hand-back: The CActiveSound ctor 0x00AAB8B0 fix (landed leaf `??0CActiveSound@` with args (void*,void*,const char*,void*,bool,bool); real args (long, CASoundBank&, const CCharString&, float, bool, bool)) appears only in NOTES.md and was not handed back as a NAME_FIXES.tsv row. The caller's pushes at 0x00AABEF4-0x00AABF09 (esi handle, edi bank, ebp name, fstp float, the Is3D and HeadRelative bytes) and the Ego_r/FableWin copy ctor ??0CCharString@@QAE@ABV0@@Z at 0x0099EC30 support the fix, so the row should go in NAME_FIXES.tsv so the coordinator respells the landed row.
- OPEN at hand-back: FindFirstSetBit (0x00A3F860) is a made-up descriptive name (its landed row says 'Descriptive name (no PDB name found)', and it is absent from egor_pdb_names / the FableWin signatures). NOTES.md's 'Made-up names (complete list)' does not include it, and the row does not mark it (descriptive).

## 009810b0 -- VERIFIED, author attempts 1

- What made it match: The real body of the same-TU CCreatureAction_SendMessage ctor (0x00982210) sits in the row file as `__declspec(noinline)`. With that body visible, c2 knows &message does not escape, so the by-value message handle is released in place (`dec [ecx+4] ; jne`). Also: `CIntelligentPointer<CThingAICreature> guardPointer; guardPointer.Set(guard);`, plus `const CThingAICreature& creature = GetCreature(); CThingAICreature* guard = 0; const C3DVector& pos = creature.PeekPos();` before the search. The intelligent-pointer conversion for the flee argument is inline.

## 00a44f40 -- VERIFIED, author attempts 2

- What made it match: Three source changes. (1) The miss arm rebinds the source element in its own scope before the bone morph is built: `const C3DSkeletalBoneMorph& added = target.BoneMorphs[t];`. VC7.1 assumes a pointer bound before a local's scope opens cannot alias that local. That assumption let it hoist the source float loads above morph's stores and drop the ctor's `BoneNameCRC = 0` store. Rebinding inside the scope keeps both, and ebx is still reused. (2) A named local `const unsigned long crc = source.BoneNameCRC;` at the top of the loop body, used by the inner while and the hit test. This fixed the esi/edi swap and the direction of `cmp [eax+4],ecx`. (3) `++i;` as its own statement after the insert.

## 00d38d20 -- VERIFIED, author attempts 1

- What made it match: Shared headers (StartRound's include set + FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, inline CTimer defs over pDSTGame, GetSquaredDistanceBetweenThings), plus a local `TRetVoid _Fail_Val;` declared first and every return spelled `return _Fail_Val.Return();`. That fixed the order of the exit cleanup blocks.

## 00704860 -- VERIFIED, author attempts 1

- What made it match: The match comes from porting onto the shared headers, using a named CScriptThingInfo local, inlining FindScriptThing and writing found/not-found as an if/else so the two CScriptThing locals share slot +14h. Reset and SetImplementation are plain out-of-line calls through their header declarations; no body-visibility opt-in is needed.

## 00a5b370 -- VERIFIED, author attempts 1

- What made it match: The source was already a match; the repair only renames the enumerator, which does not change the bytes.

## 00e36750 -- VERIFIED, author attempts 1

- What made it match: The source declares TRetVoid _Fail_Val; as its first local and writes every exit as return _Fail_Val.Return(); (the script_language.hpp idiom used by 97 landed rows). With a plain return; the compiler emits the same code but moves the shared post-wait/in-loop exit block and the once-expanded handle destructor block to the end of the function (541 v 536 bytes). No pragmas and no out-of-line opt-ins are needed. Files are in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf11/00e36750/: 00e36750.cpp, test_00e36750.cpp, payload.json, oracle.tsv and hdr/engine/CV_ArcheryCompetitionScript.h.

## 00773ee0 -- VERIFIED, author attempts 1

- What made it match: I rebuilt the row on the shared engine/STLport headers with the real PDB identities. The old local-model residue (this spilled from ebp, the two epilogues merged) came from errors in the draft's model, not from a compiler tie. The opt-ins that mattered are CDEFCLASSBASE_GETSUBDEF_NOINLINE, STLP_VECTOR_PUSHBACK_NOINLINE, STLP_VECTOR_COPY_OUTOFLINE and STLP_RANDOM_SHUFFLE_2ARG, plus the base opt-ins CTCSHOP, CTHING, CTCBASE, DEF_CLASS_BASE, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CSTOCKITEMDEF and the others they need. It needs no overlay headers and no flags. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf11/00773ee0/CTCShop_GetSaleItemsToPlace_00773ee0.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf11/00773ee0/test_00773ee0.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf11/00773ee0/payload.json. Possible NAME_FIXES item: the landed 0x00777080 leaf is a placeholder, random_shuffle&lt;CUnprovenElement_00777080**&gt;; this row calls it as std::random_shuffle&lt;long*&gt;.

