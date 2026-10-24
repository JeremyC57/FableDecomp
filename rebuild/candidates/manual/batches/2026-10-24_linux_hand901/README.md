# agent901 near-miss retry NOTES

- 004c5100: MATCH by porting the landed sibling 004c5190 recipe (same-TU noinline __copy_backward body)
- 0086fac0: MATCH, same-TU noinline __copy_backward body (sibling 004c5190 recipe).
- 00431606: MATCH (3rd grade): inlined CComparePointer written with `if (b == a) goto isFalse; if (!a) r = true; else if (!b) { isFalse: r = false; } else r = cmp;`
  -- the merged false block lands after the !b test with the eq test jumping forward (retail). Early returns get the placement but
  jump-thread the bool into the next block; `if (b != a) {...} else r = false;` puts the false block after the compare.
  Still a stand-in model (set of CBasicString<char>* under CComparePointer); a re-model with the 0x004316CF map<CCharString,long>
  types is a fidelity follow-up.
- 00459af3: MATCH, same-TU noinline explicit-specialisation body of vector<bool>(n, const bool&, const allocator&) 0x00451583
  with the allocator parameter unnamed (and `value` read): the 4th allocator temp lands on the live `false` byte.
- 00464d23 / 004675f6: MATCH, real CPersistTraits<T>::TransferBinaryIn body (noinline, explicit specialisation) -- GOTCHAS 1046.

## 00472bdd / 00770790 -- vector<T>::erase(first,last), empty __false_type tag home (MATCH)
- Both parked rows differed only in where the never-written `__destroy_aux` tag was homed (dead `last` slot vs retail's live
  `first` slot). Fix = the 0x004ADB50 live-slot recipe: a same-TU `__declspec(noinline)` template BODY for
  `__destroy_aux(first, last, const __false_type&)` with the tag unnamed and never touched (a declared-only extern
  template homes it in the dead slot). Works both for the `__false_type()` temporary form (00472bdd) and the named
  `_Trivial_destructor __tag;` local form (00770790).

## Landed-twin ports (twins.py / near.py in this dir)
- twins.py masks rel32 branch displacements and image-range absolute words in the retail bytes and compares every queue row
  against every landed (rebuild/src/compiled) and other-agent w22 src address. Exact twins found: 00475d07 = 0x0047547D,
  00475d50 = 0x004754C6 (map<CCharString,long> lower/upper bound: _Base_ptr-typed, size TU), 0091fb30 = 0x0095CFE0,
  009204f0 = 0x0095DE60 (CThingCompare_Nearest heap), 00493ff8 = 0x00580A7B (_M_insert_overflow), 005154c0 = 0x00748C10
  (unguarded insertion sort aux). All matched on the first grade of the twin's source.
- near.py (Hamming <= 8 on the masked bytes) found one-byte cousins: 007a34a0 ~ 0x0095CFE0 and 007a3fa0 ~ 0x0095DE60 (a
  bool comparator instead of a pointer-returning one: `test al,al` vs `test eax,eax`), 0057ef86 ~ 0x0041919C (float vs
  double), 004ea21e ~ 0x004E9550 (frame size).
- Levers recovered from the twins:
  * map lower/upper_bound: `_Rb_tree_node_base*` (_Base_ptr) typed walk + `#pragma optimize("s", on)` fixes the y/key
    register swap the `_Link_type` speed-form model had.
  * heap family: the comparator's real template body noinline in the TU (+ member-init C3DVector copy ctor) fixes the
    comparator-copy schedule (X/Y before `sub esp,0Ch`) and the partial_sort register rotation.
  * _M_insert_overflow: same-TU auto_inline(off) __uninitialized_copy / __uninitialized_fill_n bodies -> the
    `__false_type()` temporaries pack into the bool __atend slot padding.
  * unguarded insertion sort aux: the comparator's real body noinline in the TU and NO user-declared copy ctor -> the empty
    comparator copy is homed over the spilled `last` word.
  * GFLimitReturn<float>: one nested ternary expression `v < lo ? lo : v > hi ? hi : v` (MATCH, not just relocation).
  * CPersistContext::Transfer<T> (004ea21e, 00458833, 004792cf): the switch arms `return` instead of `break` -> the traits
    `lea` is hoisted above the first `je` and the default arm falls straight into the destructor call.
- Label corrections: 0x0057EF86 is GFLimitReturn<float> (catalog "GFLimitReturn<double>"); 0x009204F0 catalog
  "Sort_IntroSort" and 0x007A3FA0 "PartialSort_Top_CThingAICreature" are __partial_sort; 0x00493FF8 catalog
  "Std_Vector_Insert_Range_CBodyPartMesh" is vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow (as parked);
  0x00475D07/0x00475D50 catalog "RBTree_FindNode_Reverse(Order)" are _M_lower_bound/_M_upper_bound of map<CCharString,long>;
  0x007A2F90 is landed as CThingCompare_Nearest::operator()<CThingAICreature> but these callers name it
  CThingCompare_NearestToPos (bool-returning; the pointer-returning CThingCompare_Nearest is 0x004A8D80 / 0x0091F740).

## Same-TU callee bodies (empty tags and escaping keys) -- 9 more matches
- 00889cb0 / 005e2b30 vector::reserve: noinline auto_inline(off) bodies for __uninitialized_copy AND __destroy_aux (tags
  unnamed). For 00889cb0 the element needed a declared out-of-line destructor: with an empty destroy loop VC7.1 dropped the
  __destroy_aux call altogether (215v230).
- 004ba420 list::push_back: user-declared forceinline _List_iterator copy ctor (the 0x004C0050 fix) -> ebp saved in the
  prologue instead of shrink-wrapped.
- 0080e2f0 remove_if, 006d73d0 (reverse_iterator find_if): the real STLport __find_if body (4-way unrolled + remainder
  switch), noinline, tag unnamed. 006991f0: the landed __find body (0x0069CAF0) the same way.
- 006c9e40: GOTCHAS 1046 confirmed for nested pair traits: noinline TransferBinaryIn bodies for BOTH element traits
  (long 0x0040FE60, COpinionDeed* 0x006C88D0) -- the traits `this` then aliases the spilled vector-reference word.
- 006f9640, 00518dc0, 005cdbbe: a READ-ONLY same-TU noinline LowerBound stand-in (binary search) for the CVectorMap whose
  key is passed by reference -- the key's frame word then dies at the call and is shared (anim slot / literal temporary /
  fidiv divisor spill). This is GOTCHAS 665's "second escaping callee" and it worked alone in all three rows.
- 0084e390 push_back: a same-TU noinline _M_insert_overflow stand-in (tag unnamed) homes __true_type in the dead weight
  slot AND restores the copy-propagated weight read.
- Not matched: 0067f430 (traits body fixes the frame; the traits `this` still picks the dead count word over the dead
  resize temporary; resize stand-in and declaration order changed nothing).

## Family F88 (two-level CVectorMap lookups, CAnimationEntry::GetComponent<T>) -- broken
- 005e0380 RELOCATION_MATCH. Two levers together: (1) LowerBound 0x005DFA50 gets a same-TU noinline read-only body, so
  VC sees the key unchanged (one key word stored before the null test, both compares fold to the immediate);
  (2) ONE final `*pp = it->second` over `if (!map || (it = map->Map.find(type)) == map->Map.end()) { found =
  Default.LowerBound(type); if (found != end && !(type < found->first)) it = found; else it = end; }` -- VC
  tail-duplicates that store into the two default arms (separate register choices) and keeps the original last for
  the entry-map hit. The inline find ternary on the default map forms a phi (`mov eax,ecx`) and merges the tails;
  a value ternary `*pp = c ? last->second : found->second` gives the two default tails but lays the entry-map
  tail inline.
- 00867fe0 (type 8, same body) reached 125v125 with the stand-in + value ternary before the recipe was found;
  3 grades spent, parked in differ/ with the recipe as Next. Byte-identical twin 0x00867F60 (type 0x0B) is not queued.

## 004f932c vector<CAttackHistoryCombo>::operator= -- matched
- In this size TU (`optimize("s")`) VC7.1 calls the element's IMPLICIT copy ctor out of line from the inlined tail
  __uninitialized_copy; retail inlines it. A user `__forceinline` memberwise copy ctor
  (`AttackCombo(x.AttackCombo), Multiplier(x.Multiplier)`) gives retail's inline pair-vector copy + dword float move.

## 00752510 CTCLook::GetHeadCreatureAnimFrame -- improved (159v165 -> 165v165)
- An extern const 1/360 (`g_OneOver360`) stops VC folding `range * k * 0.5` into one constant; the YZ arm only
  matches with its getters left on the literal (extern k there hoists `fld [k]` above the branch). Left: the XY arm
  loads k first whichever side it is written on. /Op is far worse (231v165).

## Tail-layout rows (0085fa00 family, 0093dfc0 family) -- lane gap
- 0085fa00: separate ifs, `if (!a && !b) return false; return true;` and one big or-chain `if` all give the
  lane's order (the tail with fewer predecessors first); retail has the opposite. `return a || b` materialises an int.
- 0093dfc0: an early `return true` (or a return inside the if + a final return) makes the early exit share the main
  epilogue tail as retail does, but then VC pushes ebx/ebp/esi together after the je instead of retail's nested
  shrink-wrap (esi around the lookup, ebx/ebp around the body).

## 008fa9f0 CPhysicsCollisionPointsObject::GetPointsList -- matched
- The parked "Next idea" worked first time: STLport 4.6 `_List_iterator_base` (`_M_incr`, `==`/`!=`) under a
  `_List_iterator<_Tp,_Traits>` whose Nonconst-from-Nonconst ctor is a user copy ctor. VC7.1 then rotates both loops
  (bottom tests, source end cached) as retail; the one-struct iterator kept a top-tested inner loop.

## Statement-order x87 / store rows -- matched
- 0087c6b0 (CTurningAnimCompare::operator()), 008ab710 (remove_copy_if, MATCH), 00871d50 (UpdateCageRadius),
  006afa40 (ImpulseRespondToCollisonWithCylinder): sums written as statements (`d = a*b; d += c*e;`) in the order
  retail evaluates them; VC7.1 canonicalises one expression but keeps statement order.
- 006b8550 (RadialBlurFadeTo): `params.Fade = fade;` before `params.Length = length;` fixes the register choice.
- 0075ff00 (IsVectorBetween2OthersInThePlane): a user memberwise C3DVector copy ctor instead of the implicit POD copy.

## Double indexing / STLport begin() forms -- matched
- 007b8530 SetSeatState (MATCH) and 007b8aa0 FreeSeat (its inline copy): index the seat twice, no bool& local.
- 006445ba ClearEffects: operator[] as `*(begin() + n)` (size as `end() - begin()`).
- 006b1600 ApplyReverbSettings: index the entry twice in the copy call instead of a const reference local.
- 007661c0 GetDamageMultiplierFromAugmentationSet and 00768010 OnInitialActivate: both levers together (neither alone).
- 006a1270 IsPrimarySlotFree: begin()/end() forms on the CVectorMap.
- 00766dd0 FrameUpdate: double indexing makes it 1067v1067 (differ/); register rotation left.

## Address forming -- matched
- 0091a630 HasSeller (MATCH): a const reference local to the counted pointer read three times -> `add eax,12Ch ;
  mov eax,[eax]` (the address CSE'd before the loads fold).
- 0065fbe1 HasBuildingAsHomeOrWork: inline const-reference accessors -> `lea ecx,[eax+168h]`.
- 007fb1c0 KillTentacles: the last node's data read straight off `_M_node->_M_prev` -> `mov ecx,[eax+4] ; add ecx,8`.

## Interface lookups -- matched
- 0074fbb0 for_each<CAddObjectsNotSolidTo>: `last` read into a local and shared by a __forceinline
  lower_bound(first, last, key) and the end test.
- F88 tail (`found = LowerBound(); if (found != last && !(k < found->type)) it = found; else it = last;` + one
  `->component`): 00854030, 0079d2d0 (private FinalCleanup), 0079eaa0 (protected FinalCleanup), 0056fbed, 0072ab70.
  Tried on 20 more rows with the f88.py rewriter: no change on the large inventory/kraken/AI rows (their residues are
  elsewhere) and on the tail-order lane gap 0085fb30.

## Tree / comparator rows
- 0098fe70 _M_find (MATCH): STLport `_S_key = _Select1st()(_S_value(x))` + statement-order sort-key locals.
- 006e5990 (differ/, 96v98): `iterator it; it = find(k);` keeps the node in a register as retail.
- 00978b90 / 0097af70 (differ/): user memberwise comparator copy ctor fixes the argument-area registers; the
  software-pipelined copy is left.
