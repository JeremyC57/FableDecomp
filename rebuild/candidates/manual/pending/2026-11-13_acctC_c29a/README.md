# acctC hand-back c29a (agentC29, 2026-11-14)

| address | name | grade | test |
|---|---|---|---|
| 0x0074E860 | CTCContainerRewardHero::GetRewardItems | RELOCATION_MATCH | PASS |
| 0x00945060 | CAIStateGroup_WorkHammerAnvil::Update | RELOCATION_MATCH | PASS |
| 0x00944940 | CAIStateGroup_WorkSawingWood::Update | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Plain C++; additive header opt-ins in `hdrdiff/` (evidence in HDR_README.md).

Review items for the owner / coordinator:
- The two state-group Updates model `this` with a local class derived from the shared `CAIStateGroup_WorkBase<CAIStateGroupBase>`
  (pack(1) members at the Ego_r offsets), because the generated flat `engine/CAIStateGroup_Work*.h` cannot declare the
  protected virtual Update. Precedent: landed GoToPub::ThankBarmaid. Alternative: turn those headers into real classes.
- 0x0074E860 models CContainerRewardHeroDef / CObjectFamilyDef locally (their engine headers are quarantined).
- SawingWood keeps four calls whose results retail discards, as bare statements with the retail addresses cited.
- 0x0074FB20 is `vector<pair<long,CCharString>>::erase` (catalog: std::vector_CopyAssignSegment); 0x0074CBC0 spelt
  `std::vector<CDefIndex>::at const` (no PDB name; landed row uses a made-up class name): NAME_FIXES.tsv.
- Reserved row 0x00537E70 is NOT a function start: it is the tail of NUISystem::CList::ComputeChildrenVisibility, which
  starts at 0x005379F5 (2,102 bytes; a draft exists in pending/2026-11-03_linux_hand726/differ/005379f5.cpp). Needs re-cutting.

`open/00735fd0.cpp`: CTCCombat::AttemptToStartParticle DIFFER(993v973) (successor agentC30).
