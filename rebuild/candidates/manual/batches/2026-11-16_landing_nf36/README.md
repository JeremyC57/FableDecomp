# 2026-11-16 landing nf36: name-fix round 2, NF36 (74 landed rows, 33 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 2 continues NF34's half: tags land1115ga ..
land1116p23, 466 entries (NF34's carry and partial, re-screened after round 1 landed). Brief and records:
`naming/2026-11-16_agentNF36/`. WRONG_BINDERS.tsv there holds the per-target list of rows still binding a wrong name: the
worklist for the next round.

## Queue (466 entries; every entry labelled)

| Status | Entries |
|---|---|
| done | 47 |
| done-already | 10 |
| blocked | 72 |
| partial | 10 |
| rejected | 38 |
| carry | 289 |

- **blocked:**
  - port 60: the class has a shared header, but its binders reach it through local stand-ins; each note lists the rows;
  - authoring 7: mid-function tails and the anonymous-namespace hashes;
  - ICF 2;
  - library 3: MSVCR71 import stubs.
- **rejected (38):** one decision. These claims cite only the Sensaura GameCODA SDK (Codex passes 22/23), and neither PDB has
  them. EXTRA_RULES (2026-11-08): "No legitimate source, SDK or object exists for ... Sensaura's GameCODA (leaks are never used)".
  The landed names stay.
- **Kept:**
  - Three done self renames cite the same SDK: 0x00C609A0 AddSource → AddBuffer, 0x00C5DD70 `int` result, 0x00C49D50 class key.
    They only bring each row's own symbol in line with what its landed caller from landing p23 already binds (one address, one
    name).
  - Each name describes its body (AddBuffer takes a CEAXBuffer*), which the 2026-11-04 rule for vendor code with no PDB allows.
- **carry (289):** 56 data globals, 86 libpk/CODA descriptive-name entries and 147 other functions. All but one were probed;
  the notes list the rows still wrong.

## Rows (74; three tests changed)
- **Section 1:** 33 self renames. Among them:
  - 0x0049D610 / 0x0049D770 move from the CWorld_Methods view onto engine/CWorld.h;
  - 0x00434870 CFadeInFadeOutBase::Update;
  - 0x00434C90 CDisplayEngine::EndScreenFadeOut;
  - 0x00B990B0 UpdateBoneMatrixForWindSheer (private);
  - the seven `CAIStateGroup_<X>::GetName` static factories.
- **Section 2:** 61 callee respellings.
- **Changed tests:**
  - 0x00401F30 and 0x00A205C0 were respelled to the new names;
  - 0x008FD7D9's test is new: the old test called the removed stand-in. Its row now passes the retail literal
    "SG_BUTTERFLY_WANDER" (0x0128954C), where the landed row had "DoCreatureAction".
- **Coordinator ports (`coordinator_ports/`):** six of the GetName rows (0x008FD46B, 0x008FD94E, 0x008FDBA2, 0x008FDBCB, 0x008FDBF4,
  0x008FE14D) came back with two problems:
  - They included their shared header under a `#define X XLayout` rename and then modelled the class locally.
  - They passed the string as a cast address `(const char*)0x01288F80`.

  They now define the shared header's opt-in `FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC`, include the header directly, and pass
  the retail string literal. All six are RELOCATION_MATCH.
- **Recheck (`logs/recheck.log`):** all 74, graded from the repo after landing: 7 MATCH and 67 RELOCATION_MATCH, every test PASS.
- **Headers (opt-in only):**
  - CEngine.h `FABLE_RETAIL_CENGINE_SCRATCHZBUFFER_PDB`;
  - CEnginePrimitiveManagerRepeatedStaticMeshes.h `..._RENDERSUB_NESTEDCLIP`;
  - CWorldMap.h `FABLE_RETAIL_CWORLDMAP_CACHEMAPS`;
  - CAIStateGroup_OpinionReactionRoot.h `..._FOLLOWUPDATE`.
- **Bindings:** rebind removed 169 lines and added 144. bindings_multi_check is at baseline, and funclet_includes_check reports
  1,355 checked, 0 broken.

## Open (NOTES.md)
- **0x0049DDD0** inlines 0x0049D550; both rows need porting into one translation unit.
- **0x00B99710's own leaf** is still `QAE..@3`; the PDB spelling is `AAE..@1`.
- **0x005063E0** still binds the public, non-nested CacheMaps.
- **0x00416953** still binds `?GetLevelsDirectory@@` at 0x0041A060.
- **0x013B8616** has two names: `g_bRuntimeLevelFiles` in 0x0049DDD0 and `g_FableNavigatorEnabled_013B8616` in fable_gfmain.h.
