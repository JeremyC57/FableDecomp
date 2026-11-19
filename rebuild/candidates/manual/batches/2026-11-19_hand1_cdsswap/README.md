# Landing hand1-cdsswap (2026-11-19): 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound (626 bytes)

The hand1 lane's candidate (`pending/2026-11-18_hand1/candidates/0066a9e0.cpp` + `_test.cpp`, FableWin shape: early both-null
return, dead ASSERT arms, implicit conversions, no `name` local) lands unchanged apart from its Grade line, once the CDefString
binding swap (`naming/2026-11-19_cdefstring_swap/`) made 0x00415DD0 the conversion operator: VC7.1's plain inline heuristic
expands the conversion at the first four sites and calls ??B out of line at the fifth (0x0066AC08), as retail.

vland --land (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH + PASS (`verify.log`); the test (hand1a) kills 8 mutants (both-null
early return, separator, fallback order, sound-enabled gate). One new binding:
`?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z` -> 0x0066A920.
The only warning is RELOC_NAME on ??B (the label checker does not equate `??B` with "operator CCharString"; every caller shows it).
