# Chunk 68 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- If the leaf given to q.py / the lander matches no symbol, the grader silently grades the LARGEST function in the TU:
  when line 1 names the function differently from the catalog, check the leaf against `objdump -t` of
  `$S/wine/q/<addr>/<addr>.obj` (every leaf in this batch was checked).
- Quest-script layout: retail members sit 0x14 below Ego_r.pdb, plus 4 bytes less per PDB vector before them. Watcher
  loops = `while (!terminating) { if (cond) { if (IsActiveThreadTerminating()) return; Flag = true; } GSI->NewScriptFrame(); }`;
  a final `IsActiveThreadTerminating();` with the result dropped is the tail `jmp`. `ghidra_out/cgsi_slots_aligned.tsv`
  names every game-script interface slot.
- A C3DVector returned by a `__fastcall` hidden-pointer function must land in a NAMED local (an inline temporary pushes
  eax and evaluates later arguments before the call; the named local is re-addressed with `lea` afterwards).
- Inlined container constructors: SGI `std::set` = `header = 0; header = malloc(0x14); count = 0;` then the red header
  linked to itself, re-read after every store; STLport list = `node = 0; n = malloc(sz); n->next = n->prev = n; node = n;`.
- A static `__declspec(noinline) __fastcall` callee defined in the same source keeps edx live across the call like retail
  (but the lane then turns the loop into a count-down, parked 00ab37b0).
- A `ret N` constructor whose arguments are never read (CXMVPlayer `(bool, unsigned long)`): take the parameter types from
  the Ego_r.pdb mangled name.

Review at landing:
- 7 sources define a local struct although an engine header exists (CPlayerGui, CPlayerManager, CThingMapManager,
  CGameCameraManager, CACompressor, CNavQuadTree, CLocalDetailCacheMap): the headers carry PDB offsets that disagree with
  retail or flatten the vptr; each source says so. Expect SHARED_HEADER_AVAILABLE warnings (rejected only under
  LANDVERIFY_STRICT_HEADERS=1). Accept if the stated offset differences hold.
- Parked 00c70a60 is a free 128-byte SSE copy whose ORIGINAL is inline asm (esi/edi saved, xmm0..7 in order): pure C++
  cannot reproduce it under the byte-purity rule -- do not retry; leave it as an oracle row.
- Parked 004497e0 (tail merging), 00e24210 (24l `back()` address gap), 00ab37b0 (loop count-down) are lane gaps.
- Three rows whose class could not be established are named `_unattributed`; ~20 label corrections in NOTES.md.
