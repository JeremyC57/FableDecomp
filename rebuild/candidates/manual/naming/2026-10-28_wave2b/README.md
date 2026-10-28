# Naming pass wave 2, agent B (2026-10-28): five families, 1,064 rows, 1,297 files

Applied with the idempotent scripts in `scripts/` (each on its own verified `.files.txt`; composing them in either order
gives identical files). Agent recheck: 1,064 of 1,064 rows MATCH / RELOCATION_MATCH with tests PASS on the composed
overlay; the repo after applying is byte-identical to that overlay; coordinator sample recheck 150 of 150
(recheck_sample.log).

| Family | Retail symbol | Rows | Change |
|---|---|---|---|
| CheckCRC protected | 0x00404500 `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 143 | public -> protected (QAE -> IAE) |
| CCharString(const char*, long) | 0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z` | 227 | `int` -> `long` length (PBDH -> PBDJ) |
| AutoTiny helpers | 0x004AA840 `??1CScriptThing@@UAE@XZ` | 158 | extern placeholder helpers -> explicit `~CScriptThing` call (same call bytes) |
| ~CScriptThing virtual | 0x004AA840 | 75 | `virtual` where the model already has a vptr |
| ~CBaseIntelligentPointer virtual | 0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ` | 463 | `virtual` (and an explicit `_vptr` member dropped in 11 rows) |

Five stale bindings retired (no emitter left): `?CheckCRC@CPersistContext@@QAEXPBD@Z`, `??0CCharString@@QAE@PBDH@Z`, and
the three AutoTiny placeholder spellings. Kept: the QAE `~CBaseIntelligentPointer` / `~CScriptThing` spellings (deferred
models still emit them) and the CInterfaceMap LowerBound spellings.

Reverted during verification (not applied): 22 + 3 ~CBaseIntelligentPointer explicit-vptr rows, 4 ~CScriptThing rows
(DEFERRED.tsv). Left: LowerBound 0x0040F020 / 0x004365B0 (script ready, 103-row pilot 103/103, ~1,029 files),
GetPDefFromGlobalIndex 0x009AD9E0 (CDefPointer model rework), HEADERS.md (`engine/CScriptThing.h` lacks the virtual dtor).
