# Codex pass 11 return -- wave 1: 17 of 100 landed (2026-11-02)

Outside Codex pass over the 100 never-attempted rows reserved in `pending/2026-11-02_codex_pass11/` (brief README_CODEX.md there). The return
(`FableDecomp_pass11_Claude_HANDOFF.zip`, 78 MB, owner's Drive copy; SHA256SUMS verified) reports 54 VERIFIED (49 RELOCATION_MATCH + 5 MATCH), 38
parked on code residue, 1 compliance hold, 2 naming holds (00520670 / 00521620: callee 0x00431143 unproven), 1 extent fix (0099dcb0 is 1455 bytes,
not 1454) and 4 pre-authoring blockers (STATUS_codex.tsv, ROW_DETAILS_codex.md, START_HERE_codex.md).

Regrade on our lane (own oracle from our exe; read-only vland.py, each row in its own returned header context `rows/<addr>/selected_context/hdr`,
44 contexts; `regrade_grades.tsv`): 53 of the 54 byte-exact (5 MATCH + 48 RELOCATION_MATCH). 0x00C5FA50 CImaAdpcmCodec::Convert does not compile on
our lane: its header packs through the SDK's `<pshpack1.h>` (no new packing directive; held). Six tests fail to build on our host lane only because
the transform drops lines containing `static_assert`, which removes the second line of Codex's FABLE_STATIC_ASSERT macro (pass 10 precedent: rename
the token in the test).

Header needs decide the landing order (`row_header_needs.tsv`): 7 rows need no project header, 16 need only new header files, 31 edit existing
shared headers (77 of them; plus 177 new files). The edits use per-feature macros (FABLE_RETAIL_FACTIONS, FABLE_RETAIL_NAVIGATION_UPDATE, ...)
rather than one FABLE_RETAIL_<CLASS> per class, add a second CCharString model (engine/CCharString.h beside fable_string.h), were cut from the
headers as they were before chunks 709-717, and 15 headers exist in two to four versions across rows.

Wave 1 (this commit; `verify.log`: 17 WINS = 5 MATCH + 12 RELOCATION_MATCH, all tests PASS, bindings multi-check 0): the 7 header-free vendor rows
(zlib 1.2.1 Z_PREFIX z_deflate 0x00C006A0, send_tree 0x00C04340, compress_block 0x00C04B70, inflate_fast 0x00C06060; LZO 1.08
lzo1x_999_compress_internal 0x00C07AF0; libvorbis floor1_fit 0x00CA26C0; md5_process 0x00A3D5B0) and the 10 vendor rows whose new headers are
single-version and use no feature macros: EAX CEaxContext / CEaxSource (0x00C68F90, 0x00C65A00, 0x00C67F60, 0x00C68510; new engine/CEaxContext.h,
CEaxSource.h, CPKObject.h, EAX40.h), GameCODA CSalIIRFilter::Process x2 (0x00C6C530, 0x00C6C0F0; engine/CSalIIRFilter.h), libpk CString::FormatV
0x00C0D1B0 (engine/CString.h), DirectShow CBaseVideoRenderer::ShouldDrawSampleNow 0x00CA5850 (engine/CBaseVideoRenderer.h, sizeof 0x160) and DXUT
ImeUi GetReadingString 0x00CB4340 / DrawImeIndicator 0x00CB2F80 (engine/ImeUi.h, ImeUiInputContext.h). New files only: no landed row includes them.

Naming (`AUDIT.tsv`, every leaf and relocation symbol of the 54 against the PDB name sets, ESTABLISHED_SPELLINGS.tsv and the queue's wrong
spellings): no clash with an established spelling and no reintroduced wrong spelling. Vendor names have no PDB: they rest on the vendor sources
(zlib / LZO / libvorbis / md5 / DirectShow strmbase / DXUT ImeUi / GameCODA release objects) and two EAX dispatchers carry descriptive names with the
address suffix. Wave 1 gives 25 addresses a second spelling against older landed stand-ins (the EAX struct tags `_EAXVECTOR` vs `EAXVECTOR`, the
CEntity_Set*ComponentsProperty labels on CEaxSource send setters, `_deflate`, the GameCODA CBlockFilter / CBlockEffect stand-ins, ImeUi globals):
queued PRIORITY as `cx11` in NAME_FIXES_QUEUE.tsv. Codex's 420 NAME_FIXES rows are kept unreviewed in NAME_FIXES_codex.tsv for the naming wave.
Labels: ghidra_out/labels_codex_pass11_2026-11-02.tsv.

Remaining 37 verified rows: sources, tests and naming evidence in `pending/2026-11-02_codex_pass11/verified_pending/`; their header contexts are in the
owner's return zip. They land after the header consolidation (one FABLE_RETAIL_<CLASS> branch per class on the current headers, default paths
proven unchanged, includers rechecked). The 46 unverified rows return to the pool after 2026-11-09.
