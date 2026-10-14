# Batch `2026-10-14_linux_hand23` — delegated hand chunk 23 (CCreatureActionBase .. CCreatureAction_CastBattleCharge): 38 bodies (38 landed)

Twenty-third delegated hand chunk of the Wine lane (`$S/w22/chunk23.tsv`, 40 small-module bodies
of 17–96 bytes authored by a background agent against the live VC7.1 compiler, re-proved through
the lander's own parity and behaviour checks): creature-action constructors and copy
constructors named by their stored vtables (`BriarRoseConcentrateInto`, `SpiritReturnToHero`,
`ScorpionChargeInto`, `SummonerFlameSlice`, `StartBlocking`, `TrollVulnerableInto/OutOf`,
`BeetleTakeOff/Land`, `BuildUpUnholyPowerSpell`, `BlockRespond`, `CastWillFireballSpell`, the
three `CastBattleCharge` constructors), `CCreatureActionBase` accessors (retail size 0xa8, PDB
0xb4; the creature-action classes are byte-packed), a by-pass colour-filter request and a
script-module message-list reset.

Label corrections and the retail layout facts are in the batch sources. Parked: 0x00692A50
(first virtual's `al` returned unchanged on the false path), 0x00692C30 (`dec [ecx+4]` fold of
the def-pointer release).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14.
