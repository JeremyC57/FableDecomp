# NF168 header overlay

Every overlay header is a copy of the repo working-tree header (base copy in ../orig/) with `#ifdef` opt-ins added only. Without the
opt-in macros each header is token-identical to its base, so no landed includer changes.

## engine/CTCInventoryTrade.h
- Added `FABLE_RETAIL_CTCINVENTORYTRADE_FINALSUBTITLES` (after the `_SUBTITLES` block): the four private Ego_r.pdb / FableWin.pdb members
  FinalConstructSubtitles 0x0059D0B8, FinalRefreshSubtitles 0x0059D1B1, FinalConstructSubtitlesPC 0x0059D65A, FinalRefreshSubtitlesPC
  0x0059D761 (declarations only; the block opens with `private:` and closes with `public:`). Users: rows 0x0059D0B8, 0x0059D1B1,
  0x0059D65A, 0x0059D761, 0x0059E4D7.
- pin_asserts.py: pinned 0 (no FABLE_STATIC_ASSERT moved).
- Landed includers (`grep -rlE 'CTCInventoryTrade\.h' rebuild/src/compiled`): 64 rows. None defines the new macro, so the opt-in
  changes none of them.

## engine/CTCPerceiveThing.h
- Flat (default, generated) branch only; four declaration-only opt-ins inside the flattened class, after the existing
  `FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETFOV` block, plus a guarded `class CThing;` forward declaration before the class:
  - `FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING`: `const CThing* GetPerceivedThing() const;` 0x007446A0 (rows 0x009577E0, 0x0091DB50,
    0x00849320, 0x0089D0E0, 0x0089CD70);
  - `FABLE_RETAIL_CTCPERCEIVETHING_FLAT_CANSEEORSMELL`: `bool CanSeeOrSmellPerceivedThing() const;` 0x00661600 (row 0x0091DB50);
  - `FABLE_RETAIL_CTCPERCEIVETHING_FLAT_ISAWAREOFTHING`: `bool IsAwareOfThing() const;` 0x00643B3B (row 0x0091DB50);
  - `FABLE_RETAIL_CTCPERCEIVETHING_FLAT_SETASDIRECTLYAWARE`: `void SetAsDirectlyAwareOfPerceivedThing();` 0x007449B0 (rows 0x00849320, 0x0089D0E0).
- pin_asserts.py: pinned 0. Landed includers (`grep -rlE 'CTCPerceiveThing\.h' rebuild/src/compiled`): 73 rows; none defines the new macros, so
  the opt-ins change none of them (default path token-identical).

## engine/CTCInventoryAbilities.h
- Added `FABLE_RETAIL_CTCINVENTORYABILITIES_GETEXPERIENCESPENTONABILITY` right after `GetExperienceCostToUpgradeAbility` (retail branch):
  `long GetExperienceSpentOnAbility(EHeroAbility ability) const;` 0x005EE136 (Ego_r.pdb 0x0057CC91 / FableWin.pdb 0x01EBC6E6). Declaration
  only. Users: rows 0x005EE136, 0x005724CD.
- pin_asserts.py: pinned 0. Landed includers: 78 rows; none defines the new macro, so the opt-in changes none of them.

## Re-check at hand-back
- orig/ copies of CTCInventoryTrade.h, CTCPerceiveThing.h and CTCInventoryAbilities.h are byte-equal to the current rebuild/include headers (cmp).
- Sibling overlays re-listed: NF166 hdr/engine = CGameTimeManager.h, CParticleProgram.h, CQuestManager.h; NF167 hdr/engine = CTCCarryable.h,
  CTCCreatureOpinionOfHero.h, CTCDrunkenness.h, CTCEnemy.h. None overlays my three headers, so no member is declared twice.
