# acctC workflow hand-back wfA1 (unworked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00DFCA90 | NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main | 373 | RELOCATION_MATCH (retail_asm via the shared GFFloatToLongCeil helper) | PASS |

Verified twice: by the workflow's independent verifier and by the coordinator (`coordverify.log`, read-only vland, strict headers).
Catalog length 2,928 was wrong: the function is 373 bytes. WARN RELOC_ALIAS on the `_rand` import thunk, as on landed rows.
Needs two additive opt-ins (`hdrdiff/`; evidence in HDR_README.md).

`open/`: 006d0830 CTCWife::GlobalUpdate DIFFER(2581v2584); 006b2ca0 UpdateSunAndMoonPositions DIFFER(908v910);
00682810 InformOfDeed (stale draft, not ported); `00c271f0_HEADER_POLICY.cpp` libpk CPhysicalPlayer::Play: bytes match
(846) but strict-header vland REJECTS it (local `class C3DVector` although engine/C3DVector.h exists): needs respelling.
libpk naming conflicts for the lander are in NAME_FIXES.tsv (CPhysicalPlayer v CCodaPlayback, 0x00C35DD0 bound under two names).
