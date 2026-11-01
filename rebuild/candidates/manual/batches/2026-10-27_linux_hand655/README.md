# Chunk 655 -- 10 of 10 landed (2026-11-01): NA001 of the never-attempted pool, rows with a landed masked-body twin

First cut from `pending/never_attempted/CHUNKS.tsv` (analyze_never.py, kind twin_landed): ten never-authored rows whose masked body
equals a landed row. The agent started from the twin's source (`twin/` in the agent dir) and settled each row's own identity from
its caller chain first; every row then matched in 1-2 probes (the 12-probe rule never applied). Landed via `land_chunk.sh 655`
(vland --land 10/10, multi-check 0). Every manifest label was a propagated wrong label; the proven names are in
ghidra_out/labels_chunk655_2026-10-27.tsv (12) and the NAME_FIXES (19 rows, c655) carry the stand-ins to respell.
Rows: 0x004B9E20 std::find<CCharString*,CCharString>; 0x00A95890 / 0x00B96150 std::uninitialized_fill_n over
CCountedPointer<C3DGroup2> / CCountedPointer<CEngineSceneGrid> (MATCH); 0x00E1DF60 / 0x00E38DD0 CPersistContext::Transfer<...> of two
quest-script enums; 0x00CDCF80 TransferVector<bool>; 0x009D8250 NProfileTimer::EndProfile (MATCH); 0x00ADCA30
std::vector<CVertexGroupBlock>::operator= (STLport _vector.c verbatim, /Gr); 0x00CA9710 DirectShow CDynamicOutputPin::Release (MATCH;
the SHARED_HEADER warning for CBaseObject is the DirectShow class, not the engine one); 0x00771F30 compiler-generated ~CTCHeroPodium.
Review at landing: 0x00A97F70 vs 0x00B97E90 `_M_fill_insert` access (public vs protected; check STLport 4.6 _vector.h); the 27
stand-in bindings on 0x00404500 CPersistContext::CheckCRC; the N00a97f70 stand-in family; 0x00CA5510 `?Release@CBasePin@@UAGKXZ`
tentative (the concrete owner of table 0x012BF3C0 unproven). Lessons in GOTCHAS (1 line) and LESSONS.md.
