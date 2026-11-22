# Landing nf271 (Account A, 2026-11-22): FreeVirtualAligned onto the PDB name and three 0x01436EA8 stand-ins onto GPLandscapeRenderer (4 rows, 1 test, 1 header)

Agent NF271 (entry 1, the rest of L129, as far as a naming pass goes; entries 2-5 not started). Hand-back: `pending/2026-11-22_nf271_handback/`.
- **The plan's target was wrong:** the stand-in `LandscapeMemoryMethods::FreeAlignedVirtual` is not 0x00BD9B90 (nf262 / nf263 plans, row
  comments) but 0x00BD9CA0: retail calls 0x00BD9CA0 at 0x00BFAB06 / 0x00BFAB11 / 0x00BFAB27 and the table already pointed the stand-in there.
  0x00BD9CA0 is `?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z` (Ego_r 009c2430 + FableWin 02cb7a40 name sets; Ego_r order v
  retail 0x00BD9C40 / 0x00BD9CA0 / 0x00BD9CC0; body frees `block - block[-1]` by a tail jump to FreeVirtual). Proof (mnemonics only):
  `evidence/unitA_freevirtualaligned.md`; the disproven claim in REJECTED.tsv.
- **Unit A:** 0x00BD9CA0's self leaf moves to the PDB spelling; the row ports onto the retail branch of `engine/CEngineLandscapeMemoryManager.h`
  through a new add-only opt-in `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_FREEVIRTUALALIGNED` (every section = HEAD's but `.drectve` +libcp;
  a full Leaf line added); its test rewritten on the header, links the row, PassPattern kept. 0x00BFAAE0 / 0x00BFB590 in place (nf262 D3 /
  nf269 ruling 1): the global through the landed `_GPLANDSCAPERENDERER` opt-in, the LocalDetailEngine / LandscapeMemoryMethods stand-ins
  gone, 0x00BFAAE0's row-local manager class gains a `FreeVirtualAligned` declaration; objsec ALL_EQ, pk and both vtables identical.
- **Unit B, 0x00B684F0:** only `g_pEngineGlobal_01436EA8` moves to GPLandscapeRenderer (its pointee carried no callee spelling); objsec
  ALL_EQ; Leaf line pinned from the HEAD object. Its CallView self leaf and five callee stand-ins need a port (queued).
- **Census:** 0x00BD9CA0 2 -> 1 spellings; the 0x01436EA8 table 11 -> 9 lines; no address grows, no new split. The new name is defined only by
  0x00BD9CA0. Link (new objects + HEAD 0x00BD9B90) 0 duplicates; each control 1 LNK4006. Header: equiv_check over 1,399 rows, 0 differ
  (planted-token control flags 28 lines).
- **Table:** three stand-in lines deleted (FreeAlignedVirtual, g_LocalDetailEngine, g_pEngineGlobal_01436EA8), one PDB line added.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 rows + 0x00BD9B90 and the header's other recent includers 0x00BDA090 / 0x00BDA0D0 /
  0x00BDA110 / 0x00BDA150 / 0x00BD9CE0 / 0x00BEAA00 / 0x00BFDD40 / 0x00BEAAA0 / 0x00BEB4D0 / 0x00BE6090 (15 rows): 15 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF271's questions
1. Unit A lands as one (self port, opt-in, test, both binders).
2. 0x00B684F0: the global-only move is accepted; its CallView port (self leaf, five callees, FreeDisposalList 0x00BD9CE0 for `ClearTexturePool`) queued.
3. The CEngineGlobal family (0x00BE61B0 / 0x00BE79D0 / 0x00BF8030 / 0x00BF84C0) is queued as one header unit (renderer Settings +0x36,
   CLandscapePatchTesselator / TesselatePatch 0x00BE3910, AllocVirtual / FreeVirtual on 0x00BF8030); not held.
4. Entries 2-5 to the next agent; the host-model test list gains 0x00BFAAE0 / 0x00BFB590 / 0x00B684F0. Entry 2 first measures which symbol
   0x00BF4220 (0x00BEAA00's only landed caller) binds.
Rule slips (GOTCHAS grepped and its newest lines read, not read in full; ASM_RULES not read): noted; the next brief repeats the reading list.
