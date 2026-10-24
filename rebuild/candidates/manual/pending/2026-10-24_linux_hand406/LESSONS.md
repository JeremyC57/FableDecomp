# Chunk 406 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `iterator it = l.begin(); if (it == l.end())` on an STLport list gives `mov eax,[edx] ; cmp eax,edx` (the next
  pointer loaded into a register first), whereas `l.empty()` gives a memory compare. Use the iterator form when retail
  loads the next pointer first (0x0090AC50).
- An STLport list iterator WITHOUT a user copy ctor is pushed by value as a plain word; with one, it is built in its
  argument slot (0x0090BC60, functor/iterator arguments to for_each).
- `__destroy_aux(first, last, __false_type)` under /Gr takes the empty tag in ecx and the iterators on the stack: the
  tag counts as the first register argument (0x00913930).
- STLport 4.6 deque (`_M_initialize_map`, clear, push_back) and vector (`reserve`, `_M_allocate_and_copy` with
  `allocate = n != 0 ? malloc : 0` and the placement-new null test) match verbatim when modelled with their real
  `_Vector_base` / `_STLP_alloc_proxy` structure. vector bodies are COMDAT-folded for every element of the same size
  (0x009137F0, 0x00920F90, 0x00920FF0).
- A bool out-parameter `if (thing->GetInterface(&tc, N)) tc->F(x);` stores the component to a local and moves it
  (`mov eax,[eax+4] ... mov ecx,eax`); the FindEntry/`->component` form folds it into `mov ecx,[eax+4]` (0x009215E0).
- Under /Gr a `bool (__fastcall*)(...)` comparator is a register candidate. In `__unguarded_linear_insert(last, val,
  comp)` it takes edx when val is a class passed by value (0x009265C0), and it goes on the stack when val is a pointer
  (val takes edx) (0x00932330).
- STLport `__median` (inline, returning `const&`) IS inlined into `__introsort_loop` for pointer elements with a fn-ptr
  comparator. It selects the element's address in esi and parks the middle address in the dead comparator slot
  (0x009327F0). For CThingCompare_Nearest it stays out of line (0x0091FA00).
- copy_backward on a `T**` range is STLport's `__copy_trivial_backward` = `memmove` (0x00BFEAE6), not an element loop
  (0x00932660).
- When `__copy_backward` is a same-TU out-of-line body (`#pragma auto_inline(off)` + `__declspec(noinline)` with an
  inline copy_backward wrapper), its `random_access_iterator_tag` temporary is homed over the still-live comparator
  slot (0x00927740).
- A retry loop with an early abort and a result test:
  `do { if (!Ok()) { Terminate(); return; } ... } while (r == 4); if (r == 1) Terminate();` gives one loop copy and a
  tail-merged Terminate. `while (...)` and `for(;;)+break` are rotated or duplicated (0x0093C7B0, 0x00939DD0).
- A named `C3DVector pos = p->Pos;` passed by value keeps X/Y on the FPU and moves Z through the frame. An
  uninitialised `bool arrived;` whose address is passed is homed in the dead argument slot (0x0093C7B0).
- A same-TU `const` LowerBound stand-in (auto_inline(off), returning the entry pointer) makes VC7.1 pack the lookup key
  and the address-taken out-param into one dead argument word, matching retail's frame (0x00942B30).
- STLport list erase of a polymorphic element = unlink, `_Destroy` (`push 0 ; call [vtbl]`, the scalar-deleting dtor
  with flag 0), then `free` of the node (0x00939DD0).
- An anonymous-namespace function or functor that is only used through a template needs a non-retail caller wrapper
  in the TU, or VC drops the body (0x00905600, 0x00945DE0).
- Not reproduced:
  - `mov eax,[eax+18h] ; add eax,12Ch ; mov eax,[eax]`. Every getter, reference or __forceinline form folds it to
    `[ecx+12Ch]` (0x0091A630).
  - A by-value CThingCompare_Nearest copy of its C3DVector: ours copies Y/Z through the FPU, retail moves the words
    through ecx/edx (0x009204F0).

Review at landing:
- No row needed more than 4 grades. Six parked rows used their 4 grades. Flag sweeps (/Os /Oa /Og /G6 /G7) were tried
  on top of the forms on 0x0090DF60, 0x0091A630, 0x0091FB30 and 0x009204F0.
- Local base models next to existing engine headers:
  - CAIStateGroup_MinionFollowThing (0x009215E0)
  - CAIStateGroup_StealthSeekEnemy and CTCPerceiveThing (0x0093C7B0)
  - CAIStateGroup_MeleeCombatNearby2 (0x0093DFC0, parked)
  - CThingCreatureBase (0x00905550)

  The generated headers flatten the base (`PEnemy[0x8]` pads, LastPerceivedPos split into `_X/_Y/_Z` floats), as in the
  landed siblings. Retype when the headers carry the real CIntelligentPointer / C3DVector members.
- ALREADY-LANDED errors:
  - 0x00913590 `_Construct` is for CBezierCurve (the deque<CBezierCurve> push_back callee), not CResourceEntry /
    "RetailExtra".
  - The landed StealthSeekEnemy::Update 0x0093C830 declares the callee as `MoveOrQuit(CTCPerceiveThing*)`; it is
    `MoveTowardsEnemy(const CTCPerceiveThing*)`.
  - The landed MeleeCombatNearby2::Update 0x0093E5E0 declares `void AddCombatMessage()`; it is
    `bool AlertAlliesInVicinity()`.
  - Landed 0x009328D0 ("SElement4/SCompare functor") is `sort<CThing**, bool (__fastcall*)(CThing*, CThing*)>`.
  - Landed 0x009322D0 `__median`'s element is CThing\*.
- Retail behaviour: 0x0091F740 CThingCompare_Nearest returns the NEARER POINTER, not a bool. It is truthy for any
  non-null pair, so the retail sort is effectively unordered. The 0x0091FA00, 0x00920160 and 0x009206A0 tests use a
  strict stand-in comparator.
- Element type choices, both COMDAT-folded or propagated: the 0x00920F90 / 0x00920FF0 element is C3DVector (any
  12-byte POD folds). The 0x00932xxx element is CThing\* (the Ego_r CThingBuilding/CIsHouseLargerThan label is a
  Dinkumware propagation).
- Non-retail helpers: the `CallGetPRangedWeaponFromCreature` (0x00905600) and `CallIsCookingPot` (0x00945DE0)
  wrappers exist only to emit the anonymous bodies. Do not land them as functions.
- Catalog rows holding several functions: none. Every row is a single body.
