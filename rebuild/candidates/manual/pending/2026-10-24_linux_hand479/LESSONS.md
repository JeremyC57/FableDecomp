# Chunk 479 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport sorts with a by-value comparator whose operator() or scorer calls out-of-line engine helpers: give those
  helpers same-TU `auto_inline(off)` + `__declspec(noinline)` bodies. VC7.1 then knows the calls leave the comparator
  copy untouched:
  - it caches comparator members in registers (Hero in esi, 0x0080BAB0);
  - it hoists the inlined `__linear_insert` comparator copy out of the loop (0x007FA450).
- The per-function inline budget picks which call sites are expanded. Model the callee with its real inline body
  (not declaration-only) and let VC choose:
  - 0x007F7C10 matched with CVectorMap::LowerBound inline: VC7.1 expands it at the first of seven find_if tests and
    calls 0x004365B0 at the other six, exactly as retail.
  - 0x00809F80 matched the same way with an inline filter.
  - 0x00807C30 still falls short: 4 of 7 expansions vs retail's 6.
- The GetInterface/PeekTC pick after an *inlined* LowerBound: only the one-ternary
  `((it == last || type < it->kind) ? last : it)->tc` reuses the iterator register. The if/assign form adds
  `mov eax,esi` (0x007F7C10).
- A free `template <class T> bool operator==(const CIntelligentPointer<T>&, T*)` loads the raw pointer before GetPItem.
  A member operator loads it after (0x00807C30).
- Vector normalise in explosion functors:
  - The line-out vector as a named local, then `direction *= 1.0f / distance` through an inline `operator*=`, matched
    (0x0080DA50).
  - When the vector's X is held on the x87 stack, the inline `*=` keeps dead registers alive. Per-component in-body
    statements fix that but can flip one `Y*k` operand order (0x0080DB70).
- A CIntelligentPointer temporary (`PThing = CIntelligentPointer<CThing>(0)`) and a later named counter share one
  frame slot only when the counter is declared in a nested block. In one flat scope VC7.1 gives each its own word
  (0x007FE990: sub esp 1Ch vs retail 18h).
- An inline cross product that retail spills component by component (`fstp [t] ; mov reg,[t] ; mov [dst],reg`) is
  not reproduced by `return C3DVector(...)`, by named-local member stores or by an inline `SetCrossProduct`. VC7.1
  forwards X/Y from the x87 stack. Untried: the `(float)(double)` component casts of the landed 0x007A1870
  (0x007BEBE0).

Review at landing:
- 0x007BC38A starts at an unaligned address right after the previous function's `leave ; ret` (no int3 padding), and
  both use ebp frames. The row boundary looks right (vtable 0x0123CF20 points at it), but the TU is probably
  size-optimised or not /Gy: try `#pragma optimize("s", on)` first.
- 0x007C0AD0 NThingShot::CreateShot has two PDB overloads; pick the right mangled name for the leaf.
- 0x007FE990 / 0x007FF2F0 use a local CTCUserActionPointBase model, because engine/CTCNamedUserActionPoints.h is all
  pads with a wrong Names offset (+0x3c; retail +0x34).
- Every parked row used its full 4-grade budget except 0x007FF2F0 (1 grade, sibling of 0x007FE990). No row went over
  budget.
