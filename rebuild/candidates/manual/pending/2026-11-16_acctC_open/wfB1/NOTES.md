# wfB1 NOTES (acctC lane; rows 0x00652FA4, 0x0058578B, 0x005F59C7)

## Row 1 0x00652FA4 NPlayerGui::CDrawQuickAccessIcons::Draw -- DIFFER(4653v4653), parked again (differ/00652fa4.cpp = prev/00652fa4.cpp, regraded on this overlay: same grade)
Residue unchanged: flash sprite `fst [ebp-4]` one push early (ours 0x006540C2, retail 0x006540C3). 31 grades here (wb/*.cpp, ATTEMPTS.log), none moved it.
NEW MEASUREMENT (the useful part; tools/fl.py prints the region as `fld1 P X P Y Pm F F`):
- The interleave of pushes and x87 stores is NOT a CPU scheduler: /G5, /G6, /G7 give the identical order (wb/pgG5..7).
- It is a fixed-slot template counted in INSTRUCTIONS after the `fadd` of Y: fld st(1) at slot 6, fstp X at 8, fst Y at 11, fstp st(0) at 13 and 15,
  integer instructions (lea / push / mov / xor) filling the other slots. Holds for every compile with >= 10 integer instructions between the Y fadd and
  the ScaleAlpha call: retail d-pad 0x0065414B, retail 0x00652E21 (another function, Pm between the two pops), ours d-pad, and probe variants e1
  (operands swapped: `mov ecx,esi` moves below the fadd), m1 (`ScaleAlpha((unsigned char)(flashGraphic >> 1))`), f3 (`(unsigned char)FlashButton`).
- With only 9 integer instructions (the flash: lea P lea P `push 0` P P P Pm) OUR c2 never fills top-down: all-register pushes (`ScaleAlpha(0xff)`, wb/f1)
  give Y at slot 10 and both pops one slot early (`fld1 P X P Y P F P F`); the memory push gives `fld1 P X P Y P Pm F F` (Y at 10, first pop at 13).
  RETAIL's flash is the plain on-time template that ours produces only with 10 integers: Y at 11, Pm at 12, pop at 13, pop at 14.
  => at the time c2 placed the x87 stores, retail's flash region held ONE MORE integer instruction than ours (it would sit in slot 14 between the
  two pops, i.e. it is the LAST integer instruction before the ScaleAlpha call) and that instruction is absent from the final bytes (deleted / folded later).
  It is not a separate reload of the alpha: a separate load is placed EARLY (m1: `mov eax,[ebp+58h]` takes slot 5), which would put Y right but the push wrong.
- Not it (all keep `fld1 P X P Y P Pm F F`): alpha through `(unsigned char)(long)`, unary +, `& 0xff`, `* 1`, a second uchar local, long / unsigned long local,
  const-reference-bound temporary, pointer deref, one-element array, struct member; `CDrawBase::ScaleAlpha(..)`, `static_cast<const CDrawBase*>(this)->`;
  `false` for the 0 argument; position as `def.DPadMiddleOffset + GFAnchor..` (other bytes), named flashPos / anchored (value or reference; other bytes).
- Untested leads for the missing instruction: something between the alpha push and the call that a late peephole removes (a second `mov ecx,esi`
  for an inline member wrapper around ScaleAlpha or around the sprite constructor; a zero-register `xor` that is later rematerialised after the call;
  a conversion of the pushed byte). The listed ideas "switch as another construct", "Close / IsOpen order", "named constants" were NOT graded: they
  cannot add an instruction between 0x0065409E and 0x006540CD.
Twin 0x00651B04 (`long HUDRingAlpha` respelling): see "Sibling regrades" below.

## Row 2 0x0058578B CTCInventory::OpenInventory -- DIFFER(8131v8081), parked (differ/0058578b.cpp = prev/0058578b.cpp, regraded here: 8131, 379/379 calls)
Regrades on this overlay: prev scopes 8131 (388 real hunks), prev_noscopes 8140. Lens (tools/wf.sh, top_level root forced to 200; DIAGNOSTIC): 8120, 373 hunks,
structural hunks from 0x00585D7A (so C35's "equal to 0x0058680C under the lens" does not reproduce on this overlay = C44's hdr; first structural hunks
0x00585D7A/5E4A/5FE7 are one-instruction order hunks). Natural priorities today (tools/pt.sh, 80 s): style_count 109 -> edi, top_level 88 -> split (ebx piece).
Tried here (6 grades + 3 lenses + 1 trace):
- o1 (probe/0058578b_o1_twin_hair_idioms.cpp): the count loop's hair arms in the idioms of the matched twin 0x0058807D (`if (hair.IsNull() && tattoo.IsNull())
  {items} else if (!hair.IsNull()) {def; mod declared BEFORE `if (!def.IsNull()) def->GetSubDef(&mod)`; ...} else {tattoo}`): 8135 natural, lens 8124.
  WORSE by one null store: a flat `mod` is destroyed before hair_def in the same scope, so its `Object = 0` store stays; retail has none for the hair
  appearance pointer (0x00586A10..0x00586A1A) and KEEPS the one of hair_def (0x00586A31). So this console function is NOT spelt like the PC twin there:
  the twin's retail has the null stores (0x00588B84 / 0x00588BA0), this row's retail has none for the four GetSubDef targets.
- o2 (probe/0058578b_o2_arm_order.cpp): only the arm order obj / hair / tattoo: 8132 natural, lens 8121, same structural hunks as prev. Neutral, not adopted.
- Retail facts re-read from the bytes (EVIDENCED, for the next agent): `this` lives in memory [ebp-14h] in the count loop (0x0058685A, 0x005869BD);
  def.Object in ebx (0x0058687A); the IsWeapon argument is loaded BEFORE the two null stores of suit / appearance (0x005868BE..0x005868C5); the IsWeapon arm
  exits to 0x00586A93 (skips the suit and appearance releases), the suit arm to 0x00586A80, the appearance / expression arms to 0x00586A6F; the null store of
  the hair appearance pointer sits between the compare and the `je` (0x005869D3..0x005869D8); style_count = [ebp+1Ch] + [ebp+40h] + [ebp+10h] with the
  second operand loaded into ecx (0x00587073), stored to [ebp+0] and tested through eax; count slots: weapons 2Ch, items 18h, clothing 44h, 0x10-flag 10h,
  0x80/0x100-flag 1Ch, tattoo 40h (init order 0x00586839..0x0058684E).
- Still open (unchanged from C35): top_level needs +21 c2 priority over style_count (or style_count -21); count-loop ebx decision; natural spelling of the
  nested def > augmentation > suit > appearance scopes (bare braces = FITTED).

## Row 3 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList -- DIFFER(2649v2652), parked (differ/005f59c7.cpp = prev/005f59c7.cpp, regraded: 2649, 140/140 calls)
Tried here (5 grades + 3 traces):
- `// Flags: /Gr` on the row: byte-identical to no flag (2649, same 174 real hunks). /Gr is not the lever for this row.
- Tail-block K (tools/pt.sh + tools/tb.sh): block 87 K 8 = this, parent, const -1, textBank, PeekComponent temp #78, stealth _M_start #31, subList, const 0;
  textBank 56 -> edi, const 0 48 -> ebx (retail: const 0 edi, textBank memory [ebp+5Ch]).
- The empty `_Destroy(first,last)` loop of ~vector<pair<long,long> > is NOT the hidden block boundary: scratch-overlay probes (removed again) with the
  loop dispatched to the trivial overload (PROBE_NO_DESTROY_LOOP) and with STLport's `__pointer;` statement in _Destroy(T*) (PROBE_DESTROY_PTR) give the same
  100 blocks, K 8, textBank 56 as the baseline: c2 has removed that loop before it forms the blocks it colours.
- Retail structure re-read: the early `return 0` owns its release copy at 0x005F5B1D and jumps to the epilogue 0x005F6419 (after `mov eax,esi`), so no return
  label lies before the vector frees; the three counted-pointer destructors before 0x005F63C0 are plain calls to 0x004291DE.
- Not tried: a second null test folded into the first (allocator deallocate `if (p != 0)` text placed BEFORE the _Vector_base test), text bank read with no
  inline `this` layer through a real accessor.

## Sibling regrades on this overlay (hdr/ = C44's, unchanged by me)
0x0061E6D0, 0x00601310, 0x006096E0, 0x0058807D (wfcommon/style_ui/src) and 0x00651B04 (pending/2026-11-13_acctC_c2a/src): all RELOCATION_MATCH.
Twin 0x00651B04 respelt with `extern long HUDRingAlpha` + `ScaleAlpha((unsigned char)HUDRingAlpha)` (prev/00651b04_long_HUDRingAlpha.cpp): RELOCATION_MATCH,
c2a test PASS under vt.py. Staged in sibling/src/00651b04.cpp + sibling/test/00651b04.cpp (NOT in payload.json: row 1 did not match, the row is not of this batch;
coordinator decides whether the `long` spelling replaces the c2a source so that 0x01375744 has one type in both rows).

## Names
No new names. Row names proven by predecessors (Ego_r + FableWin manglings in each source's Leaf / Identity lines). HUDRingAlpha (0x01375744) stays descriptive.
Header overlay: no change (no new opt-in, no edit). Scratch overlay hdrx/ was created for probes and removed.

## SUCCESSOR (second agent of wfB1) -- Row 1 0x00652FA4: exact mechanism measured with c2's own list scheduler (tools/sc.sh, 16 s per trace)
State unchanged: DIFFER(4653v4653); differ/00652fa4.cpp = same source. ~75 grades + 5 scheduler traces this round (w2/*.cpp, ATTEMPTS.log).
TRUE residue of the differ source is TWO things, not one (the predecessors' frame-masked diff hid the first):
 (R1) 0x00653E01 / 0x00653E11: commutative add of two frame operands, retail `mov ecx,[ebp+5Ch] ... mov eax,[ebp+10h] ; add eax,ecx`, ours loads [ebp+10h]
      into ecx and [ebp+5Ch] into eax (the `(i + 1) % 10` strength-reduced sum of the second Draw2DText).
 (R2) 0x006540C3: flash `fst [ebp-4]` v the third `push ebx`.
(R1) is closed by ONE no-code PAREN tuple on the X sum of C2DVector::operator+ (scratch overlay probe, w2/qC.cpp:
 `return C2DVector((X+vector.X),Y+vector.Y);`): hunks 3e01 / 3e11 vanish, nothing else moves (real hunks left = R2 only). Parenthesising the Y sum instead
 (or both) wrecks the flash AND d-pad schedules (fld st(1) three slots early). The asymmetric text is FITTED / not natural: it shows that R1 is a
 candidate-id / tuple-count fact of the operator+ expansion (MATCHING_PLAYBOOK "commutative operand order follows candidate ids"), i.e. the real
 operator+ text of this TU still differs from the overlay's `return C2DVector(X+vector.X,Y+vector.Y)` by something that costs one tuple / temp on X.
 Not adopted (no opt-in written; hdr/ untouched). NAMED_RESULT_ADD stays wrong here (4701 bytes, 108 hunks).
(R2) mechanism, read from c2 (hand892 sched tools ported: tools/sc.sh <abs src> <tag> ; tools/showdag.py ; tools/cy.py <json> 69 16 36 ; tools/pm.py):
 - The scheduling region is the whole block (69 tuples, cap 81): node 16 = call GFAnchorToBottomLeft, 17..20 fld/fadd/fld/fadd, 21 fld st(1), 22 fstp X,
   23 fst Y, 24 / 25 the two pops, 26..33 lea/push/lea/push/push 0/push ebx x3, 34 push [ebp+30h], 35 mov ecx,esi, 36 call ScaleAlpha.
   TUPLE ORDER has the pops right after the copy (before the pushes); the scheduler interleaves.
 - c2 issues per cycle at most one integer-unit and one x87-unit instruction, in PRIORITY order inside the cycle. Priority (verified on all 69 nodes,
   tools/pm.py: 0 deviations except fadd#20's implicit x87 latency) = 0x100 * (4 * successor_count + H) + 0x8000 if the node loads memory + 1 if it is
   an x87 store, with H(n) = max over successors (H(s) + 4 * (1 + edge latency)).
 - Flash, cycle 25: `fst [ebp-4]` (3 successors: call, pop, pop; H = H(call)+16 -> raw 0xE0, +1 store flag) and the third `push ebx`
   (2 successors: call, push [ebp+30h]; H = H(call)+12+8 -> raw 0xE0) are BOTH ready with EQUAL base priority; the store flag puts fst first.
   Retail has the push first in that cycle. D-pad, same cycle: the corresponding push has raw 0xE8 > 0xE0 (one more register push behind it), so push
   first = retail. The tie exists only because the last push is a MEMORY push (edge latency 2 to the call, a register push has 1): 4 units short.
 - So retail's DAG had, for that push, ONE more successor (+4) or a path 4 longer (push [ebp+30h] -> call latency 3, or an extra node between the
   alpha push and the call that depends on the stack pointer); OR fst had one successor fewer. An extra INDEPENDENT instruction does not do it (it would
   land between the pops). Probe i6 (`ProbeL2(0xff, flashGraphic)`: a real push after the memory push) reproduces retail's order exactly
   (`X P P Y Pm F F` + the extra push), i4 (two memory pushes) too.
 - What does NOT change the DAG / tie (all graded or traced): /Zi /Z7 /Zd; /G3 /G4 /G5 /GB; alpha as static / member / global (xor + mov al + push eax,
   other bytes); `this` through a local `const CDrawBase*`, an inline member wrapper, a free inline wrapper (j1..j4: only flips the fadd operand order);
   the whole flash sprite through a FABLE_FORCEINLINE member helper with an `unsigned char` parameter (inl2: same DAG, operand order flips, 11 hunks;
   plain inline is not expanded: 4503 bytes); parentheses in the alpha initialiser (5 forms); parentheses around the constructor / copy-constructor
   float reads; statements after the sprite (g2 / g3: 87+ register hunks); ScaleAlpha's body visible in the TU (w2/sa1.cpp, landed text through
   CPlayerGui::ScaleAlpha: the scheduler DAG keeps the store -> call edges, the flash is unchanged, and a CRGBColour store group at 0x0065348E moves
   across the call = retail's ScaleAlpha is NOT visible to this function, keep it declared only).
 - The earlier "slot template" / "one more integer instruction" description (first agent) is superseded by the priority tie above.
 Open lead for a successor: find the source fact that gives the third 0xff push a third successor or the alpha push a longer path to the call
 (the instruction bytes are right, so it is a tuple that emits nothing, or an edge c2 adds for an operand class we have not produced), then R1's
 operator+ text. Everything else of the row is retail.
Rows 2 and 3 were not regraded this round (all effort went to row 1's mechanism): their state is the first agent's (sections above).
Successor files: probe/C2DVector_paren_probe.diff (scratch-overlay PROBE_PX / PROBE_PY / PROBE_KX.. macros; apply to a COPY of hdr/ as hdrx/ and grade with
HDRO=$W/hdrx, tools/pp.sh <name> "PROBE_PX(e) (e)"), probe/00652fa4_qC_operator_plus_X_paren.cpp (R1 closed, needs hdrx), probe/00652fa4_i6_*.cpp
(a real extra push after the alpha push: retail's flash order), probe/00652fa4_sa1_*.cpp (ScaleAlpha body visible: worse). tools/x.sh <name> builds
w2/<name>.cpp from differ/ by the replacement list w2/<name>.py, grades it and prints the flash / d-pad order (tools/fl.py); tools/rg2.py prints the
flash region. hdrx/ was removed again; hdr/ is untouched (no opt-in added, none edited), so no matched row needs a regrade.

## THIRD AGENT (wfB1) -- Row 1 0x00652FA4 MATCHED: RELOCATION_MATCH, test PASS, vland read-only PASS, addmatch done (src/00652fa4.cpp, test/00652fa4.cpp, flags /GS)
Two closers, each measured (about 95 grades in w3/, 4 comparator logs):
 (R2, flash `fst [ebp-4]` v third `push ebx`, 0x006540C3) EVIDENCED: retail's CEnginePrimitive2DSprite position constructor 0x00493AC2 is a header COMDAT
   (it sits in a run of template instantiations 0x00493911..0x00493BF4, 30 callers in many TUs) and so is its C2DBoxF corners constructor 0x004371A0. With BOTH
   bodies visible in the TU (the sprite ctor defined `inline` in the row with the landed row's text; the box ctor through the existing shared opt-in
   FABLE_RETAIL_C2DBOXF + FABLE_RETAIL_C2DBOXF_CORNERS) c2 sees that the `const C2DVector& position` temporary is not captured, the store -> ScaleAlpha-call
   edge of the scheduler DAG loses its memory latency and the 0xE0 v 0xE0 priority tie of the second agent disappears: retail order. The sprite ctor alone
   (box ctor only declared) changes nothing (w3/ct1.cpp); both = R2 gone, nothing else moves (w3/ct2.cpp). Calls stay 140/140 (both ctors called, not expanded).
 (R1, `mov ecx,[ebp+5Ch] ... mov eax,[ebp+10h] ; add eax,ecx`, 0x00653E01 / 0x00653E11) FITTED: `C2DVector text;` of the guild-seal tooltip declared one scope
   out (in the `if (topGraphic > 0)` block, just before the tooltip `if`). Mechanism, read from c2's operand-sort comparator (tools/lk/lka.py = wfD2's lk with
   the float filter removed, tools/lk/show3.py / named.py / ind.py; 1 min 50 s per run):
   - the add is `i + T`, T = the hoisted loop temporary `1 - &effectUsed` of `(i + 1) % 10`; c2 sorts the two by key = record id * 32: T's key id is 412 in every
     variant, `i` is record 426 in the old source (i first = ours) and must be below 412 (T first = retail);
   - c2 hands out record ids in chunks of 32 per table; the function's symbol records are created in IL order, and its SECOND chunk started exactly at
     `control` (416 = 13 * 32) because another table took chunk 12 during the tooltip condition (`CUserProfileManager::Get().IsHUDTooltipEnabled() && ...`).
     One more record BEFORE that condition makes the function's second chunk 12 (384..): `i` = 394/395 -> R1 right. Bisected with a dead ruler: right after
     SetPersistentPrimitive(PrimitiveHandles[1] ..) fixes R1, as the first statement inside the tooltip block does not;
   - every x87 X sum of C2DVector::operator+ (0x0065345F, 0x006535B9, 0x00653961, 0x00653F08, 0x0065409E, 0x0065414B) sorts two float indirections by
     (pointer record id * 0x2000 + 8) mod 0x10000, i.e. by id mod 8: any net change of the record count that is not 0 mod 8 flips some of them (one ruler:
     35b9 / 3f08 / 409e; that is why every k = 1..7 ruler sweep failed and k = 8 at function scope before the tooltip matched: differ/00652fa4_k8_RELOCATION_MATCH.cpp);
   - so the fix must MOVE one existing record from after the tooltip condition to before it, net count unchanged: `C2DVector text;` (no constructor code, an
     out-argument of GFACoordToVCoord) declared before the `if` does exactly that (records: text 384, control 386, i 394; T 412) and nothing else moves.
     Hoisting `CActionInputControl control;` instead also fixes R1 but moves its constructor stores (0x006533F4) = wrong bytes.
   Not adopted alternatives: 8 dead rulers (matches, device c with k = 8), the second agent's parenthesised operator+ X sum in the shared header.
 vland read-only: RELOCATION_MATCH PASS, no RELOC_IDENTITY; RELOC_NAME warnings only for catalogue labels (g_NormaliseEpsilon v ?DELTA_NOTIONAL_ZERO@@3MB at
 0x0129BA3C, GetAbilityAvailability v ?IsAbilityAvailable@CTCSpecialAbilities at 0x0072C430, CIntelligentPointer assign 0x0065533C / dtor 0x00A01C10 labels,
 0x00437820 labelled as a folded STL helper v ?GetGuildSealAlpha@CPlayerGui@@QBEMXZ): spellings inherited from the predecessors' source, bindings unchanged.
 Test: host model of the row's decisions with recording stand-ins (the style of the twin 0x00651B04's test), 00652fa4_TEST PASS under vt.py and g++.
 Header overlay: unchanged (no new opt-in). The row now also defines FABLE_RETAIL_C2DBOXF / FABLE_RETAIL_C2DBOXF_CORNERS (existing opt-ins).
 Twin note for the coordinator: the matched siblings of this TU (0x00651B04, 0x00645292 DrawIcon) matched without the visible sprite ctor; rows of this TU with
 a "scheduler tie" around a sprite construction should try the two visible bodies first.

## THIRD AGENT (wfB1) -- Row 2 0x0058578B CTCInventory::OpenInventory MATCHED: RELOCATION_MATCH, test PASS, vland read-only PASS, addmatch done (src/0058578b.cpp, no flags)
6 grades. The "top_level v style_count priority" cascade of three earlier agents was ONE fact: the row's four `def->GetSubDef(&x)` targets were address-exposed
because the member template was only declared. Retail calls the GetSubDef<T> COMDAT instances out of line (e.g. 0x0057FF92), i.e. their body is in the TU:
 - `#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY` (existing shared opt-in, engine/CDefinitionManager.h, the FableWin text) + the StatGetTypeName opt-ins of
   the instantiated sub-defs (FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME, FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME; CAugmentationDef was already on):
   8131 bytes / 388 real hunks -> 8077 bytes / 4 real hunks (w3/r2g1.cpp), calls 379/379 unchanged. (lesson 0x007BFE30 / 0x00735FD0 of wfB5 applied.)
 - the card arms in the order items / hair / tattoo: `if (hair_card.IsNull() && tattoo_card.IsNull()) {items} else if (!hair_card.IsNull()) {hair} else {tattoo}`
   (EVIDENCED: retail lays `add [ebp+40h],eax` after the hair arm, 0x00586A36) -> 8081 bytes;
 - `CDefPointer<const CAppearanceModifierDef> hair_appearance;` declared before `if (!hair_def.IsNull()) hair_def->GetSubDef(&hair_appearance);` (EVIDENCED: its
   null store sits between the compare and the `je`, 0x005869D5) -> RELOCATION_MATCH. Both are the PC twin 0x0058807D's idioms (the first agent's o1 probe measured
   them "worse" only because the GetSubDef body was not visible then).
 FITTED (kept from agentC35): bare-brace scopes for augmentation / suit / appearance in the count loop.
 Name: CTCInventory::OpenInventory (catalogue vfunc_30): Ego_r 0x005F4D20 + FableWin ?OpenInventory@CTCInventory@@UAEXXZ, vtable 0x01250DE4 slot 30 (agentC23's proof).
 vland RELOC_NAME warnings are catalogue labels only (0x00449980 `Catch@0042ae0a` v ?GetMainPlayerJoystickDeviceNumber@CPlayerManager@@QBEJXZ, 0x0066E799
 `SetMember_dword0x28` v ?SelectScreenOfType@CManager@NHeroInformationScreens@@..., 0x013B8394 g_pSoundManager v ?PSoundManager@@3PAVCASoundManager@@A), all
 spellings inherited from the earlier agents' source; _wcscmp binds to the MSVCR71 import thunk (RELOC_ALIAS warning, not an identity error).
 Test: host model of the count loop and the entry tests that use its results (0058578b_TEST PASS under vt.py and g++).

 Row 1 addendum: the visible sprite constructor is its real text -- its COMDAT copy compiled by this row's own TU (same source, `/GS`) grades RELOCATION_MATCH
 against retail 0x00493AC2 (q.py 00493ac2 src/00652fa4.cpp with the ctor's leaf), with and without `optimize("y", off)`.

## THIRD AGENT (wfB1) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; 13 grades, all byte-identical to the baseline: 2649, 174 real hunks)
Tried, each with NO byte change: dead release asserts (FABLE_RETAIL_ASSERT through fable_assert.h) at five tail positions (w3/r3a1..a5); GetTextBank through the
typed CCountedPointer operator* (FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS, w3/r3s1); DIAGNOSTIC: ConstructPCExpressionCategoryList body removed
(w3/r3nb: the vectors' escape does not feed the residue); FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR / _VISIBLE_CALLED_DTOR / both (w3/r3c1..c3);
FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE (w3/r3c4). GETSUBDEF_BODY (row 2's closer) is already on in this row.
Arithmetic of the earlier agents' what-if, for the successor: retail needs textBank 35 = 7 x 5 and the const -1 tail piece 40 = 8 x 5, ours 7 x 8 and 8 x 8, i.e.
exactly "K = 5 candidates referenced in the tail block" (ours 8: this, parent, const -1, textBank, PeekComponent temp, stealth _M_start, subList, const 0); retail's
tail has no branch between 0x005F6182 and the first vector test 0x005F63C0, so three of {PeekComponent temp, stealth _M_start field, const 0, this, parent} are not
candidates of that block in retail. Retail fact: textBank's memory home [ebp+5Ch] is the spill home of `inventory` (stored 0x005F5A47, reloaded in the slot loop).
Not tried: callee visibility of the callees that receive `this` / `parent` / the vectors in OTHER TUs' COMDATs (CTCInventory::GetAllItems 0x005BF482,
NUISystem::CComponent::FindChild), a block split by a real construct at the tail (the row closed in rows 1 / 2 by visibility, so check every declaration-only
by-reference callee of this row first: list them with objdump -dr relocation names v the retail call list).
## FOURTH AGENT (wfB1, w4/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked again (differ/005f59c7.cpp unchanged; ~35 grades + 5 c2prio traces + 6 what-ifs)
Residue re-measured: ours is retail with edi<->ebx mirrored in every region except the tail (retail const 0 edi / -1 tail piece ebx / textBank memory
[ebp+5Ch] = inventory's spill home; ours textBank edi, const 0 ebx, -1 immediate); every frame-slot / vector-order difference follows from that.
Measured this round (all byte-identical to the baseline or worse, w4/*.cpp, logs w4/*.q):
- Last-resort devices: k = 1..7 unused named locals at the top (w4/k1..k7): byte-identical (a 56 v 48 priority gap is not a tie). Inline / assert
  devices do not apply (no inline-decision residue; dead asserts already measured by the third agent).
- Console twin's `{ }` block around the headers + tail (landed 0x005F4C8F idiom), three placements (w4/v1..v3): textBank stays edi; a scope end
  does NOT create a c2 block (c2prio v1: same 100 blocks, block 87 K 8). Frame slots get worse (realdiff 269 -> 392..739).
- Natural respellings: textBank as pointer, named `component` reference, sorting-loop item reference, signed index (2651 bytes, tb still edi),
  cached QuickAccessItems[n], title.Get(), const miscThingsDef (w4/e1, e2, s1..s7): no colouring change.
- STLport text: ~_Vector_base without its own null test (real STLport 4.6 shape; scratch overlay, removed): byte-identical.
- DIAGNOSTICS (not hand-back): parent address-exposed (d1) -> K 7, no flip; raw-cast text bank (d2, as agentC11) -> K 7, no flip; both (d3) -> K 6:
  textBank 42 < const 0 48 -> const 0 edi, -1 piece #83 prio 52 ebx, textBank memory = retail's TAIL. So the tail needs K <= 6 in block 87.
- BUT a second, independent difference appears whenever textBank leaves edi (d3, escape probe x1, what-if textBank prio 42 / 20 / 1, all 2667 bytes
  with the tail right): the first const -1 piece then spans entry + pre-loop + tail (x1: piece #86 prio 88, one piece) and takes ebx at the TOP
  (`or ebx,-1` at 0x005F59E2, `push ebx` for the PC_SUB_LIST / container / dummy CCharStrings), evicting the parent vtable temp to edi and the
  inventory piece (retail ebx 0x005F5A3E..0x005F5C45) to esi. Retail's -1 piece is tail-only (compensation `or ebx,-1` on both loop exits). Per
  the hand885 note pieces grow by forward reach from entry up to a full block, so in retail some block between the top and the tail must be full
  (esi/edi/ebx taken) when -1 is split -- i.e. retail has a candidate coloured BEFORE the first split pass that holds ebx in the slot loop or the
  pre-loop region, which ours does not have. The earlier "textBank escape reproduces retail's colouring" note (chunk 1008) does not hold on
  today's text.
- Console twin c2prio (tmp/pt_tw.trace): it has the same PeekComponent inline-this local (#29 cost 4) in its tail block; its textBank loses only
  because its -1 root (160) and const 0 (115) are whole and high. So the inline-this local is real in this TU; the PC row's difference is in the
  -1 / const 0 roots (live-through cost of the slot loop), not in the tail spelling.
Next (untried): find the pre-split ebx holder: trace the split pass piece regions (c2.dll 0x1078B51C / piece ctors 0x1078B494) on x1 to list
the blocks of piece #86 and which block retail must have full; then look for a natural construct in the slot loop / pre-loop that makes one
callee-saved candidate there outrank 48 (e.g. a named loop-invariant such as `CTCInventory& inv` or `const unsigned long count = ...size()`).
Hypothesis for the successor (not measured): retail's only ebx holder in the pre-loop region is `inventory` (0x005F5A3E..0x005F5C45, it covers the
-1 references 0x005F5B9B / 5BC1 / 5BFC). If inventory's piece is coloured before -1 is split (i.e. -1's root is not in the FIRST split pass, as it
is in ours: c0 first pass ids include #1 and #18), the -1 piece's forward reach is blocked there and only a tail piece forms -- retail's shape. So
check what puts the -1 root into the first split pass in ours (its root priority -67 from slot-loop live-through) before touching the tail.
Grading aids left in w4/: m.sh (grade + tail markers: textBank register, push -1 count, `or reg,-1`), rd.py (register-aware diff), sh.py
(register / frame-normalised structural diff), slots.py (frame-slot map retail v ours).
## FIFTH AGENT (wfB1, w5/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked again (differ/005f59c7.cpp unchanged; ~30 grades + 1 c2prio trace)
Main new fact: the retail colouring is now reproduced EXACTLY by a diagnostic, so the target is fully characterised.
- w5/d4.cpp (DIAGNOSTIC, not hand-back) = baseline + parent address-exposed (DiagSink(&parent)) + raw-cast text bank (no GetTextBank inline-this #78)
  + `if (DiagFlag) DiagCall();` right after the stealth category call (a real c2 block boundary before the counted-pointer dtors / vector frees, which
  takes #31 = stealth _M_start local out of the tail block). Register-aware diff (w4/rd.py) v retail: ONLY the top (0x005F59D4..0x005F5A03: the diag
  call and parent reloaded from [ebp+7Ch] because exposed) and the inserted diag branch differ; every frame slot, both `or ebx,-1` compensations,
  the 6 `push -1`, textBank in [ebp+5Ch], const 0 edi, inventory ebx all equal retail. Tail block = {this, -1, textBank, subList, const 0}: K 5, i.e.
  textBank 7x5 = 35 and the first -1 piece 12x5-20 = 40 < inventory piece 41 -- exactly agentC10's what-if numbers (const -1 = 40, textBank = 35).
- w5/d12.cpp: the same with the exposure in a DEAD arm `if (0) { DiagSink(&parent); }` (realdiff 40): c1 marks a local address-taken even in a dead
  arm (useful mechanism fact). But in both, the top reloads parent after the CCharString ctor; retail holds parent in edi across it, so retail's
  `parent` IS a register candidate at the top.
- K 5 with parent kept as a candidate does NOT reproduce retail: w5/d9.cpp (diag boundaries after `CWideString categoryTitle;` and after the last
  category: tail block {this, parent, -1, textBank, subList}) gives the second residue (4x `or ebx,-1`, -1 piece at the top), as do K 6/7 variants
  d6 (raw + end boundary), d7 (exposed + end boundary), d8 (end boundary only). So retail needs parent OUT of both the top block's K and the tail's K,
  yet its value held in a register at the top: i.e. the variable pushed to the four category calls is not the same c2 candidate as the value
  FindChild is called on at the top. d13 (`CComponent* owner = parent;` for the top call, parent exposed) does not do it (the copy is forwarded).
- Zero-code constructs do NOT make c2 blocks (all byte-identical to the baseline, w5/p1..p7): bare label, goto+label, do{}while(false),
  for(;;){break;}, switch(0){default:break;}, while(false){}, if(true){}, each after the last category call.
Next for a successor: find the natural source in which (a) the tail's `parent` argument and the top's FindChild object are different candidates,
(b) #78 / #31 are not tail-block candidates (or an equivalent K 5). The block boundary before the vector frees and the missing inline-this have no
natural zero-code spelling found so far; retail has no branch between 0x005F6182 and 0x005F63C0.
## SIXTH AGENT (wfB1, w6/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked again (differ/005f59c7.cpp unchanged; ~27 grades + 1 c2prio trace + 3 fwsize runs)
Baseline re-confirmed on today's overlay (w6/b0.cpp = differ/005f59c7.cpp: 2649, realdiff 269, textBank edi, 19 push -1). c2prio trace w6b0 (tmp/pt_w6b0.trace)
identifies #31 exactly: the five line-321 "local" candidates #15/#25/#16/#32/#31 (spill -1, 2 refs each) are the vectors' _M_start reads in the scope-end
~_Vector_base (one per vector; #31 = stealthItems, the only one whose `cmp/push` falls in tail block 87). All measured byte-identical to the baseline unless noted:
- Callee placement: FableWin.pdb order is GetMiscThingsDef 0x01EC8FE3 -> ConstructPCExpressionsList 0x01EC9345 -> ConstructPCExpressionCategoryList 0x01ECAE7C
  (source order: the category builder is defined BELOW the row; retail emits callees first). w6/m1 (builder moved below the row), w6/m2 (builder removed,
  declaration only): identical. Removing GetMiscThingsDef's body (h1: 2663) or the GetDef template body (h2: 2660) is worse, so those stay visible.
- STL allocator text: FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND (a1), FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE (a2), both (a3): identical (#31 is not an inline-layer artefact).
- TU flags / compiler: --flags /O1 (the flag of the same-TU rows 0x005F64DD / 0x005F6793 minus /Oy-), /G5, /G7, the QFE-4035 cl (FABLE_VC71=$S/wine/vc71): identical.
  `#pragma optimize("y", off)` (neighbours' pragma) gives a plain ebp frame, 2788: retail is /Oy with the biased frame.
- Natural respellings (w6/gen.py): manager declared before inventory (t1), inventory declared after miscThingsDef (t2), const miscThingsDef (t3), `CComponent* owner =
  parent;` for the four category calls (o1), named `const CMainGameComponent& component` (c1), subList declared then assigned (s1), one five-vector declaration (v1):
  identical. Inner `{ }` around the tail (b1: realdiff 392) or around title + tail (b2: 418): worse.
- Early return as `if (GetTC) { ...; return subList; } return 0;` (i1, 2658, realdiff 138): textBank leaves edi, but the block layout changes (jle -> jg, the
  return-0 block moves) and the -1 piece takes ebx at the top (the fourth agent's second residue); result-variable form (i2) 2623. Not retail's structure.
- fwsize (FW_EXTRA="/EHsc /Gy"; without /Gy the non-COMDAT row merges with GetMiscThingsDef's .text): row 1164i/5035b v FableWin 1255i/5574b (+91i); the landed
  console twin 0x005F4C8F is +67i on the same tool, so FableWin's assert/EH overhead explains most of the gap; no statement-level signal.
Conclusion unchanged: retail needs tail block 87 K <= 5 (textBank 7K < 48 and the -1 tail piece 12K-20 < inventory 41), i.e. parent, #78 (GetTextBank inline
`this`) and #31 (stealth _M_start) all absent, which no natural spelling or allowed device found so far produces (k locals and dead asserts were measured by the
third / fourth agents; dead-arm exposure of `parent` alone (d12) is not enough). Parked.
## SEVENTH AGENT (wfB1, w7/) -- Row 3 0x005F59C7: target re-characterised (earlier diagnostics were confounded)
- KEY FACT: the fifth agent's diagnostic boundary `if (DiagFlag) DiagCall();` references const 0 (cmp [DiagFlag],<const-0 reg>), which put const 0 back
  into the block it was meant to cut. With a const-0-free boundary `if (DiagFlag == 7) DiagCall();` after the 4th category call:
  * w7/dD.cpp = baseline + that boundary + raw-cast text bank (no GetTextBank inline-this #78), parent UNCHANGED (still a candidate):
    realdiff 32 = ONLY the 3 diag instructions differ; top, frame slots, `or ebx,-1` x2, 6 push -1, textBank [ebp+5Ch] all = retail.
    Tail block 87 K5 = {this, parent, -1, textBank, subList}: textBank 35 < const 0 43, -1 piece 40 < inventory 41.
  * w7/dA.cpp = boundary + parent exposed (dead-arm DiagSink(&parent)), #78 kept: realdiff 40 (top reloads parent + diag).
  * boundary alone (dB): K6, textBank 42 TIES const 0 42 and wins on key (const 0 key 0 always loses ties) -> baseline-like.
  * So parent need NOT leave; the fifth agent's "parent out" was an artefact of the const-0 diag. Retail = (end-of-tail block cut) + (no #78).
  * Every zero-code construct of the fifth agent was measured on a base where even a real boundary has no visible effect (dB = baseline):
    re-measured on the exposed base (w7/z1..z9: label, goto, do/while(false), for(;;){break;}, switch(0), while(false), if(true){},
    if(subList){}, if(inventory){}): still no block. Single diag boundaries at 6 positions in the tail (w7/p0..p5) and early+end pairs
    (q1, q2) do not reproduce retail: only end-cut + no-#78 does.
- #78 respellings that keep the inline-this local (each with the boundary, w7/r1..r3): SMART_POINTER_ACCESSORS typed operator*, named
  `const CMainGameComponent& component`, textBank as pointer. PTextBank is protected (no direct member access from the row).
- Local exposure of parent without escape (`CComponent** p = &parent`, `CComponent*& r = parent` in dead arms) is optimised away (f1..f3).
- Second exact reproduction (model B, natural GetTextBank kept): two const-free cuts, one right after the textBank declaration (or after
  `CWideString categoryTitle;`) and one after the 4th category call (w7/q1b, q2b: realdiff 41 / 39 = only the 2 diag tests). Use DIFFERENT
  constants in multiple diag tests (two `== 7` tests make `const 7` a 2-ref candidate that pollutes K: w7/q1 looked like a failure).
  So retail = end cut + (no #78 | a cut between textBank's def and its first use). Both models need a code-less cut after the last
  category call; nothing natural found yet.
- Measured non-closers for the cut (w7/*): identical-arm if/else (m3..m5 merged early, no block; m1 `if (frame)` only extends frame's
  live range), `__assume(cond)` after the last call (a1..a4: makes blocks but also disables the fused `dec [ecx+4]; jne` release peephole
  and gives the -1-at-top residue), end-of-function spellings (static_cast return, result local, return inside/outside inner blocks e1..e7).
- Narrowed: model B's first cut works ONLY between the textBank definition and the first CCharString ctor (positions after
  `textBank` / after `CWideString categoryTitle;`: q1b, q2b); cuts after InitialiseOffsets or after category calls 1..3 (q3b..q6b) do not.
  So the two cuts sit at categoryTitle's construction and (after the 4th call) its destruction -- they bracket categoryTitle's lifetime.
  But scopes never make c2 blocks (w1..w5: do/while(false), one-pass for, braces: all fold early, only frame slots move), the landed
  console twin (same TU, same categoryTitle spelling) has no cut, and retail has no EH frame.
- More non-closers (all measured): null tests the optimiser drops (`if (subList|parent|this) { ... }` around the tail, i*1/i*2: 2649 bytes,
  no block -- VC7.1 folds null tests of dereferenced pointers early), `?:`/if returns (t1..t5), dead loops after the last call (l1..l4:
  removed with no block), 60 extra calls in block 87 (no block-size cap: still 100 blocks), `__assume(x != 0)` at any position (as_*: one
  function-wide effect, position-independent, -1-at-top residue), exposing textBank (pointer + dead-arm DiagSink(&textBank), x1: -1 at top).
- Where a successor could still look: a construct that gives c2 a block boundary with no final code (c2 removes it after colouring);
  the only such thing seen in this exe so far is the post-allocation cross-jump hoist (`push -1` before `jne` at 0x005F5E2F), which
  needs two non-identical-in-IL arms. Devices a..d of the acceptance test do not create blocks (k locals / dead asserts are removed early;
  no inline-decision residue). Parked.
- Also measured: the four categories as a loop over local constant tables (w7/u1: VC7.1 keeps a real loop, 2387 bytes -- no unrolling,
  so the tail was straight-line in the source). Header overlay unchanged this round (no regrade of matched rows needed).
## EIGHTH AGENT (wfB1, w8/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; ~45 grades + 2 c2prio traces)
Re-confirmed on today's overlay: w8/b0 (= differ) 2649 realdiff 269; w8/dD (seventh's model A diag) 2666 realdiff 32 (only the 3 diag instrs).
Every probe below swaps the diag cut of dD (raw-cast base, needs ONLY the end cut) or keeps it and swaps the textBank spelling (model B
base); success signature = textBank not in edi + `or ebx,-1` x2 + 6 push -1 at 2652 bytes. None succeeded unless noted (all w8/*.cpp + .q):
- Inline boundaries do not cut: the 4 categories + categoryTitle (h1) or the 4th call alone (h2) moved into a same-TU FORCEINLINE helper.
- Empty-arm / dispatch forms on a NON-constant condition (extern int): `switch(x){default:}`, `switch(x){case 1: default:}`, `if(c){}else{}`,
  `if(c);`, `c?(void)0:(void)0`, `if(c&&d){}`, unused `int x=c?1:2`, `if(v.size()>0){}`, `__noop(x)` (s1..s9): folded, no block.
- Conditions only c2 can fold (late folding): `bool b=false; if(b)`, `int z=0; if(z!=0)`, `if(frame!=frame)`, `unsigned four=4; if(four!=4)`
  (c1..c4), `(x&0)!=0`, `x*0`, `x-x`, `(x^x)!=0`, `&local==0`, `this==0`, `&vector==0` (k2..k8): all folded with NO block. `(x|1)==0` and
  `(unsigned)x>0xFFFFFFFF` are NOT folded (real code stays).
- `try {} catch (...) {}` after the 4th call (s10) DOES give the cut (textBank eax, or ebx x2, 6 push -1 = retail's tail colouring) but turns
  on C++ EH for the function (EH frame, 3152 bytes). `__try{}__finally{}` / `__try{}__except(1){}` (s11, s12): SEH frame, no cut.
  The TU is EH-off (no EH frame in 0x005F45B5 / 0x005F3F7B / 0x005F4C8F / 0x005F64DD / 0x005F6793; `--flags /EHsc` on b0: EH frame, 3226).
- `__assume(DiagFlag == 7)` / `__assume(subList != 0)` (as1, as2, c2prio w8as1): NOT a block (100 blocks, same as b0); it raises const 0
  (refs 67 -> 70, prio 43 -> 82) function-wide, hence the -1-at-top residue the seventh saw.
- Tail spellings: `CWideString categoryTitle = CWideString();` (st1, 2663), `for (CWideString categoryTitle;;) { ...; break; }` (en1),
  `return subList ? subList : 0;` (en2), `return frame ? subList : subList;` (en3): no cut.
- TU flags on b0: /GS /Gr /Gd /Ob1 /GX- /Zi identical; /Ow 272, /Oa 2643/279.
- Evidence on WHICH model retail is: the landed console twin (same TU, same textBank / categoryTitle spelling) keeps the GetTextBank
  inline-this local in its tail block (tmp/pt_tw.trace #29), so the inline-this is real in this TU; with it, only model B (two cuts:
  before categoryTitle's ctor and after the 4th call) reproduces retail. The twin has no cut with the identical tail spelling, so the
  PC row's cuts would have to come from context, not from the tail text.
- Other sources checked for a hidden statement: FableWin.pdb order GetMiscThingsDef -> row -> category builder (no helper in between);
  Ego_r donor ConstructPCExpressionsList is 2624 bytes / 859 instructions (retail 2652 / 869), bytes not on this machine; FableWin.exe
  bytes not on this machine either (only signatures).
Conclusion: the colouring needs c2 block boundaries with no final code at the two model-B points. Of everything measured by eight agents
only a C++ try region and the diagnostic real branch make them; neither is acceptable (EH frame / extra code). Allowed devices a..d do not
create blocks. Parked; I see no further natural lead. A coordinator could decide whether the row is "blocked: compiler-internal block
structure with no source spelling" rather than open.
## NINTH AGENT (wfB1, w9/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; 8 grades)
New levels measured (success signature as before: textBank not edi + `or ebx,-1` x2 + 6 push -1):
- `_ReadWriteBarrier()` (VC7.1 c1xx DOES know it: wide string in c1xx.dll; zero code) as the end cut on the model-A raw-cast base (w9/rwA),
  as both model-B cuts on the baseline (rwB) and as the end cut alone (rwE): all byte-identical to the baseline (2649, realdiff 269,
  textBank edi). A compiler memory barrier is not a c2 colouring block.
- Late-folded conditional temporaries (wfE3 mechanism) as the end cut: `bool b=false; if (b && CCharString("X") != "") {}` (ct1/ct2),
  `int z=0; if (z!=0 && ...)` (ct4): the cleanup test is deleted but c1's conditional-temp flags word survives (`xor ebx,ebx ; mov
  [ebp+64h],ebx` entry store, 2653) and textBank stays edi -- no cut. With a run-time condition (ct3) real code stays (2710).
- Slot-loop shape `if (!slot) continue;` (cn1): same length, frame slots move (this home becomes retail's [ebp+50h]) but the colouring
  is no closer (rd.py 799 v 616 lines); not retail's structure.
- Devices of the acceptance test re-checked against this residue: (c) k unused locals and (d) dead asserts were measured byte-identical by
  the fourth / third agents (the residue is a 56 v 48 priority gap from tail-block K, not a record-count tie; `if (0)` arms are removed
  before c2 forms blocks); (b) there is no inline-decision residue (equal calls, every callee expanded/called as retail). None applies.
Conclusion: unchanged -- needs c2 block boundaries with no final code after the 4th category call (+ no GetTextBank inline-this, or a
second cut before categoryTitle's ctor). Recommend the coordinator classify the row as blocked (compiler-internal block structure with
no known source spelling) unless new evidence (FableWin.exe bytes / Ego_r donor bytes of 0x0058465D) turns up.

## TENTH AGENT (wfB1, w10/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; 7 grades + 1 c2prio trace + 1 tuple count)
New measurements (all w10/*.cpp + .q; tools: w10/g.sh one-line grade with md5 of the built listing, tools/c2nl/nl.sh c2 tuple count):
- Regime: c2 IL tuple count of the row is 1098 (tools/c2nl/nl.sh), far below the 10000 regime-1 gate, so the global optimizer runs;
  every block has w 1 because of optimize("s"), not because of regime 1. Loop-weight levers are dead here for that reason.
- Why textBank wins edi (b0 trace re-read, tmp/pt_b0.trace): phase-1 colouring runs in priority order down to const 0, then the split
  pass starts. textBank (56) is coloured in phase 1; its allowed set is {edi, ebx} at equal cost, so table order gives edi; const 0
  (48) is left with ebx only. In the diagnostic w7/dD (re-traced as tmp/pt_dD10.trace) textBank is 35, falls below the split-pass
  point (next candidate after const 0 is 36 in both traces; dB showed 42 is still phase 1), is split, and loses ebx to the -1 piece
  (60) -> memory [ebp+5Ch] as retail. So the target is textBank prio <= ~36 = K(tail) x cost 7 with K <= 5, or cost <= 4 at K 8.
  textBank has no block it is live through without a reference (defined after the last branch: PeekComponent follows
  InitialiseOffsets, call order fixed), so only K (cut) or fewer textBank references can lower it; four GetTextBySymbol calls on one
  text bank are fixed by retail. Nothing else in the tail block can leave K naturally (this, parent, -1, subList are real references).
- Retail has no jump into 0x005F6390..0x005F63C0 (grep of all jcc/jmp targets), so the cut must come from a jump removed after
  colouring; none of the constructs tried by ten agents leaves one.
- Spellings: subList held as CComponent* with static_cast<CList*> at each use (v1): byte-identical to b0 (md5 9932bc80).
- Pragmas on b0: optimize("g", off) 5014 bytes; optimize("sy") and adding optimize("p") byte-identical; optimize("y", off) 2788.
Conclusion: unchanged. Same recommendation as the eighth / ninth agents: blocked (compiler-internal block structure with no known
source spelling) unless FableWin.exe / Ego_r donor bytes turn up.

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).

## ELEVENTH AGENT (wfB1, w11/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; ~42 grades on the row + 6 on the twin, no trace)
Tools: w11/g.sh (= w10/g.sh), w11/gt.sh (same for the twin 0x005F4C8F), w11/g2.sh (scratch overlay w11/hdr; batch hdr/ NOT changed).
- Twin tolerance (does the landed console twin 0x005F4C8F accept the cuts?): t0 (landed text) RELOCATION_MATCH; tE (end cut after its 4th call)
  and tA (cut after textBank) each = only the diag instructions differ; tR (raw-cast text bank) RELOCATION_MATCH; tD (raw cast + end cut) =
  only diag; tB (BOTH model-B cuts) breaks the twin's colouring (realdiff 148, textBank leaves edi). So model A (end cut + no GetTextBank
  inline-this) is consistent with a SHARED (header-level) cause; model B is not (it would have to be PC-specific).
- End-cut position (model-A base, w11/L0/L1/L2): the cut works anywhere from after the 4th category call up to just before the first vector
  free (L1, cut after the three CCountedPointer dtors via an inner block = L0 except the diag position). A cut after categoryTitle's dtor but
  before the CCountedPointer dtors (L2) moves frame slots (braces); L2 was only checked by the textBank signature (no edi use), not diffed against L0. So the needed boundary is "somewhere in the
  scope-end dtor run, before stealthItems' `if (_M_start)`" (block 87 must not contain #31 / const 0).
- GOTCHA (measured): MSVC does NOT read the overlay's hdr/stlport/fable_containers.h for this row. rebuild/include/fable_string.h pulls
  rebuild/include/stlport/fable_string.h, whose `#include "stlport/fable_containers.h"` is resolved by MSVC through the directory of an
  open includer (rebuild/include/) before the INCLUDE path, so the REPO copy wins (g++ -M shows the overlay copy: misleading). Positive
  control: an empty ~_Vector_base in the overlay copy changed nothing (pc) until the row included "stlport/fable_containers.h" first
  (xpc: 2574 bytes). The row grades the same either way (x0 = baseline md5), so the batch overlay's extra containers opt-ins are simply inert here.
- Header texts at that point, all byte-identical to the baseline (md5 9932bc80) on the natural and the raw-cast base, each with a working
  path (preprocessed text differs from b0 for every variant; overlay-only edits re-run with the forced include, xv1/xv3/xr1/xr3): no _Destroy
  loop (__type_traits<pair<long,long> > true in the row; R1), ~_Vector_base without the outer `if (_M_start)` (xv1), FABLE_RETAIL_STLP_MALLOC_
  ALLOC_DEALLOCATE (vB2, opt-in in the repo copy), both (xv3), FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND (h3, repo copy),
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (dB1) and _DTOR_DECL_ONLY (h4) (both opt-ins exist in the repo and overlay copies).
  The raw-cast text bank alone (R0) is also byte-identical to the baseline.
- Row-local, all byte-identical: unused 6th vector after stealthItems / before categoryTitle, unused CDefPointer (uB1..uR3: dtor branches on
  a known-null pointer fold with no block); `CTCInventory* inventory = 0;` (z1, z2); the row under `#pragma auto_inline(off)` (aiB, aiR).
- Anniversary PDB (Xbox 360, 4k copy): the PC function does not exist there; the console twin GetExpressionsListUIComponent is rewritten
  (locals tot_num_of_expressions, ptop_list, ptc_inventory, menuparam; module 95 Unity_tc_inventory_baseEtAl). Dead end for this row.
Conclusion: unchanged. Same recommendation as agents 8-10: classify blocked (needs a c2 block boundary with no final code in the scope-end
dtor run; no source or header text found). Only new evidence (Ego_r donor / FableWin.exe bytes of this function) could reopen it.

## TWELFTH AGENT (wfB1, w12/) -- Row 3 0x005F59C7 still DIFFER(2649v2652), parked (differ/005f59c7.cpp unchanged; 2 grades + 1 c2prio trace)
- Baseline re-graded on today's overlay: w12/b0.cpp = differ, md5 9932bc80 (same as w10/w11).
- NEW MECHANISM (first code-free c2 block cut found on this row): duplicating the tail in both arms of the existing frame test
  (w12/j1.cpp: `if (frame) { frame->AddPositionChild(title.Data); InitialiseOffsets(4); <tail> } else { InitialiseOffsets(4); <tail> }`)
  compiles to 2650 bytes, NOT ~3200: c2 colours the two arms as separate blocks (two textBank candidates #69 / #73, trace
  tmp/pt_w12j1.trace) and merges the identical arm code AFTER colouring by cross-jumping (scheduling inside the merged tail differs
  from b0: `mov ecx,esi ; push 4`), so the join at the end of the tail is a real colouring block boundary with no final code.
- Why it does not close the row: arm 2's tail block K = 6 {this, parent, -1, textBank, GetTextBank inline-this #78, subList} -> textBank 42,
  arm 1's K = 7 (+frame) -> 49; and const 0 FALLS to 36 (constants lose w*K per block they are live through without a reference, and the
  duplicate adds blocks), so both textBank copies stay phase-1 edi (realdiff 682 v b0 472). Reaching retail needs both arms at K <= 5 AND
  identical colouring in both arms (else no merge): impossible with #78 present, and arm 1 also carries `frame`. The only real branch
  near the tail is the frame test, so no other duplication point exists; model B's first cut (after textBank's def) cannot come from it.
- The priority formula as printed by c2prio (for successors): per block, w = 1 << loop depth, K = candidates referenced in the block; a
  referenced candidate gains w*K*cost, a candidate live through a block without a reference loses w*K; cost 2 per variable reference,
  0/1 per constant reference. So textBank (one block) is K x 7 and only a cut or fewer tail-block candidates can lower it.
Conclusion: unchanged -- blocked (needs a code-free colouring cut at the end of the tail AND K <= 5; the only code-free cut mechanism
found (post-colouring cross-jump merge of duplicated arms) cannot give K <= 5 here). Same recommendation as agents 8-11.
