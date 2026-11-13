# Chunk 3020 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A12EE0) A spilled SECOND induction pointer `lea eax,[ecx+N] ; mov [esp+X],eax` in a pointer loop's preheader (+ `add reg,stride`
  in the latch, used as `mov eax,[esp+X] ; mov eax,[eax]`) is a named reference / inline `this` to a member of the element that is used
  MORE THAN ONCE: `const C3DMeshVolume& mesh_volume = it->second;` read twice. Single-use references, pointers, `(*it).second`, helper
  calls and char-pointer arithmetic all copy-propagate and fold into `[ecx+N]`. The IV is biased to the middle field (it+8 here).
- (0x00A12EE0) An inline that FableWin emits first after a function (FableWin.pdb address order) is called by it: TransformPlane here.
  Fit its body with fwsize and then fix its statement order: which local's fields are first referenced decides commutative x87 operand
  order downstream (normal copied before the point -> retail's Dot operand order).
- (0x00A12EE0) `const T* p = smart.Get(); if (p)` v `if (!smart.IsNull())` v `if (smart.operator->())` decides the reload order of two
  spilled values in a loop latch (0x00A13294).
- (lane) Probe file names like V1.cpp / R1.cpp / S1.cpp / T1.cpp make cl.exe under Wine return 0 with no object (the `/Fo.../V1.obj`
  path is split into options): use lower-case names.
- (tooling) agent3010's tools/c2h7/key.py works with `{"target": ""}`; the row's events are those before its own `pipe` event
  (tools/kana.py prints every 3+-operand add with operand hashes and leaf record ids).

- (0x00A12EE0, closed) c2 allocates symbol records in CHUNKS OF 32 ids from a shared pool; field records created late (point+4,
  point+8) land in whichever chunk has room, so an x87 term order that "no uniform shift can produce" is a chunk-boundary effect:
  one more function-level named symbol moved the second chunk (and a matrix load leaf's hash), and the closing natural spelling
  moved one record from after the first overflow to before it: `const CPlane& source_plane = source_planes[i]; CPlane plane =
  source_plane;`. Diagnose with two rulers together (a dead local at the top AND a helper text with one local fewer); if that pair
  matches, look for one natural statement that does both. Tools: agent3010 c2h7/key.py (k6.py / sids.py here) and c2h6/recs2.py.

Review at landing:
- 0x00A12EE0 RELOCATION_MATCH uses FABLE_FORCEINLINE TransformPlane (plain inline is refused at the site: 887v1144) -- `// Inline-budget:` line in the source.
- 0x00A12EE0 matches on a record-count edge (+1 dead local breaks it): any later change to a shared inline it expands (C3DVector, CMatrix3x4,
  CVectorMap, STLport vector) must regrade it.
- TransformPoint has three header texts (default / _SUMS / _LTR) for one retail body; this row uses _LTR.
