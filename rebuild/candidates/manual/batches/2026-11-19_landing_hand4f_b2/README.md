# Landing hand4f b2 (2026-11-19): CAnimationSet::GetAnim (0x005DC340)

Strict vland --land result: RELOCATION_MATCH + PASS, no flags.

## The unreachable end
The row matches only with an `_MSC_VER`-guarded `__assume(0)` after the FableWin loop. That is an unreachable-end marker, accepted as an advisor decision
(owner delegated); see EXTRA_RULES 2026-11-19.

Evidence:
- Retail has no end / key test (0x005DC360..0x005DC376 is an endless loop) but keeps a dead `mov edi,ecx` at 0x005DC349.
- Ego_r's VC10 release body drops the same two tests.
- FableWin /Od has the tests and no code after the loop. It compiles asserts in, so the source was not `ASSERT(false)`.
- The fall-off-end spelling and 14 other alternatives differ.

## Portability
At landing the coordinator added `#elif defined(__GNUC__) __builtin_unreachable();` to source and test. The function returns a reference, so the portable
build needs a terminator. The MSVC bytes do not change.
