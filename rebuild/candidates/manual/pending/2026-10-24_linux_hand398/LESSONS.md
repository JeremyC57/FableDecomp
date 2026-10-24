# Chunk 398 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Separate inline calls into separate statements to share an empty tag's slot: `it = remove(b, e, v); erase(it);`
  homes both empty tag temporaries in the dead argument slot, while `erase(remove(..))` nests the scopes and gives the
  second tag a new frame word (`sub esp,0Ch`) (0x007FFDB0).
- A bool predicate that ends in `mov al,1 ; ...` / `xor al,al` is `if (a) { ..; if (b && c) return true; } return
  false;`. An early `return false` duplicates the epilogue, and `return a && b` gives `mov eax,1` (0x00809270).
- To get retail's `fld ; fcomp st(1) ; fstp st(0) ; test ah,41h ; jne` from an inlined float comparator (`a < b` via
  two GetCompareValueForPoint calls), name both compare values (`float va = f(a); float vb = f(b); return va < vb;`)
  (0x00808AA0, 0x00807250).
- An implicit (memberwise) comparator copy with a user-declared member layout is inline when the function copies it
  once (0x0080B050, 0x0080B430). When the function copies it three or more times, VC7.1 emits it out of line and
  calls the shared copy ctor 0x007F9730 (0x0080B5A0). A comparator copy that both `if` arms need is hoisted above the
  branch (0x00809E40, 0x0080B5A0).
- `and esp,-8` in a function with no doubles comes from an 8-byte class temporary (CIntelligentPointer, vptr +
  pointer) that lives inside a loop. Write the plain STLport body; no alignment trick is needed (0x00809700).
- STLport `_Vector_base(n, a)` with `_STLP_alloc_proxy` `_M_end_of_storage(a, 0)`: the three zero stores share
  `mov eax,0` rather than `xor`, because the flags of `n` are still live for the `n != 0` allocate test (0x00808470).
- A tag-forwarding front end (`find_if` → `__find_if(.., random_access_iterator_tag())`) homes the tag on a live
  argument slot only when the callee body is in the TU and not inlined (`auto_inline(off)` stand-in above the
  caller) (0x00806640).
- LTCG callee knowledge: a push_back loop whose `false` byte is stored once above the loop needs STLport's explicit
  memberwise pair copy ctor plus a same-TU `_M_insert_overflow` stand-in compiled `auto_inline(off)` (0x007FF8A0).
- STLport `_Rb_tree::clear()` inline re-reads the header for each of its four stores, and a function's scalar zero
  stores placed before it share one zero register (0x008018B0).
- Family gap, unresolved: with an inlined two-call float comparator, retail places the second call's argument load
  (`mov ebx,[esp+14h]` / `lea eax,[esp+24h]`) between the first call and the `fstp` of its result; VC7.1 RTM, QFE
  4035 and /G7 all place it after the fstp (0x00807170, 0x00807250). Park these after 2 grades.
- Pipelined argument copies (each load issued one store ahead, two loads hoisted above `sub esp,N`) appear for free
  in the loop bodies (0x00808120, 0x00808CF0) but not in the non-loop else arm of `__linear_insert` (0x00808AA0).

Review at landing:
- 007ff8a0 used the full 4 grades and depends on a same-TU `_M_insert_overflow` stand-in (`auto_inline(off)`). It is
  not graded; make sure the lander keeps it out of the landed symbol set.
- 008025d0 and 00802d10 (parked) model CTCTargetingPlayer / CTargetingDef locally while
  `rebuild/include/engine/CTCTargetingPlayer.h` exists. Retype with the engine header at landing.
- 007ffdb0 has a real retail quirk: `erase(remove(..))` uses the single-position erase, so removing a thing that is
  not a user still pops the last slot (STLport `erase(end())` is UB). The source is faithful; the test pins this
  behaviour.
- Already-landed label: caller 0x0058148D names 0x00800020 `RecalculateMultiplier`. The PDB name is
  CTCHaste::UpdateActionSpeedMultiplier.
- Propagated labels: 0x006A7380 `_Cons_val` is CThingFilter_IsInFOV::operator()(const CTargetPoint&), and
  0x004DA468 / 0x004DA43B `BuildTreeArray` are `_Rb_tree::_M_erase`. Check them with label_trust.py before reuse.
- The parked rows used 3 to 4 grades each, and none went over budget. No catalog row holds more than one function.
