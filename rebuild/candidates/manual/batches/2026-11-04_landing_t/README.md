# Landing 2026-11-04t: chunk 757 (retry of 40 parked same-length rows, 302-393 bytes, C++ only), 2 landed

## Result
- **Landed 2 of 40** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - **0x00911A90 CAIStateGroup_WanderAroundInitialPos::Update.** One inner `{}` scope around the two GetInterface lookups and the
    GetNearbyWanderPosition query, with a bool result, gives retail's esi/edi assignment.
  - **0x00A1ABF0 CNavigatorAStar::IsNewDestinationGoingToCauseRestart.** A named float for `GetReachRadius(false)`, then
    `radius += reach; radius *= radius;`, gives retail's x87 order.
- **Asm rules (owner, 2026-11-04):** no asm, helper, macro or pragma anywhere.
- **Parked 38**, each after 10+ grades (`pending/2026-11-04_linux_hand757/differ/`, `STATUS.tsv`). The residues:
  - register swaps and rotations (16);
  - instruction scheduling (13);
  - frame-slot order (4);
  - VC7.1's own ordering of commutative fmul operands (3);
  - one SIB base/index order.
  - At this size the parked residues rarely move: 2 of 40, against 11-18 of 40 for the 81-299 byte retries.
- **Header debt:** both rows keep local CThing / navigation / brain / navigator models. vland warns (SHARED_HEADER_AVAILABLE) but
  does not reject.

## Headers
None changed. The agent drafted and then withdrew an unused `FABLE_RETAIL_CAISTATEGROUPBASE_ACTIONWAIT` overlay.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04t.tsv` (2). `apply_labels.py` made 2 catalog and 2 manifest renames.
- **Queued PRIORITY** (`land0411t`):
  - 0x00A1A170 CNavigatorAStar::IsEqual (private const);
  - 0x00A475C0 CNavigatorBase::FindNearestNavigableNode (protected const);
  - 0x006CB6E0 `?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z`: the landed rows 006cb6e0 / 006cb620 /
    006cb840 spell it non-const with a `C3DVector*`.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,671 genuine of 49,568; 5,811,988 function bytes (56.85%).
