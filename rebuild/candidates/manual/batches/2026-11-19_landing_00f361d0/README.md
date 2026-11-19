# Landing 0x00F361D0 NFractal::CPerlinNoise::Get3DNoise (2026-11-19, 873 bytes)

hand2c, after three Fable advisor rounds. vland --land (strict headers): RELOCATION_MATCH + PASS (`logs/vland_land.log`).
The fix is in the shared opt-in FABLE_RETAIL_CPERLINNOISE_GETNOISE (engine/CPerlinNoise.h), landed earlier the same day with
0x00F36010 / 0x00F36540, its only other users: `SCurve` returns `(t*t*(3.0f-2.0f*t))` and `Interp` returns `(a+(b-a)*t)` (same
instructions every way; c1 leaves one no-code PAREN tuple per bracketed float sub-expression and c2 schedules in 81-tuple regions --
measured with c2 DAG dumps). Both siblings re-verified on the new text (`logs/recheck_siblings.log`: RELOCATION_MATCH + PASS).

How it was found: Get3DNoise is one basic block in four 81-tuple scheduling regions; the only no-code tuples in region 1 are SCurve's
PARENs (placement after (3-2t) and on the product fixes site 0x00F36303 / 0x00F3630D), and one PAREN fewer per Interp shortens
region 2 so the cut at 0x00F3640F lands as retail's. Disproved on the way: FableWin's double literals (retail loads dword constants),
right-associative SCurve (breaks both siblings), include order (a tuple count, not an ordinal tie).
