# 0x00521CF0 CThingManager::LoadMainPlayerCreatureState -- READY (p25Q, Account B, 2026-11-21)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00521CF0 | CThingManager::LoadMainPlayerCreatureState(CCharString& old_script) | 446 | RELOCATION_MATCH (no flags) | PASS (links the REAL row object, so the swap runs through the TU's own LoadEntitiesFromScript / ActivateThings / CopyHeroToHero; 3 scenarios, full call log vs a model, CCharString balance, flag state, version / level members) | 12 / 12 KILLED |

Strict check (`vland.log`): `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv` -> RELOCATION_MATCH, behaviour
PASS. One expected warning: RELOC_NAME GetReplacedUIDScript -> 0x0083E1B0 labelled CFourierAnalysis::CFourierAnalysis (a folded
`mov eax,ecx ; ret` COMDAT; see NAME_FIXES note).

## Same-TU copies (`// Fidelity: same-TU strong definition of 0x... (retail TU-mate)`)
The three copies (0x0051E2F0 ActivateThings, 0x0051F0E0 CopyHeroToHero, 0x00520D00 LoadEntitiesFromScript) are byte-for-byte the landed
texts now in the repo (commit 774766b5a for the two re-ports): from `void CThingManager::...` to the closing brace, checked by script
against rebuild/src/compiled/00/51/*_0051e2f0.cpp, *_0051f0e0.cpp and 00/52/*_00520d00.cpp. Nothing sits between a `// Fidelity:` line
and its body (the `g_LoadTickInterval` extern and the NProgressDisplay declaration are above the first copy). The same TU graded at each
copy's address is RELOCATION_MATCH at 0x0051E2F0, 0x0051F0E0 and 0x00520D00 too.
Why they are needed (probes/): without ActivateThings, without CopyHeroToHero, without LoadEntitiesFromScript, or without all three, the
row is DIFFER(433v446) each time -- VC7.1 c2 keeps loaded_things._M_start in edi across the three calls only when it sees their bodies.

## Text
- Shared headers: CThingManager (GetPlayerManager PEEKPLAYERMANAGER, WorldMap, LOADMAINPLAYERCREATURESTATE / THING_TYPE_IDS landed),
  CThing (GetPos, GetPhysicsTC, ENTITYINITLEVEL landed), CTCPhysicsBase (SetPosition), CWorldMap, CPlayerManager, CPlayer,
  CThingPlayerCreature, CLevelLoader (GetLoadAllSectionName), and the NEW engine/CThingScriptUIDReplacer.h.
- Ego_d's locals (uid_replacer, script, sections, loaded_things, pos, level, hero, loaded_hero, it, loaded_thing); the comment decodes
  the Ego_d line table 1198..1236. Lines 1216 (216 B), 1228 (12 B) and 1229 (207 B) have no retail code (no locals, one file block,
  nothing inlined; 1216 / 1229 are the oversize debug-check lines of FINDINGS 2026-11-20 p25G).
- GetPlayerManager() is required by retail (the direct PlayerManager member grades DIFFER(445v446), probes/playermanager_member).

## Labels
- `// Inferred:` `WorldMap.GetMapNumberAt(pos)` with the member read directly; `GetWorldMap().GetMapNumberAt(pos)` (accessor in FableWin
  and Ego_d) grades RELOCATION_MATCH too (probes/getworldmap), and Ego_d's 43-B line fits neither.
- Three `// Fidelity:` copies (above). No brackets, flags or inline-budget controls.

## Header changes (hdr/, hdr.diff) -- add-only
- NEW engine/CThingScriptUIDReplacer.h: Ego_d LF_CLASS (sizeof 4): private CCharString ReplacedUIDScript +0; public ctor
  (const CCharString& previous_script) 0x0083E1F0 and GetReplacedUIDScript() const 0x0083E1B0; private GetPlayerManager / GetWorldMap /
  GetThingManager / GetSwapUniqueIdentifier(long) const (Ego_r.pdb + FableWin.pdb names). Declarations only; sizeof assert (new file,
  nothing to pin). Newly flagged rows: none (no landed row models this class).
- The other opt-ins this row uses (LOADMAINPLAYERCREATURESTATE, THING_TYPE_IDS, ENTITYINITLEVEL, KILLPARTICLES, AAM TCBASE) landed with
  774766b5a.

## Names (NAME_FIXES.tsv)
- 0x00521CF0 CThingManager_LoadMorphFromScript -> CThingManager::LoadMainPlayerCreatureState (Ego_r + FableWin + Ego_d).
- 0x013B8A88: three landed descriptive spellings (g_SuppressInventoryBaseCopy, g_bApplyCopiedHeroStats, g_bCopyingThings) ->
  NThingManager::HeroSwapInProgress. Only Ego_d names it (public data symbol); second source is the role (this row sets / clears it
  around the hero swap). Flagging the single-PDB evidence for your call.
