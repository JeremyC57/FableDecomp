# Naming pass wave 2, agent A (2026-10-28/29): eight families, 495 rows, 721 files

Applied with the idempotent scripts in `scripts/` (run from the repo root: `python3 fam_X.py $(cat fam_X.files.txt)`;
they need `famlib.py` and `fam_getter_common.py` beside them; forward and reverse order give identical trees). Agent
recheck on the tree with wave 2B already in: 495 of 495 MATCH / RELOCATION_MATCH with tests PASS; the repo after applying
is byte-identical to that overlay; coordinator sample recheck 150 of 150 (recheck_sample.log).

| Script | Retail symbol | Rows |
|---|---|---|
| fam_getframe | 0x0049D870 `?GetFrame@CWorld@@SIJXZ` (was GFGetCurrentFrame and ~57 other spellings) | 181 |
| fam_cbase_ns / fam_cbase_global | 0x0099A2D0 `??0CBaseClass@@QAE@XZ`, 0x0099A2E0 `??1CBaseClass@@UAE@XZ`, 0x0099A2F0 `??0CBaseClassNonCopyable@@IAE@XZ`, 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ` (CBase placeholder roots) | 130 + 84 |
| fam_ccrc | 0x004014A0 `?Calc@CCRC@@SIKKPBXK@Z` | 17 |
| fam_uimanager | 0x0041E5F2 `?GetInstance@CManager@NUISystem@@SIPAV12@XZ` (cdecl -> fastcall static) | 11 |
| fam_profilemgr | 0x0040D2A0 `?Get@CUserProfileManager@@SIAAV1@XZ` | 36 |
| fam_defmgr | 0x0044C6B0 `?Get@CGameDefinitionManager@@SIAAV1@XZ` | 32 |
| fam_rwsco_private | 0x00993CA0 (local CDataInputStream models declaring it public -> private) | 9 |

Bindings: 45 old spellings retired (no emitter left; every PDB spelling was already bound); 6 kept (BINDINGS.tsv KEEP rows,
still emitted by deferred rows). Labels: `ghidra_out/labels_naming_wave2a_2026-10-29.tsv`. Header items (HEADERS.md):
`engine/CDataInputStream.h` private ReadWithSrcChunkOverflow (36 rows), `engine/CWorld.h` static GetFrame (1 row).
Deferred (DEFERRED.tsv, 579 row x target entries): mostly CBase roots with placeholder virtuals or no vptr (315), the
CFrontEndManager models of 0x0041E5F2 (133), other-class GetFrame models (46).
