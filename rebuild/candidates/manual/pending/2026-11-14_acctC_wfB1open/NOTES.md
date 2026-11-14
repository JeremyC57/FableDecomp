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
