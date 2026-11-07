# Landing 2026-11-07x: authoring chunk 849 (6 of 6 matched: 5 landed, 1 held for a naming decision)

All six are held Codex pass-12 / 13 RELOCATION_MATCH sources rebased onto our headers with additive opt-ins.

| Row | Grade | Note |
|---|---|---|
| 0x00501450 CWorldMap::GenerateRegionConnectivityGraph | **RELOCATION_MATCH + PASS, landed** | 11 attempts: out-of-line HasTC operator(), visible STLport insert_unique, a named `CPlayer& player` local (shaping lever, noted) |
| 0x0058E99F CTCInventory::OpenInventoryShortcut | **RELOCATION_MATCH + PASS, landed** | `/O1 /Oy-`; FABLE_RETAIL_CTCINVENTORY branch |
| 0x0041D21B NUISystem::CManager::CreateComponent | **RELOCATION_MATCH + PASS, landed** | `/Os /Oy-`; eight NUISystem `*_MEMBERS` opt-ins + five new NUISystem headers for retail's allocation sizes |
| 0x009CF3A0 CBankFile::WriteSymbolHeaderFile | **RELOCATION_MATCH + PASS, landed** | CBankFile row-local (engine/CBankFile.h is on hold; header debt noted) |
| 0x0057E2F1 CTCHeroStats::OnSerialise | **RELOCATION_MATCH + PASS, landed** | `/O1 /Oy- /Oi`; CTCHEROSTATS_SERIALISE branch; STLport list::size() as distance(begin(), end()) |
| 0x00AE2E20 CParticleProgram::OptimiseSpringOrder | RELOCATION_MATCH + PASS, **held** | binds `__destroy_aux<set<unsigned long>*>` at 0x00AE76E0, already bound at the twin 0x0051AC70 by landed 0x0051C470 (`pending/2026-11-07_linux_hand849/held/`) |

- **Headers:** 30 headers (18 edited with additive opt-ins, 12 new); default paths textually unchanged; the agent rechecked 40 landed
  includers (all RELOCATION_MATCH). The CArray.h PUSHBACK_FASTPATH opt-in waits with the held row.
- **Names:** labels `labels_landing_2026-11-07x.tsv` (0x0058A763 is OpenPCStatusMenu, not OpenPCSkillsMenu; placeholders and STLport callees).
  Queue `land0711x`: seven respellings of landed bindings plus 0x00AE7880 (CArray<set<unsigned long> > ctor v the landed vector<set<long> > name, REVIEW).
