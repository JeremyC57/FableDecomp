# Landing 2026-11-04u: chunk 759 (retry of 40 parked same-length rows, 395-486 bytes, C++ only), 7 landed

## Result
- **Landed 7 of 40** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - The rows:
    - CActionEventTriggerParticle::SetDataFromString and CActionEventExplosion::SetDataFromString;
    - CAIStateGroup_OpinionReactionRoot::FleeIsRunnable;
    - CMessageEventManager::AddMessage;
    - CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions;
    - CTCShop::SetShopKeeper;
    - CThingGameObject::GetShotHitParameters.
  - The lever that worked most often: replace an old local component-map model with `std::bitset<274>` plus the real
    `engine/CVectorMap.h`, in the PeekTC shape (key copied into a local, then an explicit LowerBound / end() test). Five of the seven
    rows needed it. Their PDB signatures (const references) did the rest; details in `agent_NOTES.md`.
- **Asm rules (owner, 2026-11-04):** no asm, helper or `volatile`. The only pragmas are `pack` (struct layout) and `intrinsic(sqrt)`.
  A same-TU `__declspec(noinline)` callee body (0x006AEEA0, as in the landed CTCWound::AddWound) replaces the old
  `auto_inline(off)` wrapper.
- **Parked 33** (`pending/2026-11-04_linux_hand759/differ/`). The residues are register ties, scheduling, x87 operand order and frame
  slots.
  - **The 10-grade budget was not met for 29 rows:** 18 got only a baseline grade, and the rest 2-9 (`STATUS.tsv`). They are
    queued for a follow-up chunk that applies the PeekTC / CVectorMap lever first.

## Headers (1 changed)
- `fable_string.h` gets the new sub-macro FABLE_RETAIL_CCHARSTRING_SUBSTRING, declaring FindCharPos 0x0099E5A0 and
  GetSubString 0x0099EC70.
- hdrcheck2: `default=same`, and every existing macro is unchanged.
- `ppcmp.py`: all 3,379 includers same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04u.tsv` (15). `apply_labels.py` made 14 catalog and 9 manifest renames.
- **Queued PRIORITY** (`land0411u`):
  - 0x00B4FC10 is `CEngineShadowRenderer::ChangeVideoOptionShadowQuality(long, float)` returning bool (both PDBs). It was landed as
    SetShadowMapSize.
  - 0x00A01B50 / 0x00A01B90: GetPItem / SetPItem take `CBaseObject*`. Two bindings spell them with `CThing*`.
  - 0x00485815 is `GetSubDef<CCreatureDef>`.
- **Open, not resolved:** 0x00662000 is called as GetCreatureType (PDB) but labelled GetMovementMode.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,678 genuine of 49,568; 5,815,062 function bytes (56.88%).
