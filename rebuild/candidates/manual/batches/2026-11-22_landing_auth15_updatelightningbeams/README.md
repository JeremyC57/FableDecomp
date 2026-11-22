# Landing auth15 (Account A, 2026-11-22): 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (1 row, 1 header opt-in)

AUTH9 -> AUTH10 -> AUTH11 -> AUTH14 -> AUTH15, with three advisor rulings. Hand-backs: `pending/2026-11-22_auth14/008e18d0/README.md` (the
match), `pending/2026-11-22_auth15/008e18d0/` (NOTES, evidence, proof logs).
- **Grade:** RELOCATION_MATCH (2073 B), stock VC7.1 at `/Gr` and the DEFAULT inline threshold, no hook, no forcing. Test PASS.
  LANDVERIFY_STRICT_HEADERS=1 dry run clean; the only warnings are 2 RELOC_NAME on demangled manifest labels for std::sort 0x0076C000 /
  0x0076C1A0 (targets right).
- **Flags** `/Gr`: creature_modes.obj is -Gr (Ego_d's carved S_ENVBLOCK and FableWin module 212). Earlier `/d2inlT65` drafts rested on a
  wrong premise: AUTH10's 2044v2073 baseline was taken without the header overlay.
- **Text:** Ego_d-corrected by AUTH11 (C1-C4: GetParentThing reads, storm-rotation lines, `rhs` before `weapon_pos`, the VC6 for-scope
  idiom). AUTH11's GetTC wrapper (C5) changes no bytes and is left out.
  - Locals equal Ego_d per block, with names and types (function 12, loop-1 6, loop-2 3, sky 5).
  - Remaining Ego_d differences, with reasons, are in NOTES: layout +3 at 3279 / 3305, the four IsDebuggerPresent assert lines, and the
    +10-B prolog (frame about 552 B, so not __chkstk).
- **Header:** engine/NParticleEngine_CParticleUpdateParameters.h, add-only `FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR`.
  - All three PDBs list the dtor as public virtual compiler-generated.
  - pin_asserts 0, assert_names_check 0; equiv_check: 0 other rows differ.
  - With it defined, all 22 landed callers of 0x006E2500, its own row and sibling 0x007A1380 still grade RELOCATION_MATCH. 12 of the 14
    live ones emit a dtor copy equal to 0x006E2500. The two size TUs 0x007BE3E0 / 0x007BE4E0 would not (33 B v 73 B), but they stay on
    the default path; queued.
- **Pick-any:** leafrel 0 problems. ~CParticleUpdateParameters = 0x006E2500, _M_insert_overflow = 0x0076ADA0, LowerBound = 0x0040F020.
- **Recheck** (`logs/recheck.log`): 70 rows = this row, the landed rows of the creature_modes range 0x008DD000-0x008E3000, 0x007A1380,
  0x006E2500, and the 21 other landed callers of 0x006E2500. 6 MATCH + 64 RELOCATION_MATCH, all PASS.
- **Queued** (tag land1122auth15): the two size TUs; the TU's flags (0x008DD7E0 also matches at /Gr; 0x008DDAD0 is at /d2inlT65).
