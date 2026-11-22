# Landing nf269 (Account A, 2026-11-22): the six Delete binders onto the PDB CEngineLandscapeMemoryManager spellings (6 rows, 2 tests, 1 header)

Agent NF269 (entry 1, D5 of nf267, done; entries 2-5 not started: context spent on the chained briefs). Hand-back:
`pending/2026-11-22_nf269_handback/`.
- **Call targets re-proven** two ways: exe scan + per-site disassembly of which member is pushed (`evidence/callsites.txt`, mnemonics only),
  and the member types from the PDB layout headers (CEngineWaterBackgroundSubPatch.h, `_quarantine/CLandscapeLayerMesh.h`, CWaterPatchMesh.h)
  against which disposal list each queue body uses.
- **0x00BEAA00 / 0x00BFDD40:** ported onto the retail branch of `engine/CEngineLandscapeMemoryManager.h` (the landed `_DELETEVERTEXBUFFER`
  opt-in); their local two-member manager model goes; FreeBlock -> DeleteVertexBuffer. Every section equals HEAD's but `.drectve` (+libcp).
- **0x00BFD480 / 0x00BFDE20:** in place in each row's existing local manager model: AppendFreeEdgeNode / FreeBlock -> DeleteIndexBuffer /
  DeleteVertexBuffer (0x00BFDE20's members take the Ego_r names; HEAD had the vertex / index names swapped). objsec ALL_EQ.
- **0x00BEAAA0 / 0x00BEB4D0** (July rows on `candidates/outline_glow_compiled.h`): new opt-in `FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER`
  swaps the header's stand-ins (DAT_01436ea8, `LandscapeLayerList::InsertNode(int)`, a local manager) for GPLandscapeRenderer and the retail
  manager branch, typing +0x10 as `CVertexBufferWin32*`. Existing lines only wrapped in `#ifdef`; 3 asserts pinned; equiv_check over the
  header's closure 1,374 rows, 0 differ (planted-token control flags both includers). Both rows objsec ALL_EQ; both tests rewritten on the
  shared headers (PassPatterns kept; they link the rows).
- **Census:** 0x00BDA090 2 -> 1 spellings, 0x00BDA0D0 3 -> 1; 0x01436EA8 stays 11 (two rows move from DAT_01436ea8 to GPLandscapeRenderer, four July
  rows keep DAT_01436ea8). No address grows; no new symbol name. Link 0 duplicates (control 1); vtcheck HEAD v new identical.
- **Table:** InsertNode / FreeBlock / AppendFreeEdgeNode lines deleted (all binders moved; 0x00BF9B40 / 0x00BF9B90 / 0x00BFDD70 only declare
  them); the DAT_01436ea8 line rewitnessed 0x00BEAAA0 -> 0x00BE8920 (which still binds it).
- **Recheck** (`logs/recheck.log`, landed tree): the 6 rows + 0x00BDA090 / 0x00BDA0D0, the declarers 0x00BF9B40 / 0x00BF9B90 / 0x00BFDD70, the
  new witness 0x00BE8920 and 0x00BE6090 (13 rows): 12 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT; 0x00BE8920 DIFFER(328v365) PASS, the same DIFFER it grades at HEAD (a July candidates-header row; nf131 / nf141 / nf192 logged it), and no file it compiles changed.

## Coordinator rulings on NF269's questions
1. 0x00BFD480 / 0x00BFDE20: the in-place respell is accepted (byte-identical objects, PDB names); their port onto the shared headers (their
   row-local CMovableResource model blocks the retail branch) is queued.
2. 0x00BEAAA0 / 0x00BEB4D0: the candidates-header opt-in is accepted for this binder-only pass. Queued: the CWaterPatchMesh unit
   (0x00BEAA00 ReleaseBuffers, 0x00BEAAA0 `??1CWaterPatchMesh@@UAE@XZ`, 0x00BEB4D0 `??_GCWaterPatchMesh@@UAEPAXI@Z`: self renames with
   vtable-slot binders) together with the 0x00BFDD40 split (`?DeleteForegroundVertexBuffer@CLandscapeLayerMesh@@QAEXXZ` v 0x00BF38F0's
   `?ClearBlock@CTextureOwner@@QAEXXZ`).
3. Landed as one commit (0x00BEAAA0 / 0x00BEB4D0, their tests and the overlay together; the other four with them).
4. The four unchanged host-model tests carrying stand-in names are queued with D6.
5. NF269's open entries (rest of L129, D4 one-definition pass, D6 0x00BE6090's test, 0x009B0AC0) go to the next agent.
Slip (one `rm -f` on a variable path, a single link-control copy): noted.
