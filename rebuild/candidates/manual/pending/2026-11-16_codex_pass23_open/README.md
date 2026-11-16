# Codex pass 23: rows not landed (2026-11-16)

From the Codex pass 23 hand-back (`FableDecomp_pass23_handoff_2026-11-16.zip`). Each folder holds the row-root source and test,
PROBES.md, NAMING_EVIDENCE.md and the row's NAME_FIXES / LESSONS / BOUNDS_FIX where present (no history). The headers these
drafts need are already merged into `rebuild/include` (landing p23).

| rows | state | what remains |
|---|---|---|
| 0x007D00F0 | PARKED 1016/1016 | frame 0x28 v 0x2c and local-slot placement |
| 0x0089C910 | PARKED, 974 v 976 code bytes | `fcompp` v `fcomp st(1)` + `fstp st(0)` |
| 0x012152D0 (parent 0x00C425E0 DecodeXboxAdpcm) | PARKED 221 v 217 | extra receiver lea, scratch / address-load order |
| 0x012117A5, 0x012117AD (parent 0x00C12E50 CSoundEventManager::PlayStreamed) | PARKED 741/741 | one offset-store permutation (13 bytes, positions 477-490) |
| 0x012139DB, 0x012139E3, 0x012139EB, 0x012139F6 (parent 0x00C2CBD0 CEventScript ctor) | PARKED 132/132 | one owner-load permutation (10 bytes) |
| 0x00C29A3C, 0x00C29A30 (parent 0x00C298D0 vector::_Insert_n) | RELOCATION_MATCH for Codex; not landed | needs VC7.1's Dinkumware `<vector>` ahead of the repo STL; the harness has no per-row include order (same as landing gb) |
| 0x01210E95 (parent 0x00C0AF90) | RELOCATION_MATCH for Codex; naming-blocked | `CUnprovenAudioChannelBank` / `CUnprovenAngularChannel` placeholder names (CLAUDE.md: no Unproven placeholders in landed code) |
| 0x01212472 (parent 0x00C1C460) | RELOCATION_MATCH for Codex; naming-blocked | `CUnprovenSoundWorld_00c1c460` and three address-suffixed helpers |
