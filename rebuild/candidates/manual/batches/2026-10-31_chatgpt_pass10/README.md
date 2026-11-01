# ChatGPT pass 10 return -- 57 of 100 landed (2026-11-01)

Outside ChatGPT pass over the 100 parked rows reserved in `pending/2026-10-31_chatgpt_pass10/` (brief README_CHATGPT.md there; package
SHA256SUMS verified). ChatGPT reported 57 VERIFIED_MATCH, 31 PARKED, 12 BLOCKED. Regraded on our lane like the Codex pass: own oracle from
our exe (`oracle.tsv`), read-only vland.py regrade (`regrade.log`: 57/57 = 2 MATCH + 55 RELOCATION_MATCH, all tests PASS after one
test-only fix: our host test transform drops any line containing `static_assert`, which deleted the package's one-line FABLE_STATIC_ASSERT
macro, so the typedef token `fable_static_assert_` was renamed `fable_sassert_` in the tests), then `vland.py --land` (`verify.log`,
57 WINS, bindings multi-check 0). Per-row flags from ChatGPT's STATUS (`/Gr`, `/O1 /Oy-`, `/Oy-`) recorded in build_candidates.ps1.

Naming was audited symbol by symbol before landing (`audit/REPORT.md`, `audit/AUDIT.tsv`: 57 leaves + 294 bindings + 181 NAME_FIXES):
no wrong leaf or binding; 45 rows fully two-source proven; 12 rows carry data / SDK names resting on ChatGPT's raw-PDB reading or the
GameCODA SDK source with the address identity proven. Decisions (coordinator under the owner delegation 2026-11-01, "do what retail did"):
- single-source raw-PDB data names with proven identity land (Codex precedent); the GameCODA SDK names (00c35e30 `CODA_SourceSetPlayCursor`,
  CSalBufferBase::GetDesc, CCodaContext::SetError, CCodaSource::SetPlayCursor, the `CODA` global) land as the vendor source's names
  (owner 2026-11-01: CODA is ordinary reconstruction from the vendor SDK; the archived SDK is 2.10, retail is a Fable 1.5 fork);
- established-spelling clashes: 0x013B86A0 -> `?PMainGameComponent@CMainGameComponent@@1PAV1@A` (two independent PDB readings + landed
  bindings), 0x0143E8F8 -> `?pDSTGame@@3PAVCGameScriptInterfaceBase@@A` (SetDSTGame proves the type and name), 0x0143E90C -> the const
  `$$CB` form; 0x0129BA3C stays EPSILON and 0x013B8390 stays PDisplayManager (ESTABLISHED_SPELLINGS.tsv updated);
- 0x013B89FC CQuestManager PClass: two raw-PDB readings say private (`0V`); the landed 005fd410 (chunk 674) binds `2V` -> naming queue.
ChatGPT's 181 NAME_FIXES: 155 ACCEPT queued as `cg10` (NAME_FIXES_QUEUE.tsv), 21 UNPROVEN + 5 CONFLICT queued as not-applied with the verdict.
55 catalog labels proven wrong applied (`ghidra_out/labels_chatgpt_pass10_2026-11-01.tsv`). The audit's `NAMING_QUEUE.tsv` lists 1,236
leftover stand-in spellings at 170 addresses with their binders (naming waves 17+). The 43 unmatched rows: ChatGPT's best sources in
`pending/2026-10-31_chatgpt_pass10_differ/differ/`; retried on our lane as chunk 680 (owner-allowed `volatile` for the CFileInstaller rows,
manifest splits for the defective extents such as 00854b90's 21-byte span over a 566-byte body). RESERVED.tsv -> RETURNED.tsv.
Lessons: `LESSONS_chatgpt.md`.
