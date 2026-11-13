# 2026-11-12 landing fo -- 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction (one Fable author pass)

Source: `pending/2026-11-12_linux_handWF17F/`. Three Opus author passes in workflow wf17 left this row at DIFFER(710v720, 9 hunks).
Third row of the Opus-only workflows that Opus could not match; under the owner's rule it got ONE author pass with
`args.authorModel = 'fable'` (scout, both verifiers and the repair stage on Opus). Matched on the Fable author's second attempt;
the Opus verifiers then raised three real findings about the callee bodies, fixed in the workflow's repair stage (still a match).

**What made it match (callee-body visibility):** CHitParameters' nine-argument constructor 0x00789238 must be visible in the TU
while still called out of line. VC7.1 then knows the two `const C3DVector&` arguments are only read, so the direction vector does
not count as escaped: the subtraction is built X first, X/Y stay on the x87 stack and only Z is homed (`fst [esp+28h] ; fmul
[esp+28h]`). The same holds for CThing::GetPos 0x004C73D0 and CThingPhysical::IsInRange 0x006A4DF0, whose visible bodies flip the
`fld range ; fadd [radius]` order. Both of those carried bodies grade MATCH against their own retail addresses from this same
source. The constructor body lives in engine/CHitParameters.h (opt-in HIT_CTOR_BODY, with real C3DVector members under
VECTOR_MEMBERS); as an out-of-line function it grades 186v178 (the implicit DamageMap construction differs), so it is a header
body used for its mod/ref facts, not a second row for 0x00789238 -- recorded in NOTES.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1.

**Coordinator fix at landing:** vland warned RELOC_CONFLICT: the row called 0x0066FB10 as `CTCCoopSpirit::AddExperience`, a symbol
the landed 0x006AD580 binds to 0x0066FB00. The two bodies are byte-identical (`[ecx+10h] += amount`). Ego_r.pdb orders
ApplyMovementVector, AddExperience, AddScore, GetScore exactly as retail 0x0066FAE0, 0x0066FB00, 0x0066FB10, 0x0066FB20, so
0x0066FB10 is `?AddScore@CTCCoopSpirit@@QAEXJ@Z` (FableWin has the symbol too). The row and its test now say AddScore; rechecked
(RELOCATION_MATCH, PASS, `logs/recheck_addscore.log`) and rebound: one symbol per address again.
Headers (additions under opt-ins with no other user; defaults unchanged per hdrcheck2): CHitParameters.h,
CCreatureAction_SpiritAttack.h. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fo): 0x00670000 GetMeleeTargetRange (landed as CTCPhysicsBase::GetAirResistance), 0x0066FB10 self row -> AddScore,
0x0066FB30 -> ResetScore, 0x0066FB40 -> GetSpiritDef (Ego_r order).
