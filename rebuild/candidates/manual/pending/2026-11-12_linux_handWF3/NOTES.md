## 0x00931200


## 0x00931200 CAIStateGroup_MinionReactToCriminal::IsStillValid() const -- RELOCATION_MATCH (3 grades: 2 compile fixes + a1 DIFFER + a2 MATCH)
- What made it match: the port onto the shared headers alone. The parked draft's "register plan" residue (358v387) was model error
  (local CThing / list / pointer stand-ins). Source = IS_THING_ALIVE(PHome) guard; `CThingAICreature& creature = GetCreature();`
  before the first PHome read; int-valued `&&` for HeroInsideHouse; CThing::GetTC(TCI_VILLAGE_MEMBER,&p) (GETTC_EXPANDED) && GetPVillage();
  `const std::list<...>& people = GetPVillage()->PeekVillagePeople()` iterated with const_iterator; HasTC(TCI_GUARD) forceinline; break.
- One real trap: `#include "engine/CAIStateGroupBase.h"` must come BEFORE `engine/CAIBrain.h`, otherwise the inline body of
  CAIStateGroupBase::GetCreature() (defined at the bottom of CAIBrain.h) is not seen and GetCreature is CALLED (391v387, a1).
- Headers / flags: no compiler flags. Overlay hdr/engine/CAIStateGroup_MinionReactToCriminal.h adds
  FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL (polymorphic class over CAIStateGroupBase, typed PHero / PHome, virtual
  IsStillValid() const, private IsValid() const, the three *InsideHouse flags `mutable`). Defines used: CCHARSTRING, CCLASS,
  DEF_CLASS_BASE, CTCBASE, CTHING (+GETTC_EXPANDED, +HASTC_FORCEINLINE), CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE,
  CTHINGAICREATURE, CTHINGPLAYERCREATURE, CINTELLIGENTPOINTER (+COMPARE_NULL_INT), IS_THING_ALIVE, CTCVILLAGE (+PEEKVILLAGEPEOPLE),
  CAISTATEGROUPBASE (+MEMBERS, +GETCREATURE), CAIBRAIN_GETCREATURE.
- Not proven: `mutable` on the three flags (the function is const in both PDBs and writes them; a const_cast spelling would give the
  same bytes; neither PDB records mutability). Locals `creature`, `village_member`, `people`, `person`, `it` are descriptive.
- Name evidence against landed spellings: the landed sibling rows 0x00930EE0 / 0x00931010 / 0x00930E20 use the module spelling
  `CAIStateGroupMinionReactToCriminal` (no underscore) in their file names; the PDB class is `CAIStateGroup_MinionReactToCriminal`
  (Ego_r 0x009544F0 / FableWin 0x02B5A2F0 leaf ?IsStillValid@CAIStateGroup_MinionReactToCriminal@@UBE_NXZ). The landed 0x00930EE0 row
  declares IsValid non-const; PDB is ?IsValid@CAIStateGroup_MinionReactToCriminal@@ABE_NXZ (private const). The old draft's
  CThing::GetPosition for 0x004C73D0 is CThing::GetPos (?GetPos@CThing@@QBEABVC3DVector@@XZ, engine/CThing.h).
- differ/00931200.cpp is the stale parked draft (superseded by 00931200.cpp).

## 0x009DC6E0

# 0x009DC6E0 CRenderManager2D::Draw2DTexture (box overload) -- RELOCATION_MATCH (wf3 author, 2026-11-12)
- What made it match: (1) ported onto the shared headers (C2DBoxF, CRGBColour::GetD3DCOLOR, CTVertexRHWColSpecTex1, CRenderManager2D;
  `CTexture&` and `GFVBoxToABox(const C2DBoxF&, C2DBoxF&)` per the PDB); (2) statement order: all eight X/Y stores first in vertex
  order (gives the TLY/BRX/BRY load + register copy and the TLX reload), then the eight U1/V1 constants (immediates, outside the
  `xor eax,eax` zero's lifetime), then per vertex `Z = 0; SetRHWFromZ(); SpecularColour = 0; DiffuseColour = argb`;
  (3) the RHW comes from the header's inline `SetRHWFromZ()` (folds to `RHW = g_ScreenRHW` for the known Z 0) -- plain
  `v[i].RHW = g_ScreenRHW` in any order tried left a third register copy for vertex 3 and other `lea` registers at the calls.
- Headers / flags: no overlay header, no flags, no pragma. Opt-ins used (all already in the repo headers):
  FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ, FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR, FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX,
  FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ (private Add2DTri). Strict-header regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Names: nothing made up. The old draft's GetARGB / Diffuse / Specular / U / V / `CTexture*` spellings were wrong (dossier); the
  landed UV sibling 0x009DC870 still carries the same local-model spellings (GetARGB, Diffuse, Specular, U, V, pointer-typed
  GFVBoxToABox) -- a port candidate, not a NAME_FIXES row (its retail symbol name is right).
- Ideas tried before the match (21 fg grades): draft per-vertex order (384), retail store order as source order (394), X/Y block
  first (374), U/V block after the mode test (406) / first (400), inline SetXYZ/SetRHW/SetColour/SetU1V1 setters (same bytes as
  direct stores), U/V block before the Z/RHW block (390, fixed the immediates), chained / copied RHW (fld/fst), X/Y permutations
  (388..392, worse), SetRHWFromZ at the plain-RHW positions (390), per-vertex Z/SetRHWFromZ/Spec/Diffuse (MATCH).

## 0x00761980

# 0x00761980 CTCShapeManager::DrawShapes -- wf3 author pass 1 (DIFFER 381 v 389)

Final report header: rows matched 0; row in progress 0x00761980 (about 110 grades); rows untouched 0; probes 11 listings + 26 p-files; tool calls about 95.

## State
- Best source: differ/00761980.cpp (shared headers only, strict-header clean in spirit: no local class models). Grade DIFFER(381 v 389), 5 hunks,
  ALL inside 0x00761A05..0x00761A75 (the start-point copy and the lifted vector). Prologue, bl=0xff constant, iterator frame slot, loop
  shape, the virtual calls, the line ctor call, the selected-shape tail and the epilogue match retail byte for byte.
- Exact residue: retail copies the first point with an UNSPLIT block copy `mov eax,edx ; mov ecx,[eax] ; mov edx,[eax+4] ; mov eax,[eax+8]`,
  stores it to the frame vector (+0x20) and then RELOADS Y and X from that slot as dwords (0x00761A21 `mov eax,[esp+30h]`,
  0x00761A31 `mov edx,[esp+30h]`) to build the lifted vector (+0x2c; Z = `fld [slot.Z] ; fadd 0.15f`). Ours loads the components
  through esi (Z, X, Y) and forwards the registers to the lifted vector (no reloads): 8 bytes shorter.

## What is proven (keep)
- Port onto shared headers fixed the old draft's residue by itself (iterator slot + bl): typed STLport `vector<CCountedPointer<NShape::CShape> >::iterator`,
  `const CWorldMap& map = PeekWorld().PeekWorldMap()`, PDB const-ref signature, `NShape::CShape* shape = it->Get()`.
- NEW IDENTITY: the copy comes from the inline `bool NShape::CShape::GetStartPoint(C3DVector&) const` (FableWin.pdb
  ?GetStartPoint@CShape@NShape@@QBE_NAAVC3DVector@@@Z 0x01818274 / 0x025D6620; not out of line in Ego_r / retail). Second source: with the
  overlay's body, the natural source of sibling 0x00760910 CTCShapeManager::GetPrincipalVector
  (`C3DVector start; if ((*SelectedShape)->GetStartPoint(start)) { out = ParentThing->GetPos() - start; return true; }`) grades
  RELOCATION_MATCH with shared headers (probe2/gpv.cpp). The landed 0x00760910 row uses a local by-value `operator-(C3DVector)` hack instead.
- Lift = `C3DVector top(start.X, start.Y, start.Z + 0.15f)` (retail reads Z, Y, X = right-to-left ctor arguments).
- The retail shape "classic block copy + plain reloads from the slot" is reproduced ONLY when the lift sits after a join the copy does not
  dominate (v43: `if (!empty) start = front; if (!empty) {...}`; v48: `if (empty) continue; GetStartPoint(start);`), but those keep the
  second test in the code and flip the allocation (iterator in ebx, frame 0x60, no bl). Retail has ONE test and the v28 allocation.

## Ideas tried (do not repeat) -- every one leaves the same 381 residue unless noted
- Copy spellings: copy ctor from `*begin` / `front()` (375, x87-forwarded); declare+assign (381); memcpy intrinsic; implicit trivial copy ctor
  (probe overlay); declared before loop / at loop top; `FABLE_RETAIL_C3DVECTOR` body ctor; `FABLE_RETAIL_C3DVECTOR_PACKED` (alignment 1).
- Lift spellings: 3-float ctor; `top = start; top.Z += 0.15f` (ctor and assignment forms; the assignment form deletes `start`, 373);
  `start + C3DVector(0,0,0.15f)`; `+ C3DVector(0,0,1) * 0.15f`; `- C3DVector(0,0,-0.15f)`; `-0.0f` zeros; named offset local; temporary inside
  the line ctor argument list; memberwise Z,Y,X / X,Y,Z.
- Late-folded identities (GOTCHAS 207 / 219): `/ 1.0f` on the element, on `start`, on the sum, on the offset; `(…)/20`. No effect or worse.
- GetStartPoint body forms: early return (both polarities), `bool found` local, `?:` / `&&` with comma, size()==0 / Points[0], iterator
  local, for/while loop returning on the first point. Caller forms: direct `if`, `bool has_start`, `continue`, result ignored + empty() test,
  lift before the test, one-trip loops / do-while(0) / empty `if` between copy and lift.
- Barriers: `__assume`, `__noop`, named red colour between copy and lift, really escaped `start` (extern call probe), C3DVector[2] /
  C3DVector[1] destinations (array[2] gives the classic copy but the registers are still forwarded), volatile reference read (probe only:
  gives fld/fstp copies, not retail's dword reloads -- volatile is NOT the answer).
- Flags: /Op (classic copy + reloads appear, but frame 0x68 and this/info swap ebp/edi), /Oa (breaks the loop), default.

## Best next ideas
1. Find what makes VC7.1 keep the struct assignment unsplit AND stop forwarding inside ONE block with the v28 CFG: read the Ego_r build of
   DrawShapes (0x00772C30, 0x1F0 bytes = 107 bytes longer than retail) on the owner's machine -- it very likely shows a statement between
   GetStartPoint and the lift that retail compiled out (the 0x008031E0 lesson). That is the missing fact; nothing on this box can show it.
2. Try the join family with the second test on values the optimizer folds LATE (v49 with iterator locals folded mid-way and gave a partial
   X reload): e.g. GetStartPoint written with begin/end locals plus the caller testing the returned bool, or a caller-side `&&`.
3. (tried after this list was written, no effect: CRGBColour 3-argument ctor / Initialise chain as the red colour, named or temporary;
   /Op with memberwise lifts.) Remaining untested level: a statement that puts a real call or an indirect store between the copy and
   the lift in the IL yet leaves no instruction there -- the Ego_r listing is the way to find it.

# 0x00761980 CTCShapeManager::DrawShapes -- wf3 author pass 2 (still DIFFER 381 v 389, 6 hunks)

Header: rows matched 0; row in progress 0x00761980 (about 110 grades in this pass, 220 in all); rows untouched 0; probes: listings only; tool calls about 100.

## What this pass proved (keep)
- CALLEE VISIBILITY (new, keep): the CEnginePrimitiveLine constructor 0x004C3090 is an inline that retail calls here but expands in sibling
  0x007617A0 of the same TU. With its real body visible and out of line (overlay opt-in `FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE`,
  `__declspec(noinline)` on the in-class definition) the red colour stores move after `lea eax,[red] ; push eax`, exactly as retail. Everything outside
  0x00761A07..0x00761A46 now matches. differ/00761980.cpp carries it. CThing::GetPos 0x004C73D0 visible (noinline) changes nothing.
- FableWin debug sizes (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv): GetStartPoint 0x025D6620 = 67 bytes / 26 instructions, which is
  exactly `if (Points.empty()) return false; pos = Points.front(); return true;` at /Od with the implicit (block) operator= (hand count), so the
  body in the overlay is right. FableWin DrawShapes = 777 bytes / 218 instructions, ours at /Od = 424 / 142; Ego_r DrawShapes = 481 bytes / 144
  instructions against retail 386 / 122: both PDB builds carry more code than retail (editor build), so they are not a statement-count oracle.
- Retail-wide scan: only 16 same-block `mov [esp+d],r ; mov r2,[esp+d]` sites exist in Fable.exe. The landed ones are (a) the third field of a
  user-copy-ctor chain into a by-value argument (0x0093C7F2, 0x008C1738) and (b) scalar constructor stores read back by a 16-byte implicit struct
  copy through a const reference of a FORCEINLINE function (0x0098A42E, CShaderRenderManager::UpdateObjectSpaceLighting: `C4DVector vect = src`).
- What forwards and what does not (all graded on the row): VC7.1 forwards every plain 4-byte copy out of the 12-byte copy, whatever the spelling,
  even after a late constant fold (`/ 1.0f` on the offset, on start, on the sum). The reload survives only when the read is (1) in another
  basic block at optimisation time (the join family of pass 1), or (2) an x87-evaluated stage: a PAREN-grouped identity such as operator+ with
  `(X+other.X),(Y+other.Y),Z+other.Z` (repo opt-in `FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP`) and `start + C3DVector(0,0,0.15f)`, or
  `(float)(start.X * 1.0)` (GOTCHAS 219). Family (2) gives EXACTLY 389 bytes with retail's allocation (bl, iterator slot), but the two stages are
  `fld [start.X] ; fstp [top.X]` in X, Y, Z order where retail has `mov eax,[start.Y] ... mov edx,[start.X]` (integer registers, order Z, Y, X).
  Saved as differ/00761980_grouped_operator_plus.cpp (DIFFER 389 v 389, 12 hunks).

## Ideas tried in pass 2 (do not repeat) -- same 381 residue unless noted
- Codeless uses of start between copy and lift: empty inline by const ref / by pointer, dead pointer local, inline-false guarding an escaping
  call or a store, conditional jump to the next statement (inline void with early return, do{if(c)break;}while(0), if(c) goto next).
- Late-folded guards around lift+draw: `1.0f/1.0f > 0`, null test of &start (folded early); `start.Z == start.Z` (real code, 384).
- Identical arms: `if (c) {lift+draw} else {lift+draw}` (454 / 438, not merged: different temporaries), lift duplicated in two arms with the
  same variables (417 / 391, not merged), ternary constants, reference ternary `c ? start : start`.
- Second copy stages: user copy ctor local, by-value inline parameter (user and implicit copy), block `v = src` through a const-reference inline
  parameter on a named start (386) and on a copy-ctor temporary (372), scalar reads of a copy-ctor temporary (375, x87-forwarded).
- GetStartPoint behind a second inline level (forwarder, forwarder with its own return, int-returning forwarder); as an out-of-class non-inline
  or inline definition in the TU (both polarities).
- Stage 1 as float-typed memberwise copies (inline getter, in the body): 375, x87-forwarded. Stage 1 copy ctor + block `top = start`: 385.
- TU flags on the best source: /G5, /Ox, /Ob1, /Ow, /O1 /Ot (381); /G7 (379); /GL (error); /Op with the ctor visible (390 / 373 / 401).
- Really captured start (extern call before the loop, in the loop, after AddPrimitive): stores pinned in X, Y, Z order, reads still forwarded.
- With the ctor visible: lift temp in the argument list, `start + C3DVector(0,0,.15f)` (named and in the list), copy ctor + Z+=, block top = start
  (373), copy ctor + `+= C3DVector`, Initialise(x,y,z+.15), ctor(x,y,z) then Z+=, start / top at function scope, PAREN tuples in the lift (381, 7
  hunks: scheduling moves, no reload), C3DVector pts[2] (382 / 381 / 396), the join family again (365 / 379 / 380: allocation still wrong).
- Late zero in the offset without grouping: `start + C3DVector(0,0,.15f)/1.0f`, `C3DVector(0,0,.3f)/2.0f`, `C3DVector(0,0,1)/(1/.15f)`: 381.
- Grouped identities: operator+ grouped with 0.0f zeros / -0.0f zeros, operator- grouped with (0,0,-.15f) (fsub), `(float)(X*1.0)`,
  `(float)(X+0.0)`, grouped sums with a named offset vector: all 389 with x87 copy stages; literal `(start.X - 0.0f)` folds early (381).

## Best next ideas
1. Stay in family (2): find the identity stage that VC7.1 emits through integer registers and in argument order Z, Y, X. Retail's
   `fld [start.Z] ; mov eax,[start.Y] ; fadd ; ... ; mov [start.X],ecx ; mov edx,[start.X]` reads Y and X as dwords AFTER the Z load, i.e. as
   right-to-left by-value float arguments that were materialised. Candidates not yet graded: a grouped operator+ whose X/Y sums feed a by-value
   float parameter of a second inline level (e.g. `C3DVector r; r.Initialise((X+o.X),(Y+o.Y),Z+o.Z); return r;` or operator+ written through
   `C3DVector r(*this); r += other;` with a grouped operator+=), `FABLE_RETAIL_C3DVECTOR` body ctor (`{X=x;Y=y;Z=z;}`) under the grouped operator+,
   the grouped operator+ with the result bound through the copy ctor (`C3DVector top(start + off)` vs `= `), and the identity on an integer
   register path (x87 is chosen when all three scratch registers hold the split copy: try the grouped form with stage 1 spelled so the copy is
   the unsplit template, e.g. start declared at function scope + grouped operator+).
2. Read the Ego_r / FableWin bodies of `??HC3DVector@@QBE?BV0@ABV0@@Z` (operator+, donor 0x018C0B60) on the owner's machine: the header comment
   says its grouping came from that body; its exact local/temporary structure decides whether the stages are x87 or integer.
3. c2 tuple dump (tools/decomp_pipeline/linux_lane/lane_tools/c2probe) of differ/00761980.cpp against differ/00761980_grouped_operator_plus.cpp to
   see which tuple kind becomes `mov r,[m]` (plain float assignment) and which becomes `fld/fstp` (PAREN-evaluated).
- (pass 2, last batch) X and Y read as integer bits through `*(const unsigned long*)&start.X` (either order): 373, the copy is forwarded and
  start disappears -- type punning is NOT the answer. Grouped operator+ with start at function scope: 389, same x87 stages (stage 1 stays split).
- Files: best source differ/00761980.cpp (381, 6 hunks); size-equal alternative differ/00761980_grouped_operator_plus.cpp (389, 12 hunks, x87
  stages); overlay hdr/engine/{CEnginePrimitiveLine.h,NShape_CClosedShapeBase.h,CTCShapeManager.h}. No landed name was proved wrong in this pass
  (NAME_FIXES.tsv from pass 1 stands).

# 0x00761980 CTCShapeManager::DrawShapes -- wf3 author pass 3: RELOCATION_MATCH (389 v 389)

Header: rows matched 1 (0x00761980); row in progress none; rows untouched 0; grades this pass 57 (291 in all); probes: listings of graded variants only; tool calls about 45.

## What made it match
- One statement: the owner thing is named in a reference BEFORE the lifted vector is built,
  `CThing& parent = GetParentThing(); C3DVector top(start.X, start.Y, start.Z + 0.15f); ... parent.GetPos() ...`
  (`CThing* parent = ParentThing;` and `CThing& parent = *ParentThing;` give the same bytes). Everything else is pass 2's best source.
- Why: VC7.1 hands out its scratch registers in rotation (eax, ecx, edx, eax, ...) in evaluation order and reuses a register that still
  holds a just-stored value instead of reloading it. Retail's order is: block copy (eax = source pointer, ecx = X, edx = Y, eax = Z), then
  ecx for the parent (0x00761A39 `mov ecx,[edi+4]`), then edx for start.X and eax for start.Y, then edx for info.Engine. The parent's ecx
  overwrites the register that held X, so X is reloaded into edx, which overwrites the register that held Y, so Y is reloaded too: the two
  "unexplained" reloads and the unsplit block copy. With the parent read at the call (in the argument list) the X and Y reads come
  straight after the copy, find their values in registers and are forwarded (381 bytes).
- Proof of the mechanism on the row (all in ATTEMPTS.log): pure component copies in the order Z, X, Y reload X and Y (bp3); memberwise lifts
  with Y before X reload X only and copy Y register to register (bc3 / bf2 / bf4, 387); X before Y forwards both (381).
- INFERENCE, flagged in the source header (`// Inferred:`): the bytes prove that the parent pointer was evaluated between the start-point
  copy and the X / Y reads; the exact wording of the local is not provable. Behaviour is identical to reading the parent in the argument list.

## Needs
- Headers: the row overlay hdr/ unchanged from pass 2 (CEnginePrimitiveLine.h `FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE`,
  NShape_CClosedShapeBase.h GetStartPoint / EditorDrawHighlightPoints opt-ins, CTCShapeManager.h DRAWSHAPES opt-in). No new opt-in.
  `CTCBase::GetParentThing` is the repo header's FableWin.pdb inline accessor (engine/CTCBase.h under FABLE_RETAIL_CTCBASE).
- Flags: none. Strict-header regrade (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH. Test: match/00761980_test.cpp, vt.py PASS.
- Files: match/00761980.cpp, match/00761980_test.cpp, payload.json, oracle.tsv. differ/00761980.cpp is the superseded pass-2 source.
- Made-up names: none (locals only: shapeColour, selectedColour, map, shape, start, parent, top). No landed name proved wrong in this pass.

## Tried in pass 3 before the match (no effect, for the record)
- /Op on memberwise lifts (373); second block copy with Z overwritten (381 / 395); the join family with a Z,Y,X lift (379 / 380);
  GetStartPoint through a pointer local (381 / 387); C3DVector pts[2] with a Z,Y,X lift (388); start or top at function scope (387);
  offset vector at function scope (381); PAREN tuples round the Z sum (387, scheduling only); identical block-copy arms (365 / 371 / 374,
  merged early, leaves a dead `cmp`); an 8-byte C2DVector / __int64 move of X,Y (373); a dead-assignment `if` between copy and lift (381 / 387).

## 0x00A89740


## 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated (one-state) -- RELOCATION_MATCH (wf3 author, first attempt, 24 grades)
- What made it match: port onto the shared headers with the in-place form the dossier derived (`C3DVector pos(helper.Position);
  Bones[i].LocalizationMatrix.TransformPoint(pos); CalcBoneTransformation(state,*this,i,transform,scale);
  pos = scale.GetScaled(pos); transform.TransformPoint(pos); return pos;`, no-bone return last) gave 389v389 at once with the same
  instructions; the only residue was the epilogue's `mov eax,[result] ; pop esi` scheduled four x87 instructions early. That went
  away when TransformPoint's rows are spelled as plain left-to-right sums in the order retail evaluates them,
  `E3k*zz+E2k*yy+E1k*xx+E4k` (new opt-in FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS) instead of the parenthesised default.
- Headers / flags: no compiler flags. Defines CMOVABLERESOURCE, C3DMESHSTATS_DUMMYOBJECTS, C3DMESH2_HELPERPOINTS (new), CMATRIX3X4,
  CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS (new), C3DVECTOR, C3DVECTOR_GETSCALED (new); overlay = hdr/ (3 headers, hdr/README.md).
  Strict-headers regrade: RELOCATION_MATCH. Test 00a89740_test.cpp: vt.py PASS.
- Local models (no shared header exists): C3DHelperPoint, C3DBone2 (Ego_r member names), C3DAnimationManager (declaration of
  CalcBoneTransformation 0x00AA0FA0, PDB reference signature -- the old draft's pointer spelling was wrong).
- Names: all from Ego_r.pdb + FableWin.pdb; nothing made up; no landed name proved wrong.
- Ideas tried before the match (all 389v389 or worse): scale.GetScaled(pos) vs pos.GetScaled(scale) (same bytes); named `bone`
  local (4 hunks); transform/scale declared first (same residue); early return / `== -1` first (385); pos copied before the test
  with one return (381); `return C3DVector(pos)` (same residue); assignment-form copy ctor via CTCTARGETINGPLAYER_UPDATE_LIST (same
  residue); GetScaled into a new named local (365); second transform written out in the row (also flips the FIRST transform's Z
  row term order) with outer parens (+1 instruction) / all parens (-2); header body variants A..F: only B (plain ZYX sums) matches.
- For the twins (0x00A898D0, 0x00A89A70, 0x00A89E40, 0x00A8A220, same TU, parked on "x87 operand order"): try this overlay
  (C3DMESH2_HELPERPOINTS + CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS + C3DVECTOR_GETSCALED) first.
- Scratch left in the row directory (not part of the hand-back): v1..v14.cpp, tA..tF.cpp, *.out, r.txt, o.txt.

## 0x009122A0


## 0x009122A0 CBezierCurve::GetPointOnCurve -- RELOCATION_MATCH (author, first attempt)
- What made it match (two source facts, both were model errors in the parked draft, not compiler ties):
  1. the squares go through the engine's inline `GFSqr(float)` (PDB `?GFSqr@@YIMM@Z`, already in engine/C3DVector.h under
     `FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER`): `GFSqr(1.0f - t)` has its argument evaluated before the object expression
     `Point2 * 3.0f * t`, which gives retail's early `fld (1-t)` and the square spilled to the dead t slot. `GFSqr(t)` is needed too
     (t*t written out: 390v390, 2 hunks).
  2. the sum is a named local returned by value (`C3DVector result = ...; return result;`); `return <expr>;` is 374v390 (the four
     component spills of the final additions are missing).
  Also: `pow(x, 3.0f)` (math.h float overload); `pow(x, 3.0)` on a float base does not compile (C2666) with engine/C3DVector.h's math.h.
- Headers / flags: overlay hdr/engine/CBezierCurve.h (`FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE` declares the member); existing
  `FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER` of the repo's C3DVector.h (no overlay copy). No flags, no pragma. Strict-headers regrade passes.
- Ideas graded that did NOT match (do not repeat): written-out squares (358v390); named `float omt` local (362v390); `pow(x, 2)` int
  overload (453v390); unparenthesised `* (1-t) * (1-t)` (382v390); ADD_XY_GROUP / SCALE_X_PAREN with or without the helper (no effect
  on the residue); template const-ref square helper (366v390).
- Names: no made-up names (GFSqr, GetPointOnCurve, Point1..Point4 all from the PDBs). No landed name proved wrong.
- The identity of the square helper is an inference from the bytes (an inline float(float) square whose argument is evaluated early);
  GFSqr is the engine's only such helper in the PDBs, but GFSqr leaves no call in this function to prove the name.

## 0x00BE7BE0


## 0x00BE7BE0 CLandscapeBackgroundPatch::Render(bool, long&, bool&) -- RELOCATION_MATCH (33 grades, first authoring attempt)
- What made it match: the tail of the function is not Render's own code but three header inlines named in FableWin.pdb --
  `HasWater()` (null test), `CEngineWaterRenderer::AddBackgroundWaterPatchToRenderList(patch, alphaed)` and, nested in it for sea
  water (types 3/4/5) and called directly for a water-only patch, `AddMidgroundSeaPatchToRenderList(patch, alphaed)`; both take the
  sub-patch pointer BY VALUE (`QAV...@@_N`) and pass its address to `CArray::push_back(const T&)`. That explains every old residue:
  two stores of the pointer to the arg0 slot (one per by-value copy), one PolyCount test (each helper starts with `!IsEmpty()`; the
  nested one folds), the cross-jumped sea arm and esi = 4 (the sea helper is `if (alphaed) A.push_back(p); else B.push_back(p);`,
  not a list select), four out-of-line push_back calls in the background helper.
- Proof of the helper bodies: retail has unreferenced out-of-line copies of both at 0x00BE9030 (sea, `ret 8`) and 0x00BE9080
  (background with sea expanded, `ret 8`); not in the manifest. Same statements in the same order.
- Headers / flags: `// Flags: none`. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_RENDER,
  FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS, FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST (+ existing
  FABLE_RETAIL_CARRAY_PUSHBACK, FABLE_RETAIL_CENGINEWATERRENDERER, FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH). The STLport
  `_M_insert_overflow` body must be VISIBLE (no FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY): declaration-only puts the two pointer
  copies in the arg1/arg2 slots instead of arg0.
- Inline budget (needs the coordinator's eye): the two helpers are FABLE_FORCEINLINE in the header opt-in. As plain `inline` VC7.1
  expands them only at the constant-`false` water-only site and calls them at the variable-`alphaed` site (180 v 391). Retail expanded
  both there; same bodies either way. `..._ADDTORENDERLIST_PLAIN_INLINE` switches the header back to plain inline (for a TU that
  should emit the copies 0x00BE9030 / 0x00BE9080).
- Names: all PDB (FableWin / Ego_r) except parameter names (`withOcclusion`, `queryHandle`, `occluded`, `patch`, `alphaed`), which
  are descriptive, and the enum `EWaterType { WATER_TYPE_NONE }` (enumerators unproven; 3/4/5 = sea, 8 = ice stay numeric).
  IsEmpty's body is `!(PolyCount > 0)` (`cmp word,0 ; jbe`); HasWater's is `WaterSubPatch != 0` (28 bytes in FableWin).
- Landed names proved wrong: see NAME_FIXES.tsv (0x00BF71D0's declaration of this function, 0x00BE8FF0, 0x00BE8F10).
- Ideas graded on the way (all DIFFER): direct switch / if-chain with CArray::push_back at six sites (nothing expanded, 320);
  helpers with the dispatch in Render (238 / 234); list select `(alphaed ? A : B).push_back` (388, sea arm laid out `add ; jmp ; add`,
  no esi = 4); empty test left in Render (387, late pointer store); declaration-only overflow (387, wrong slots).

## 0x0081F240

# 0x0081F240 CMap::CopyPos -- RELOCATION_MATCH (wf3 author, 5 grades, strict headers OK, test PASS)
- What made it match: ported onto engine/CMap.h + engine/CHeightMap.h and spelled every access through the PDB inline helpers
  (`SetEngineThemeAtIncludingExtraCells(to.X, to.Y, source.GetEngineThemeAtIncludingExtraCells(from.X, from.Y, i), i)` in a
  `for (long i ...)` loop; two explicit `SetEngineBlendAtIncludingExtraCells(..., source.GetEngineBlendAtIncludingExtraCells(..., n), n)`
  calls for n = 0, 1). The helpers alone reproduce the old "register pressure" residue (this->HeightMap reloaded 3x, source cells
  base spilled). Last hunk was `jb` v `jl`: the layer counter is a signed long (params are unsigned long per the PDB).
- Needs: overlay hdr/engine/CMap.h (`FABLE_RETAIL_CMAP_ENGINEACCESSORS`), hdr/engine/CHeightMapCell.h
  (`FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS`); defines THEMEPALETTE, CHEIGHTMAPCELL, C2DBOXI, CHEIGHTMAP_GETCELL, SCOPED_PTR. No flags, no pragma.
- Not graded: blend as a 2-trip `for (long j...)` loop (v5.cpp; fg.py stopped at the first match). Unsigned-counter forms v1-v3 all gave only `jb`.
- No made-up names: all helper names are FableWin.pdb / Ego_r.pdb leaves (see hdr comments). GetCell on a const CMap works because
  boost::scoped_ptr::operator-> returns T* (PDB also has a private const PeekCell; not needed).
- Name/signature fixes for landed rows (NAME_FIXES.tsv): 0x0081EF30 last parameter is `unsigned long` (…@IAEXJJKK@Z), landed row spells
  `long`; landed caller CWorldMap_UpdateAllEdges_0063dfd0.cpp declares `CopyPos(CMap& src, ...)`, PDB is `const CMap&` (ABV1).
- differ/0081f240.cpp is the stale parked draft (superseded by 0081f240.cpp in payload.json).

## 0x00646580

# 0x00646580 NPlayerGui::CDrawMeleeActionButtonEffects::Draw -- RELOCATION_MATCH (393v393), default and LANDVERIFY_STRICT_HEADERS=1

What made it match (in the order it was found):
1. Ported onto the shared headers with the dossier's identities (CDrawBase, CPlayerGui, CPlayerGuiDef, C2DVector, C3DVector, CRightHandedSet,
   CIntelligentPointer, CEnginePrimitive2DSprite, CIEngine, CEnginePrimitivePrepareInfo, CParticleEmitterManager, CDisplayManager).
2. `if (!IsShowSpecialAbilityButtons()) { ... } else FlourishEffect = 0;` -- retail lays the reset arm last (0x006466F2).
3. Sprite arm = two named locals before the sprite statement: `C2DVector anchored = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);`
   `C2DVector position = def.AbilityButtonFlourishGraphicOffset + anchored;` (operand order offset + anchored gives `fld [anchored] ; fadd [def]`),
   and the sprite's position argument is `C2DVector(position)` (the copy gives retail's `fld st(1) ; fstp ; fst ; fstp st(0) ; fstp st(0)` stores
   just before ScaleAlpha, and keeps 0 in ebx for the whole function). INFERENCE, marked in the source comment: the copy's exact source
   spelling is not proven (same idiom as the landed DrawButton 0x00645099); behaviour is identical.
4. Last residue (second uv temporary at [ebp-24h] instead of [ebp-8], 3 hunks, 393v393) was CALLEE BODY VISIBILITY (the Q32 lesson):
   GFAnchorToBottomRight 0x00643D0C is defined earlier in the same retail source file; with its REAL landed body in the TU
   (`#pragma auto_inline(off)`, out of line) VC7.1 knows the callee does not keep the hidden result pointer, so the `anchored` local's
   slot is free after its last read and the uv temporary reuses it. The in-TU body itself grades RELOCATION_MATCH against 0x00643D0C
   (`FABLE_HDR_OVERLAY=$W/hdr q.py 00643d0c 00646580.cpp GFAnchorToBottomRight`).

Headers / flags it needs: no compiler flags; pragmas `optimize("y", off)` + `optimize("s", on)`. Overlay opt-ins ($W/hdr, see hdr/README.md):
FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS (new, NPlayerGui_CDrawBase.h), FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS (new,
CPlayerGui.h; FableWin ?IsShowSpecialAbilityButtons@CPlayerGui@@QBE_NXZ), FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH (new, CPlayerGuiDef.h).
Out-of-line real body of a landed callee in the row file: NPlayerGui::GFAnchorToBottomRight 0x00643D0C.
The class has no shared header: local model deriving from the shared NPlayerGui::CDrawBase with the Ego_r.pdb member names
(Effects, FlashTimer, ComboLoadingIconTimer, FlourishCounter, FlourishEffect). No made-up names.

Landed names proved wrong (see NAME_FIXES.tsv): 0x00643D5D is CDrawBase::ScaleAlpha (landed as Cons_val_CEntry garbage label);
0x0064BE45 Update's local member "EffectCount" (+0x40) is FlourishEffect and "PCurrentAbility" (+0x44) is PMeleeAbilityDef (Ego_r.pdb);
landed DrawButton 0x00645099 calls CIEngine::SetPersistentPrimitive (slot 23) "AddPrimitive" on a "Renderer" (it is info.Engine).

Ideas tried that did NOT match (do not repeat): single-expression sprite position (uv temporaries stored before the GFAnchor call, 0 not
in ebx); `GFAnchor(P) + offset` operand order (Y summed first, via eax); position named without the copy (direct fstp stores, 387);
`offset + GFAnchor(P)` temporary (read through eax, slots permuted, 392); block scope + assignment (bitwise copies, 399/411);
`GFAnchor(P) += offset` (stores through eax, 401/410); `position(offset); position += anchored` (memory copy before the call, 405);
inline helper wrapping anchor + offset (not inlined under optimize("s"), 388).

## 0x0052C920

## 0x0052C920 NUISystem::CChangingStateComponent::ProcessChangeState -- RELOCATION_MATCH (author pass 1, 4 grades)
- What made it match: (1) the port onto the shared headers (engine/NUISystem_CChangingStateComponent.h, NUISystem_CUIState.h,
  C2DVector::IsNonZero / DELTA_NOTIONAL_ZERO instead of the local GetLengthSquared / g_Epsilon); (2) the colour delta spelled as a
  COPY of the target colour followed by four channel `-=` against the component's Colour (`CRGBColour delta = target->Colour;
  delta.R -= Colour.R; delta.G -= ...; delta.B -= ...; delta.A -= ...;`) with FABLE_RETAIL_CRGBCOLOUR, whose copy constructor
  reads all four channels through Initialise before any store. That gives retail's `mov al,t.R ; sub al,[c.R]` memory operands for
  R/G, the ebx byte register, and the R,G,A,B store order. A 4-argument constructor of the four differences leaves the old
  393v395 residue (graded again as v1); the old draft's operator- did the same.
- Headers / flags: existing opt-ins only -- FABLE_RETAIL_C2DVECTOR, FABLE_RETAIL_C2DVECTOR_NORMALISE, FABLE_RETAIL_CRGBCOLOUR.
  FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX is NOT needed (plain `X*X+Y*Y` already gives the Y-first squares here). No pragma,
  no extra flags, no overlay header. Strict-headers regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Names: no made-up names in the row. The colour subtraction is written on the public channel members in the function body,
  because neither PDB has a CRGBColour operator-= / operator- to name (an inline one would leave no symbol, so retail may well
  have had `delta -= Colour`; not provable, and not needed).
- Landed spellings the dossier flags as wrong (seen again in the landed sources, see NAME_FIXES.tsv): rows 0x005546F0 and
  0x0052C780 call slots 134/135 `GetStateInfo` / `OnStateChanged`; PDB: `GetState` (const slot 134 / non-const slot 135).
- Files: source $W/0052c920.cpp, test $W/0052c920_test.cpp, payload.json + oracle.tsv written by addmatch.py; the stale
  differ/0052c920.cpp was removed (draft_original.cpp keeps the parked draft).

## 0x00848B70


## 0x00848B70 CCreatureAction_PickUpGenericBox::PerformAction -- RELOCATION_MATCH (4 grades: 1 SRC_FAIL, 1 DIFFER 390v402, 2 MATCH incl. strict)
- What made it match: (1) the port onto the shared retail headers (CThing GetTC/PeekTC expanded form, engine/ThingAlive.h IS_THING_ALIVE
  template inlined, CDefClassBase_GetSubDef.h with the GetSubDef<CCarryableDef> body visible/out of line, CTCCarryable::PCarryingTC,
  CCarryableDef::ActiveCarrySlot, CClass::GetType) removed every register / frame residue of the old draft at once; (2) the two
  FinishAction() calls are the ELSE arms of nested ifs (`if (alive) { ... if (!carried) {...} else FinishAction(); } else FinishAction();`).
  Early returns put the two arms in line (DIFFER 390v402); the else form gives retail's tail placement and shrink-wrapped epilogues.
- Headers / flags: no overlay opt-in needed ($W/hdr is empty), no // Flags: line, default flags. Macro set is in the source
  (FABLE_RETAIL_CCREATUREACTION_GENERICBOX + _CCREATUREACTIONBASE + _CCREATUREACTIONBASE_PCREATURE, _CTHING + _CTHING_GETTC_EXPANDED,
  _IS_THING_ALIVE (not _NOINLINE), _CDEFCLASSBASE_GETSUBDEF_NOINLINE, _CTCCARRYING_PUTTHING, _CTCCREATUREMODEMANAGER ...).
  Passes LANDVERIFY_STRICT_HEADERS=1. Test: 00848b70_test.cpp (vt.py PASS).
- Names: all from the PDB extracts (dossier); no made-up names. Old draft spellings proven wrong by the dossier: GiveUp -> FinishAction,
  CTCOwnable::Owner -> CTCCarryable::PCarryingTC, CAbilityDef::CarrySlot -> CCarryableDef::ActiveCarrySlot (draft only, not landed).
- Local variable names (ptc_carryable, ptc_carrying, held, def, ptc_modes) are descriptive (locals are in no PDB extract).

