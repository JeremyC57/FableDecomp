# Landing hand4a b5 (2026-11-19): CTCCombat::PlayCombatStrikeSounds (0x00733B70) -- fully sourced

vland --land, strict headers: RELOCATION_MATCH + PASS; no fitting, no brackets. Name proven (catalog label `_global::CTCCreature_PlayDeathFlourish`
was descriptive): ?PlayCombatStrikeSounds@CTCCombat@@QAEXXZ, Ego_r 0x0074CA00 + FableWin 0x024FFB00 (`NAME_FIXES.tsv`). Source: the FableWin
body with Ego_d locals (one `if` over GetTC / GetSubDef / PeekTC / MeleeCombatPlayStrikeSounds; `const CCharString& sound_symbol_name`;
GetPSoundPair; PlayAttachedSample with CSoundInit() via the existing CSOUNDINIT_INLINE_CTOR_TYPE_LAST).
New opt-ins (`hdr_README.md`): CHeroCombatDef.h STRIKESOUNDS (MeleeCombatPlayStrikeSounds +0xE2 in the existing pad; Ego_r +0xFE, the same 0x1C
shift as MeleeStrikeNumFramesPause), CTCCombat.h STRIKESOUNDS (declaration + FableWin inline GetParentCreature / GetPHeroCombatDef),
CTCHeroExperience.h ONHITSOUND. All 141 landed rows including those three headers rechecked (`logs/recheck_header_users.txt`).
