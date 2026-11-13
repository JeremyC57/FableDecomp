# Chunk 2044 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A9F540 / 0x00AA0090) FableWin.pdb's COMDAT run after a function lists UNDECORATED names for file-static helpers
  (ApplyRelativeAnimation, ApplyRelativeAnimationAlpha, ApplyPartialAnimation, ApplyPartialAnimationAlpha): retail keeps their
  uncalled LTCG custom-convention copies (`test al,al` / esi / ecx register arguments, 0x00A9D710..0x00A9DC20) while every caller
  expands them. Write them as statics in the row's TU; VC7.1 only expands them FABLE_FORCEINLINE (Inline-budget line).
- (0x00A9F540) A callee whose body holds an asm helper (ConvertTimeToFrames + GFFloatToLongFloor) must still be VISIBLE in the TU:
  the call stays out of line but its reference parameters stop escaping, so the caller hoists the results into callee-saved
  registers for the following loops (2249 -> 2288 bytes). Header-inline body, not a same-TU noinline copy.
- (0x00A9F540) A `count * 5` keyframe scratch with a 64-bit range check that returns: `__int64 n = count * 5; if (n < 0 || n >
  0x7fffffff) return; pool.AllocateType<T>((unsigned long)n);` -- a checked COPY of the unsigned count makes the inline
  AllocateType multiply 64x64 through `__allmul`.
- (0x00AA0090) Interpolations `fld a ; fld b ; fsub st(1) ; fmul t ; fadd st(1)` are the shared GFGetInterpolatedValue
  (FableWin ?GFGetInterpolatedValue@@YIMMMM@Z); the landed rows' local `InterpolateFloat` is the same body under an invented name.
- (0x00AA0090) `info->GetAnimation(mesh)` with `PeekAnimation` called before GetAnimation's pushes = a named
  `const C3DAnimationInfo* info` local; calling through the expression pushes the outer arguments first.
- (0x00AA0090) x87 commutative orders of an expanded helper can differ between two twins that share the helper text: the twin
  matched, the blended row needs ~15..31 dead-float-locals' worth of extra IL (q sums) plus a mod-4 symbol count (dot product);
  dead global stores and dead float locals move them independently. tools/feat.py + sweep3.sh measure this for free.
- ltcg_grade.py / tools/lt.py reproduce retail's LTCG custom-convention copies of file statics (`/GL` + `/LTCG`, internal symbol
  extracted from the map): a 1-second check of a static helper's text against its uncalled retail copy.

Review at landing:
- 0x00A9F540 needs the hdr/ opt-ins listed in hdr/README.md; FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY makes ConvertTimeToFrames
  a header inline with landed 0x00A52650's body (that row defines it out of line through a stand-in struct; regrade it if folded).
- The row TU emits an out-of-line ConvertTimeToFrames COMDAT (asm helper inside) and expands GFFloatToLongNear six times: grade
  retail_asm; the per-site table in NOTES.md has 9 measured spellings + the hand747 15-spelling table for the same GFFastArcCos /
  GFFastSin sites, not the full 30+10 per site (coordinator to decide).
- 0x00AA09F0 (landed) binds 0x00AA0090 as `...QAEXPBVC3DAnimationBlendState@@0MPAVC3DVector@@PBVC3DMesh2@@PAVCMatrix4x4@@K_N@Z`
  (pointer / non-const parameters) and names itself with the same wrong parameter kinds; PDB spellings in NAME_FIXES.tsv.
- g_ArcCosTable / g_ACosTable and g_CosTable / g_cosTable are both bound in reloc_bindings.tsv for 0x013CB530 / 0x013CD550.
- Uncatalogued functions 0x00A9D700 (GetAlphaWeight COMDAT), 0x00A9D710, 0x00A9D980, 0x00A9D9E0 (static Apply* copies with LTCG
  conventions: not gradable by the .obj lander); 0x00A9D730 / 0x00A9D740 catalogued "SetMember*" look like Initialise / Uninitialise.
- (follow-up, ASM_RULES rule 4 table) A forced-inline GFFastArcCos / GFFastSin fragment fed the row's own inputs is enough to
  measure all six Near sites at once (each fragment holds the three conversions; counts 3x); probe/asm/tbl/gen.py + run.py write and
  grade the whole 99-fragment TU in one compile per flag set. VC7.1 /arch:SSE2 is the one flag that yields a `fistp dword` from C++:
  a double (floor / ceil / fmod result) cast becomes `fnstcw ; or ah,0Ch ; fldcw ; fistp dword ; fldcw` (truncation wrapper) on an
  all-SSE TU -- never retail's bare `fld ; fistp dword`. /fp:* is not a VC7.1 option (D4002, ignored).
- (0x00AA0090 resume) /opt/fable_lane/tools/fwsize.py (FableWin /Od sizes per inline helper) found the missing IL: BlendWith is
  48i = SetSlerp call + ONE named reference (+2i/+6b), PostApplyRelativeAnimationAlpha 32i = one named reference. Which member the
  reference names moves record-id x87 orders by many dead-local units: `other.Rotation` side matched 0x00AA0090, `this` side matched
  the twin 0x00A9F540 -- grade BOTH twins after every shared-body change (both.sh + mktwin.py build the twin from the candidate).
- FableWin static helpers measured at /Od are __fastcall (/Gr engine): compile statics fastcall before comparing sizes; a +1i/+3b per
  forwarded float argument is later-compiler noise, not text.
- C4DVector named-result operators reproduce FableWin sizes but add frame copies in VC7.1 release: the retail grade overrules FableWin.
Review at landing: the twin's helper bodies change if differ/ (or alt) is adopted -- differ/00a9f540.twin_with_new_bodies.cpp grades
RELOCATION_MATCH and must replace src/00a9f540.cpp's helper text for one-body-per-row consistency (coordinator decision).

- (0x00AA0090 + twin 0x00A9F540, matched as a pair) The "needs N dead floats of IL" residue was a shared inline helper's TEXT one level
  further down: GFFastSin (expanded 4x in each row through SetSlerp / SetSlerpFromIdentity) written with named table values
  (`i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1];`). fwsize flagged it (+8i); the decisive evidence was the
  SIBLING: GFFastCos with the same naming leaves the identical +2i/+1b residual and Sin - Cos equals FableWin's 3i/12b. Byte-neutral in
  release (all 9 landed users still match). Lesson: fit every DIFF helper (not only the ones the row's own text touches) and use a sibling
  helper's FableWin size to separate text from later-compiler noise.
- A dead-float ruler (k = 0..16 at the row top) of both twins per family is cheap (~1 s per grade) and shows which family has a flat
  matched plateau on one side (Family A twin) and a threshold on the other (blended >= 4): add natural IL to the threshold side.
Review at landing: GFFastSin / GFFastCos are row-local texts in the landed rows (no shared header); if a shared GFFastSin header is made,
use this text (all users measured RELOCATION_MATCH with it).
