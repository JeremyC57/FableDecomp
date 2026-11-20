# 2026-11-20 landing nf123: PRIORITY slice 8 (222 rows)

Records: `naming/2026-11-20_agentNF123/` (selfcheck 0 problems). 32 entries: 17 done, 6 done-already, 7 blocked (q3941: 63 stand-in rows
left, blocked: port; q3970 0x0086E4B0 whole-row STL stand-in; q4397 0x00DFDE30 two forms DIFFER; q4958 0x00934920; q6401 / q6402 the same
4 rows 0x008C6B20 / 0x008F5060 / 0x008F5220 / 0x008F5490, waiting on a header claim: type CTCDPhysicalObstruction::ThingsToIgnoreObstruction
as a vector of CIntelligentPointer<CThing> and settle its push_back at 0x004AD520; q6403 0x009551A0), 2 deferred (q6394 / q6395: their binder
0x008E9A30 was NF126's lock; now NF130's, with a one-definition fix). 98 rows not-a-binder.

## Rows (222): 4 MATCH + 218 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 20 self renames (RENAMES s1), among them the CCombatActionBase constructor / GetSoundPair, the CCombatAction_* and CCreatureAction_*
  constructors, CTCDPhysicalObstruction SetLifeTimeSeconds / SetParams, CScriptedMapBrush SetThingToDelete / SetThingToAdd,
  CAIBrain::PeekCurrentStateGroupName, CThingAICreature::IsBrainUpdateZoneActiveThisFrame, std::_Construct<CScriptThing>, and
  CCombatSequenceBase::~CCombatSequenceBase (q3941, QAE -> UAE).
- 202 binder respells onto the PDB spellings of those constructors and helpers.
- 0x008AC120 needs `/Gr` (row `// Flags:` line): added to its CompilerFlags; the HEAD-side gate compile used the old flags.
- Coordinator decisions (advisor precedent, owner delegated): q3941's self rename lands although 63 rows keep per-class `Dtor` stand-ins: the
  gate reads it as an existing split at 0x008B0A60 whose count does not grow. 0x00830130 moves from the `Run_0088b810` stand-in to
  `?IsUpdateZoneActiveThisFrame@CAIBrain@@QBE_NXZ` (both PDBs); the self row 0x0088B810 still defines the non-const QAE form, so the address
  stays at 2 spellings (queued).

## Headers (add-only opt-ins, default path token-identical; assert_names_check 0 mismatches)
LHHttpHeaderStatus.h _RESET; LHHttp2.h _DTOR, _SENDREQUEST, _RESET; LSocket.h _DTOR; CScriptedMapBrush.h _THINGSETS (pin_asserts pinned its
14 asserts to their existing tags); CAIBrain.h _ISUPDATEZONEACTIVE; CTCDPhysicalObstruction.h _SETPARAMS (1 assert pinned).

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0; no funclet includes a changed row. 886 rows rechecked: 886 good / 0 bad.
No-new-split gate 0. Table spelling sets: 10 shrank (0x007FD4B0 5 -> 2, 0x00857F30 4 -> 3, ...). Grew as new witness rows only (the row binds
the same spelling before and after): 0x004365B0, 0x006644F0, 0x00859A20, 0x008D46D0, 0x01294764; 0x0088B810 0 -> 1 is the case above.
NAME_FIXES queued: 0x00693C70, 0x00857F30, 0x008D86E0 constructors by reference; 0x01375550 ConstantFPS (row 0x007F2100 defines a stand-in
global); 0x0088B810 const.
