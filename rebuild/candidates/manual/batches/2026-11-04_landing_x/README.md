# Landing 2026-11-04x: chunk 762 (follow-up on chunk 759's 29 under-budget parked rows, 395-486 bytes, C++ only), 5 landed

## Result
- **Landed 5 of 29** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  Every row got 10+ grades this time (5-18; `STATUS.tsv`).
  - **CAIStateGroupBase::AIAction_MinionWander (0x0097F870):** a `bool found` flag in place of the draft's gotos flips the
    callee-saved pair. It also uses PeekTC over the real CVectorMap and the PDB manglings.
  - **CMessageEventManager::AddMessage (0x00687540):** the shared CBaseObject / CIntelligentPointer (_ASSIGN) headers.
  - **CPlayer::SwitchPlayerCharactersWith (0x004888C0):** the same headers, plus each player number read into a named local before
    `SetPlayerNumber(n)`.
  - **CEngineSubPrimitiveRenderer::RadixSort (0x00B847D0):** a pass counter (`shift = pass*10`) and a named radix local.
  - **CWorldMap::UnloadRegion (0x004FEEC0):** `CArray<long>& maps = Regions[region].Maps` in place of `CRegion& r` fixes an
    ebx/ebp swap.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile`.
- **Parked 24**, all with 10+ grades (`pending/2026-11-04_linux_hand762/differ/`). The residues are scheduling, frame-slot
  sharing, zero-register placement and VC7.1's fixed order for float dot-product sums.
- **Header debt** (vland warns, does not reject): 0x0097F870, 0x00687540 and 0x004FEEC0 keep their landed siblings' local models.
  `engine/CWorldMap.h` is the PDB layout and does not match retail's offsets.

## Headers (3 changed, each behind a new opt-in sub-macro)
- `CPlayer.h`: FABLE_RETAIL_PLAYER_SWITCHCHARACTERS.
- `CEngineSubPrimitiveRenderer.h`: FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_RADIXSORT.
- `CArray.h`: FABLE_RETAIL_CARRAY_GETASCARRAY_START.
- hdrcheck2: `default=same` for all three. `ppcmp.py`: all 662 includers same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04x.tsv` (8). `apply_labels.py` made 8 catalog and 3 manifest renames.
- **Queued PRIORITY** (`land0411x`):
  - 0x00487DD0 is `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ`. Four landed files spell it
    GetPlayerCharacter (0x00957F20) or GetControlledCreature (0x0089ED80, 0x005A45F6, 0x005A731F).
  - 0x004C7440 CThing::SetPlayerNumber.
  - 0x006D5AD0 CThingAICreature::GetCurrentStateGroupScriptPriority.
- **Not claimed:** 0x00640430, where both RemoveMap and SetMapAsUnloaded exist in Ego_r.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,704 genuine of 49,568; 5,822,206 function bytes (56.96%).
