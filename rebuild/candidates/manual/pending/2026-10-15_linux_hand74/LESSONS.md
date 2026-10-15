# Chunk 74 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CScriptedMapBrush cell accessors: the bounds test as an inline `C2DBoxI::Contains(x, y)` and the row width as
  `Width()`, indexing `Cells[(y - TLY) * Width() - TLX + x]` -- retail compares against memory and does
  `mov esi,edi ; sub eax,esi`; testing the members directly makes the lane cache them and push ebx.
- STLport vector member: `_M_start(0), _M_finish(0)` plus an `_STLP_alloc_proxy` end member built from (allocator, 0)
  puts `mov ecx,esi` between the second and third zero store; a plain three-pointer init list puts it before all three.
- Small clears: a `bool x[4]` cleared by a loop = one dword zero store; an intrinsic `memset` of an 8-byte member =
  `lea eax ; mov esi,eax ; mov [esi],edx ; mov [esi+4],edx`; `member = T()` stores through the argument register.
- STLport `deque<T*>` copy constructor (0x0059B4FE): a frame-pointer size TU (`optimize("y",off)` + `("s",on)`); the empty
  allocator and `__false_type` temporaries both sit at `[ebp+0Bh]`; the iterator needs a user default constructor
  (returns through the hidden pointer in ecx under `__fastcall`) and an implicit copy (by-value iterators = `movsd` blocks).
- A float-returning virtual forwarder needs a named `float` local to keep `call ; ret` (else the lane emits `jmp`).
- `$S/w22/agent74/probe/qx.py` (via `pqx.sh`) grades any address/length straight from Fable.exe (no oracle row needed) --
  same idea as `chunk_pool/oracle_from_exe.py`.

Review at landing:
- The agent reports that the LANDED source of GetThemeBlend (0x0082E3D0) does not grade MATCH under plain q.py while
  its CScriptedMapBrush model (above) does. Re-grade the landed file with `verify_and_land.py` (it applies the pragma
  sweep the plain grader does not); if it truly differs, replace it with the new model and re-land.
- The `asm_bake` rows GetTheme (0x0082E330) and SetThemeBlend (0x0082E420) can probably be re-authored as C++ with the
  same accessor model (CLAUDE.md byte-purity sweep).
- Five rows skipped as multi-function rows (0088b7e0, 008aae20, 00c3eb60, 00c40e20, 00ca5510) -- logged in
  `rebuild/candidates/manual/SKIPPED_ROWS.tsv` with their pieces.
