# Linux container lane — 2026-10-13

Session on branch `FullDecomp` (head `8d6ce97c` at start) in a cloud Linux container
(4 CPU, 15 GB, Ubuntu noble, Java 21, Python 3.13, no Windows, no VC7.1).

## What the container can and cannot do

| Capability | Status | Evidence |
|---|---|---|
| Retail `Fable.exe` | yes | extracted from the user's RAR (RAR5; Ubuntu `7zip` lacks the codec, `unrar` works): 16,666,624 bytes, linker 7.10, 6 sections, entry `0x401067`, `.text` 0xE2B8A8 |
| Ghidra 12.1 headless | yes | `ghidra_12.1_PUBLIC_20260513.zip`; full auto-analysis of the 14.8 MB `.text` took ~20 min on 4 cores |
| Reproduce the named DB | yes | `LabelApply.java` fed a 57,097-row TSV generated from `rebuild/manifest/functions.tsv`: 55,540 renamed, 12,660 functions created, 0 failed |
| Byte-match grading | **no** | needs `cl.exe` 13.10.3077 on the Windows box; nothing authored here can be promoted past `candidate` from the container |
| Byte-exact oracle extraction | yes | `pe_oracle.py` logic ported to env-driven paths (scratch); `crawl/rowtrim.py` works unchanged (capstone from pip) |
| Shape templates | yes | opcode-skeleton index over the landed `MATCH`/`RELOCATION_MATCH` set finds byte-exact siblings for ~40% of the smallest unlanded bodies |

## Landed in the tree

- `rebuild/candidates/manual/batches/2026-10-13_linux_b1/` — 29 commented, header-typed
  candidate sources with self-contained VC7.1 behaviour tests, `payload.json` + `oracle.tsv` in the
  exact `verify_and_land.py` input format, and a README with the grading command. All 58 TUs pass
  a host syntax check (`g++ -m32 -std=c++98 -fsyntax-only -I rebuild/include`), the purity gate
  and the shared-header policy check (0 warnings).

- `rebuild/candidates/manual/batches/2026-10-13_linux_b2/` — 42 more candidates, restricted to
  catalog-unique labels on header-backed classes; 84 TUs pass the same three checks. Its README
  tabulates seven retail-vs-PDB layout shifts (CBankFile, CTCHeroStats, CTCInventoryTrade,
  CTCInventory, CLandscapeBackgroundTreeNode, CNavigatorManager, CQuestManager) that the generated
  headers do not yet reflect, and one relabel (0x00CACBE0 is a `__stdcall` COM AddRef helper).

- `rebuild/candidates/manual/batches/2026-10-13_linux_b3/` — 23 more (same selection rule); 46 TUs
  pass the three checks.
- `rebuild/candidates/manual/batches/2026-10-13_linux_b4/` — 28 more, including five CTCVillage
  functions placed by the refined container rule; 56 TUs pass the three checks.

## Findings worth keeping

- Seven of the smallest unlanded "families" are not what their labels say. The 747 bodies labelled
  `CActionDoCreatureAction::GetActionName` are constructors: each builds a `CCharString` at `this`
  from a distinct literal (`"CTCTargetingPlayer"`, `"SG_PREPARE_MEAL"`, ...) with length `-1` and
  returns `this`. The strings are thing-component class names and AI state-group names.
- `CUserProfileManager` "NewProfile" fields sit 0xBC bytes after their live counterparts in retail
  (Ego_r.pdb says 0xC0): the shipped build dropped one 4-byte field before that block. Getters
  labelled `GetCameraSensitivity` (0x0054A530/0x0054A590) are `TextureDetail`/`MeshDetail`; the one
  labelled `IsCameraResetting` at 0x00548710 is `VerticalSyncEnabled`.
- `CTCWife::ClearAppearanceChangedNoticed` writes `FrameNoticedChangeInAppearance[category - 2]`
  (array at +0xAC per PDB, store at +0xA4 + 4*category).
- `CEngineSceneGridCell::InvalidateAll` clears bits 0..2 of the +0x44 bitfield (BoundingBoxValid,
  TotalCountValid, RenderMaskValid) up the `Parent` chain, stopping at the first already-invalid cell.
- `CThing` +0x48 is `CVectorMap<ETCInterfaceType,CTCBase*> Components` (PDB), so the body labelled
  `CThing::GetPBaseDef` at 0x007334B0 (reads +0x48 as a ref-counted pointer) is mislabelled; skipped.
- **Container-size rule.** Relative to Ego_r.pdb, a retail `std::vector` / `CArray` / `std::map` /
  `std::set` / `CVectorMap` is 4 bytes smaller and a retail `std::list` is 8 bytes smaller (head
  pointer only); a retail vector is `{first, last, end}` with `first` at +0. Members move earlier by
  4 per vector-like and 8 per list that precede them. Proof: CTCVillage (17 lists + 1 set before
  `Initialised` = 0x8C; retail reads AmountOfFineAlreadyPaid +0xA0, HeroEnemyOfGuardsUntil +0xD4,
  CurrentIsHeroCriminal +0xE4, CrateTasks +0x60 exactly there), CPlayer (InitialPlayerModeSet
  +0x216 after the PlayerModes list), CTCHeroStats (Marriages +0x1D8 after six vectors/maps),
  CBankFile (-0x10 / -0x24), CTCInventory (-0x28), CTCInventoryTrade (-0x2C), CQuestManager (-8),
  CTCCombat (-8), CClassFactory (-4), CTCTurncoat (-4), CTCCreatureGroupBoastingCrowd (-4).
  `gen_class_headers.py` can derive retail layouts mechanically from the PDB member types; until
  then the batch TUs carry `<Class>_Retail` views.
- Callee labels reached from these bodies are mostly propagated garbage (`CPixelShader::CountReferences`
  at 0x00662000 is called on a creature, `CWorld::DrawGetEnvironment` at 0x00449700 on a
  CPlayerManager). Only landed `PASS` identities were used for naming.

## Resume

1. On the Windows box: run the grading command in the batch README; `--land` the wins; add the
   DIFFER addresses to `tools/decomp_pipeline/crawl/gen_tried.txt`.
2. To regenerate the container-side artefacts: Ghidra project lives only in the container scratchpad
   (not committed); rebuild it with the headless import in `docs/BUILDING.md` and
   `LabelApply.java <tsv from functions.tsv>`.
