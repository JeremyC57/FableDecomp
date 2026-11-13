# Chunk 3001 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A4D920) A row whose frame equals a same-TU out-of-line function's frame plus one local object usually expands that function
  into the local. CalcLocalBoneTransformationRelative is `C3DKeyframe local; CalcLocalBoneTransformation(local, bone, time);`
  followed by the rest copy and C3DKeyframe::PostApplyRelativeAnimation. 0x00A4D330's frame is 0x38, plus 0x30 gives 0x68. A
  hand-expanded body with the same instructions sat one x87 pair off for two chunks; the real nesting matched first time.
- (0x00A4D920) A same-TU member whose body reaches an asm helper (GFFloatToLongNear inside GFFastArcCos / GFFastSin) is never
  auto-inlined by VC7.1, at any /d2inlT. When retail expands it, the definition is FABLE_FORCEINLINE (ASM_RULES rule 3.3). VC7.1
  still emits the referenced inline as a COMDAT, which is the out-of-line copy other TUs call (0x00A4D330).
- (0x00A9EB90, 0x00A4D920, 0x00AB2520) x87 / SIB commutative operand order is a per-function c2 key. Dead IL moves it: named
  locals initialised and never read, dead stores. Position does not matter: inserting at entry, mid-function or at the end flips
  at the same count. Unused uninitialised locals, other functions in the TU, and every copy-propagated spelling leave it alone:
  named pointers, constants, references, helper locals inside forceinline callees, a callee body written out at the call site.
  Diagnostic: sweep `long dN = (long)arg * 3;` statements (tools/sweep.py, grid.py). If a count gives MATCH, the shape is right
  and the residue is this key (0x00A9EB90: 6 dead locals + 24 dead stores -> RELOCATION_MATCH). Such dead code is a diagnostic
  only, never a deliverable.
- (0x00AB2520) FableWin.pdb's donor order gives the inline nesting. ?PostMultiply@CMatrix3x4@@QAEXABV1@@Z is 0x40 bytes right
  before ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z, so it is `CMatrix3x4 copy(*this); SetMultiply(copy, m);`, the 2006 PreMultiply
  pattern mirrored: 66 -> 23 lines. GetDeterminant (right after SetInverse) and SetMultiplyNoTranslation are byte-neutral there.
- (0x00A12EE0) VC7.1 folds `it + 8` into `[ecx+8]` for every source form tried: member chain, reference, explicit char-pointer
  arithmetic, two reads, index loop. Retail's spilled second outer IV at [esp+14h] is not produced by any of them, including the
  minimal probes in probe/osr/.
- (tooling) A relocation-masked fitness must not zero bytes at OUR relocation offsets in the retail image once the sizes differ.
  The chunk 2001 search.py did that, which garbles the retail disassembly. Normalise operands per instruction instead (tools/fit.py).
- (tooling) An `__asm` macro (GF_GET_CYCLE_COUNT) inside a one-line `{ ... }` body swallows the closing brace, and cl under Wine
  spins until the timeout. Inline asm also cannot name a class member (C2415).

Review at landing:
- 0x00A4D920's object also emits COMDATs for ?CalcLocalBoneTransformation@C3DAnimation2@@QBEXAAVC3DKeyframe@@KM@Z (retail
  0x00A4D330, not landed; ours is 1084 bytes against the 1083 retail extent, unchecked) and
  ?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z (retail 0x00A52650, landed under the wrong leaf, see NAME_FIXES.tsv). Check
  the link for duplicate or mismatched COMDAT selection.
- 0x00A4D920 uses the FABLE_FORCEINLINE CalcLocalBoneTransformation definition (rule 3.3 asm-helper reason, not an inline-budget
  device). The row also defines its GF / CQuaternion slerp helpers row-locally, as the landed 0x00AA3C50 does.
- 0x00AB2520 (parked) pair-lands with 0x00AB1CC0 BuildBoneInfoTree (same-TU noinline body in differ/00ab2520.cpp).
- engine/CMatrix3x4.h in hdr/ duplicates chunk 2006's FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY block: merge once.
- 0x013CB530 / 0x013CD550 carry two bound spellings each in reloc_bindings.tsv (g_ArcCosTable / g_ACosTable, g_CosTable /
  g_cosTable).
