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
