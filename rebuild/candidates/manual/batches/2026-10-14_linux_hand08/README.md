# Batch `2026-10-14_linux_hand08` — hand-authored CAIStateGroup_KidPlayHopscotch .. CAIStateGroup_MinionMotherProtectFamily functions: 40 bodies (37 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the ninth chunk of
class-labelled open functions of 17–96 bytes: state-group `IsRunnable` / `OnActivate` / `Update`
bodies (yell-for-help and curiosity finders inlined into their `IsRunnable`), the static
`__stdcall` form of `MeleeCombatNearbyBase::UpdateCurrentAction`, `CMusicTimer::Reset`,
`CSubtitleRenderer::ResetCutsceneState`, an opinion-source activation and a mesh-info validity
query. One chunk row (0x00957BE0, `CrowdBase::Cleanup`) over-captured the next function past
the padding; the 9-byte body landed against the trimmed oracle.

Lessons recorded by the pass: a combat or message manager reached through an inline accessor
materialises in eax with the push between it and `mov ecx,eax`, a direct member chain loads
into ecx and pushes before the world call; retail message filters are `operator&&` chains
binding pointers to the operand temporaries (the empty filter lands in a slot's high byte);
`test al,al ; setne al` is `return byte ? true : false`; a `PerformActionAndWait` result parked
in edi is a compare folded after register allocation; a global read before a call survives only
through a named local; independent zero stores keep statement order. Three bodies differ by a
register choice or the order of two pointer stores around an out-of-line constructor.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
