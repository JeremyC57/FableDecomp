# 2026-11-19 landing nf99a: naming round 8, NF99 (GameCODA) non-splitting subset (12 rows)

NF99 re-authored 17 GameCODA rows onto the SDK 2.10 CodeView names (records: `naming/2026-11-19_agentNF99/`, selfcheck 0 problems).
Advisor decision (owner delegated): a landing may leave an existing name split but never create one. Five rows bind one of eight
addresses whose landed owner rows still carry the old names (0x00C69C60 / 0x00C69FB0 / 0x00C6A200 / 0x00C89750 / 0x00C89AA0 /
0x00C89B60 / 0x00C74BB0 / 0x00C74AB0), so they are held for NF99 part 2, which ports the owners first:
0x00C6A5D0, 0x00C75CE0, 0x00C783F0 (direct binders), 0x00C6A180 (binds the new leaf of held 0x00C75CE0) and 0x00C6AEF0
(Commit; held with its callee Translate). Their five headers (CFloatReverbDspEngine, CFloatReverbParameterTranslator,
CFloatShortReverbModule, CFloatShortReverb, CFloatFilterParams) are held with them.

## Landed (12 rows + 1 test)
- Self renames (RENAMES_applied.tsv, 6 file moves): 0x00C5A500 CSalSwBaseVoice::SetPitch, 0x00C5CE40 CSalSwBuffer::Release,
  0x00C6B720 CSalSwFilter::ClearHistory, 0x00C6BBD0 SetFrequency, 0x00C6BC80 SetLP, 0x00C6BD00 SetEQ.
- The Release set lands whole: 0x00C5CE40 with binders 0x00C5B300 / 0x00C5A1F0 / 0x00C54BD0 (and the test); ClearHistory's binders
  0x00C55030 / 0x00C551D0 / 0x00C5C2D0. The only other landed binder of these six, 0x00C54C10 (Account B), already bound the SDK names,
  so the landing heals a split rather than making one.
- 0x00C5A1F0 binds 0x00C59710 as `?Stop@CCodaVoice@@UAEXXZ` (the landed owner's own leaf); 0x00C3F3D0 still binds the stand-in
  `?Stop@CVtable_012B5C10@@UAEXXZ`. That split already existed in reloc_bindings.tsv before this landing (left as is, queued).
- Headers: engine/CSalSwFilter.h (SetFrequency / SetQ / SetGain / IsHistoryClear opt-ins) and engine/CSalSwBuffer.h (IsEmpty opt-in), additive.

## Checks
Recheck from the repo (`logs/recheck.log`): 12 MATCH / RELOCATION_MATCH, every test PASS. assert_names_check 0 mismatches;
bindings_multi_check 1 (the NullCoord baseline); funclet_includes_check 1355 / 0 broken. Includers: no other row includes the two
headers; the binder 0x00C54C10 rechecks clean (`logs/includer_recheck.log`).

## Queue
q11405, q11406, q11410, q11411, q11412 done; q11476 done (no self row; the binder was already right). q11414, q11415, q11477, q11492,
q11493 held for part 2. NF99's 25 NAME_FIXES queued from q11498 (land1119nf99).
