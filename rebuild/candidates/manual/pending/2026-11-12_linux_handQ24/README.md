# Chunk Q24 (six parked rows, one at a time, Opus 5.5)

3 of 6 matched; 2 landed in landing dr:
- 0x00584B86 CTCInventory::AssignThingPC: ported onto the shared headers (`#pragma optimize("y",off)` + `("s",on)`).
- 0x009289B0 CAIStateGroup_BanditPlot::Act: the landed sibling 0x009285D0's flow (early `return result`); RTTI shows the class derives
  CAIStateGroup_BanditPlayDice, so the "plot" at +0x18 is the inherited `CTCDiceGame* DiceGame`.
HELD BACK (held_back/): 0x00622A00 CTCInventoryQuests::OpenInventory matches only with a STAND-IN body of ConstructQuestList
(0x0061B610) in its TU (just a GetInstance() call; retail's body is ~2 KB). It waits until the real ConstructQuestList lands in the same
TU. 0x0061B610 is reserved by the second account (pending/2026-11-11_acctB/RESERVED.tsv), so it is not worked here.
DIFFER: 0x00AE1BC0 CParticleSimulation::ReadFromFile (1158v1158, one register join after the alpha-array read; /Oa fixes it but
rewrites the frame), 0x009AF7A0 SaveBinaryDefinitions (not ported; `this`/stream ebx/ebp swap), 0x00B4A700
CEnginePrimitiveRenderer2D::Render (not ported; work/00b4a700_p1.cpp in the agent workspace reached 927v926).
