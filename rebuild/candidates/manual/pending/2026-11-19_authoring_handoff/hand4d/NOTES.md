# hand4d NOTES (2026-11-19)

## 0x00540320 NUISystem::CEditBox::KeyPressed -- MATCHED (RELOCATION_MATCH 755v755 + test PASS, strict vland; batch 1)
- What closed it: porting FableWin.pdb's /Od statement structure (0x0225E8F0): each handled branch clears DeleteAllOnKeyPress itself;
  the IME test wraps the inner key chain (`else if (!CIME::Get().IsIMEActive())`); the delete-cut branch falls through to the single
  UpdateDisplayText (retail tail-duplicates it at 0x005404EF; the old draft's explicit `UpdateDisplayText(); return;` + goto was the
  cause of the three eax/edx vptr swaps); Ego_d locals (ch, it, part1/part2, width/ptext/i); home scroll as FableWin computes it:
  `Adjustment = -(Text.GetLength() - i + 5)` -> lea [x-5] whose same-register peephole gives retail's `add edi,-5` (every `i-len-5`
  in-place spelling gives `sub`). VC6 for idiom on the three loops (FableWin xor/je). Flags /Zc:wchar_t, no overlay.
- Test: fake vptr tables (member-function-pointer unions) for the box (144/147), text component (148) and CManager (14); CWideString
  doubled. 4/5 mutants killed (the 5th, `<=` on the part2 cut, is behaviour-equivalent).

## 0x0063F7C0 ForEachInRoughArea<CThingPhysical, Bind3<IsNot, HasNavigationInformation, OverlappingBoundingBox>, CThingFindNavigationLayerAt> -- MATCHED (batch 2)
- RELOCATION_MATCH 785v785 at the first grade of the port: swap the old FOREACHINROUGHAREA_BODY for the landed egod body
  (FOREACHINROUGHAREA + _CONSTPROC + _EGOD + ROUGHAREA_EGOD, C2DBoxI opt-ins as 0x0066A1E0). Filter / finder locals renamed to Ego_d's.
  True length 785 (catalog row 769 is short). Test: hand4c's map harness + fake thing / physics vtables; 4/5 mutants killed (`>=` on
  the layer max is behaviour-equivalent).
- Row 1 (0x00540320) was already landed by the coordinator when batch 2 was built (vland skipped it): payload holds only new rows.

## 0x00727220 CApplyAdditionalImpulseIfInCombatAndTooClose::operator() -- MATCHED (batch 3)
- RELOCATION_MATCH at the first compiling grade: the PDB class (Ego_d record: Position, Radius, CTCPhysicsBase& Owner, const
  CThingCreatureBase* OwnerCreature; static fastcall GetToleranceBand with the function-local static tolerance_band; static fastcall
  IsToApplyAdditionalImpulse(const CThing&) with GFDebugPDynamicCast; private GetPushRotationExtentDegrees 45) + Ego_d locals in FableWin
  order over the shared headers (landed sibling 0x00726160's include set + CTHINGCREATUREBASE chain). Overlay NCREATUREMODE_PUSHABLEBYSELFAVOIDANCE.
- Names: see NAME_FIXES.tsv (operator() is const and takes CThing*; the bool is NGlobalConsole::AllowCreatureActionRepulsion).
- Test: row body verbatim, fake physics vtables; 5/5 mutants killed.

## 0x00774D20 CTCShop::BuyItem(long) -- OPEN (best DIFFER(1149v1149) ndiff 34, src/00774d20.cpp; older best in differ/)
- IDENTIFY: catalog label _global::CTCShop_SellHighlightedItem is descriptive. Ego_r.pdb ?BuyItem@CTCShop@@QAEXJ@Z (0x00782AB0, tc_shop.obj #41
  after GetFreeSaleItemPlaces #39 / StockDisplay #40 = retail 0x00774B40 / 0x00774C60) + FableWin.pdb 0x0261E960; the only caller is the
  landed CTCInventoryTrade::SellHighlightedItem 0x005A3436 (binds 0x00774D20 as BuyItem). Callee bindings agree (reloc_bindings).
- Port: Ego_d locals/scopes, retail construction order of the CDefPointers (shop_def, pdef, pshop_def, def_class_base, stock_item_def),
  FableWin spellings IS_THING_ALIVE(phero) and GFPower (both no-ops). 1154v1149 straight port.
- HELPER FINDING (for the coordinator): GetFreeSaleItemPlaces written on the shared headers (Ego_d names allow_blocked, places, n,
  BLOCKED_RADIUS, p_thing; erase(begin,end), AddSaleItemPlacesFromThing(&GetParentThing()), GetPFirstThing(CFilter_Bind2<IsNot,
  In3DRange>(...), &p_thing), push_back; STLP_VECTOR_ERASE_RANGE_NOINLINE / PUSHBACK_NOINLINE / COPY_OUTOFLINE) grades RELOCATION_MATCH
  at its own address 0x00774B40 -- it can replace the landed local model. Defining it before BuyItem removed the `places` begin reload
  (1154 -> 1150). The landed SellItem text also grades RELOCATION_MATCH at 0x00774730 from this TU.
- Residue: one register-role swap at 0x00774D27/32 (this ebx, &Stock ebp in retail) that cascades. Probes: same-TU price bodies (e,f) no
  change; IS_THING_ALIVE/GFPower (g,h) no change; find NOINLINE / DECL (i,k) no/worse; operator[] DECL_ONLY (j) flips this/Stock but by
  making def escape (value leaves the def slot) -- rejected as base; SellItem body earlier (l) no change.

### 0x00774D20 coordinator levers (2026-11-19, grades 23-31)
- Lever 1 (scope blocks): Ego_d's BuyItem record has every pointer/CDefPointer at function scope, then blocks {pshop_item}, {places},
  {profit, ptc_inventory_base, bought_price {experience}}, {value {new_morality .. morality}} -- the source already has exactly that
  nesting. The record order is sorted by bprel (VC10 frame order), not declaration order. Probes: hero/stat pointers declared at the
  top (p1) = no change; all three early CDefPointers declared together (p3) = ndiff 37 (worse); ParentThing->PeekBaseDef (p2) = no change.
- Lever 2 (VC6 for idiom): BuyItem has no loop of its own (FableWin has none either); N/A.
- Lever 3 (zero constant): retail GetFreeSaleItemPlaces reads its parameter as a byte (`mov al,[esp+0x44]` at 0x00774B94): bool, as
  both PDB manglings (_N). `0` for `false` = no change; `< 0` / `= 0` int literals on MaxStock = no change; `Quantity++` = no change;
  `Stock.end() == Stock.find(def)` = ndiff 36. Retail also stores the zero register into float MaxStock (0x00774DE2), so c2 already
  CSEs 0 / 0.0f / null by bit pattern; the `push esi` difference is the zero's live range (retail keeps esi until 0x00774E6C because
  pshop_def takes ebp after &Stock dies; ours gives pshop_def edi and re-materialises `push 0`).
- Lever 4 (MinGoldForDonation): the FableWin text differs from retail in this block (FableWin: `value -= Min-1` statement, Max+1-Min,
  MaxMor*log_value/max_log_value; retail divides MaxMor/max_log_value BEFORE GFPower(value..) -> different revision; ours matches retail's
  order). The Min CSE in ebx follows from `this` sitting in ebp (ours) instead of ebx (retail): retail's ebx holds `this` until the
  GoldDonated store and only then is free for Min. Same permutation, not a separate expression.
- Lever 5: GetFreeSaleItemPlaces real body grades RELOCATION_MATCH at 0x00774B40 from this TU; handed back as the batch 9 re-port (FableWin text).

### 0x00774D20 state at hand-off (2026-11-19, 49 grades, OPEN)
- Best: DIFFER(1149v1149), 34 lines differing. Source: src/00774d20.cpp, whose GetFreeSaleItemPlaces copy is the batch 9 text up to
  clear()/area.
- Residue: a register-role permutation from 0x00774D7B.
  - Retail: the CSE'd zero is esi and lives until `push esi` (the `false` of GetFreeSaleItemPlaces, 0x00774E6C). def is loaded into
    edi at 0x00774E07 and dies after AddSaleItem (0x00774E97). pshop_def takes ebp at 0x00774E49. def is reloaded from its arg slot
    at 0x00774F18.
  - Ours: zero in edi, dead after 0x00774E2C, so the call gets `push 0`. def in esi, kept to 0x00774F18. pshop_def in edi. The rest
    cascades: ebx/ebp for the donation block, one extra spill of MaxGoldForDonation at 0x00775052.
- Tried, beyond the earlier probes and coordinator levers 1-5:
  - Direct-init def_class_base; !places.empty() (worse); places.front(); places(…) direct init.
  - Opt-ins STLP_MAP_FORCEINLINE_BEGIN, TREE_FIND_MEMBER_TEMPLATE_NOINLINE/DECL, TREE_MFIND_NOINLINE, TREE_MINSERT_NOINLINE,
    TREE_CREATE_NODE_NOINLINE, TREE_CONSTFIND, TREE_BOUNDS_OUTOFLINE, MAP_ORIGINAL_INSERT_BODY, CDEFPOINTER_RESET,
    CDEFPOINTER_RELEASE_COUNTREF, CDEFINITIONMANAGER_GETDEF_INDEX_INLINE/BODY.
  - None helps; several are worse.
- Best next idea: make the zero's live range reach the `false` argument, so it takes esi and def takes edi.
  - The allocator weight comes from the zero's uses. Find why `false` is not CSE'd with the CDefPointer null stores here.
  - Try the default-argument form GetFreeSaleItemPlaces() if a PDB shows one, or a bool local.
  - Or shorten def's register life, so the use at 0x00774F18 reloads.

## 0x0085B2A0 -- CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<..Combat..> > (MATCHED, batch 4)
- IDENTIFY: the catalog label GetPBestThingInRoughArea<CFilter_Bind3<IsAICreatureAwareOf,..>> was propagated and is wrong. The callees
  (IsNot, CombatIsStrikeToStrike, inline PeekTC<CTCHitLocations> + GetBoundingSphere, CombatCanCollideOrInteractWithThing, a list node)
  and the caller 0x008593FF fit the Bind4 combat instance; names from Ego_r.pdb 0x0083B010 and FableWin.pdb 0x0282DAA0 (NAME_FIXES.tsv).
- Re-port: the shared ROUGHAREA_EGOD body (the 0x00878430 recipe) + the PDB filter classes (Ego_d records 0x5145F / 0x51457 / 0x2601E,
  ctor parameter names from Ego_d) + CThingFilter_CombatOverlappingSphere::operator() in FableWin's statement order (0x0282F400).
- Grades: plain inline 622v775 (sphere test out of line), sphere forced 581v775 (Bind4 operator out of line), both forced RELOCATION_MATCH.
  The out-of-line Bind4 copy 0x0085B160 shows the source had it plain inline: the forced pair is a fitted budget lever (// Inferred: line).
- Overlay: hdr/engine/CFilter_Bind4.h FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE (new; the header has no asserts).
- Test: one map / one layer, 12 things: the stamp, the type test, the four filters with short-circuit call counts, strict sphere test with
  both radii and Z, GetBoundingSphere's `true`; mutants `<=`, `false` flag, single radius all FAIL.

## 0x008DD7E0 -- NCreatureMode::CModeThunderLightningStorm::UpdateDamage (MATCHED, batch 5)
- Port over the shared headers in FableWin's statement order (0x02859F70) with Ego_d's locals (p_thunder_battle_def, hit_creatures, it,
  pcreature, hpParams, ptc_entity_events, ptc_elec, the dead lifetime_in_seconds / lifetime_of_main_beams). The filter is
  Bind2<IsNot, In2DRange> (the draft guessed a sphere); the box is C2DBoxI(ToC2DCoordI(), (long)StormRadius) (RADIUS_INITIALIZERS).
- Steps: first compile 740v744 (explicit CCharString temp built after the other AddTC args); implicit const char* conversion 744v744
  ndiff 19 (the direction vector's X/Y/Z all on the x87 stack before the stores). No shared operator- / ctor spelling moved it
  (INITIALISE_SUB, NAMED_RESULT 760; XY_GROUP, X_GROUP, ORIGINAL_COMPONENT_CTOR, COPY_FORMAL, CTOR_FORCEINLINE, explicit copy = same).
  Named position references + component-wise C3DVector(...) = RELOCATION_MATCH (Inferred line; names descriptive).
- Retail's electrocuted-effect path tests only the presence bit (no LowerBound), so HasTC, not FableWin's GetTC (Inferred line).
- Overlay: hdr/engine/CThunderBattleDef.h (new header). Test: 5 creatures (dead, null, already hit, has effect, dies of the hit);
  mutants (HasTC sign, move-back, alive recheck, direction sign, find sense, radius) all FAIL.
- NAME_FIXES: 0x008E3C50 (two donors), 0x006C4060 (single source, flagged).

## 0x008185B0 -- NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint> (open)
- Ego_d record (`anonymous namespace' spelling) matches the landed sibling template text exactly (0x0087AA10 / 0x0087AFF0 / 0x0078B7C0
  share one text, all RELOCATION_MATCH), so the template text is settled; only the functor varies. FableWin GetValue (0x024EB2C0):
  cos_x = GFCos, sin_x = GFSin, `Ca*cos_x + Cb*sin_x + Cc*cos_x*sin_x` (Ego_d locals sin_x, cos_x).
- Port: sibling text + FableWin functor = 1360v1360 ndiff 15 (two inlined GetValue sites factor differently: c2 reassociates the
  expression per site). Grouping `(Ca*cos_x)` or `(Cc*cos_x)*sin_x` fixes the do-loop site (ndiff 10). The remaining site is the
  refinement's val_b = GetValue(r_end): retail keeps val_a on the x87 stack through fsin and factors by cos; ours stores
  val_a_converging.second first and factors by sin. No-effect probes: 14 parenthesisations, 7 term orders, local order / const /
  implicit conversion / single declaration / named result, pair init forms (ctor, make_pair, grouped declarations), draft order.

### 0x008185B0 state at hand-off (2026-11-19, 78 grades, OPEN)
- Best: DIFFER(1360v1360), 10 lines differing. Source: src/008185b0.cpp. Flags /Gr, as the three landed siblings 0x0087AA10 / 0x0087AFF0 /
  0x0078B7C0.
- Residue (one site): the refinement `val_b = func.GetValue(r_end)`, retail 0x00818827..0x00818855.
  - Retail factors by cos: `fld st(0) ; fmul [Cc] ; fadd [Ca] ; fmul [cos slot]`, then `+ Cb*sin`. It keeps val_a on the x87 stack
    through fsin, stores val_a_converging.second (`fstp [esp+40h]`) after fsin, and pops a duplicate of sin (`fxch ; fstp st(0)`).
  - Ours factors by sin, like the other four sites. It stores val_a_converging.second before fsin, and loads the cos slot twice.
- Per-site classifier: cls.py <g_file> prints S / C for each fcos site. Retail is S S S C S. Every probe gives all S or all C:
  - GetValue texts: 14 parenthesisations, about 10 term and operand orders, locals order, const, implicit conversion, single
    declaration, named result.
  - All C: o6, b6, pp0100.
- Refinement context probes: no flip.
  - c1..c7, d1, d3, d5, e2, e5, e6, e8.
  - These vary: single declaration of val_a/val_b, r_start/r_end split, accuracy placement, pair-init order, pair member order,
    val_b after the val_a pair, roots_pair by value, the pair ctor forms, `float val_b; val_b = ...`, it->first, and GFSin/GFCos
    definition order.
- Other probes:
  - Shared engine/GFSin.h + GFCos.h with extern TWO_PI (FABLE_RETAIL_MATH_TRIG_TWO_PI): all S, 141 lines differing.
  - Flags: /Op, /G6, /G7, /Oy-, /Ob1, /Ob2, /O1, /Os, /QIfist, no /Gr. None flips; most are worse.
- Best next idea: the decision is c2's tree factoring, and it depends on the site's context, not GetValue's text.
  - Try the template's refinement written as FableWin's own GetRootsNumerically<this functor> instance.
  - Or put this functor's TU neighbours in the file: the retail caller of 0x008185B0 and the functor's other members.
  - Failing that, escalate. 78 of 100 grades are used.

## 0x00AEDE60 CParticleEmitter::SetOrientation -- RELOCATION_MATCH (batch 6)
- Retail expands Set(CRightHandedSet, translation) and SetInverseNoTranslate but calls the compiler-generated
  C3DOrientationInterpolator::operator= 0x006B0E00. /d2inlT sweep: no flags 157v796 (both helpers called), T120-220 433v796
  (Set inline, inverse called), T221 433, T222+ 867v796 (operator= expanded too). So threshold cannot give the mix.
- Fix: existing opt-in CMATRIX3X4_SET_RHSET_INLINE (force-inline Set, as CTCShop 0x00772BB0) with PLAIN_INLINE_PASS23 dropped, plus a new
  overlay opt-in CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE. Without the overlay: 362v796. With it: RELOCATION_MATCH, no flags.
  The parked "mirrored fmul operand order" vanished with the FableWin det/rec text.
- Test: 7 mutants tried (start/end swapped, no end MakePerpendicular, wrong translation, inverse of EndTransform, inverse before Set,
  no copy) -- all killed.

## 0x00A58130 NBBBMaths::CLinearSpline::ProcessPoints(long) -- RELOCATION_MATCH (batch 7)
- IDENTIFY: catalog `_global::CPSCSplineBase_GenerateCurvePoints` is wrong; the Ego_r donor order and the shared header bind
  ProcessPoints (NAME_FIXES.tsv). Ego_d locals: smooth_iterations (the header's parameter name `subdivisions` is descriptive; the
  definition uses the PDB name), the rest as the Codex draft. FableWin nested typedef TVectorCntr (vector<C3DVector>) is not in the
  shared header yet; I did not add it, because an in-class opt-in would move the header's FABLE_STATIC_ASSERT line.
- First port on the shared headers (STLP_VECTOR_C + OVERFLOW_OUTOFLINE + INSERT_OUTOFLINE): 1272v1272, 4 lines differing. This is the
  Codex residue: the push_back capacity load/compare sits after point_12.Z's fld.
- Sweep: ADD_XY_GROUP leaves 2 differing lines; with CROSS_MEMBER_X_GROUP added, RELOCATION_MATCH. CROSS alone leaves 4; about 22 other
  C3DVector opt-ins leave 2 or more.
- The GFArcCos spelling (FableWin revision) gives 1272v1272 with 4 lines differing in all 12 add/cross combinations; kept `(float)acos * 1/(2 pi)`,
  documented in an Inferred line.
- Test: independent reference model, 6 cases plus 40 collinear clamp cases. Mutants: 8 FAIL (c3 step, angle sign, length fraction,
  p_first carry, both clamps, point_23 edge), 1 hangs (smooth_counter = 1 never meets 0 iterations; mt.sh times out with no output,
  rerun twice), 1 equivalent survives (`num_pts < 2`: two points give no corner either way).

## 0x00A8F1B0 C3DPrimitive2::ConvertToVBFormatStaticBump -- RELOCATION_MATCH (batch 8)
- IDENTIFY: Ego_r/FableWin private member; the landed caller ConvertToVBFormatStatic 0x00A92740 spells it TransformVertexBufferWithNormals
  (NAME_FIXES.tsv). FableWin 0x032F9EF0 text: VC6 for idiom, Data->Vertices[c_vert] (no PeekVertex), operator^ (not Cross), Dot.
- Port on shared headers: 1469v1469 ndiff 2, the same fnstsw / fstp st(0) swap Codex had. One-define sweep: ORIGINAL_GROUPED_CROSS_PASS22,
  CROSS_XYZ_GROUP, DOT_ORDERED_ZYX, DOT_ORIGINAL_SCOPE_PASS23 each match. Kept the operator^ text with the donor formal; DOT_ORIGINAL_SCOPE combined
  with it stops the 4x unroll (331 bytes). Zero literal 0.0 (double) is worse (ndiff 7); 0 / 0.0f the same.
- Overlay: C3DPrimitive2.h opt-in CONVERTSTATICBUMP (GetVertexCount + the declaration), new CTVertexNormTex1BumpBase.h / CTVertexNormTex1Bump.h.
- Test: 11 vertices (unrolled 4x + remainder 3), sign +, -, exactly 0, cross order, zero vertices; 7 mutants killed.

## 0x00774B40 CTCShop::GetFreeSaleItemPlaces -- RE-PORT, RELOCATION_MATCH (batch 9; coordinator lever 5 for 0x00774D20)
- The shared-header body kept in src/00774d20.cpp grades RELOCATION_MATCH at 0x00774B40. So does FableWin's own text (0x0261ABB0:
  clear(), the dead `C2DBoxI area`, GFFilter_Bind2 into PeekWorld().PeekThingSearchTools().GetPFirstThing), and so does a version
  without area and with erase(begin(), end()). Handed back the FableWin text with Ego_d locals as a re-port of the landed local model
  (same-TU auto_inline(off) stand-ins, In2DRange filter). vland needs --recheck-landed (vlr.sh). Test: 6 mutants killed.
- Effect on BuyItem: none by itself. The BuyItem TU's copy of the body is the same text up to clear()/area.
