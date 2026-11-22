# Landing nf267 (Account A, 2026-11-22): the CEngineLandscapeMemoryManager Delete trio family as one unit (7 rows, 3 headers)

Agent NF267 (entry 1 done: the Delete trio family NF265 found swapped; entry 2, the rest of L129, not started). Hand-back:
`pending/2026-11-22_nf267_handback/`. Rulings it follows: `pending/2026-11-22_nf265_handback/RULINGS.md` D1-D3.
- **The swap fixed:** at HEAD 0x00BDA150 ProcessDisposalList and 0x00BD9CE0 FreeDisposalList defined the public `DeleteIndexBuffer` /
  `DeleteVertexBuffer` / `DeleteTexture` (QAE) with destroy-now bodies, while the three queue rows 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 defined
  the private `...BlockWhileBusy` names. Both PDBs (Ego_r + FableWin name sets; Ego_d field-list access) put the public names on the queue
  bodies and the private AAE names on the destroy-now helpers. Now: the two disposal rows' same-TU helpers carry the private names (in place,
  `private:` / `public:` labels where they stood, no reordering; each object's non-debug sections byte- and relocation-equal to HEAD's,
  50 sections, ALL_EQ); the three queue rows are renamed onto the public names (catalog + files: `..._Delete<X>BlockWhileBusy_<addr>` ->
  `..._Delete<X>_<addr>`) and ported onto `engine/CEngineLandscapeMemoryManager.h` (one declaration opt-in each), tests rewritten on the header.
- **0x00BE5C30 ClearTextures / 0x00BE6090 CLandscapeBackgroundPatch:** NF265's held drafts, regraded; 0x00BE6090's two vtables
  vtcheck-identical to HEAD.
- **Table:** six stand-in lines deleted (`?ReleaseTexture@CTexMgr`, the three `Native::LandscapeMemory::Delete<X>`,
  `Native::Renderer::UnregisterSubPatchForZSorting`, `?gRenderer@Native`: each lost its only binder); four PDB lines added (the three
  `CEngineLandscapeMemoryManager::Delete<X>` and `CEngineLandscapeRenderer::UnregisterSubPatchForZSorting` `AAJ`). `GPLandscapeRenderer` kept.
- **Census:** 0x00BDA110 3 -> 1, 0x00BDA090 3 -> 2, 0x00BDA0D0 4 -> 3, 0x01436EA8 12 -> 11; no address grows; each new Delete<X> name has one
  definer; new list<T*> pick-any COMDATs have no table line or catalog entry. Link: 4 duplicates at HEAD, the same 4 after (below, D4).
- **Recheck** (`logs/recheck.log`, landed tree): the 7 rows + 0x00BE5C60, 0x00BEAE50, 0x00B677A0 and the six other Delete binders
  0x00BFD480 / 0x00BEAA00 / 0x00BEAAA0 / 0x00BEB4D0 / 0x00BFDD40 / 0x00BFDE20 (16 rows): 16 PASS (15 RELOCATION_MATCH, 1 MATCH), 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF267's questions
- Landing: all seven rows and the three headers together (a mixed set would put two bodies under one name again).
- D4: accepted as carried HEAD debt. 0x00BDA150 and 0x00BD9CE0 each still define FreeVirtual and the three helpers strongly (4 duplicates
  before, 4 after, only renamed); the helper copies inline FreeVirtual (72 / 72 / 75 bytes against retail 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0's
  29 / 29 / 28). A one-definition pass (which must leave the two disposal rows' bytes unchanged) is queued for the next agent.
- D5: the six other binders keep their stand-ins (AppendFreeEdgeNode, FreeBlock, LandscapeLayerList::InsertNode), as D1 allows; queued with
  the rest of L129.
- D6: 0x00BE6090's self-contained host test (old Native:: stand-ins, does not link the row, PASSes) is queued for a rewrite on the shared headers.
- NAME_FIXES queued: the BlockWhileBusy names for the uncatalogued 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (VB / IB order from Ego_r only),
  0x00B677A0's self row `PAJ` v the PDB `AAJ`, 0x00BE6090's dtor, and the D5 binders.
