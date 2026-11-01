# Pass10 compiler and evidence findings

This is a working record, not a completed handoff. STATUS.tsv and each accepted row's reviews/<address>/RESULT.json are authoritative. Only independently recompiled, binding-reviewed, dual-tested rows count as VERIFIED_MATCH. Recut diagnostics do not count against the original assigned span. All new work remains local; no repository operations are authorized.

## Recent confirmed fixes

- 004fc760 CWorldMap::DrawGetMyPlayerMapNumber: remove the whole-record alias, read the indexed Set field, and separately bind Pos. The original player pointer, player number and position-reference types restore the native base/index choice. 85 bytes; 1800 cases per compiler, with direct and secondary-interface dispatch checked.
- 00a65ff0 CMemoryAllocatorFixedSize constructor: computed fields and PCurrentAllocatorChunk/ExtraMemUsedByOS/NoEmptyChunks belong in the initializer list, but PFirstChunk/PLastChunk zeros remain in the body. This restores native store order. Restore the original virtual overrides as well. 169 bytes; 5040 cases per compiler.
- 0041db84 NUISystem::CManager::ShowErrorMessage: a switch case for error zero, instead of an if, restores three implicit string-temporary frame lifetimes. Original same-TU GetTextBySymbol is also required. 281 bytes; 512 cases per compiler; all 15 bindings checked.
- 00492fb4 CSubtitleRenderer::SetText narrow wrapper: the original function has no named wide-string local. Passing the empty wide literal as an implicit conversion keeps all five temporaries in the same full expression. 149 bytes; 1152 cases per compiler; 11 calls checked. Its donor native-wchar spelling versus the package-required unsigned-short ABI is explicitly documented in its row, not silently equated.
- 006b3030 CEnvironment::UpdateWind: load the real X/Y/Z values into named scalar locals, then accumulate Z squared, Y squared, X squared sequentially. This restores both x87 operand sequences and the delayed EDI restore. 619 bytes; 1440 cases per compiler; seven bindings checked. The seed is a function-local static in both original PDB module scopes. GFastOneOverSqrtTable has 128 dwords, not 512 elements. Its native runtime initializer, not executable file bytes at a BSS virtual address, supplies the fixture values.
- 00a69c20 CTextureManager::AllocateVolumeTexture: CIVCountedPointer::Reset releases then adopts. The caller separately invokes AddRef through out.Data.Get(). Moving AddRef into Reset or using the earlier raw texture pointer loses six bytes and changes registers. 186 bytes; 1088 cases per compiler; four bindings checked. Original private fields and private CPreallocTexturePool inheritance are preserved.

## Verification conventions

A raw PDB name set is incomplete for local statics and globals: inspect module-scoped DATA32 and public/type records when necessary. Check all exact emitted symbols with objdump. A symbol's matching spelling alone is insufficient; decode each retail call/data operand and independently establish the identity.

Use the original byte span and SHA-1 of the executable. A relocation-masked diff can become nonsense when instruction offsets shift. Inspect raw disassembly to locate the first real difference; never infer a match from equal length.

Fixtures retain the selected target body. Any test-only host-width adaptation, unused-class reduction or boundary stub must be explicit. Test count means cases executed per compiler, not a number of distinct tests. A fixture PASS for a parked row is useful behavior evidence and never a byte-match claim.

The per-row immutable history/attempt_NN directory and grade_NN.json record every compiler trial. The user removed the numerical attempt cap for this pass. Previous strategies must still be checked before trying a new one, and the closest selected source must not be replaced merely because another candidate has the same length.

## Verified54:007af0f0 CTCRockTrollShield::OnHitAtPos

Original hitPos copy after GetPos restored frame/register roles; plain float normalization factor restored size; sequential Dot Y,Z,X accumulation removed the final x87 pair mismatch. Final grade35 is533bytes with7 checked bindings and1944 cases per compiler. Independent review corrected a prose-only69/70 physics overload reversal in00894500; its accepted code was already correct. Online compiler research is recorded in research/RESEARCH_NOTES.md; the later parser result below credits the first successful research lead.

## Verified55:0057f9e9 marriage binary reader

Update the original StreamPos member with +=no_bytes after the copy, leaving overflow_check as validation only. Assigning the cached validation value back to the member removed two native MOV loads in this caller. Selected25 is413bytes with4 independently checked private overflow-helper bindings and4928 cases per compiler plus rejected-read guard. ReadSLONG uses ReadType<int> only in the GNU64 fixture to preserve four-byte wire width. See independent review for boundary limits.

## Verified56:009b9c60 CStringParser::SkipWhitespace

Repeat the complete `(p - String) - oldPosition` expression in MoveStringPos and return. Caching the endpoint or count gave subtraction-before-copy; repeating the expression produced retail’s copy-before-subtraction. Grade46 matches124bytes with the one CRT binding independently decoded, and7680 cases pass per compiler. Source advice came from https://miminya.me/projects/2ezdecomp/cookbook/ ; PDB and executable evidence establish the result. No prohibited qualifiers or alternate calling convention were used.


## Final verified57 and diagnostic recovery

00c35e30 matched through original GameCODA API/types and IsCreated/IsEmpty accessor composition. See START_HERE_CLAUDE.md for SDK version restrictions, SIMD limitations, file-installer name corrections and final disposition. This file preserves chronological findings; final manifests supersede earlier working-status wording.
