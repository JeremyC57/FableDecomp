# Chunk 397 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Inline getter evaluated before the arguments (007f0300):** write `obj->GetDefinitionManager().GetDef(args)` with an inline getter, not the raw member `obj->DefinitionManager->GetDef(args)`. With the getter, VC7.1 evaluates the object expression BEFORE the arguments, so the this-pointer is loaded first and kept in edi, saved together with ebp. The raw member is loaded after the argument calls.
- **CDefPointer `operator->` keeps the def in a register (007fdf60):** read the def through CDefPointer's inline `operator->` rather than a raw `const T*` member. VC then keeps the def pointer across an inlined STLport `size()` loop in a shrink-wrapped ebp (`push ebp ; mov ebp,eax` inside the if). The raw member reloads it from `this` after the loop.
- **`limit - size()` operand order (007fdf60):** `PSummonDef->MySummonLimit - (long)list.size()` loads the limit AFTER the size loop, as retail does. An inline getter on the left (`GetNumberMySummonLimit() - GetNumberMySummonedCreatures()`) loads it before the loop.
- **STLport `min` by reference (007fdf60):** `min(a, b)` = `b < a ? b : a` homes both operands in frame slots and gives `lea eax,[esp+N] ; jl ; lea eax,[esp+M] ; mov reg,[eax]`.
- **Shared final `return 0` (007fdf60, 007f0300):** put the work inside `if (cond) { ... return x; } return 0;`. This gives retail's shared trailing `xor eax,eax` / `xor al,al` exit. An early `if (!cond) return 0;` emits its own exit inline. A final `if (n < min) return 0; return n;` turns into a branchless `setl ; dec ; and`.
- **Two CDefPointer locals sharing one slot (007f0300):** each GetDef instantiation needs its own same-TU `auto_inline(off)` stand-in (I-3). Declare them as two non-template overloads defined ABOVE the caller.
  - VC then enregisters the first def after its GetDef and reuses its slot for the second def.
  - Once a def has been dereferenced, VC drops the destructors' null tests.
- **Functor returned by value under /Gr (007fda80):** the functor needs a user constructor. With one, the hidden result pointer goes in ecx and `first` in edx. A POD functor puts the hidden pointer on the stack and moves `first`/`last` into ecx/edx.
- **The SGI algorithm family over a by-value comparator (007f9ff0 .. 007fae60):**
  - Write each algorithm as a plain non-template `__fastcall` function in `namespace std`, with the landed comparator `operator()` body inlined (CThingCompare_BestForTargeting2, 0x007F9F00).
  - C3DVector's user copy constructor makes the comparator copy memberwise inline in most members. In `__final_insertion_sort` and `sort` it goes through the out-of-line copy constructor 0x007F9730 on its own.
  - Eight of nine matched on the first grade.
- **CTCKraken retail layout:** STLport lists are one word, so everything from +0x30 sits 8 below Ego_r.pdb. Tentacles +0x44, AttackingTentacleIterator +0x48, Phase +0x60, Mode +0x64.
  - HaveAllTentaclesBeenHit is `if (!Phase) return false; return Tentacles.size() == 0;` (exact MATCH).
- **Retail def fields 4 below Ego_r.pdb:**
  - CEngineThemeDef: MaterialDef +0x8c (Ego_r +0x90).
  - CMaterialDef: IsDiggable +0x4d (Ego_r +0x51).
- **Template `__find` must stay a template (007f7750):** an out-of-line STLport `__find` must be a noinline TEMPLATE with its real unrolled body in the TU. The instantiation is compiled after the caller, so the caller does not keep `last` in edx across the call.
  - SGI remove is `first = find(...); i = first; return first == last ? first : remove_copy(++i, last, first, val);`.
  - Write the erase as two statements, `newEnd = remove(...); erase(newEnd, end());`.
- **A CCharString for a `const&` parameter (007f1ac0, 007fd4b0):** make it a named local in its own block, which gives `lea eax,[esp+N] ; push eax`. A temporary passes the constructor's returned this instead.
- **x87 math declarations (007f9480, 007f8220, 007f1840):**
  - Declare `extern "C" double __cdecl exp(double)` (and sin/cos/atan2/fmod) by hand with `#pragma intrinsic`. `<math.h>`'s float overloads add x87 copies.
  - Write an angle product inside both sin and cos so VC CSEs it.
  - A named float `distSq` local stops VC from distributing `* -0.01f` over the sum.
- **STLport `_M_increment` (007e9250):** a static by-value helper whose result is stored back (`_M_node = _M_increment(_M_node)`) gives retail's `mov eax,edx ... mov edx,eax`.
- **Out-param PeekTC register choice (007f59f0):** the if-reassign pick leaves the component in eax. The single ternary `((it == last || K < it->kind) ? last : it)->m_pTC` puts it in ecx.
- **Inline lower_bound in GetTC (007f7b70):** when GetTC writes lower_bound inline, the search cursor must be the picked entry itself, and the functor's `operator()` then needs `__forceinline`.
- **Tests under VC7.1:** compare float results with a tolerance, even for symmetric expressions. 007f1840's `f(a,c) == f(c,a)` failed under the lane's x87 build and passed under g++.

Review at landing:
- No row needed more than 4 grades. 007fa160 and 007fb1c0 used all 4 and are parked as scheduling or register gaps.
- Local base models sit next to existing engine headers. Each of these rows models the class locally even though a header exists:
  - CTCOracleMinigame.h (007e5f70)
  - CTCActivationReceptorBase.h (007f7750)
  - CWorld.h (007f0300)
  - CSummonDef.h (007fdf60)
  - CThingCreatureBase.h (007f0300, 007fda80)
  - CTCBase.h (every CTC* row)
- Already-landed sources need renames; their bytes are unaffected:
  - `CApplyImpulseIfCloseToCombatLine_operator_007fd8d0.cpp` is CKeepOutOffSmearedOutLine::operator()(CThing*) const (FableWin.pdb / Ego_r.pdb).
  - CTCDPhysicalObstruction::UpdateAdditionalImpulse 0x007FD580's stand-in `ForEach` is std::_For_each<CThingCreatureBase**,CKeepOutOffSmearedOutLine> 0x007FDA80.
  - `CTCKraken_OnDie_007fb230.cpp` is written as `Dest_val(void*)`. Its two helpers are CTCKraken::KillTentacles 0x007FB1C0 and EndHeadEffects 0x007FB150.
  - CTCKraken_EndAttackBeam 0x007FC800's `Kraken_PreEnd` / `Kraken_Finish` are KillBeamEffect 0x007FB870 and KillTentacles 0x007FB1C0 (the latter is a tail jump).
- The sort-family sources (007f9ff0 .. 007fae60) declare their out-of-chunk siblings as plain non-template std functions:
  - `__adjust_heap` 0x7FA320
  - `__insertion_sort` 0x7FA450
  - `__unguarded_insertion_sort` 0x7FA610
  - `__pop_heap` 0x7FA680
  - `__partial_sort` 0x7FAA20
  - `__introsort_loop` 0x7FAC80

  Keep those names in mind if those rows land later with the same model.
- 007f0300 carries two GetDef stand-in bodies in its TU (the I-3 idiom), so the object holds extra functions next to the leaf.
- No catalog row in this chunk holds several functions.
