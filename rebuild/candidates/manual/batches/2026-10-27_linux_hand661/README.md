# Chunk 661 -- 10 of 10 landed (2026-11-01, NA006 of the never-attempted pool: landed-twin rows)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA006). Landed via `coordinator/land_chunk.sh 661` (vland --land 10/10,
9 RELOCATION_MATCH + 1 MATCH, PASS, multi-check 0; 9 rows first probe): the dynamic initializers 0x0121A670 CThing::TCsToBeRemoved,
0x0121AC08 REACTION_MANAGER_DEF_INSTANTIATION_NAME, 0x0121AE9C TITLE_NAME ("EXPRESSIONS TITLE"), 0x0121F074 STrapCycle (all .CRT$XCU
entries, compiler code, no library grade); the ??1 / _Destroy members of four triples 0x004B6A80 ~pair<CCharString,CPersistInfo>,
0x006FB8D0 _Destroy<pair<CCharString,CSymbolInfo>>, 0x007803D0 NFire::~CFirePoint, 0x00768BF0 ~AttackParticleAndEndPoints;
0x0067C370 ~CBlurredAttitudesDef; 0x00C5EE20 the silent audio output's Uninitialize (descriptive, MATCH). No row parked.
51 NAME_FIXES queued (c661; the 41 `g_CCharString_<addr>` anonymous-namespace globals respell under the lane's TU hash per policy).
Labels: ghidra_out/labels_chunk661_2026-10-27.tsv (20). Decisions at landing (owner delegation 2026-11-01): engine/CParentDefClassBase.h
gets an implicit destructor in migration wave 10 (retail inlines it into derived implicit dtors; recheck the 0x00430300 / 0x004303F0
callers then) -- until then 0x0067C370 keeps its local CParentDefClassBase on the engine/CDefClassBase.h chain; the audio slot-5
spellings unify on ReleaseBuffer in wave 15. Unproven: the TCsToBeRemoved element type (retail never reads the tree; CUnproven_00527950
placeholders). Lessons in GOTCHAS (1 line) and LESSONS.md.
