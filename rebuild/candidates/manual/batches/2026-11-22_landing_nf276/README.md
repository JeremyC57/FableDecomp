# Landing nf276 (Account A, 2026-11-22): the CLandscapeLayerMesh unit and D4's S3 with the FreeVirtual owner made pick-any (7 rows, 2 CompilerFlags)

Agent NF276 (entries 1 and 2 done; entry 3, the landscape host-model tests, not started). Hand-back: `pending/2026-11-22_nf276_handback/`.
No header changed; no test changed; no reloc_bindings change.
- **0x00BFDE20** self leaf `??1CLandscapeLayerMesh@@QAE@XZ` -> `UAE` (Ego_r 00a494c0 + FableWin 02ddd720; its only caller 0x00BFE900 already binds
  UAE: 2 -> 1 spellings), on the shared CMovableResource and the retail CEngineLandscapeMemoryManager branch; local model on the Ego_r layout
  with both private virtual overrides, +0x3C `Next` (was a stand-in), inline class delete (FableWin `??3CLandscapeLayerMesh@@SIXPAX@Z`). Leaf
  93 = 93, relocations HEAD's. The TU now emits `??_GCLandscapeLayerMesh` (38 bytes, pk EQ 0x00BFE900, cmpobj SAME as HEAD 0x00BFE900's) and
  `??_7CLandscapeLayerMesh`, slot-for-slot retail 0x012A835C's targets (slot 3 Verify 0x009D5100 has no landed definer: queued earlier).
- **0x00BFDDB0** self leaf stand-in `QAE_NH` -> `?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z` (both PDBs + RTTI slot 2; no call site), so
  the new vtable's slot 2 does not add a spelling; MATCH, objsec ALL_EQ; local CEngineLandscapeMap in 0x00BF7F70's shape (header quarantined).
- **0x00BFDD70 / 0x00BFDE80** onto the shared CMovableResource, objsec ALL_EQ (0x00BFDD70 gains a Leaf line and the Ego_r member names). All
  four take `LANDSCAPE_TEXTURE_MAPPING_DIRECTION` from `engine/CEngineLandscapeMeshBuilder.h` (MAPPINGBLEND opt-in) instead of a local copy
  with a made-up enumerator; each object ALL_EQ to its previous form.
- **Link (unit):** 0 duplicates in every order; slot 0 is retail's body in every order (both `??_G` copies SAME). Which `??_7` survives is
  order-dependent: 0x00BFDE20 first keeps the right 4-slot table; 0x00BFE900 first keeps its HEAD 3-slot local-model table (ruling 1). At HEAD
  `??1 ... UAE` did not resolve at all, so no order is worse.
- **D4 S3 + owner:** the landed S1 rows 0x00BDA150 / 0x00BD9CE0 graded with `/d2inlT114` (now their build_candidates CompilerFlags, the owner's
  own): leaves RELOCATION_MATCH, relocations unchanged; FreeVirtual copies 68 -> 97 bytes, pk EQ 0x00BD9B90; Deallocate 63 -> 92, EQ 0x009A4350;
  the three BlockWhileBusy helper copies 72 / 72 / 75 -> 101 / 101 / 104 (still NEQ, no landed owner; retail's are 29 / 29 / 28). Owner
  0x00BD9B90 made pick-any (nf88 form: FreeVirtual `inline` plus an anonymous-namespace address-taking helper, `// Fidelity: scaffolding`):
  cmpobj SAME for all four COMDATs incl. the leaf, only NODUPLICATES -> ANY. Link: 0 duplicates and every order keeps an EQ 97-byte copy
  (owner pick-any with the S1 rows alone would let some orders keep the 68-byte copy, so the set lands together).
- **Recheck** (`logs/recheck.log`, landed tree, flags from build_candidates): the 7 rows + 0x00BFE900, the queue rows 0x00BDA090 / 0x00BDA0D0 /
  0x00BDA110, 0x00BFDDD0, 0x00BFDD40 and 0x00BF38F0 (14 rows): 14 PASS (13 RELOCATION_MATCH, 1 MATCH), 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF276's questions
1. 0x00BFE900's local models (its own 3-slot `??_7CLandscapeLayerMesh`, `??_7CMovableResource`, a pick-any `?Verify@CMovableResource@@UBEKXZ`
   returning 0 where retail returns 0x7B, a CanRelocateData copy) are carried HEAD debt; the re-port of 0x00BFE900 as a fold of 0x00BFDE20's file
   (the 0x00BEB4D0 pattern; check its 20 KB host test first) is queued as the next agent's first entry.
2. The 0x00BFDDB0 self rename lands with 0x00BFDE20.
3. The D4 set lands together: 0x00BDA150, 0x00BD9CE0, 0x00BD9B90 and the two CompilerFlags lines.
4. One commit for all three sets.
Slips (two grades launched in the same call as their precheck after comment-only edits; both regraded later): noted.
