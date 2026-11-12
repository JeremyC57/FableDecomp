# Chunk Q26 (eight parked rows, Opus 5.5, stay-on-task rule)

All 8 rows reached: 33 grades, 0 probes, about 160 tool calls. 1 matched and landed in landing du:
- 0x00846DD0 CCreatureAction_PerformExpression::FinishAction: ported onto Q25's framework for its sibling 0x00847520; fixing the
  identities removed the old ebp/ebx residue (vtable slot 10 is CDefClassBase::GetInstantiationName, 0x006925C0 GetFramesRunning,
  bit 2 at +0x1C9 is CThingAICreature::IsRespondingToFollowAndWait, the +0x6C tests are CThing::GetTypeFlags()).
7 still differ (differ/, residue on line 2):
- Ported: 0x009A3C20 CStreamingFontBank ctor (i/j in each other's registers in the glyph loop), 0x00B1A870
  CPSCLight::DatabaseSerialise (one xor where retail folds the memory operand), 0x00D39960 StartRound (block placement of the
  terminate check; alternate saved as 00d39960.alt_ifnotT.cpp.txt).
- NOT ported, only regraded (1-3 grades): 0x00EB40C0, 0x006EC850, 0x00A8AD40, 0x0090E5D0. Eight large rows was too many for one
  chunk; chunk Q27 takes these four plus 0x00D39960 with a full port required.
