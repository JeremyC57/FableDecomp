# Batch `2026-10-14_linux_eh7` — hand-authored small EH-framed bodies: 8 bodies (8 landed)

Seventh hand pass (Wine lane, `/EHsc`): an AI state-group destructor (member at +0x2f8, out-of-line
base), `CPixelShaderData::~CPixelShaderData` (release through the shader's virtual slot 14 with 1),
a UI component unregistering from its owner's registry member, a two-member constructor where the
second member is built from the first's address, a `novtable` destructor whose array of four
0x28-byte elements goes through `__ehvec_dtor`, `CCreatureAction_SummonerFlameSlice`'s destructor
(virtual slot 5 through the constant vtable slot via an inline helper, a member holding a critical
section deleted through the import table, EH state 1 for two tracked objects), the dllimport copy
constructor of `CCardPositionsDef`, and `CHobbeTorrent::Init` (a 16-byte stack visitor). Parked:
`CTCFishingSpot` destructor @ 0x00C3CAA0 (84 v 86: the DDK compiler drops a redundant
`mov ecx,esi` before the helper call that retail keeps).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets11`.
