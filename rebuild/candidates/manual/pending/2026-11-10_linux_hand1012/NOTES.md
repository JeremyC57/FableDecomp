# Chunk 1012 notes (agent1012)

Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported. The overlay changes are listed in `hdr/README.md`. All rows
use the shared headers. No local padded class models, invented namespaces or stand-in bodies were kept: the only same-TU bodies
are retail functions with retail bodies (GetDef index form, GetPBestThing, LoadGameState / AutoLoadInternal, STLport vector copy).

## Result
- **RELOCATION_MATCH (4, in `src/`):** 0x006B1A20, 0x006B2260, 0x0049FFD0, 0x004A0780. Each has a test in `test/`.
- **DIFFER, parked (5, in `differ/`):** 0x004A5E90, 0x004A34F0, 0x0062BEF0, 0x0062BD80, 0x008A8260. Each was graded once. Each
  was probed many times for free first (c2prio / masked diff, plus the frame-packer trace `tools/fr.sh` / `fr2.sh`, which is
  hand886's phook.py).
- **Not attempted (7):** 0x0089C910, 0x0089CE70, 0x008A7A70, 0x0088EDC0, 0x004A1840, 0x004A1CB0, 0x0062B070. Their priors are
  local padded models. Rebasing them is the next pass's work.

## Identities proven (two sources each; see LABELS.tsv)
- 0x006B1A20 = CAtmosProcess::UpdateDayNightAtmosSounds and 0x006B2260 = CAtmosProcess::Update. Sources: the PDB name set, the
  landed UpdateFrame call, and the bodies. The catalog's CGameScriptInterface_* placeholders are wrong.
- Definition manager:
  - 0x009AD3B0 GetNoInstantiatedDefsOfType
  - 0x009AD410 GetDefGlobalIndexFromName
  - 0x009ADE10 GetPDefFromClassIndex (const overload)
- Game time:
  - 0x006BB1E0 / 0x006BB790 CGameTimeManager::IsDaySounds / IsNightSounds
  - 0x006B2BD0 CEnvironment::GetRainStrength
- Rain marker:
  - 0x0083D460 CThingMarker::Create (__fastcall static)
  - 0x007E35F0 / 0x007E3610 CTCAtmosPlayer::SetPos / SetAtmosDefIndex
- 0x0057D990 CTCHeroStats::SetHeroKickPositionForCurrentRegion. 0x004AC2C0 is the GetPBestThing instance; Ego_r's mangling
  matches STLport binary_compose / logical_or.
- Game events:
  - 0x009F17A0 CGameEvent::GetDataPackage
  - 0x009F18F0 CGameEventDataPackage::GetData
  - 0x0049D850 CWorld::GetPlayer(unsigned long) (IAE)
  - 0x00634190 CPlayer::RemoveMode(EPlayerMode)
  - 0x00487DC0 CPlayer::GetPControlledCreature
- Projectile weapon:
  - 0x00698B20 CGameCameraManager::PlayCameraSoundThroughObservedThing(const CCharString&) const
  - 0x00789AB0 / 0x00789C90 CTCHeroRangedAttacks::IsToUseFirstPersonTargetingMode / IncrementProjectileWeaponMode
  - TC 167 +0x0C = CTCSpecialAbilities::GetAbilityMutexLock (inline)
- Summoned creatures:
  - TC 145 = CTCSummonSpell, Minion at +0x11, inline GetSummonedCreature() by value
  - TC 154 = CTCGhostSword, Minions vector at +0x10, inline GetSummonedCreatures() by value
  - 0x008AC540 = vector<CIntelligentPointer<CThingAICreature> > copy ctor
- 0x004A3730 = CWorld::AutoLoadInternal, 5 bytes, `jmp LoadGameState`. The catalog's 0x004A34F0 extent runs into it
  (SPLITS.tsv; graded with QLEN=564).

## Name fixes (NAME_FIXES.tsv)
- Landed definers and models:
  - `?M@C006b2bd0@@QAEMXZ` should be CEnvironment::GetRainStrength.
  - `CGameEvent::GetData(CMemoryBuffer&)` should be GetDataPackage(CGameEventDataPackage*).
  - `CMemoryBuffer::Read` should be CGameEventDataPackage::GetData.
  - `CGameCameraManager::PlayCameraSound` should be PlayCameraSoundThroughObservedThing.
  - long-typed CPlayer::RemoveMode stand-ins should take EPlayerMode.
- Catalog names for 0x006B1A20, 0x006B2260 and 0x004A5E90 (q.py labels 0x004A5E90 "AutoLoadRegionLeaving").

## Parked rows: residue and what to try next
- **0x004A5E90 AutoLoadRespawn**, DIFFER(1722v1722).
  - Residue: one instruction. In the manual-save LoadGameState copy, c2 hoists `lea ecx,[esp+10h]` above the `je` to the shared
    path destructor. Retail keeps it at 0x004A6242. The other three copies match.
  - Tried: four LoadGameState body spellings, plus const-ref, bool and `!` forms of the manual branch.
  - Next: find what makes retail's push register differ, or a block shape that stops the successor-prefix hoist. The landed
    LoadGameState body has to stay as it is.
- **0x004A34F0 AutoLoadRegionLeaving**, DIFFER(564v564).
  - Residue: frame order only. Retail has fullPath+heroState at F+0 and path at F+4.
  - Packer trace: slot weights are path 11 and fullPath 7 + heroState 4 = 11. That is a density tie, and c2's qsort breaks it
    the other way.
  - Next: one more IL reference on heroState, or one fewer on path, with no code change. The path refs are in the landed
    LoadGameState, so look at heroState first.
- **0x0062BEF0 EAUseProjectileWeapon**, DIFFER(483v483).
  - Levers that worked:
    - `{ CPlayer* viewer = GetPlayer(..); if (viewer->HasMode(..)) return; }` gives retail's push order.
    - `IS_THING_ALIVE(&*thing)` binds the template's const& to a temporary, which homes in the dead event slot.
    - Disjoint scopes for ranged and camera give an 8-byte frame.
  - Residue: the camera out-local joins the this-spill slot (F4); retail has it in ranged's (F0). Packer: ranged, this and
    camera all weigh 2, ordered FIFO, so camera joins the latest slot.
  - Next: raise `this` to weight >= 4, or find the spelling where camera precedes this.
- **0x0062BD80 EAControlledCreatureStrafe**, DIFFER(337v354).
  - Residue 1: retail materialises the has-weapon bool (`mov al,1` / `xor al,al` / `test al,al`), with the weapon in ebp zeroed
    first. Every named-bool, forceinline-helper and assignment form folds it.
  - Residue 2: retail packs uid with action. Ours packs uid with the C3DVector temporary, because the uid node conflicts with
    action.
  - Next: a typed reader on the data package (FableWin has GetULONG / GetFloat / GetUHUGE ... forwarding to GetData) may give
    retail's node set. The bool shape suggests an inline bool-returning accessor that was not force-inlined.
- **0x008A8260 GetHeroSummonedCreaturesList**, DIFFER(456v466).
  - Lever that worked: the same-TU noinline vector copy body took it from nd=96 to nd=7.
  - Residue: retail duplicates the 10-byte epilogue for the null/dead-hero exit and returns `bl` there. With a second
    `return found;`, c2 constant-folds it to `xor al,al` and drops ebx.
  - Next: an early-exit form that keeps found live (unfolded) on that path.

## Header debt
- CTCFollowed (TC 188, GetFollowers 0x006D5850) has no shared header yet; 0x008A7A70 needs it.
- CPlayerGui / CInputManager / the TC 0xF6 / 0xE8 / 0xFC / 0xC6 / 0x11 / 0x29 classes are needed by 0x0062B070.
- CScriptThing's inline ctor/dtor are still supplied per TU by the landed GameScriptInterface rows. Here ~CScriptThing is called
  out of line (0x004AA840), which the header's declaration already gives.

## Tools added ($A/tools)
- rdis.py: capstone disassembly of retail.
- probe.sh / nd.sh / pd.py: free c2prio object plus masked diff, with QLEN support.
- calls.py
- qq.py: q.py with QLEN.
- fr.sh / fr2.sh / fr.py / phook.py: c2 frame-packer node order, weights, joins and final offsets. Copied from agent1008b
  (hand886).
