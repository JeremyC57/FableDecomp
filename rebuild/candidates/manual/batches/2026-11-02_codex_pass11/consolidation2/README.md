# Codex pass 11, consolidation 2: 10 rows landed, 3 respelled (2026-11-03)

`verify.log`: 11 WINS, all RELOCATION_MATCH + PASS.

**New rows (10):**
- The 8 CThing-family verified rows from Codex pass 11:
  - 0x0047A9F0 CHeroLogBook::AddEntry
  - 0x00606300 CTCInventoryMap::OnSerialise
  - 0x00666EF0 CThingCreatureBase::AutoPlaceNearBuilding
  - 0x00667450 CThingCreatureBase::ReactToGenericStrike
  - 0x00667D30 CThingCreatureBase::OnHit
  - 0x007355F0 CTCCombat::SetCombatType
  - 0x0074A5D0 NCreatureHiding::GetValidPositions
  - 0x00775C60 CTCShop::OnSerialise
- Retry agent a's 2 matches:
  - 0x00520670 CThingManager::LoadHeroStateBinary
  - 0x00521620 NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary

**Respelled landed rows (3):** the STLport `vector<CPersistedDeadEntity>::_M_insert_overflow` 0x0052B030 and its helpers 0x005269B0 / 0x005269F0 (`respell_005269b0_005269f0/`, both MATCH).
- They were named over a descriptive `CUnprovenElement` element.
- 0x00521620 proves the element type, because it calls 0x005269B0 for CPersistedDeadEntity.
- The three now use one instantiation with no pointer casts.

**Headers (`HEADERS_README.md`, `hdrcheck.txt`):**
- Every changed header reads default=same.
- CGameComponent / CMainGameComponent are folded into `fable_game.h` as the FABLE_RETAIL_CGAMECOMPONENT / FABLE_RETAIL_CMAINGAMECOMPONENT branches.
- GFMaths.h (an unevidenced `__forceinline`) and CCRC.h were included by nothing, so they were not landed.

**Includer rechecks:**
- 994/994 on the overlay (agent).
- 154/154 on the landed headers (coordinator, `includer_recheck_grades.tsv`): every definer of a changed macro or includer of fable_game.h / fable_map.h / scoped_ptr.h, plus 80 sampled CThing / CDefPointer includers.

**Names:**
- `SECOND_SPELLINGS.tsv` and `NAME_FIXES_retry_a.tsv` are queued PRIORITY in NAME_FIXES_QUEUE.tsv (prefix cx11c).
- Labels: `ghidra_out/labels_codex_pass11e_2026-11-03.tsv`.
