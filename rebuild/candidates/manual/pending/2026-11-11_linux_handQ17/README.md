# Chunk Q17 (this/iterator register swaps and GetInterface reworks, Opus 5.5)

7 of 20 rows matched and landed in landing dd:
- 0x009711D0 MoveIntoCover
- 0x00966030 and 0x00965640 OnReplace
- 0x0093C030 SeekScriptTarget::Update
- 0x00795090 CTCUnholyPower::FinalCleanup
- 0x0095B820 HoverHornetQueen::Spawn
- 0x00A87700 CSBCAgents::Process

13 are in differ/ with the residue on line 2. Two were ported this wave: 0x008BF500 (412v412, one scratch register) and 0x00555A90
(595v595, colouring). Eight were not reached. Chunk Q18 takes those ten.
