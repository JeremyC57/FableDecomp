# Chunk 218 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU callee as a stand-in body.** When retail keeps a result object's count block in a register across the copy-out but ours spills it, compile the previous function of the TU above, under `#pragma auto_inline(off)`. Use its landed body plus the ctor it calls (LoadAnimFile 0x00A99420 above 0x00A99510; LoadMeshFile 0x00A9B640 above 0x00A9B740). VC7.1 then knows the object does not escape. The same works for non-const finder stand-ins, so their `ret` shape is known (0x00AC1020 GetMap, 0x00ABC080 CountVertices).
- **Bit fields split with `:0`.** Byte-granular `test byte ptr [x+0Fh]` / `[x+10h]` flag reads are unsigned char bit fields split with a zero-width field; one packed field tests the wrong byte. A flag passed as a bool shows up as `mov cl ; and ecx,0FFFFFF01h` (0x00C35770).
- **`/GS` buffer scope.** In a `/GS` body, the `_wsplitpath` / `_wfindnext` buffers are function-scope locals. Block-scoped ones let a dead temporary share their words and shrink the frame by 4 (0x00999850, frame 0x430).
- **Dead argument slots.** A copied CCharString used only for a lookup, written as a named local in its own block, lands in the dead by-value/ref argument slot and is re-addressed with `lea ecx`. An unnamed temporary pushes the ctor's eax instead (0x00A9F0D0).
- **An early failure exit is its own statement.** `Terminate(); return;` as the early failure exit: VC cross-jumps it into the common Terminate call and hoists `mov ecx,esi` above the `je` (0x0093D030).
- (0097da40) lesson -- the creature reached through an inline `GetCreature()` accessor rotates eax/edx in the wait loop; direct `((CThingCreatureBase*)PBrain->Creature)->` casts at each use match (found from the 0x0097DB70 sibling on the 4th grade)
- (0093ad70) an inlined component lookup whose result stays in the search register (esi) is the binary search written in place on ONE iterator variable; a separate inline lower_bound helper returning `first` copies it to eax (3 forms tried before)
- (0093ae80) the IS_THING_ALIVE result kept in a named bool lets an earlier `if (!f()) return false;` keep f's zero al (no shared xor exit); `p = T(x)` through an inline operator= that fetches GetPItem() into a local before SetPItem; the frame rate read into a named local before the frame call (held in edi). NB: filtering `call` lines out of a udiff can hide the only real difference (a lea scheduled around a call).
- the GetInterface end/hit select written as ONE ternary (`*ppOut = (cond ? last : it)->component`) loads the component straight into ecx on the hit path and the never-written miss value from the key word (0x0094E6B0, parked for registers only)
- (009207a0) an XY distance compared against a call result must be written straight into the comparison (`if (Dist(a, b) > tc->GetRange() * 0.5f)`): a named distance local lets VC sink the sqrt below the call and spill dx/dy
- (0096c880) a by-value C3DVector argument built from three named float sums (`C3DVector(x, y, z)` per call) lets VC home the third sum in a dead by-value argument slot; a C3DVector local takes three frame words. A turn-speed float literal must match the pool bits (0x3B360A13 is 0.0027777f, not 1/360)
- **Uninitialised out-pointer on the miss path.** When a loop's inlined GetInterface leaves the out-pointer unwritten on the miss path, VC treats the variable as loop-carried. It pre-loads its register from an earlier lookup (`mov edi,[esp+10h]` before the loop at 0x0095C0B0), and the retail miss behaviour is "keep the previous value". `CTCBoss* current = boss;` is the faithful source, but it did not fix the register choice.
- **Recurring lane gaps (not source-shape) in this band:**
  - Frame packing: a dead lookup key, a temporary or an uninitialised out-pointer should share a word with a later temporary. Examples: 0x00966290, 0x00956B00, 0x0094A2B0, 0x00977B10, and 0x00970DF0, where the away-vector temporary and the CFilter_Bind2 pair share words in retail.
  - Callee-saved register rotation when four registers are live: 0x00970790, 0x009210A0, 0x0094E6B0, 0x0095C0B0.
  - The inline-asm SSE path of C3DBoundingBox::CalcShortestDistanceToPoint[Squared] (0x00B49810 / 0x00B9C500) cannot be reconstructed (asm_bake only).

Review at landing:
- These rows matched only on their 4th grade (the limit): 0x0097DA40, 0x0093AD70, 0x0093AE80.
- Every matched source uses a local model, not an engine header:
  - Rows whose class has a header: 0x00AAE2C0 (C3DAnimFileMovementVectorChunk.h has the PDB vector offset +0x34; retail is +0x2C), 0x00AC1020 (C3DMeshFileMaterialChunk.h), 0x00ABC080 (C3DMeshFileSubMeshChunk.h), 0x0097DA40 / 0x0097DB70 (CAIStateGroupBase.h).
  - CAIStateGroup_HoverHornetQueen.h has CurrentCirclePoint / CircleMarkers 4 bytes too high and TimingCritical 8 bytes too high. Its 16-byte vectors are 12 bytes in retail; CircleMarkers first/last are at +0x50/+0x54.
  - Check whether each header can be used, or record why it cannot.
- Same-TU stand-in bodies under `auto_inline(off)` are in 0x00A99510, 0x00A9B740, 0x00AC1020 and 0x00ABC080. The landing session should check that they match the landed callee bodies, and that the callees are emitted out of line (they are not part of the graded leaf).
- landed 0x00A2D5F0 C3DAnimationInfo::CAnimationData ctor names its argument `size` (stored at +0x38); its only caller here (0x00A26730) passes the owning C3DAnimationInfo* (`push esi`), so it is the owner pointer
- 0092afe0 (parked): CCreatureAction_PlayCombatAnimation is 0x118 bytes in retail (PDB 0x130): the LookAtMap frame only matches with 0x118
- landed CTCTalk::PlayAISpeech 0x006E3B30 source declares the first parameter as `unsigned long text`; 0x00922750 passes a CCharString address there (Ego_r.pdb has both overloads: ABVCCharString and K) -- check which overload 0x006E3B30 is
- Catalog rows holding the wrong function:
  - 0x00A9B740 is LoadMeshFileCompressed (cataloged as LoadAnimFileCompressed).
  - 0x00C35770 is audio-library code (cataloged as C3DAnimationSequence::InitializeFromChunk).
  - 0x00B9C500 is CalcShortestDistanceToPoint (cataloged as ...Squared).
  - The landed 0x007AEEC0 is CountLivingHelperCreatures (manifest: CountEffects, shared with 0x007AEF10).
