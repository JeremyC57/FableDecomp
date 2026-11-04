# Landing 2026-11-04l: chunk 750 (14 mixed small rows from the groups ledger), 14 landed

## Result
- **Landed 14 / 14** (`logs/land.log`). Before landing they were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): 14 RELOCATION_MATCH + PASS.
  - No row has asm, a helper, a pragma or `volatile`.
  - The agent's own vland run is in `vland_result.txt`, and its notes, status and labels are beside it.
- **What the rows are.** Each catalog label was propagated from a neighbour; the real names come from the vtable each row stores,
  its callers and its strings:

  | Address | Name |
  |---|---|
  | 0x00558FD0 | `NUISystem::CDraggableInto::~CDraggableInto` |
  | 0x00559020 / 0x00559030 | its `vector deleting destructor` adjustor thunks (this -4 / -0x18; `??_E...@W3AE` / `@WBI@AE`) |
  | 0x0077B290 / 0x0077B9F0 / 0x0077BE80 / 0x0077C440 | `NGraphicAppearance::` ~AnimatedMesh / ~StaticMesh / ~StaticMeshFadeOutOnProximity / ~Sprite (one label had covered all four) |
  | 0x008FFBEE / 0x008FFD84 / 0x008FFDCE | ctors of CAIStateGroup_KidPlayHopscotch / _WorkSawingWood / _WorkHammerAnvil |
  | 0x00C207F0 | `CSoundEvent::~CSoundEvent` |
  | 0x00C20810 | `CSoundEvent::StopListening` |
  | 0x00C21040 | `CSoundEvent::Describe` (/EHsc) |
  | 0x00C21780 | `CSoundEvent::Play` (was "CInputProcessRebootGame_HandleInputEvent") |

  - The three CAIStateGroup ctors sit at unaligned addresses but are real functions: each has its own vtable, and its factory
    allocates exactly the PDB class size.

## Headers (2 changed, each only behind a new retail macro; neither header changed in the repo since the overlay base 7d2859ef7)
- `NGraphicAppearance_CTCGraphicAppearance.h`: behind FABLE_RETAIL_CTCGRAPHICAPPEARANCE, adds the `UnregisterComponent` declaration
  (0x004C0000).
- `CString.h`: behind FABLE_RETAIL_CSTRING_BUILDERS, adds the libpk CString constructors, destructor and `__fastcall operator<<`
  overloads, including `operator<<(double)` at 0x00C0DED0.
- hdrcheck2 before the merge: `default=same` (`logs/hdrcheck2_pre_merge.txt`).
- `includers.py`: 2 landed sources include these headers. `ppcmp.py`: both are SAME (`logs/ppcmp.txt`), and the agent's recheck of
  both passes (`SIBLINGS.tsv`).

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04l.tsv`, 23 labels. `apply_labels.py` made 20 catalog and 20 manifest renames:
  the 14 rows, plus CSoundEvent's ctor and vector deleting destructor, CSampleRef::GetSample / ~CSampleRef, CStreamBuffer's ctor /
  dtor, CSoundSample::GetSize / LoadData, and CString::operator<<(double).
- **Queued PRIORITY** (tag `land0411l` in `NAME_FIXES_QUEUE.tsv`): the chunk's 12 NAME_FIXES. These are the parameter types of
  PlayPhysical, WaitForPhysicalPlayer, Attach and Release, and the CSoundEvent placeholders.
  - **Review needed:** 0x00C25C60 `GetSample` returns `CSoundSample*`, so its landed callers WaitForMemory and Update, which pass the
    result on as `long`, need a re-model, not just a respell.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage (`write_decomp_dashboard.py`, `update_readme_progress.py`)
46,606 genuine of the 49,568-function catalog (94.03%); 5,798,905 function bytes (56.70%).
