# Landing 2026-11-05q: chunk 790 (4 of 6 rows landed, 2 parked)

One expert agent authored the rows: the last six script rows of the groups ledger. All four landed rows are plain C++ and grade
`genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D26190 | NScript::CQ_DragonBossFightScript::DoMission | RELOCATION_MATCH PASS (4 grades) |
| 0x00E93120 | NScript::CV_HiddenBootyScript::CM_HiddenBooty::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00EE95C0 | NScript::CExpression_FishScript::Main | RELOCATION_MATCH PASS (16 grades) |
| 0x00D78670 | NScript::CQ_HeroSoulsScript::Main | RELOCATION_MATCH PASS (2 grades; 82 strings paired by hand) |

- **Key spellings:**
  - 0x00D26190: a declared-only explicit specialization of `~vector<CScriptThing>` keeps the out-of-line call to 0x008AC970.
    The morality constant is 0.5f (q.py masks float constants; the agent's float check caught it).
  - 0x00E93120: FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION. vland's one RELOC_NAME warning (`??BCDefString@@QBE?AVCCharString@@XZ`
    against the label `CDefString::operator CCharString`) names the same function two ways.
  - 0x00EE95C0: `__forceinline` GetPos, and the squared distance spelled `X*X+Y*Y+Z*Z`.
- **Headers** (`hdr_README.md`): three new headers, all behind FABLE_RETAIL_NSCRIPT: CQ_DragonBossFightScript.h (0x98),
  CExpression_FishScript.h (0x48, CScriptBase-based), CQ_HeroSoulsScript.h (0x50). No existing header changed; hdrcheck2 reads NEW
  for all three.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05q.tsv` (the six rows, plus 0x00D26BA0
    NScript::CQ_DragonBossFightScript::RunEnemySpawning, unlabelled before).
  - 3 queue lines (tag `land0511q`):
    - 0x00CBE551: the label is the autoname DistanceCalculator_GetVectorDistance, but the function is GetDistanceBetweenThingAndPosition;
    - 0x00EE9590: landed as a stand-in, but it is CExpression_FishScript::OnPersist;
    - CChickenSign::Main 0x00E64DF0: its +0x5c member is PrizesWon, not FinalSector (a member respell; the bytes do not change).
  - The agent's NOTES.md lists junk first spellings in reloc_bindings.tsv for 0x0099A2E0, 0x004AA840, 0x008AC970, 0x00A13C60 and
    0x008ADF90.
- **Landing:** `vland --land` (`logs/land.log`); labels applied (`logs/apply_labels.log`); the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand790/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00E64210 CV_ChickenKickingScript::CKickedChicken::Main | DIFFER(2803v2803), 8 lines, 52 attempts | x87 operand load order in the final `direction.Dot(a)`: retail loads `a` first in all three products |
| 0x00DC5F80 CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main | DIFFER(4259v4245), 36 lines, 10 attempts | The CIsThingAlive / CIsThingConscious copy-inline case (retail calls 0x004ABE90 / 0x004AA840); knock-on ebp == 0 |
