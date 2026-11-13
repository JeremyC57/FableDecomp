# Lessons (chunk 2005)

- **0x00A4D920:** A visible inline body for a callee that VC7.1 inlines anyway (ConvertTimeToFrames) fixes dead-slot frame
  packing. c2 then knows the reference arguments are not captured, so the time parameter slot is reused for a local.
- **0x00B51570:** The loop spelling `for (long v = 0; v < 512; v += 4, ++i)` with indexed `rain[v + k]` gives retail's
  128 down-counter (LFTR) and the 0x30 pointer bias. Pointer increments (`rain += 4`) give no bias.
- **0x00B51570:** A loop body is weighted about 5x for inlining. The same GFFloatRandom is out of line in the rain loop
  (chain stores) and expanded in the snow loop. Model it as a plain `inline` body, not forceinline.
- **0x00B51570:** For 24 constant stores that c2 reorders, a tiny inline helper (`SetIndex(p, n, v)`) adds parameter
  tuples. That splits the 81-tuple scheduling region and keeps source order.
- **0x004978A0 (GFFloatRandom):** The body spells `range *= 65536.0` and `result *= (1.0 / 65536.0)` as compound
  assignments. Folded expressions reorder the fmul and fld.
- **x87 commutative order:** c2 loads the higher candidate id first. A CSE temp shared with an earlier named variable
  (`1 - t` versus `oneMinusFraction`) flips one fmul pair, as at 0x00A4DAC6. Lever searches over spelling rarely fix it.
  Trace the candidate ids instead.
- **0x00AB2520:** CCharString built from `const char*` needs `static_cast<const CCharString&>(ptr)`, an implicit temporary.
  `CCharString(text)` gives the wrong temporary addressing, and plain assignment picks `operator=(const char*)`.
- **0x00AB2520:** Inlined CMatrix3x4 PostMultiply and SetInverse term orders differ from the landed header in this TU.
  Review at landing, before you change the shared header.
- **0x00A12EE0:** CVectorMap::Sort and std::sort must both be forced inline (opt-ins) to get retail's inline `__lg` and
  final insertion sort calls (0x008274B0, 0x008270E0, 0x00825B40).
- **Review at landing:** Pair-land 0x00AB1CC0 (the same-TU noinline body in differ/00ab2520.cpp) with 0x00AB2520.
- **Review at landing:** 0x01436E98 has three landed spellings (GPResourceManager, g_pEngineData, g_pResourceManager).
