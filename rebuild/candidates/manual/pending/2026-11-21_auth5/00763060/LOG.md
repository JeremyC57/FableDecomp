# 0x00763060 NShape::FractureShape -- AUTH5 log
## Step 0 (VC10 SP1 evidence; no grade spent)
- a. Compiland: Ego_d FableLib\debug\tc_geometric_shapes.obj (raw S_OBJNAME at PDB file offset 0x1914470; this PDB's live MSF directory
  lists only 29 modules, so the library symbol streams are found by raw scan, as egl.py does). Full cmd: `-W4 -WX- -MP -Od -Oy- -DLIB_NOT_DLL
  -DWIN32 -DNDEBUG -D_WINDOWS -DGAME_COMP_DEBUG -DUSE_VIRTUAL_KEYBOARD -D_CRT_SECURE_NO_WARNINGS -D_CRT_NON_CONFORMING_SWPRINTFS -D_MBCS -Gm-
  -EHs -EHc -MDd -GS- -Gy -fp:precise -Zc:wchar_t -Zc:forScope -GR -Yustandard_headers.hpp -Fp.. -Fo.. -Fd.. -Gr -TP -analyze- -Zm150` =
  odall10 `--lib fablelib` (no extra code switch). -Gr confirmed: Ego_d mangled ?FractureShape@NShape@@YIX... (YI = fastcall).
  Note: an STLPort dir is on its -I list, but the TPI types are VC10 Dinkumware (_Vector_iterator<_Vector_val<..>>, _Iterator_base12).
- c. --stl validated on THIS compiland: standalone text (step0/stf.cpp family) reproduces tc_geometric_shapes' own vector<C3DVector>
  members exactly: move ctor 106, _Assign_rv 334, get_allocator 31, find_if<_Vector_iterator<..C3DVector..>,CIsPoint..> 303. No macro needed.
- b/d. Standalone --stl FractureShape text step0/f4.cpp: 54 of 60 Ego_d lines exact. Decoded (exact sizes):
  204 `iterator it_start = it_anchor;` 16; 207 `it_result = std::find_if(it_start, duplicate_shape.end(), F(a,b,circ));` 168
  (the `++it_start` form is 173, `it_start++` in the call 170); 210 `it_start++;` 28 (`++it_start;` = 8); 212 `if (it_result != end())` 88;
  241 `long distance = std::distance(it_begin, it_result);` 92 (`it_result - it_begin` = 15); 252/253 `iterator it_del_start; / it_del_end;` 15/15;
  255/267 `vector splice_one(duplicate_shape)` / splice_two 19; 256 `it_del_start = splice_one.begin(); ++it_del_start;` 84;
  257 `it_del_end = splice_one.begin(); it_del_end += distance;` 88; 268 `it_del_start = splice_two.begin(); it_del_start += distance + 1;` 91;
  225/248 push_back(*it_result) 20. Unexplained: 197 74 (begin()+1 = 69), 198 18 (`a = *it_begin` 17), 280 143 (return 142),
  283 25 (else-if 11), 288 5 + 291 197 (an else arm retail lacks: debug-only).
- So AUTH1/p25Q's 199/204 rejection rested on ATTRIBUTION, and it does not survive: Ego_d itself increments it_start AFTER the call
  (own statement, line 210). 199 = 17 B plain `C3DVector b = *it_anchor;` and 204 = 16 B plain copy stand (exact under VC10).
  Open: Ego_d's text starts the search at it_anchor (begin+1); retail starts at begin+2 (esi = ebx+18h at the first call).
- Ego_d S_BPREL32 frame (step0/locs.py): a -24, it_anchor -36, it_begin -48, found_appropriate -49, duplicate_shape -72, it_start -84,
  distance -88 (function scope: fits 241), b -100, it_result -112, quit -113; blocks: line_b -152 / result_point -136 / line_a -132 /
  triangle -172; found block: it_del_start -264, it_del_end -252, splice_one -240, splice_two -220, size_1 -200, size_2 -196, triangle -192.
  Our VC10 text gives the same named layout except found_appropriate/it_begin swapped (no size effect).
- (WITHDRAWN, advisor: 197/198 were mis-assigned) "Ego_d searches from begin+1". CORRECT decode, all of 195..204 exact under VC10
  (step0/f5.cpp; 56 of 60 lines exact, the 4 left are 280/283/288/291 at the tail):
    195 `iterator it_begin = duplicate_shape.begin();` 16   197 `C3DVector a = *it_begin++;` 74   198 `iterator it_anchor = it_begin + 1;` 18
    199 `C3DVector b = *it_begin;` 17 (`*it_anchor` also 17, but gives b = begin[2])   204 `iterator it_start = it_anchor;` 16
    241 `long distance = std::distance(duplicate_shape.begin(), it_result);` 92 (it_begin also 92; retail `sub ecx,ebx` picks begin()).
  Ruled out by size: `C3DVector a; a = *it_begin++;` 90, `1 + it_begin` 49. Semantics = retail: b = begin[1], first search at begin+2.
  So Ego_d and retail agree in behaviour here; no revision difference is needed for this row.
## VC7.1 grades (q.py, /Gr)
1. r0 = repo E0: DIFFER(1132v1146), 58 lines (baseline).
2. e1 = Ego_d-decoded text (find_if(it_start)+`it_start++;`, std::distance [PASS22_LOWER_BOUND_COMPARE+BODY defines], iterator decls +
   begin-then-advance, push_back(*it_result)): 1134v1146 / 54 (searches from begin+1, semantically not retail). e1m (it_result-it_begin) same.
3. e2 = e1 + `C3DVector b=*it_anchor++;`: 1137v1146 / 53 (increment after call; no `mov ecx,eax` copy, no loop alignment).
4. e3 = e1 + `it_start=it_anchor+1`: 1134 / 78.  5. e4 = e1 + `C3DVector b;b=*it_anchor++;` (Ego_d order): 1146v1146 / 53.
6. e5 = e4 with a before it_anchor: 1146v1146 / 2 (a.Z store `mov [esp+48h],edx` before `mov ecx,eax`; retail after). = p25L E2-a-first.
7. e6 `it_start=++it_anchor` 1134/78; e7 `b=*it_anchor; it_anchor++;` 1137/53; e8 a-first + `b=*it_anchor++` 1137/6.
8-10. e9 (it_begin+1), e10 (`b=*it_anchor;it_anchor++;`), e11 (b declared before it_anchor): all 1146v1146 / 2.
So the Ego_d-exact spellings at 207/210/241/252..257/268 are neutral for retail; the residue sits only in the TLC-only start advance.
11. h1 = Ego_d-exact text (195..241 above) on e1: DIFFER(1136v1146), 37 lines. The loop increment now sits after the call as in
    retail; left: preheader folds b's read to [ebp+0Ch] and builds it_anchor as `lea eax,[ebp+18h]` (retail: `lea eax,[ebx+0Ch] ;
    mov ecx,eax ; b via [ecx] ; add eax,0Ch`), no `lea esp,[esp]` loop alignment, begin in ebp / zero in ebx (retail swapped).
12. h1b (distance from it_begin, semantically off by one): 1141/21.  13-16. h5 (b before it_anchor) 1136/77; h6 (parenthesised /
    direct-init) 1136/37; h7 (`b=*it_begin++`) 1136/37; h8 (`1+it_begin`) 1136/37.  17-18. h1 + C3DVECTOR_ORIGINAL_COPY_FORMAL 1136/37;
    + LIBPK_IMPLICIT_COPY_PASS23 1197/129.
Stall -> ESCALATE.md (26 grades; best Ego_d-exact h1 1136/37, from r0 58).
27. Advisor: frame decides the roles. VC10 step0/f6.cpp (it_anchor = begin() at 195; a = *it_anchor++; it_start = it_anchor + 1;
    b = *it_anchor; it_begin = begin() at 204; distance(it_begin, it_result)): same 56 exact lines AND the named frame now equals Ego_d
    (found_appropriate -49, it_begin -48, all others). g1 = that text under VC7.1: DIFFER(1134v1146), 78 lines (same preheader fold).
    h1 is therefore NOT Ego_d-exact (frame). Packet updated.
