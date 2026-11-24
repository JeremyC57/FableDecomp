# Landing: 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents as Inferred (Fable 5.2 authoring trial; owner decision, 2026-11-22)

Authored by AUTH22 (17 grades) and finished by AUTH23 (4 grades), both on Fable 5.2 (`pending/2026-11-22_auth22/0066b370/`,
`pending/2026-11-22_auth23/0066b370/`). **Owner decision 2026-11-22: land as Inferred** (asked together with 0x0093D270).

- **Grade:** RELOCATION_MATCH, test PASS, stock flags, strict headers; rechecked from the repo after the table line (`logs/`).
- **Text:** Ego_d locals and blocks; line 208's two accessor calls as Ego_d has them (no friend line, no direct member reads: no PDB holds a
  single friend record for any class, and the members are private in all three).
- **Inferred 1:** engine/CAnimSet.h `FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED` (off by default): the two percentage accessors return
  `const float&`. All three later VC10 PDBs give them by value; retail passes both with no temporary (0x0066B6E0, frame 0x74); the by-value
  form keeps a stored temporary under VC7.1 (frame 0x7C, DIFFER(1844v1808)). Same mechanism as 0x0093D270's CCombatManager getter.
- **Inferred 2:** the stop-event arm (retail 0x0066B750..0x0066B77D: GetPItem x2 and the emitter's slot-12 virtual) has no code in Ego_d's
  lines 216..227 (zero bytes): it meets clause (1) of the standing criterion "or the reverse" (AUTH23); the text stays Inferred and the
  forwarder name (`StopAllCriteriaContainingString`) stays descriptive.
- **Headers** (add-only; AUTH22's three opt-ins + three new headers, AUTH23's two): CTCSoundPlayer.h `_PLAYALLSOUNDEVENTS`, CAnimSet.h
  `_ANIMINDICES` (four FableWin accessors) + `_PERCENTAGE_REF_INFERRED`, CThingSoundEmitter.h `_STOPALLCRITERIACONTAININGSTRING`, new
  CSmallVector.h / NAnimationEvents_CEvent.h / NAnimationEvents_CManager.h, and CDefinitionManager.h
  `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY`.
- **The pick-any device** (advisor ruling 2026-11-22, its conditions met): an `inline` explicit specialization of
  GetSubDef<CEntitySoundDef> inside the owner row 0x0065F604's own pragma pair (`optimize("y", off)` / `("s", on)`), body = the template's
  tokens with T substituted, ending with `optimize("", on)` (valid only in a TU with no optimize pragma above the include; the comment says
  so). Scaffolding for link determinism, not a claim about retail's text. The coordinator added the `_MSC_VER` guard its neighbours carry
  (same tokens under MSVC). `extern template` was tried first: VC7.1 3077 dies with C1001. A row-tail pragma re-modes every template copy.
- **Pick-any** (`logs/pickany_vs_retail.tsv`, `logs/leafrel.txt`): leafrel 0 problems; all 11 owned copies EQ (GetSubDef<CEntitySoundDef>
  162 B EQ 0x0065F604). Its `"CEntitySoundDef"` literal at 0x01235DD0 had no table line: added as a first witness.
- **reloc_bindings:** first witnesses `GetAnimEventsBetweenTimes` 0x006F99B0, `GetSoundEventManager` 0x006F90C0,
  `g_bPlayAnimationSoundEvents` 0x01375758, and the literal (`logs/reloc_bindings_first_witnesses.tsv`).
- **Header proof:** pin_asserts 0 pinned (AUTH22's CThingSoundEmitter.h pin is in the overlay), assert_names_check 0 mismatches; equivalence over 57,023 landed rows: 0 differing (the row itself is graded above).
- **Name fix** (AUTH23, queued): the unlanded draft's `CreateScaledEffect` 0x006E11F0 is `?Create@CTCDParticleEmitter@@SIPAVCThing@@JJMABVC3DVector@@_N@Z`.
