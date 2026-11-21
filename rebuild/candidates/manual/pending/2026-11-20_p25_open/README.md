# Account B, 2026-11-20: open rows from the p25 run (best sources and notes)

Rows Account B worked on 2026-11-20 and could not match under the rules. Each directory has the best source (`<addr>.cpp`),
the author's notes, and, where the source needs header opt-ins, `hdr.diff` (add-only against the header at commit time).
Retail bytes, oracles and PDB dumps are left out. The FINDINGS_LOG.md entries of 2026-11-20 hold each row's measurements; where an
entry cites "Account B scratch w26/...", the files are here. Rulings: docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md.

| row | function | best grade | what is left |
|---|---|---|---|
| 0x0064FDB4 | CQuestInfoElementBar::DrawAt | DIFFER(2593v2593) | frame 0x164 v 0x16C: one never-addressed 8-byte slot in retail |
| 0x00D01630 | CQ_BanditCampScript::CGate1GuardOuter::Main | DIFFER(5161v5178) | movie dtor expanded at all 34 sites; needs a forced parent Ego_d does not have (S15 + auto_inline(off) round ~CTimer in this best) (the T36..T50 window from 0x00EFA4F0 was tested 2026-11-20: negative, no flag-only route; FINDINGS addendum) |
| 0x00B0DC20 | NParticleEngine::CPSCUpdateNormal::Update | DIFFER(5604v5602) | SetOrientation copy folded to [esi+4] with no earlier same-member read (/d2inlT200) |
| 0x00C73050 | Audio_PlaySourceAndProcessQueue (CAudioSource stream update) | DIFFER(672v672) | tail-item temp edi v retail edx: c2 masks it to callee-saved when a per-tuple counter hits 3 at `!sound->IsLooping()` (p25N/NOTES.txt, hook c2prio_x.py); 8 targeted respellings negative (/d2inlT150) |
| 0x00A4EFC0 | C3DAnimationSequenceData::InitialiseFromFileData | DIFFER(3491v3491) | stock CARRAY_INDEX_ASSERT + ASSERT_DEAD_ARM plus an Ego_d-shaped AllocateType with a stock arm at its 261-B line (p25P, Account B scratch w26/p25P r5/best_nb.cpp, overlay hdr6): one residue left, the position compressed-size schedule at 0xA4F7BE..0xA4F82F |
| 0x005BFD5E | CTCInventoryBase::OnSerialise | DIFFER(425v425) | callee-saved register rotation only; not a symbol tie (symlog: dummies renumber the pool, bytes never move); parked for good |
| 0x00833C00 | CThingAICreature::DrawPreparePrimitivesForRendering | DIFFER(2296v2300) | retail keeps `info` in ebp and re-reads info.Engine after each primitive temporary; ours spills it; frame one dword off |
| 0x007602F0 | NShape::IsShapeConvex | DIFFER(1126v1126) | epilogue pop / fadd order; flips only with a float local Ego_d does not list (bracket_closing_angle.cpp); closing2 fixed with a named Ego_d iterator (p25O) |
| 0x00763060 | NShape::FractureShape | DIFFER(1132v1146) | find_if start increment at the loop top v retail peeled into the preheader; only texts that contradict Ego_d reach it (fitted match kept as evidence/bracket_match_anchor.cpp); helpers 0x00760020 / 0x00760190 landed |
| 0x008E18D0 | NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams | DIFFER(2045v2073) | loop-2 and sky-block LowerBound sites have the same instantiation, f and score but opposite retail decisions; callee texts proven (fastcall STL uninitialized helpers needed); no T matches (best T64..66 2149); the do{}while(0) version is a device |

All stay `reserved (acctB ...)` in UNMATCHED_ROWS.tsv for a return pass. NAME_FIXES found on the way are in the coordination log.
