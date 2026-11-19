# 2026-11-19 landing nf98: naming round 8, NF98 (24 rows + 1 regrade)

NF98 took round 8's unattempted ports (records: `naming/2026-11-19_agentNF98/`, selfcheck 0 problems). 17 entries: 6 done, 2 blocked
(q11261 / q11262: the dozen-class local models of 0x009551A0 / 0x00956380, family budget spent), 2 deferred (q11274: its last row 0x0055FC90
was held by NF97; q11282: held by NF96), 7 open (not started). Round 9 agents NF100 / NF101 have the open and deferred entries.

## Rows (24)
- 11 self renames (RENAMES_applied.tsv, 6 file moves), among them 0x009BF5E0 CDisplayManager::CopyBackBufferToTexture, 0x00942270
  CIsVeryCloseMotionlessCreature::operator(), 0x0086B810 (the pick-any owner of GetRandomTextureMorphs), and the three CThingSearchTools
  template instantiations 0x008E3C50 / 0x0084F850 / 0x006DFA50 (catalog names were a propagated GetPBestThingInRoughArea<..> label).
- Binder ports onto the shared headers: SetCurrentAction / ~CCombatActionBase / the CCreatureAction_* ctors (0x00924930, 0x0072CBA0,
  0x0072C790, 0x008A9100, 0x0074ACE0, 0x009378F0), GetSurfaceLevel const (0x00642880, 0x009BF350 and in-place respells in 0x0066CB00,
  0x009FD4E0, 0x00B2C4E0, 0x00B87680, 0x00B89A20), GetCreatureType (in-place respells in 0x00914B20, 0x00986E30, 0x0092D7C0).
- One-definition moves: 0x0072D360 (SetPItemRemoved body to the header opt-in) and 0x0086BE40 (GetRandomTextureMorphs body to the header
  opt-in, emitted pick-any).
- 0x0086B9D0 regraded unchanged (it includes CRandomAppearanceMorph.h without the opt-in).

## Headers (12, all additive `#ifdef` opt-ins; base copies unchanged since NF98 started)
CCreatureActionBase, CCreatureAction_CastStrengthBerserk, _CastWillEnflameSpell, _SheatheItemToInventory_ForEnflame, _UndeadReturnToEarth,
_UndeadRiseUp, CDisplayManager, CPixelFormat, CRandomAppearanceMorph, CTCCreatureGenerator, CTCInventoryWeapons, CTCKraken.
assert_names_check: 0 mismatches.

## Name splits (advisor rule: never create one)
Five addresses gain or change a binding, all already split before this landing: 0x00925780 / 0x00925820 (the Kraken action ctors: 0x00924930
now binds them in the TU's anonymous namespace, as the landed 0x00924B90 does), 0x00A0B480 (0x009BF350 now binds the PDB
`?BeginRender@CRenderManagerCore@@QAE_NXZ` instead of a `_Methods` stand-in; 0x009BEF20 keeps `QAEXXZ`, queued), 0x013BAD30
(SHEATH_ACTION_NAME: an anonymous-namespace hash that changes with the TU), 0x01436E18 (0x00B87680 now records its existing
`g_pRenderManagerCore` use; the source spelling is unchanged).

## Checks
Recheck from the repo (`logs/recheck.log`, 25 rows): RELOCATION_MATCH / MATCH, every test PASS. bindings_multi_check 1 (the NullCoord
baseline); funclet_includes_check 1355 / 0 broken. Includer recheck (`logs/includer_recheck.txt`): 562 rows, 562 good, 0 bad.
