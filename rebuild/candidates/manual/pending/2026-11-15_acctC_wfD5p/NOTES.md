# wfD5 NOTES

## 0x009D8CF0 CRenderManager2D::ClearRenderTarget -- RELOCATION_MATCH, test PASS, addmatch done, vland WINS (no RELOC_IDENTITY)
- Port: the old STENCILOP_INLINE opt-in is gone from today's header; its three setters are now repo opt-ins
  FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE (Fail/ZFail) + _OUTLINE_INLINE (Pass): that restores 3385v3385.
- Closer (all FITTED, WF_RULES device a): repo opt-ins SETRENDERSTATE_DESIRED + SETRENDERSTATE_BOOKMARK + ADDSTATE_DIRTY (named-symbol,
  code-identical texts) + `CShaderRenderManager::Get()` re-called at the three shader calls (no `shaders` reference) + a named
  `CDisplayManager& display` for GetViewport / GetRenderTargetDimensions. The per-site bookmark test/or operand order is a symbol-count
  effect of the WHOLE function (removing `shaders` flipped a stencil site BEFORE it); each of the five devices is needed (dropping any
  one flips 2..6 sites). Using `display` for the SetViewport calls changes length (3331): retail re-calls GetDisplayManager().
- Flags /GS (cookie read 0x009D8CF6). Test: hand864's model test (009d8cf0_TEST PASS). No overlay header change.
## 0x009D9C80 CRenderManager2D::DrawBufferedPolys -- RELOCATION_MATCH, test PASS (hand864 model test), addmatch done, vland WINS
- Closer (EVIDENCED, no fitted device): the alpha-list window reset is `current_window.Initialise(0.0f, 0.0f, 0.0f, 0.0f)` (FableWin.pdb
  ?Initialise@C2DBoxF@@QAEXMMMM@Z, inline in release; existing overlay opt-in FABLE_RETAIL_C2DBOXF_INITIALISE from agentC43) instead of
  memset: retail 0x009DA64B one zero register for 7 stores. memset gave a second zero piece (+16 bytes of pads); plain member stores fixed
  the zero register but flipped 11 bookmark sites (symbol count); the Initialise call has exactly retail's symbol population.
- No flags. No overlay header change.
## 0x00F3BDD0 CTriangleStripifier2::FillInTriStrip -- parked DIFFER(2976v2998) (first pass, ~25 grades)
- Name: Ego_r 0x00BD7790 / FableWin 0x0331EFE0 ?FillInTriStrip@CTriangleStripifier2@@AAE... (two sources). Header: today's repo header
  lacks the private helpers; overlay copy re-applies the chunk-730 opt-in FABLE_RETAIL_CTRIANGLESTRIPIFIER2 (decls only, hdr/README.md).
- Residue = ONE register swap in the scoring loop (structure, reference counts and frame homes identical: retail ebp 40 / edi 70 refs v
  ours edi 46 / ebp 62 over 0xF3C060..0xF3C790): retail poly ebp (home [esp+40h]) + best_score edi (home [esp+18h]); ours swapped. The 22
  missing bytes are all `[ebp+...]` disp8 / `lea ebp,[ebp+ebp*2]` encodings + the alignment pads that follow.
- Tried (no change): best_* declaration order (3 orders), best_* at for(;;) / function scope, poly declared at j-loop / for(;;) /
  function scope / before best_*, const poly, poly int, best_score int / unsigned long, j long. A `const CArray<long>&` alias for
  vertex_to_poly_map[vert] is WORSE (2965, retail re-reads &map[vert] from its home [esp+54h] each latch).
## 0x00531EC0 NUISystem::CComponent::Update -- parked DIFFER(3523v3642) (first pass, ~45 grades)
- Port: today's NUISystem_CComponent.h lacks the chunk-862 opt-in; re-applied as FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS on a copy of
  today's header (Deletion typed CDeletion, PositionChildren typed set; 4 asserts pinned; hdr/README.md). Draft back to 3455v3642.
- Retail call list: 8 x ConvertVector 0x0052E580 (loops 1 and 2) / loop 3 expands it (2 expansions) | deletion copy: malloc + insert
  0x0042B4C3 | THIS_INSTANCE: malloc, malloc, insert, ~list 0x0042ABCA (CALL) | PER_PARENT: free (erase), malloc, ??0CDeletion copy
  0x0042CD84 (CALL), ~list 0x0042ABCA (CALL) | `deletion` dtor expanded (free loop + free) | __find 0x00534EB0, __copy 0x00535000, ??3.
- With FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE (+ _INLINE; the landed 0x0052E580 text) the heuristic inliner decides:
  default T74: loops 2+3 expand (CCCCIIII, 3902); /d2inlT52..56: CCCCCCCCII = retail's ConvertVector split (3523v3642, 104 hunks).
  The deletion sites want other T values: ??0CDeletion called only at T>=60, ~list (with FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST) called
  only at T>=66 (then the tail __find/__copy/??3 also matches), and the THIS_INSTANCE new_deletion dtor is expanded at every T tried.
  No single T fits; budget path (/Og- /d2vol2..10 + pragma optimize g) is structurally unlike retail (calls _M_increment, list ctors).
  Scoped `#pragma inline_depth(0)` round the loop-2 ConvertVector statements also stops the C2DVector ctor / operator+ expansions
  retail has there (3646v3642); inline_depth(0) round the THIS arm calls ??0CDeletion / vector ops instead.
- Next: per-site device that only blocks ConvertVector in loop 2 (e.g. inline_depth(1) or the call through a by-value temp), then
  T66 + DTOR_IN_LIST for the deletion tail; fix the THIS_INSTANCE dtor (maybe new_deletion's scope / a CDeletion() temporary).
## 0x00F3BDD0 CTriangleStripifier2::FillInTriStrip -- RELOCATION_MATCH, test PASS, addmatch done, vland WINS (supersedes the park note above)
- Closer (FITTED statement order, device a): the assignment order inside the 15 scoring arms. A greedy per-arm search (tools/greedy1.py,
  tools/combo2.py; ~120 grades) over the 6 orders: best_score assigned FIRST in arms 13 and 19 (`best_score = N; best_poly = poly;
  best_vert = ...`), LAST in the others (VPS for 3/2/6/10, PVS for 7/5/12/11/9/17/18/16/15). Assigning it first changes c2's priority
  tie between poly and best_score (retail ebp / edi); assigning it first in the arms that retail tail-merges (`mov [esp+18h],edi`
  at 0x00F3C24A) splits the merge -- only arms 13/19 tolerate it. Last fix: `last_vertex = vert;` before `reversed = !reversed;` in
  the missing-vertex block (retail stores 0x00F3C90D / 0x00F3C911).
- Test: model test (std::vector stand-in for CArray) with three meshes, expected strips traced on the host; the mutant without the
  score-13 branch fails. (The score-7 arm is behaviourally redundant with score 6: a random search of 200k meshes found no difference.)
- No flags. Overlay: FABLE_RETAIL_CTRIANGLESTRIPIFIER2 (decls only).
## 0x00A593F0 NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle -- parked DIFFER(3043v3027) (first pass, ~15 grades)
- Draft ports onto today's headers unchanged (3043v3027, 60 hunks); first ~230 instructions match.
- First structural divergence (edge loop 0x00A597D3..): retail reloads points._M_start ([esp+7Ch]) at every use and keeps the idiv
  divisor 3 in ecx for BOTH `% 3` (one `mov ecx,3` per iteration); ours holds _M_start in ecx from the preheader and uses esi=3 then
  ecx=3. edi is free in both loops (ebp = aligned frame pointer), so retail simply does not CSE the begin loads. Loop IV: retail ebx =
  i+3 with a countdown slot [esp+0BCh] = 3 (ours the same).
- x87: product = major*minor loads major first in retail ([esp+20h]), ours minor ([esp+6Ch]); source operand order and declaration
  order do not move it; Radius copied as an int (`mov eax,[edi] ; mov [esp+6Ch],eax`) after the major fdiv in retail, before in ours.
- Tried, no change: major/minor declaration order, product operand order (both ways), `line_end` reference, `*(points.begin()+..)`,
  FABLE_RETAIL_STLP_VECTOR_INDEX_START, int loop index (unsigned is worse, 3050). `product = Radius * major_radius` and `points[i % 3]`
  re-key commutative orders from the function START (symbol-count effect, 117 / 136 hunks).
- (Update, later) A `#pragma inline_depth(0)` ... `()` pair round an arm's CLOSING BRACE controls only the scope-end destructor: the
  THIS_INSTANCE new_deletion dtor becomes a call (3507v3642, 82 hunks) -- but of ??1CDeletion, where retail calls the member list
  dtor 0x0042ABCA with ecx = &new_deletion.AssociatedParents (retail stack math: Method [B+68h], list [B+6Ch], `lea ecx,[esp+6Ch]` at
  0x005326A9 with esp = B). inline_depth(1) / (2) at the same place expand everything (with and without STLP_LIST_DTOR_IN_LIST).
  So retail expands the (implicit) ~CDeletion and calls ~list: needs a level the implicit dtor does not provide; candidates: a
  user-declared inline ~CDeletion (FableWin/Ego_r both name ??1CDeletion@NUISystem@@QAE@XZ; no opt-in has it yet) + inline_depth(1).
- Dead statements before the deletion block (to grow the caller's tuple count) changed nothing.
## 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn -- parked, draft graded only (DIFFER(3554v3579), 196 hunks)
- The earlier draft models CCharString / CStringParser / CPersistContext / CReactionMatch_* / COpinionSet / std::list LOCALLY (padded
  local classes), which the strict-header lander rejects: it must be re-authored on engine/CStringParser.h, CPersistContext.h,
  CSymbolMap.h, CPersistTraits.h, fable_string.h (FABLE_RETAIL_CCHARSTRING + FORCEINLINE text ==, as landed sibling 0x006C9490), the four
  existing CReactionMatch_* headers, plus NEW headers for CReactionMatch_MoralityRenownBlob, CReactionMatch and COpinionSet (missing in
  rebuild/include/engine). Not started this session (budget went to the four closer rows). Residue of the old draft per its line 2:
  retail keeps the name's PStringData in esi across the operator== chain and threads the null path (five repe cmpsb).
- (Update) Probe: a user-declared inline ~CDeletion() {} (overlay probe opt-in, removed again) + closing-brace inline_depth(0/1) behaves exactly like the implicit dtor (0: ??1CDeletion called; 1: all expanded).
## Names
- Row names proven (two sources each): FillInTriStrip (Ego_r 0x00BD7790 / FableWin 0x0331EFE0), ClearRenderTarget / DrawBufferedPolys
  (header identity lines; Ego_r + FableWin name sets), CComponent::Update (vtable slot 1 + Ego_r / FableWin, per draft identity line).
- No made-up names introduced. vland RELOC_NAME warnings for 0x009D9C80 are catalog labels: ?SetAWindow@CRenderManagerCore (both PDBs,
  LABELS.tsv), ?D3DTextureUVModeTable@CRenderStateManager (header static member), vector<CQuickDrawTriInfo>::erase 0x009E15E0 (STLport).
## Header overlay changes this session (hdr/README.md)
- engine/CTriangleStripifier2.h: FABLE_RETAIL_CTRIANGLESTRIPIFIER2 (decls only, re-applied chunk-730 block; no asserts in file).
- engine/NUISystem_CComponent.h: FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS (chunk-862 block on today's header; 4 asserts pinned).
  The 5 wfcommon style sources that include this header (style_ui 0058807d / 00601310 / 006096e0 / 0061e6d0, style_wfA 00598a1c)
  preprocess to IDENTICAL token text with the old and new overlay (checked with g++ -E -P), so no regrade is needed.
## 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn -- RELOCATION_MATCH, test PASS, addmatch done, vland WINS (successor; supersedes the park note above)
- Source on shared headers only: CStringParser / CPersistContext / CPersistTraits (FLOAT_TRANSFERIN_BODY) / fable_string (FORCEINLINE text ==, the
  sibling 0x006C9490 CBasicString Compare / operator== template texts in the TU) + the reaction-match class opt-ins (hdr/README.md) + COpinionSet
  in CTCCreatureOpinionOfHero.h; FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE (push_back 0x0067BCC0 called). No flags. Grades ~45 (successor).
- What closed it, in order: (1) COpinionSet() visible but noinline (header opt-in) = retail's edi-cached OpinionValues start in the Sphere fill
  loop and the separate loop-exit free copy (c2 knows the set's address is not kept; with Sphere's inline ctor body visible too) -> 3530, calls
  equal; (2) the trailing `if (!(parser.ReadAsSymbol() == ")")) return;` (retail evaluates the compare, 0x006794F1) -> 3579 equal length;
  (3) Pyramid read into six scalars, not float f[6] (retail frame: yMax in the dead parser arg slot) -> 1 real hunk; (4) the last hunk was the
  edi/esi restore order after the inline strcmp at 0x00678A06: k = 4..6 unused locals fix it, and so does ONE named function-scope
  `CPersistTraits<float> floatTraits;` (FITTED, natural; naming the EOpinion / type traits too flips it back). Unused locals not used.
- Tried without effect on that last hunk: loop index declaration / type, float temp scope, centre.OpinionValues[i], continue-form exit,
  bool comma, FABLE_RETAIL_CARRAY / STLP_VECTOR_INDEX_START, COpinionSet ctor text (5 v 5,0.0f). Removing the TU CBasicString texts: 3601.
- Names: all callees bound in reloc_bindings or PDB-proven (row Ego_r 0x006AA810; ctors Ego_r 0x006A94C0 / 0x006A9510 / 0x006A9560 /
  0x006A95D0 / 0x006ACC90). 0x00679910 is landed as CPersistTraits<EInputKey>::TransferIn (an ICF-folded generic enum reader); here it is the
  EOpinion instance of the same generic body: vland gave no RELOC warning.
## 0x00A593F0 CCylinder::GetFirstIntersectionWithTriangle -- successor pass: DIFFER(3032v3027), 51 real hunks (was 3043v3027 / 56)
- Fixed (EVIDENCED): slope as `float slope = it->X / it->Y; slope = slope / major_radius * minor_radius;` (the one-expression form makes
  c2 emit 1/major/Y*X, retail fld X / fdiv Y / fdiv major / fmul minor at 0x00A59AD6: -11 bytes); loops 2 and 4 as
  `for (it = begin; it != end; ) { C2DVector pos = *it++; ...}` (retail `fld [edx] ; add edx,8 ; fld [edx-4]` 0x00A599A0 / 0x00A59C00;
  FableWin instantiates _Vector_iterator<C2DVector>::operator++(int) in this TU); loop 3 through `const C2DVector& pos = *it` (FableWin has
  no iterator operator-> for C2DVector anywhere); non-const iterators and points.front() (FableWin ?front@...QAEAAVC2DVector@@XZ first
  instantiated in this TU, 0x03248D20); functor members PlaneOrigin / AxisX / AxisY (Ego_r struct_layouts); shared
  FABLE_RETAIL_STLP_BACK_INSERTER instead of the local std copy. The visible noinline push_back specialisation was removed (0x004428B0 is
  not this TU; declaration-only FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE gives the same bytes). transform's STLport body is local in
  namespace std (needs a shared opt-in before landing).
- Residue = ONE register web: points._M_start lives in ecx from the edge loop to loop 4 and the found tail in ours (reloads after every
  push_back / free); retail reloads it at each edge-loop use with ecx = divisor 3, and homes it in ebx only from loop 3 (0x00A59AA1).
  Probes: deleting loop 3 OR loop 4 alone gives retail's edge loop byte for byte -> the later loops' uses form the web.
- Tried, no effect: k = 1..7 unused locals (top, edge loop, loop 3 scope), shared iterator variable, points via a const reference
  (edge loop / loop 3), transform + functor operator() visibility, C2DVector NAMED_RESULT_SUB/ADD/MUL (worse), radius / square
  declaration orders, `line_end` reference, (i+3)%3 -> i%3 or i = 3..5 (restructures, 3002).
## 0x00531EC0 CComponent::Update -- successor pass (~25 grades), still parked; new measurements (tools/cdup.py = call diff by target, tools/usw.sh)
- At the DEFAULT inline threshold (/Gr only; T65+ behaves the same) and WITHOUT FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST (wup/d2.cpp = differ minus the
  closing-brace pragma minus DTOR_IN_LIST) the call list differs from retail in exactly two places: (1) loop 2's four ConvertVector sites
  are expanded (retail calls 0x0052E580 at 0x00532321 / 0x00532345 / 0x00532463 / 0x005324A0), (2) the THIS_INSTANCE new_deletion
  destructor is expanded (retail calls ~_List_base 0x0042ABCA at 0x005326AD). PER_PARENT's ??0CDeletion 0x0042CD84 / ~_List_base calls,
  the `deletion` expansion and the expanded ChildrenToDelete.erase (__copy 0x00535000, ??3) all match at default. Size 3902v3642.
- Loop 2's ConvertVector flips between /d2inlT57 (called) and T58 (expanded); loop 3's stays expanded down to T52: a per-site cost that
  grows with the caller, so retail's caller had grown more by the time loop 2's sites were costed (or its helpers are bigger).
- No effect on either residue: C2DVector NAMED_RESULT_ADD / SUB / MUL (FableWin operator+ is the named-result text, 26i v 22i),
  FABLE_RETAIL_CDELETION_INLINE, /d2inlS10..50, closing-brace inline_depth(1) (all expanded) / (0) (calls ??1CDeletion, retail ~_List_base).
- fwsize hints (FableWin is Dinkumware + asserts, so only leaf texts count): ConvertVector FableWin 28i/81b v ours 29i/95b; none of 11
  candidate texts measured 28i/81b (result copy / in-place / ConvertX-ConvertY / C2DVector(...) returns: 20..29i, 50..95b).
- Next: the two residues need the caller to "grow" before loop 2 / the THIS arm are costed: look for a retail statement the source lacks
  in loop 3 or the deletion arms (an extra copy / temporary / accessor call), or accept /d2inlT57 (CV right) and find per-site devices for
  the deletion arms (at T57 the remaining diffs are THIS dtor, PER_PARENT copy expanded with list copy ctor called, PER_PARENT dtor
  clear+free, erase called).
## 0x00A593F0 CCylinder::GetFirstIntersectionWithTriangle -- third pass (wfD5 successor 2): DIFFER(3027v3027), ONE scheduling swap (was 3032v3027 / 51 hunks)
- CLOSED the register web (EVIDENCED, lesson wfB5): the landed body of the same-TU callee NBBBMaths::IsPointInsideTriangleInThePlane
  (0x00A58A60, rebuild/src/compiled/00/a5/NBBBMaths_IsPointInsideTriangleInThePlane_00a58a60.cpp; it precedes the row in the retail TU) is
  defined above the row in differ/00a593f0.cpp (`bool __fastcall`, same statements). With the body visible c2 knows the callee neither
  captures nor writes `points`, so points._M_start is no longer promoted from the edge loop on: 51 hunks -> 2, equal length, 15 calls = 15.
- Residue (2 real hunks, 0x00A59728 / 0x00A5972C): retail `fld [edi] ; fdiv [esp+20h] ; mov eax,[edi] ; mov [esp+6Ch],eax`, ours
  `fld [edi] ; mov eax,[edi] ; fdiv [esp+20h] ; mov [esp+6Ch],eax` (minor_radius' integer copy one slot early, right after
  major_axis.AccurateNormalise() 0x00A59721). The three other fg hunks (0x00A59645 / 0x00A59689 / 0x00A596BE) show equal R / O text.
- Measured in a scratch lab (tools/lab.py, tmp/lab/l*.cpp): the scheduler puts one integer instruction between the first `fld` after a
  call and the following x87 op exactly when the last non-call instruction emitted BEFORE the call was x87 (here `fstp [esp+90h]`, the
  end of major_axis = minor_axis.Cross(normal)): l4 (x87 before the call) = ours, l5 (none) / l9 (integer copy before the call) /
  l12 (a second call in between) / ly (integer stores after the call) = retail's order. Not an alias fact (l13 / l14), not the callee's
  return type (lv / lx), not a /G5 /G6 choice (/G7 rewrites the function).
- Tried on the row, no change to the swap: minor / major declaration and assignment orders (m1..m6, n2..n9; major first adds a product
  order hunk at 0x00A597BE, so minor is declared first), major_radius after the points declaration (p1..p4), no-op float expressions for
  the copy (r1..r8), k = 1..7 unused locals at the top and before the pair (kt*, km*), dead release assert arms round the pair (a1..a5,
  fable_assert.h), major_radius stored back into cos_angle (pe / pf), major_axis construction / call forms (w2, w6, w7; w4 / w5
  restructure). In the lab: empty if / for / while(0) / goto / block scopes / empty inline call after the call (u1..u7, tb, tc, te).
- Next idea: retail's scheduler saw an integer instruction between `fstp [esp+90h]` and `fld [edi]` that is not in the output, or no
  x87 history: look for a source form of `major_axis = minor_axis.Cross(normal)` / the normalise call whose IL keeps an integer
  instruction last before the call (e.g. the `lea ecx,[esp+88h]` this-pointer NOT hoisted: retail has it at 0x00A596D7, ours too).
## 0x00531EC0 CComponent::Update -- third pass (wfD5 successor 2): DIFFER(3642v3642), calls equal, 15 real hunks (was 3523v3642 / 3902v3642)
- KEY MEASUREMENT (c2 inline trace tmp/inl_d2.txt of wup/d2.cpp + wfcommon C44 krange.py): at the default threshold every wrong decision
  is a static-FREQUENCY fact. Loop 2's four ConvertVector sites need a divisor k > 3.20, the THIS_INSTANCE ~_List_base k > 2.01, while the
  THIS copy ctor (k < 4.87) and the PER_PARENT default ctor (k < 4.27) must stay expanded; loop 3 takes any k < 10.36. One uniform divisor
  k in (3.20, 4.27) = TWO HALVINGS of everything from loop 2 on gives retail's call list exactly (3642v3642).
- Cause in c2's model: `for` loop 1 leaks x5 into loops 2 / 3 (body 5 / 25 / 125). Loop 1 as `while` / for-without-increment (k = 5) fails
  (wup2/v1, v2: THIS copy + PER_PARENT ctor become calls, loop 1 expands ConvertVector).
- Device that reproduces it: two release asserts of the frequency-halving form `{ if (!(c)) __assume(0); }` (new overlay opt-in macro
  FABLE_RETAIL_ASSERT_ASSUME in hdr/fable_assert.h) anywhere before loop 2 (wup2/v3, v4, e1..e4, f1..f5). Evidence for TWO asserts: the
  trace range above and fwsize (FW_EXTRA=/EHsc): FableWin's Update is +177i / +675b over the /Od row (about two asserts of 82i / 318b).
  The macro FORM changes record ids: the do/while(0) or `||` expression form flips one x87 pair at 0x00532475 (17 hunks), the braced / bare
  `if` form does not (15 hunks). The existing FABLE_RETAIL_ASSERT dead arm does not change frequencies.
- Remaining clusters (equal length, equal calls): A 0x00532633..0x0053266C THIS arm (Children[i] pointer kept in ecx then homed at
  [esp+34h] in retail; ours homes eax at [esp+30h] at once), B 0x00532747 `push edi` one slot early, C 0x005329E8 loop 3 `ParentPosition +
  Position` X pair order (retail fld [esi+4Ch] / fadd [esi+34h], Y the other way), D 0x00532C46 loop 3 std::find argument order (retail
  computes &ChildrenToDelete[i] in edx before loading end()).
- (0x00531EC0, later in the same pass) Clusters A and B closed (EVIDENCED): the deletion chain as a plain if / else-if with `RemoveChild(i);`
  in every arm and a nested `if (it != end) { erase; if (empty) {...} RemoveChild(i); }` (no gotos). Retail's `mov ecx,esi ; push edi ;
  call [eax+100h]` at 0x00532747 is c2's cross-jump of identical call statements (lesson 006823d0); the goto spelling gave push / mov and
  moved the THIS arm's Children[i] temp to eax. differ/00531ec0.cpp = wup2/g1.cpp with the header macro: 3642v3642, 9 real hunks.
- Open (3 clusters, all operand / load order): loop 1 `ConvertVector(Position) + RelativeParentPosition` X pair at 0x005320CE (retail
  fld [esi+100h] / fadd [eax]; Y the other way round), loop 3 `ParentPosition + Position` X pair at 0x005329E8 (retail fld [esi+4Ch] /
  fadd [esi+34h]), loop 3 std::find(begin, end, ChildrenToDelete[i]) at 0x00532C46 (retail `lea edx,[ecx+edi*8]` before `mov edx,[esi+C0h]`).
  The loop 1 pair and the loop 2 pair at 0x00532475 trade places: two `PParent != this` asserts fix 0x005320CE and break 0x00532475.
- Tried, no gain: k = 1..7 unused locals at the top (gk*), 93 assert condition x placement variants (wup3/s_*: only the two states above;
  `&&` / float-compare conditions lose the halving: 105..142 hunks), FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD (3648), find through a named
  reference (j3: 16 hunks).
- Next: the three flips are c2 operand-sort ties (record ids before each expansion): sweep statements that add records between the loops
  (the real asserts' conditions), or the per-site operator+ operand spelling in loop 1 / loop 3 only; then test (none written yet), vland.
- (0x00531EC0, last step of this pass) A THIRD assume-assert before loop 3 (wup4/t_* sweep, 108 variants: 48 give 7 hunks, 60 give 9)
  puts BOTH the loop 1 pair (0x005320CE) and the loop 2 pair (0x00532475) right. differ/00531ec0.cpp = wup4/t_00_01_0.cpp
  (`this != 0`, `PParent != this` before loop 1; `this != 0` before loop 3): 3642v3642, 7 real hunks, all in loop 3 (x87 X pair
  0x005329E8, std::find load order 0x00532C46). FableWin's byte gap fits three small asserts (3 x 223b = 669b v 675b).
- Next: both leftovers are load / operand order in loop 3: sweep what sits between loop 2 and the loop 3 sites (the third assert's
  condition and position, a fourth assert inside loop 3's body AFTER the ConvertVector sites keeps erase k < 10.36 only if it follows
  the erase site), then write the test (model CComponent with counted children) and run vland.
## 0x00A593F0 CCylinder::GetFirstIntersectionWithTriangle -- MATCHED (wfD5 successor 3): RELOCATION_MATCH, test PASS, addmatch done, vland WINS
- Closer (FITTED, ~60 new grades): the C3DVector::Cross TEXT. The all-parenthesised body `C3DVector((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),(X*o.Y-Y*o.X))`
  (the landed FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION branch of C3DVector.h) moves retail's `fdiv [esp+20h]` ahead of the minor_radius
  integer copy at 0x00A59728. Found by grading every existing Cross opt-in: COLLISION -> RELOCATION_MATCH, CROSS_MEMBER_X_GROUP /
  CYLINDER_CROSS_GROUPED -> unchanged 6 hunks. Since COLLISION also changes Dot / IsNonZero, a NEW overlay opt-in
  FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED carries the Cross text alone (hdr/README.md); the row defines only it.
- No-change tries this pass (record them so nobody repeats): radii as a C2DVector (156 hunks), double-tree / compound / 1/x*y spellings
  of major_radius (d1..d9), 24 FABLE_RETAIL_ASSERT_ASSUME placements x 3 conditions round the pair, points declaration before / between
  the radii (p1..p6).
- src/00a593f0.cpp = w5/cg.cpp; test/00a593f0.cpp models the whole algorithm (edge crossings, inside-vertex fallback, centre-in-triangle
  fallback, height window, degenerate / parallel rejects) with geometric checks. Name: Ego_r/FableWin (row leaf + anonymous-namespace
  functor CTransformWorldCoordsToPlaneCoords, FableWin 0x03246880 ctor).
## 0x00531EC0 CComponent::Update -- fourth pass (wfD5 successor 3): DIFFER(3642v3642), ONE real hunk (was 7 real hunks / 2 clusters)
- CLOSED (EVIDENCED) the std::find cluster at 0x00532C46: the landed text of the same-TU callee std::__find<CCountedPointer<CComponent>*,
  CCountedPointer<CComponent> > (0x00534EB0, rebuild/src/compiled/00/53/std_find_00534eb0.cpp) above the row, `__declspec(noinline)` inside
  `#pragma auto_inline(off/on)`, with FableWin's free `__fastcall` operator== (??$?8) -- the same device the landed same-TU sibling
  RemoveChild 0x00533BC0 uses. The forwarder now spells `iterator_category(__first)` (by-value pointer overload + a
  `const _List_iterator_base&` overload) exactly as RemoveChild's (TU consistency; byte-neutral). differ/00531ec0.cpp = w6/b3.cpp.
- Left: loop 3 `ParentPosition + Position` X pair at 0x005329E8 (retail fld [esi+4Ch] / fadd [esi+34h]; Y pair fld [esi+38h] / fadd
  [esi+50h] -- retail mixes the operand order between X and Y at this one site; loop 2's site has Position first in both).
- Measured this pass (tools/pairs.py prints every Position/ParentPosition fld/fadd order of a compile; w6/):
  * every spelling of the site moves X and Y together: `Position + ParentPosition` (s1: X right, Y wrong), unnamed temporary arguments
    (s5/s6), `+=` forms (s2/s3), field-wise `+=` in both orders (h1..h4), explicit C2DVector(x, y) in both orders (h5/h6: also flip loop 2);
  * dead zero-initialised locals (k = 1..24, long or float, ANY position: top / before loop 2 / before loop 3 / inside loop 3 give the
    same result) re-key globally: k = 3 / 8 / 11 fix the loop 3 pair but flip the three loop-1 pairs (0x00532050, 0x005320C2,
    0x0053214F); no k fixes both; uninitialised unused locals do nothing;
  * 72-variant sweep of the assume-assert set (conditions this != 0 / PParent != this, assert 2 at the top or before loop 2, assert 3
    either condition or none) x k in {0,3,8}: floor 7 hunks (= this state); assert 3's condition (9 integer / float / `||` forms) does
    not touch the pair; `&&` or a 4th assume-assert breaks the inline frequencies (3261v3642);
  * 30 FABLE_RETAIL_ASSERT dead-arm placements (top / before loop 2 / before or inside loop 3, 1..3 arms, two conditions) and the same
    combined with k = 3 / 8 (48): floor 7 / 8;
  * deletion calibration (one real statement removed in loops 1/2/3): the loop 3 X pair never flips; later loop 3 pairs flip with
    statements before them, not after.
- Not tried / next level: c2's own candidate ids at that site (hand892 tools/c2prio_op.py + c2dbg.py breakpoint harness) to find which
  record makes X and Y differ in retail; a header-level C2DVector operator+ text change would move loops 1/2 (they match now).
