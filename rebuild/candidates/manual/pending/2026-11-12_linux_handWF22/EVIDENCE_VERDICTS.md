# Workflow wf22 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 007e1fc0 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged and still matches. The one change is a README note explaining the #if-to-#elif rewrite in the C3DVector.h overlay.

## 004bd6e0 -- VERIFIED, author attempts 1

- What made it match: These were type and name changes only, so the bytes did not change. CArray has the same 12-byte layout in retail, and renaming the global only changes a relocation symbol.

## 00a24520 -- VERIFIED, author attempts 1

- What made it match: Moving onto the shared headers fixed the order of the global pool swap. Writing `ok &= prim->Upload...()` in each of the four arms, instead of a shared `result` local, made VC7.1 spill the primitive index into the dead `flag` home slot (as retail does) rather than spilling `ok`. That also fixed the esi/edi choice for bank and index. uploadFlags stays as `= 0; if (MapFlags&2 && DP3BumpEnable) = 2;`.

## 0054fbc0 -- VERIFIED, author attempts 1

- What made it match: Moving the function onto the shared headers fixed the callee names and the release idiom (CIVCountedPointer RELEASE_MEMBER plus CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES). Four source-level facts then closed the gap: (1) the text-id test is `textId > 0`; (2) the plain-entry arm comes first and copies GetText into a named local `CWideString text` before `Value = text`, so the two plain arms are identical and the compiler merges them; (3) the space/newline join is an if/else; (4) the game group loop keeps a `const CArray<long>&` to the group's Entries and reads `ids.begin()[i]`, while the front-end loop copies the CArray<long> and reads `ids[i]`. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf22/0054fbc0/SetValue_0054fbc0.cpp and /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf22/0054fbc0/test_0054fbc0.cpp; payload.json and oracle.tsv written by addmatch.py.

## 004c0560 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The source is unchanged. It uses the shared HelperPoint body with same-TU inline GetAllMeshes and IsResourcesLoadedForMesh, an inline C3DMeshObject ctor and dtor, and a while-loop mesh iteration.
- OPEN at hand-back: The source says GetAllMeshes and IsResourcesLoadedForMesh are 'Same-TU real inline bodies', but it never names the landed row they were copied from. Both bodies are verbatim copies of landed 0x004BE3E0 (NGraphicAppearanceCTCGraphicAppearance_IsResourcesLoaded_004be3e0.cpp, lines 34-48), and only NOTES.md says so. The source cites 0x004BD420 for GetAllMeshes, but that landed row is spelled differently (non-const, and it calls component->GetAllMeshes rather than GetMeshesUsed). For IsResourcesLoadedForMesh it cites only 0x004BC090, which has not landed. Add 'copied from landed 0x004BE3E0' to the comment for both bodies.

## 0063b3f0 -- VERIFIED, author attempts 2

- What made it match: (1) Read the Y-cell compare through the FableWin.pdb inline C3DVector::ToC2DVector(): `if ((long)from->ToC2DVector().Y == toY)`, using the existing repo opt-in FABLE_RETAIL_C3DVECTOR_TO2D. The 2D temporary gives the from->Y load a different value number, so VC7.1 no longer reuses that __ftol2 result for yHit.Y. Retail calls __ftol2 again at 0x0063B489. (2) Removed the floorX float local: `xHit.X = (float)fromX;` and `0.0f == from->X - (float)fromX`. With that, the fild temp, dY and the int-to-float result land in retail's slots (+14h, the dead from slot, the dead to slot), and `xHit.X += 1` reloads xHit.X. (3) Tail: `valid = true;` at the end of the crossesX branch and of the crossesY branch, then `if (valid && !IsPosInMap(*segStart)) {...}` in place of goto done. This gives retail's bool slots (valid +11, crossesX +12, crossesY +13) and its block layout, where the both->xHit path falls into the IsPosInMap block.

## 00f3a890 -- VERIFIED, author attempts 1

- What made it match: The row was moved onto engine/CRangeCompressor.h and engine/CArray.h, with FABLE_RETAIL_STLP_VECTOR_C and inline out-of-class GetBlockSizeFromFlags, CalcHeaderSizeNeeded and CalcBitsNeeded. That alone still graded 844v857. The fix was to put the real landed bodies of the same-TU callees (CalcMemoryNeededBitCompressed 0x00F3A270, TestRangeBias 0x00F3A5D0, TestRedundantBitStrip 0x00F3A710) before the row under #pragma auto_inline(off). With those bodies visible, VC7.1 knows the callees do not keep the CScriptEntry& references. So entries[0].Flags stays in ebp across vector::operator=, entries[1]'s constructor store is dropped, and `this` is spilled. A probe with throwaway bodies confirmed this first; the real bodies replaced them. The last step was CalcBitsNeeded in the `bits = bit + 1; break; return bits;` form, which also fixed the loop's frame-slot layout. Things that changed nothing: STLP_VECTOR_C alone, an explicit instantiation of operator=, and calling vector::operator= directly.

## 009c9280 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The match is unchanged from the original author's work. The repair changed only comments and the test.
- OPEN at hand-back: Overlay hdr/engine/NGameText_CDataBank.h is not 'repo header plus added lines only': `diff rebuild/include/engine/NGameText_CDataBank.h $W/hdr/engine/NGameText_CDataBank.h | grep -c '^<'` = 2 because the author rewrote two existing comment lines (35-36, the CGroupEntry +0x1C note inside the FABLE_RETAIL_NGAMETEXT_GROUPENTRY block) and hdr/README.md does not explain it; restore the original two lines and put the new struct_layouts_egor.tsv citation in an added line instead.
- OPEN at hand-back: The test cannot fail when the first pick's loaded fast path is removed: mutant $W/verify_evidence/mC.cpp (`if (IsLoaded(GetTextInfo(id)) || allowPending)` -> `if (allowPending)`) still prints 009c9280_TEST PASS, because the fallback loop starts at the same index and returns the same entry. The only observable difference is a spurious BeginLoadingText request, and the test never checks it. Add a check that picking a loaded entry with allowPending=false leaves loadRequests unchanged (for example on group 5 with "GUARD", or on group 9).

## 00646f67 -- DIFFER, author attempts 3

- What made it match: Not matched. Best source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf22/00646f67/differ/00646f67.cpp. It is the pass-2 version, unchanged; line 2 carries the residue. This pass is recorded in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf22/00646f67/NOTES.md.

## 00f3ba70 -- DIFFER, author attempts 3

- What made it match: not matched: Still 8 hunks, and they are all in the inlined push_back growth path (_M_insert_overflow, __true_type). Retail puts len in ebx. It also treats the list as a separate value there: it reloads it into ebp from [esp+6Ch] after the malloc, then restores the outer `out` value in ebx at 0xF3BD0E. Retail also has a `jmp; lea esp,[esp]` loop-head pad before 0xF3BCF0. Ours keeps the outer list in ebx and puts len in ebp. The pad and the register choice can be produced separately: probe ovT gives the pad but keeps our registers. Everything else is byte-identical.

