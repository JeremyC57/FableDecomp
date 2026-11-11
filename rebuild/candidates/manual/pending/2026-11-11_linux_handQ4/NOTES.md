# Chunk Q4 notes

**Grade every source here with `FABLE_HDR_OVERLAY=$A/hdr`** (gg.sh does it). All header changes are behind new `FABLE_RETAIL_*` opt-ins
(hdr/README.md, Q4 entries at the end: CWSTRINGPARSER_LINES, CTCBERSERK_INITABILITY, CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFLOATS,
CTCSOUNDPLAYER_EXTRATAG). They are draft-only: no matched row uses them yet.

## Matched: 0
src/ and test/ are empty; finish.py writes an empty payload.

## Near misses (sources in differ/, residue on line 2 of each)
- 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon DIFFER(1088v1088), ratio 0.942 -- only residue: reload order at the end of the alive
  block (retail `mov edi,[esp+34h]` then `mov esi,[esp+88h]` at 0x007CD314; ours esi then edi). c2prio: weapon id 10, IV temp id 50.
- 0x00497FD0 CProgressDisplay::DisplayCenteredText DIFFER(1076v1076), ratio 0.928 -- noLines spill placement/home ([esp+24h] at the draw
  pre-header 0x004980FE in retail) and the 4th draw's Y load before `sub esp,8` (0x0049834B).
- 0x008F61A0 CHeightMap::Resize DIFFER(518v530) -- sizeX + 1 needs a frame home ([esp+1Ch]); ours has none.
- 0x0078C900 CTCBerserk::InitAbility DIFFER(1132v1152), ratio 0.651 -- prologue to the effect blocks matches; the conditional-temporary
  flag word gets ebx in ours (frame 0x20 v 0x18), retail keeps it at [esp+10h] and the && result in bl.

## Identities proven (LABELS.tsv)
DisplayCenteredText 0x00497FD0, InitAbility 0x0078C900, RadialBlurFadeTo float overload 0x006B8730, CWStringParser Init 0x00A0C8E0 /
SkipWhitespace 0x00A0C910 / ReadAsStringUntilCharacter(wchar_t) 0x00A0CCD0. No made-up names. NAME_FIXES.tsv: none.
- 0x004199C0 is not a function start (skipped, per brief).
- 0x006E3B90 is probably CTCTalk::Update(CAnimateThing&, const CAnimateThingUpdate&) (ret 8 + PDB name set) -- one source only, not labelled.

## Read only this pass (what each needs: line 2 of differ/<addr>.cpp)
0x006E3B90, 0x007348C0 (callee placeholder names 0x007394C0 / 0x00737880 / 0x00666820 must be proven first), 0x00B640E0, 0x00B63C00,
0x00541A00, 0x00C604E0, 0x008F8F50, 0x00973900, 0x00A4D330, 0x00BA4C10, 0x00BD2360, 0x007B5E50 (needs a CTCWaterWader header + the
ceil-idiom retail-asm helper). landed_twins.py found no landed twins among the rows.

## Header debt
- No shared engine/CTCWaterWader.h (its landed neighbours 0x007B5B40 / 0x007B5CA0 use local models).
- CWStringParser's retail class has no user destructor; the repo header declares an out-of-line virtual one (kept on the default path).
