# Naming pass wave 4, agent B (2026-10-29): remaining tails, 520 rows, 582 files

Agent recheck on the tree with wave 4A in: 520 of 520 MATCH / RELOCATION_MATCH with tests PASS; the repo after applying
is byte-identical to that overlay; coordinator sample recheck 150 of 150 (recheck_sample.log).

| Script | Retail symbol | Rows |
|---|---|---|
| fam_lb4 | 0x0040F020 / 0x004365B0 `CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound` (non-canonical map shapes) | 404 |
| fam_uimgr4 | 0x0041E5F2 `NUISystem::CManager::GetInstance` (stand-in classes; also `ScrollMenu@CManager`) | 70 |
| fam_getmgr4 | 0x0044C6B0 `CGameDefinitionManager::Get`, 0x0040D2A0 `CUserProfileManager::Get` | 27 |
| fam_cbase_g4 + hand/ | CBaseClass / CBaseClassNonCopyable roots | 3 + 14 |
| fam_gpd4 | 0x009ADE10 `CDefinitionManager::GetPDefFromClassIndex(const CCharString&, long) const` (catalog says GetPDefFromGlobalIndex); 0x009AD9E0 const form | 9 |

Bindings: 55 old spellings retired (incl. three anonymous-namespace hashes that change with file text), 4 new spellings
bound; KEEP rows in BINDINGS.tsv. Labels: `ghidra_out/labels_naming_wave4b_2026-10-29.tsv` (0x009ADE10
GetPDefFromClassIndex, 0x009ADA10 protected GetPDefFromGlobalIndex). Reverted in verification: LowerBound 0x00692F30
(DIFFER) and four rows whose unedited tests stub the old LowerBound. Left: DEFERRED.tsv (512 rows with reasons; mostly
LowerBound stand-ins under other class names).
