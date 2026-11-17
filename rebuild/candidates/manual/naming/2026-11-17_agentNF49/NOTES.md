# NF49 notes (round 7, 2026-11-17): definition-manager templates and the special-effects callees

Slice: the 12 entries of ENTRIES.tsv, 54 distinct binder rows (WORKLIST.tsv, 67 lines). Every number below is counted from STATUS.tsv,
APPLIED.tsv, work/grades.log and recheck_final.log.

## Outcome
- Rows: 52 ported (all RELOCATION_MATCH + test PASS in recheck_final.log, one vland run over every row in rows/), 0 not-a-binder,
  2 blocked (0x008B9E50, 0x008F3630; reasons and attempt counts in STATUS.tsv; attempts in work/attempts/), 0 deferred.
- Entries: 11 done, 1 blocked (nf41:e110 GetSubDef<CSpecialEffectsDef>: 14 of 16 binders ported, the two blocked rows left).
  nf41:e77 and nf44:12 are the same address (PeekDefinitionManager) and both close with the same 4 rows.
- Grades: work/grades.log has 134 lines = 54 start-of-round probe compiles of the repo copies (ORIGPROBE, 1 per row) + 80 grades.
  Max per row 6 (0x008B9E50, blocked). 0x00917130 has 4 lines: probe, its port grade, and two lane checks after the last container
  restart (the first returned nothing while wineserver started, the second RELOCATION_MATCH); no source change.
- Header overlay (hdr/, see hdr/README.md): two opt-ins, both off by default:
  FABLE_RETAIL_CAISTATEGROUPBASE_CAIFILTERBASE_RESTORE_PROTECTED (engine/CAIStateGroupBase.h) and
  FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT (engine/CMaterialDef.h). The 49 rows ported before this session use no overlay.
- RENAMES.tsv: section 1 = 2 self-leaf respellings (0x0082D3B0, 0x00972830); section 2 = 89 caller respellings generated from an
  orig (pO) vs ported (pS) relocation diff by offset (work/diffrel_all.txt), anonymous-namespace hash changes dropped.

## The three rows finished this session
- 0x009378F0 CAIStateGroup_MinionReactToUnconsciousPerson::Update (nf41:e78, nf41:e77, nf44:12): moved onto CAIStateGroupBase.h
  (_MEMBERS _AIACTIONS _PERFORMACTION _FINDAGUARD _PEEKDEFINITIONMANAGER(_PROTECTED)), CDefinitionManager.h GETDEF_NAME_BODY,
  CMessageEventDef.h and the shared CIntelligentPointer<const CThing> (_CONVERT_INLINE at the first site, _CONVERT_MUTABLE_OUTOFLINE
  0x0091F990 at the second). Besides the two slice targets it also corrects LookAtThing / MoveToThingProximity (QAE -> IAE),
  FindAGuardAndGiveMessage (local types -> PDB), the 0x0091F990 converting ctor (ABV -> AAV, FableWin) and the vtable symbol.
  _FINDAGUARD's `public:` (for the nested filter) made PerformActionAndWait public (a regression from the row's correct IAE): the
  new overlay opt-in restores `protected:`. 3 grades (probe + 2).
- 0x0063EB20 CWorldMap::DestroyThemeAt (nf41:e183): def models + same-TU GetDef stand-in -> shared headers (GETDEF_GLOBAL_BODY);
  the local CWorldMap model's PeekDefinitionManager respelled protected const CGameDefinitionManager& and GetStrongestEngineThemeAt
  long* -> long& (both Ego_r, as landed 0x0063DD70). // Leaf: set to the full mangled self name. 3 grades.
- 0x008F7950 CreateDustEffectForCreature (nf41:e183): only the def / manager / material slice moved onto the shared headers at global
  scope (GetDef<CEngineThemeDef>, GetDef<CMaterialDef>, made-up CMaterialDef::Get -> GetRandomSlideFootParticleEffect via the overlay
  opt-in). 2 grades. The function itself and 12 callees still bind NDust008f7950 model spellings (NAME_FIXES.tsv; port list: the
  NCombatActionHelpers TU with sibling 0x008F7BD0).

## Follow-ups for the coordinator (NAME_FIXES.tsv)
- 0x007F0300 (ported here) still binds GetStrongestEngineThemeAt with long* (PAJ); one-token fix, not done because the final
  recheck was already running.
- Landed 0x009810B0 binds AIAction_PerformActionAndWait public (QAE); add the RESTORE_PROTECTED opt-in.
- 0x0069AD10 and 0x0081FA20/0x0081FA90 as recorded earlier in NAME_FIXES.tsv.
- 0x0063EB20 keeps its local CWorldMap / CCoordVector models: engine/CWorldMap.h lacks SetThemeAtBlock, SetThemeBlendAtBlock,
  ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) and DestroyThemeAt (port list); the vector<C2DCoordI> insert_overflow
  0x00618550 is STLport (NF48's family), left as is.

## Locks
NF49 holds locks only on the 52 rows in rows/. The locks of the two blocked rows (0x008B9E50, 0x008F3630) were released.
