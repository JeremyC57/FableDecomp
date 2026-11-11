# Chunk 2007 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2007, 0x007CFB3E / 0x007CFE3D) Retail `fmul ; fchs ; fadd` for `-(a*b) + c` comes from a named float holding `-(a*b)`; one
  expression (or `c + -(a*b)`, `-a*b + c`) folds to `fsubr`.
- (chunk 2007, 0x007CFB45) An inline callee (std::max<float>) expanded at a deep site that no retail-plausible control flow brings above
  VC7.1's threshold is measurable with the c2 inline trace (c1 19, f 0.0273, score 115): `/d2inlT116` as the row's `// Flags:` closes it,
  and the expansion also fixed the callee-saved colouring (def / input_process ebx / ebp) that the out-of-line call had swapped.
- (chunk 2007, 0x006DF0BE) A CDefString passed to a `const CCharString&` parameter as an implicit conversion re-takes the temporary with
  `lea` (retail); `.GetString()` pushes eax and moves the frame. Retail then calls GetString 0x00415DD0, so the TU needs
  FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (plain inline calls the ??B COMDAT 0x00415D70: RELOCATION_MATCH but a wrong target).
- (chunk 2007, 0x006DEEE0) Locals of a later branch declared at the enclosing block's scope conflict with an earlier sibling block's
  locals in the frame packer (their scope covers it): putting the play-game texts in the `else` of the page test saved a frame word (0xD8 ->
  0xD4); the packer probe (hand H40b phook.py via tools/pk.sh) shows each node's live set and offset.
- (chunk 2007, 0x006DE95D) Retail's list walk `end in ebx, next in memory, jmp into an aligned loop head with a back-edge stub` is
  `for (it = l.begin(), end = l.end(), next = it; it != end; it = next) { ++next; ... }`; `*it++`, `cur = it++` and `next = it; ++next`
  (with or without a cached end) all colour it / end the other way round.
- (chunk 2007, 0x006DE8C0) An all-done tail written as `if (n <= 0) { ...CWideString text... }` instead of `if (n > 0) return; ...` lets
  the text share a frame word with the loops' temporaries (frame 0x38 -> 0x30).
- (chunk 2007, 0x006DED90) A same-TU private helper (CTCTrophy::PlaySoundW, landed 0x006DDCD0) that retail expands at one site and calls
  at another (0x006DF3B1, GlobalUpdate's TU) needs its body in the row's TU; declaration-only calls it.
- (chunk 2007, 0x006DED69) `dec [ecx+4] ; jne` releases of a GetSubDef<T> handle need FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (and
  engine/CDefClassBase.h included before CDefinitionManager.h, else C2039 on GetSubDef).
- (chunk 2007, 0x006658D6) `fld Y ; fld X ; fld st0 ; fmul st1 ; fld st2 ; fmul st3 ; faddp` before a call is
  C3DVector::GetSquaredMagnitude2D() (or `X*X + Y*Y` written out); a C2DVector(X, Y) temporary is copied through integer registers and
  squared after the calls.
- (chunk 2007, 0x00665CEB) `fcomp [m] ; test ah,41h ; je else` is `if (!(a > m)) then else`; `a <= m` gives `jp` (NaN goes the other way).
- (chunk 2007, 0x006658F6) `PeekComponent().GetConstantFPS()` (a static called through an object expression) keeps retail's
  PeekComponent call whose result is unused, then reads ConstantFPS in place.
- (chunk 2007, 0x00665C94) GetTC / PeekTC pick the non-const / const LowerBound (0x0040F020 / 0x004365B0): a const component pointer where
  retail calls 0x0040F020 still grades RELOCATION_MATCH; only rel.py / vland shows the wrong target.
- (chunk 2007, 0x0066591A) Two independent stores (update.PercentageComplete = 0, IdleCounter = 0) are scheduled in source order:
  declare the update first.

Review at landing:
- 0x007CF9A0 carries `// Flags: /d2inlT116` with an `// Inline-budget:` note (owner decision 2026-11-08, preference 2).
- 0x006DEEE0 uses FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION with an `// Inline-budget:` note (preference 3, the 0x008A8C60 precedent).
- 0x006DE8C0 defines CTCTrophy::PlaySoundW (landed 0x006DDCD0) in its TU as a same-TU body; 0x007CF9A0 defines
  GetSubDef<CTavernGameShoveHaPennyDef> (landed 0x007D1260) likewise (the chunk 2002 sibling's pattern).
- The catalog label of 0x006DEEE0 changes from `CTCTrophy::vfunc_26` to `CTCTrophy::GlobalUpdate` (finish.py's oracle.tsv already says so).
- Respell the landed rows 0x008302D0 (`?Run@T@@QAEXXZ` -> CThingAICreature::SetStateGroupEnabled) and 0x008EF910 (UpdateCoinSounds takes a
  bool), and the bound spellings in NAME_FIXES.tsv.
- The overlay depends on chunk 2002's hdr/ opt-ins (CTCTavernGame.h, CRightHandedSet.h, CInputProcessTavernGame.h).
- The global 0x013B8774 keeps the established but probably wrong spelling `g_primitiveDrawDistanceScale` (used here as an animation gap
  rate); review when a data-symbol dump exists.
