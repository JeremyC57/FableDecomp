# Landing 2026-11-07u: Fable expert retry chunk 844 (4 of 11 landed, 7 parked)

Retries of the 11 rows chunks 839 / 840 parked, 50+ attempts per row where needed.

| Row | Grade | Note |
|---|---|---|
| 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType | **MATCH + PASS, landed** | STLport `_List_iterator(const iterator&)` user copy ctor: the by-value iterator leaves retail's first-return `xor eax,eax` ahead of the pop |
| 0x0073ACC0 CTCCreatureModeManager::AddMode | **RELOCATION_MATCH + PASS, landed** | find_if over a list iterator with a user copy ctor; 0x007421C0 is push_front (chunk 840 said push_back; test corrected) |
| 0x007534B0 CTCLook::IsPosInViewRange | **RELOCATION_MATCH + PASS, landed** | three separate `return false;` give retail's shared `xor al,al`; semantics corrected (retail returns false for a null or dead creature); respelled at landing to the PDB `GFGetSignedAngleDifference` (rechecked) |
| 0x00E482F0 CV_BanditTollScript::CheckForLeaderDead | **RELOCATION_MATCH + PASS, landed** | single-site `do { } while (0);` (line-2 `// Inline-split:` note): VC7.1 inlines loop-nested sites first, so the budget-limited operator= expands at that site only |
| 0x00C52BB0 GetAMDModelName | DIFFER(140v140), 4 | eax / ecx register tie on the family-5 Model load |
| 0x0058301A CreateEditBoxActions | DIFFER(321v321), 2 | address-taken store ahead of a call-result-pointer load |
| 0x0047ED76 SetToSave | DIFFER(366v366), 2 | `fsubp` / `push ecx` scheduler tie |
| 0x00E76E90 HeroFlashesLamp | DIFFER(1613v1612), 10 | return-block order in the second inlined recursion copy |
| 0x00DE4710 DisappearFollowers | DIFFER, 473 | per-exit vector-dtor split |
| 0x00775350 UpdateStock | DIFFER, 791 | function-wide inline budget |
| 0x00D0EE70 CheckForFirstAreaMassacre | DIFFER(1991v1989), 466 (was 640) | per-exit dtor split; declared-only vector dtor specialisations make every exit a call |

- **Names:** all 11 proven. Labels `labels_landing_2026-11-07u.tsv`: 0x0070B470 CTCTurnSource::GetState (descriptive; catalog CWorld::DrawGetEnvironment
  was a propagated wrong label), 0x00CD319B the vector<STB> dtor. Queue (`land0711u`): 0x007421C0 (the landed 0x0073ACC0 binds a stand-in
  CModeList::push_front until the STLport list is modelled), 0x00A441E0 (the PDB has only GFGetSignedAngleDifference; 0x006650E0 still binds the
  invented `?AngleDifference@@YGMMM@Z`; the agent proposed the opposite, but the PDB decides), 0x0070B470.
- **Headers:** none landed; chunk 839's STLport opt-ins stay in `pending/2026-11-07_linux_hand844/hdr/` for the parked 0x00775350.
