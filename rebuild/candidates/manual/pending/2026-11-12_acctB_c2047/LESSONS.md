# Chunk 2047 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A9E1E0) CMatrix4x4::AlignedSetMultiply's x87 arm reads through the element accessors: `for r for c (*this)(r, c) = a(r,0)*b(0,c) +
  a(r,1)*b(1,c) + a(r,2)*b(2,c) + a(r,3)*b(3,c);` (FableWin emits ??RCMatrix4x4@@QBE?BMHH@Z / QAEAAMHH@Z right after AlignedSetMultiply,
  i.e. it is their first user). The old `v +=` / Data body gives non-retail column term orders in every user; the accessor body made the
  transposed x87 loop retail's at once and improves all four chunk-748 parked users (opt-in FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS).
- (0x00A9E1E0) x87 column orders of an expanded matrix-multiply loop follow the outer function's named-symbol count mod 8 (r3004 lever):
  a dead-local diagnostic sweep showed k = 2, 6, 7 mod 8 fixes the second loop; `unsigned long i;` declared once at function scope for two
  loops plus a named `const C3DBone2* bone = mesh.GetBone(i);` were the natural names (a named `const CMatrix4x4& parent_matrix` works too).
  The function-scope counter also fixed the loop's induction-pointer base (`lea eax,[esp+58h]` v `[esp+54h]`).
- (0x00A9E1E0) A per-function feature screen of the hard part (here: the two x87 loops extracted from `mov edi,4` to `jne`, relocations
  masked, tools/feat.py) lets natural spellings be screened for free; only the closing candidate needs a grade (3 grades to match 2472 B).
- (0x00AA39A0 / 0x00AA3AF0) Retail's uncalled C3DKeyframe 4x4 conversions expand CQuaternion::ToMatrix[Transposed]: forced conversions
  reproduce them with NO flag (plain needs /d2inlT308) -- the forced header body is the simpler explanation; with the conversions forced the
  C3DKeyframe wrappers score ~300 and must be forced too in callers that expand them.
- (0x00A9E1E0) FableWin.exe.signatures.tsv debug instruction counts are plain /Od (GetParent 9, GetBoneCount 9, C3DKeyframe::ToMatrix 16 match
  /Od exactly, /RTC1 adds one each): a 13-instruction FableWin COMDAT is a one-call wrapper (AlignedSetMultiplyTransposed = AlignedSetMultiply(b, a)).
- A 16-byte-aligned frame (`and esp,-16`) with a CMatrix4x4 local is `__declspec(align(16)) CMatrix4x4 local;` -- CMatrix4x4 itself is
  not aligned (0x00B4FC40 has CMatrix4x4 locals and a plain frame).

Review at landing:
- NAME_FIXES.tsv: landed rebuild/src/compiled/00/aa/C3DKeyframe_ToMatrix_00aa3af0.cpp is ?ToMatrixTransposed@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
  (0x00AA39A0 and 0x00AA3AF0 both carry ?ToMatrix today: one name, two addresses). Its local model body differs in spelling from the
  overlay's shared inline (which also matches 0x00AA3AF0 out of line, probe/ool/ooltf.cpp).
- The row needs the hdr/ opt-ins in hdr/README.md (new: CQUATERNION_MATRIX4_TRANSPOSED, CQUATERNION_MATRIX4_FORCEINLINE,
  C3DKEYFRAME_TOMATRIX4_INLINE, CMATRIX4X4_SETMULTIPLYTRANSPOSED(_FORCEINLINE), CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS; existing:
  CMATRIX4X4, CMATRIX4X4_ELEMENT_ACCESS, CQUATERNION_MATRIX4, C3DMESH2_BONECOUNT / _BONES (duplicate GetBoneCount guarded), C3DBONE2_PEEK,
  C3DANIMATIONMANAGER_MESHTRANSFORMS). prior/00a9f540.cpp stays RELOCATION_MATCH under this overlay.
- Grade retail_asm (GF_SSE_MATRIX_MULTIPLY through the inline AlignedSetMultiply, two sites): the per-site table (35 plain C++ + 13
  intrinsic spellings x 2 sites x 3 flag sets, with a positive control) is in NOTES.md and probe/asm/.
- Candidate default body: the accessor x87 arm of AlignedSetMultiply (one body for all rows); regrade the chunk-748 users if folded.
