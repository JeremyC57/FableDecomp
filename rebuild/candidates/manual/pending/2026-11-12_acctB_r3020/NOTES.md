# Hand chunk 3020 (CPhysicsMeshObject): 1 of 1 byte-exact (RELOCATION_MATCH)

Row 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes: RELOCATION_MATCH(1144v1144). Source: src/00a12ee0.cpp (grade with
`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3020/hdr`). Behaviour test: test/00a12ee0.cpp (three mutation checks), PASS under g++ (tall.sh)
and VC7.1 (tall.py). Read-only vland: RELOCATION_MATCH PASS; its four RELOC_NAME warnings are catalog labels of COMDAT-folded callees
(0x00A110D0 labelled DeleteViaVtable0 = CCountedPointer<CContainmentVolume>::DeleteData; 0x00825B40 labelled _Copy_impl = the folded
__unguarded_insertion_sort_aux; 0x008274B0 / 0x008270E0 the sort helpers), not identity errors. Grades: grades.log (one line each;
`nd` = non-branch differing lines; ruler / probe lines are diagnostics, never hand-ins). The previous best (DIFFER 1144v1144) is kept as
cand/differ_prev_00a12ee0.cpp.

## What matched (1)
- CPhysicsMeshObject::GetNavigationLevelVolumes: the last residue (inlined TransformPoint X row, X*E11 and Y*E21 products swapped)
  closed with `const CPlane& source_plane = source_planes[i]; CPlane plane = source_plane;` in the plane loop (session 2, grade 184).
  Cause and mechanism (c2 key trace + record dump, tools/k6.py, tools/sids.py; agent3010's c2h7/key.py and c2h6/recs2.py):
  * The x87 add order is c2's linearizer order by operand hash. X-row products hash (k6(Exx) + 0x30) + 64 * (point field record id)
    mod 65536. k6 is the matrix load leaf (E11 a008 in the old source, c008 in "mode B"; E21 = E31 = 8008).
  * c2 symbol records come in CHUNKS OF 32 ids taken from a shared chunk pool (temps use other chunks). Old source: second symbol
    chunk at 192 (E11 a008), point+4 in the LAST slot of the third chunk (415), point+8 bumped to the next chunk (577) -> Y > X > Z.
  * One more symbol before the first chunk overflows (any extra named local at function level: a dead long, `Mesh.Get()` twice, a
    named `new` pointer) puts the second chunk at 224 and E11's leaf at c008 ("mode B"), but also bumps point+4 out of the third
    chunk (576 / 578) -> X > Z > Y. Mode B plus one symbol fewer between the overflow and point+4 (diagnostic: dead long + a
    TransformPoint with no zz copy = RELOCATION_MATCH, grade 176) gives X > Y > Z.
  * The natural source that does both: `const CPlane& source_plane = source_planes[i];` adds a function-level symbol and the
    plane copy no longer creates its own record later. h2 matches with and without the no-zz diagnostic (grade 185), so it is not the
    zz copy; edge check: +1 / +2 dead longs on top of it break it again (grades 187 / 188), as expected for record-id hash orders.

## Spellings: FITTED v PROVEN
- PROVEN (two sources): CMatrix3x4::TransformPlane expanded (FableWin first COMDAT after the row), its helper texts (fwsize SAME:
  TransformPlane 48i, TransformPoint 61i, TransformVector 55i, GetNormal, GetDistance, C3DVector ctors, CIVCountedPointer::Get), the
  callees (LABELS.tsv).
- FITTED: `const CPlane& source_plane = source_planes[i]; CPlane plane = source_plane;` (local name descriptive; the shape is a c2
  record-count lever, behaviour-neutral); `const C3DMeshVolume& mesh_volume = it->second;` used twice (the second induction pointer);
  `const CPhysicsMesh* mesh = Mesh.Get(); if (mesh)` (latch reload order; FableWin emits IsNull + Get after the row); TransformPlane
  statement order (normal copied first); the _LTR TransformPoint text (one of three repo texts for one retail body).

## Session 1: what closed the 26-byte / induction-pointer residue
- Retail's second outer IV `it+8` in [esp+14h] (0x00A12F08 `lea eax,[ecx+8]`) is VC7.1 strength reduction of a NON-FORWARDED inline
  `this`: a named reference `const C3DMeshVolume& mesh_volume = it->second;` used TWICE
  (`mesh_volume.GetConvexVolume()->GetPlaneCount()` and `mesh_volume.GetConvexVolume()->PeekContainmentPlanes()`). One use is copy-
  propagated and folds into `[ecx+8]` (all earlier ~100 spellings had one use); two uses keep the reference a variable, which OSR turns
  into its own induction pointer biased to the ConvexVolume field; the two loads CSE into the single `mov eax,[eax]`. Probe ladder
  (probe/, all compile-only): a multi-use inline `this` (3 fields, or 2 uses in different blocks, or a dead `if (z)` block) makes the IV;
  a direct `it->second.x` expression or a single-use reference never does. Grade 11: 1118v1144 -> 1144v1144.
- TransformPlane: FableWin emits ?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z FIRST after the row (first user); body fitted with fwsize
  to 48i (FableWin 48i/139b). `C3DVector normal = plane.GetNormal();` FIRST (the normal's field records are created before the point's,
  which gives retail's Dot operand order: point component loaded first, `fmul [normal.Z]` on the live point.Z), then the point, then
  in-place TransformVector (repo opt-in, FableWin 55i SAME), `normal = normal.AccurateNormalise();` (retail copies the returned
  reference back, 0x00A13003..0x00A13025), TransformPoint, `plane.Initialise(normal, normal.Dot(point))`. FABLE_FORCEINLINE (plain inline
  is called out of line in the row: 887v1144).
- TransformPoint: the existing nested opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR (rows E1k*xx+E2k*yy+E3k*zz+E4k) gives retail's
  Y and Z rows; the default form gives (Z,Y,X) rows wrong.
- Latch reload order (0x00A13294 `mov ecx,[esp+10h]` before `mov edx,[esp+24h]`): `const CPhysicsMesh* mesh = Mesh.Get(); if (mesh)`
  (FableWin emits CIVCountedPointer IsNull + Get right after the row; `if (!Mesh.IsNull())` alone fixed nothing there).

## History (session 1 park note, now closed)
- 00a12ee0 CPhysicsMeshObject::GetNavigationLevelVolumes DIFFER(1144v1144): TransformPoint X row first two products swapped (retail
  0x00A1300D/0x00A13014 X*E11 then 0x00A1301B/0x00A13022 Y*E21; ours Y*E21 then X*E11) | tried: Mesh test forms (operator->, IsNull,
  IsNull==false, Get()), named matrix reference (3 placements), loop index types / != / pointer walk, CPlane copy-init, level const,
  CCountedPointer copy-init, RELEASEDATA dtor, Dot texts (FableWin 23i fits `dot=..; dot+=..` but break the Dot order), operator*(float)
  named result (code changes), SCALE_X_PAREN, TransformPoint copy-declaration orders (5 permutations), TransformVector texts (E-first,
  parenthesised), TransformPlane statement orders (v7/v14..v22), dead-local rulers (long/float, top / inside TransformPlane, k=1..16:
  X row never right) | measured (c2 key trace, tools/kana.py on tmp/key_c80.json): X-row products hash Y e7f8 > X d238 > Z 1078
  (records 415 / 200 / 577); retail needs X > Y > Z; a uniform id shift cannot produce that order (wrap analysis), so the residue needs
  a non-uniform record change between the early point record (200) and the late ones | next: (1) trace which variables records 200 /
  415 / 577 are (symdesc came back empty with target ""; agent3010 used target = the function compiled just before the row: here
  ?Insert@?$CVectorMap@JV?$CCountedPointer@VCContainmentVolume..., see the pipe events) and find the natural text that creates the
  point's Y/Z records earlier or later; (2) TransformPoint / TransformVector texts proven by another TU's retail copy; (3) TLC-era
  CCountedPointer asserts (FableWin Get 22i, operator-> 26i v ours 9i).

## c2 key trace of the differ source (tmp/key_d.json; target "?Insert@?$CVectorMap@JV?$CCountedPointer" gives names)
- Records: point (= point+0) 200, point+4 415, point+8 577; normal 193, +4 411, +8 413; plane 27 / 405 / 409; TransformPoint copies xx 202,
  yy 203. The X row reads the point fields directly (xx forwarded), the Y / Z rows use xx / yy.
- X row operand hashes (descending = evaluation order): T(E21)*point+4 e7f8, T(E11)*point d238, T(E31)*point+8 1078. Fit:
  h = hT + 64*id (+const), hT(E11) ~ 0xA038, hT(E21) = hT(E31) ~ 0x8038. Retail needs h(X) > h(Y) > h(Z):
  * either the point's own record id in 288..383 (now 200) with point+4 / point+8 unchanged (only the X row and the Dot's X term
    (still last) move: point's record is used nowhere else),
  * or point+4's id below ~328 with point+8's hash still below it.
  A uniform shift of all ids cannot do it (all three hashes move by 64*d). Locals of the inlined helpers get ids together
  (normal 193, point 200, xx 202, yy 203), so a statement order inside TransformPlane moves the point id by only a few.

## Names
- Proven (two sources, LABELS.tsv): the row, CPlane::Initialise 0x00A42140, CContainmentVolume ctor 0x00A752D0 / Initialise 0x00A755C0,
  C3DVector::AccurateNormalise 0x00A14440. Inline-only PDB names used: CMatrix3x4::TransformPlane / TransformVector / TransformPoint,
  CPlane::GetNormal / GetDistance, CIVCountedPointer::Get / IsNull, C3DMeshVolume::GetConvexVolume, CContainmentVolume::GetPlaneCount /
  PeekContainmentPlanes (FableWin). Descriptive names (functions): none; local variable names (mesh_volume, source_plane, ...) are descriptive. NAME_FIXES.tsv: none.

## Header debt
- hdr/README.md "Chunk 3020 changes": TRANSFORMPLANE rewritten (no landed user), CPLANE_ACCESSORS and CIVCOUNTEDPOINTER_GET new.
- TransformPoint has three texts in the repo (default, _SUMS, _LTR); this row needs _LTR. The x87 orders are c2 record-id hashes, so one
  real text can look like several; a landing session should pick one body and regrade every user.

## Skipped
- none.

## Label corrections
- none proven (vland RELOC_NAME warnings above are catalog labels of COMDAT-folded bodies; left for the landing session).
