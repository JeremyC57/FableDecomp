# Naming pass wave 3 (2026-10-29): deferred tails, 461 rows, 527 files, two header members

Agent recheck of the composed tree: 715 of 715 rows MATCH / RELOCATION_MATCH with tests PASS (461 edited + 254 whose only
change is an included header); every includer of the two changed headers (292) rechecked with no byte change.
Coordinator: repo after applying is byte-identical to the agent's composed overlay; sample recheck 200 of 200 (150 edited
rows + 50 header-only includers, recheck_sample.log).

| Script | Retail symbol | Rows |
|---|---|---|
| fam_rwsco_hdr | 0x00993CA0 private `CDataInputStream::ReadWithSrcChunkOverflow` via the header's friend hook | 35 |
| fam_cbase_ph / fam_cbase_nsph / fam_cbase_alias | 0x0099A2D0 / 2E0 / 2F0 / 300 CBaseClass / CBaseClassNonCopyable roots (placeholder `v00` -> virtual dtor, `// Leaf:` lines added) | 192 / 35 / 40 |
| fam_getframe_other (+ fam_hand) | 0x0049D870 `?GetFrame@CWorld@@SIJXZ` from stand-in classes | 43 (+3) |
| fam_uimgr | 0x0041E5F2 `NUISystem::CManager::GetInstance` (file-scope CFrontEndManager models) | 45 |
| fam_cdefptr | 0x009AD9E0 `?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z` | 69 |

Headers: `engine/CDataInputStream.h` gains the private member plus an opt-in friend hook
(`FABLE_CDATAINPUTSTREAM_FRIEND`), `engine/CWorld.h` gains `static long __fastcall GetFrame();`; the generator change is
`gen_class_headers.patch` (a `HEADER_MEMBERS` table), applied to `tools/decomp_pipeline/gen_class_headers.py`. The
CScriptThing virtual-dtor header item was dropped (52 of 384 includers break).

Bindings: 67 old spellings retired, 11 new spellings bound to the old rows' targets, 5 kept (BINDINGS.tsv). Catalog label
fixes for the next catalog sweep (LABELS.tsv): 0x0099A300 is `CBaseClassNonCopyable::~CBaseClassNonCopyable`, 0x0041E5F2
is `NUISystem::CManager::GetInstance`. None of the 51 probable decisions found a second source. Left: DEFERRED.tsv (148).
