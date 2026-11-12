# 2026-11-12 landing ee -- 0x008031E0 GetPBestThingForMeleeAttack lands on the owner's decision

Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1, evidence verifier ok): 0x008031E0
CTCTargetingPlayer::GetPBestThingForMeleeAttack. Four agents (Q28, Q31, Q33, F3) worked it; F3 closed it.

It carries one INFERRED statement, marked as such in the source header: `if (p_thing == &parent) return p_thing;` after the
AreThingsEnemies call, where both outcomes are the function's `return p_thing`. Retail's bytes show the parent value had a second
use that left no code; this is the only construct found that reproduces that. The exact wording is not provable.
**Owner rule (2026-11-12): an inferred statement is acceptable as long as it makes the row behave exactly as the retail code does.**
Here the bytes are identical and the statement cannot change the result.

Header: CTCTargetingPlayer.h MELEE opt-in (declaration; additions only, default=same, new macro).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
