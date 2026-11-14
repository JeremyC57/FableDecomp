# acctC hand-back c36a (agentC36, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0074AF50 | CTCCreatureGenerator::CreateCreaturesUndead | 856 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Nothing in the function text is fitted (closers: `break` not `return` inside the loop; push_back through the implicit
CIntelligentPointer<CThing> conversion).

REVIEW ITEM: the row needs a plain earlier definition of `HasHitCreationLimit` in its TU (retail has no `mov ecx,esi` before
the following PeekWorld call, 0x0074AF6B..8A and again at 0x0074B310..27). The hand-back realises it as the header opt-in
`FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION` (text of landed 0x0074A190, non-inline). Coordinator
preference: move that definition into the row source above the row (a non-inline definition in a shared header is multiply
defined at link time); the bytes do not depend on where the text sits.
Name fixes (NAME_FIXES.tsv): 0x0074D0D0 CTCGeneratedCreature::SetGenerator (landed as AssignTarget); 0x004FAD50
~CThingAICreatureInit; 0x0074DD20 CCreatureAction_UndeadIdleUnderground ctor takes CThingCreatureBase&.

`open/`: 0x007BC38A InitMovement DIFFER(880v880), one instruction (parked); a probe for 0x006823D0 showing the three
frame-reaching spellings are all fitted (parked).
