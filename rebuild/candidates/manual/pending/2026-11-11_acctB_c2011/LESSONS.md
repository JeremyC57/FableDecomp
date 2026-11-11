# Chunk 2011 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00BF1340, 0x00A255A0) A short LOD walk that retail keeps top-tested (`cmp eax,4 ; jae` at an aligned head, unconditional back
  jump) has its bound test inside an inline predicate (`bool HasLOD(l) const { return l < 4 && size[l] != 0; }`, loop
  `for (l = 0; HasLOD(l); ++l)`); with `l < 4 && ...` written in the loop condition VC7.1 threads the known-true first test and
  rotates the loop (test at the bottom), whatever the loop keyword.
- (0x00BF17D0, 0x00BF1E01) A CEnginePrimitiveHandle temporary returned by AddStaticMesh and destroyed at once is expanded in retail:
  use FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR (FABLE_FORCEINLINE ~CEnginePrimitiveHandle, no out-of-line ??1 in Ego_r.pdb or
  Fable.exe); CLEAR_DTOR's plain inline body costs c1 87 and is called at every frequency (and the frame order changes with it).
- (0x00BF1394..0x00BF1790) In functions with two inline CMatrix3x4 products (PreMultiply / PostMultiply through SetMultiply on a
  copy), the canonical x87 term order of one product depends on how the OTHER product's operands are spelled: naming the
  inverse (`const CMatrix3x4& inverse = mesh->PeekInverseTransform();`) fixes the pre-multiply sums and flips the first
  post-multiply sum. Unused locals (the "k unused locals" lever) change nothing here; used named references do. A generator over
  natural reference spellings (named generator / primitive alias / manager receiver) found retail's set in ~480 compiles.
- (0x00BF18B0) Sibling overloads differ in a constant argument: the animated AddGeneratedObjects asks GetMesh with flags 0, the
  static one with 4 -- read every push, do not derive the second overload from the first blindly.
- (0x0129CF04) C3DMeshStats declares C3DMesh2's six animated dummy / helper queries pure virtual (slots 4..9 = _purecall); the
  overload pair of each name takes the slots in reverse declaration order (one-blend overload declared first, blend-pair one in
  the earlier slot).
- x87sim.py (`$S/tools/x87sim.py`) on both sides of a q.py output gives an exact per-sum residue list; a cheap fitness for
  generator searches over x87-order levers.

Review at landing:
- Both rows need this chunk's overlay opt-ins (hdr/README.md) plus chunk 2006's CMatrix3x4 SETMULTIPLY opt-in; land the headers first.
- The static row's x87 order depends on a `static_mesh` alias of the parameter and a named manager reference (no source evidence beyond
  the bytes; they are plain C++ and portable). Generator compiles beyond the 30-grade budget: see NOTES.md "Grade accounting".
- 0x0143B934 is bound to two invented names (g_LODTable_0143b934 by 0x00B9E950, g_interpolationTimes by 0x00BD7110): it is
  CurrentInterpolationInfo + 0x54 (NAME_FIXES.tsv).
