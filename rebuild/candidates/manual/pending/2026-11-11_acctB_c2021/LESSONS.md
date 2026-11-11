# Chunk 2021 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00B40BD0) An inline body that copies a global pointer into a local (`const CEngineWorldMap* worldMap = g_pEngineWorldMap;`)
  is not neutral: in the first expansion of a function the local survives as its own candidate, the expansion's long-lived values
  (MapWidth CSE, x / y parameter copies) run out of registers one colouring step earlier, and the split pass turns the MapWidth CSE
  into two copy-forwarded pieces (= `movzx` reloads) instead of retail's register + spill slot (`mov [esp+38h],ecx` 0x00B40BF3,
  reloaded 0x00B40CC0); later expansions already used the global candidate. Reading `g_pEngineWorldMap->` in place fixed the
  register and slot choice of the whole expansion. c2prio's per-candidate result (`split: eax #77, eax #26` vs `split: ecx #126`)
  is the cheap fitness for it.
- (0x00B40BD0) A function returning a running float total after nested early exits: nested `if (theme > 0) { ... }` with one
  `return depth;` inside and `return 0.0f;` after gives retail's shared `fld [depth]` exit and the constant-zero block at the end;
  flat `if (theme <= 0) return ...;` gives a second inline epilogue and the last sum kept on the x87 stack.
- (0x00B40BD0) Reusing one `index` local for three layers puts them in one frame slot; retail's separate slots ([esp+24h],
  [esp+2Ch]) are separate locals per layer.
- (0x00B40BD0) A one-store scheduling swap (`mov [esp+28h],ecx` before vs after `fmul [k]`) was closed by grouping
  `(blend * WaterHeight) * (1.0f / 255.0f)`: the PAREN tuple between the two fmuls delays the second fmul's issue one slot in c2's
  list scheduler. The hand892 scheduler trace (tools/sched/t13.py, here tools/sched.sh + schedv.py) showed the pick order of every
  variant without a grade; the same grouping in the other two layers changes nothing there.
- (0x00B3F5E0 / 0x00B40BD0) A header inline that retail expands everywhere in one TU but calls out of line in another TU's loop
  (CWaterPatchMesh::GetWaterType 0x00BEAB40 calls PeekThemeId 0x00B43720) is plain inline with a TU-level inline budget, not
  __forceinline: use the smallest per-row `/d2inlT<N>` from the inline trace (T444 / T576 here) and note the TU-wide value that
  fits all rows (>= 576 for the CEngineMap TU, which also covers 0x00B41770's T301).
- (0x00B3F5E0) `C3DVector normal(dx.X, dy.X, dx.Y * dy.Y)` from two normalised C2DVectors: VC7.1 stores only Z and reads X / Y
  straight from the 2D vectors in the inline C3DVector::Normalise; the fourth neighbour height stays on the x87 stack for
  `fsub st(1)`.
- (TU evidence) The CEngineMap TU's accessor COMDATs sit before its ordinary members: 0x00B3ED00 PeekWorldX, 0x00B3ED10 PeekWorldY,
  0x00B3ED20 PeekWidth, 0x00B3ED30 PeekHeight, 0x00B3ED70 MapHasDataAtPoint (plus 0x00B3ECF0 `mov [ecx+8],eax`, 0x00B3ED40
  `mov eax,[ecx+18h]`, 0x00B3ED50 `mov eax,[ecx]`, 0x00B3ED60 `mov eax,[ecx+19Ch]`): the header accessors the Peek bodies use.

Review at landing:
- Merge hdr/engine/CEngineMap.h (`FABLE_RETAIL_CENGINEMAP_PEEK_BODIES`) and hdr/engine/CEngineLandscapeRenderer.h
  (`FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME`) into rebuild/include (additive opt-ins; hdr/README.md).
- Both rows carry `// Flags: /d2inlT<N>` + `// Inline-budget:` lines (owner device 2026-11-08); forced-inline bodies also match
  but contradict 0x00BEAB40.
- 0x00B3F5E0 reaches GFFloatToLongNear only through the header inline QuantiseLandscapeHeight (purity.py does not see such a reach;
  the row still carries its `// Retail-asm:` lines).
- Regrade the parked siblings 0x00B43720 / 0x00B43870 / 0x00B439C0 / 0x00B41770 with the new opt-in (their local `worldMap` copy is
  the lever this chunk measured).
- 0x01231724 is a float literal pool (__real@3b808081), bound by landed rows as named globals (g_OneOver255, g_fReciprocal255_01231724).
