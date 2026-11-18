# 2026-11-17 landing nf70: name-fix round 7, NF70 (87 rows: follow-up entries queued at nf62-nf66, NF64's held rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF70 took the 95 follow-up entries queued at the nf62-nf66 landings (q10724..q10818) and NF64's 14 finished rows that were held until
their callers landed: 174 rows, plus 9 it claimed. Brief, entries, worklist and records: `naming/2026-11-17_agentNF70/`.

## Rows (STATUS.tsv, 183)

| Status | Rows |
|---|---|
| ported and landed | 87 |
| ported, held by the coordinator | 1: 0x006C9B70 (below) |
| not-a-binder | 55 |
| blocked | 40: 3 blocked/header, 37 blocked/port (STATUS.tsv gives each reason) |

**Among the landed rows:**
- NF64's held sets, landed with their callers: six STLport tree `operator=` rows, 0x00868CA0 `CHitParameters::AddDamageWithAttribute`
  with 0x007E2600, 0x00A90F00 `C3DPrimitive2::RemapBoneIndices`, 0x00694020 `CCreatureActionBase::FrameUpdate` (`MAEXXZ`, as both PDBs
  give it) with 0x008D5000 `CCreatureAction_FireMissileWeapon::FrameUpdate`, 0x00467831 `vector<bool>::_M_insert_overflow` with its
  caller 0x004676EF, and 0x004E9021.
- 0x0081F910 is `CMap::GetStrongestEngineThemeAt` (was `GetDominantEngineThemeAt`); the two rows that defined its body again now get
  it from one CMap.h opt-in (`GETSTRONGESTENGINETHEMEAT_BODY` / `_DEFINE`), so it has one definition.
- Made-up names replaced by PDB names: 0x00AC3AF0 `C3DClothPrimitive::RemapBoneIndices` (was `_global::Forward_RemapBoneIndices_via0x4`),
  0x00C26580 `CPooledResource::Deactivate` (was `NScript::CIsThingAlive::operator()`), 0x00C2DDE0 `PKCTRList<CSegment>::~PKCTRList`,
  0x00B430A0 `CCountedPointer<CStaticMapBankStateBlock>::DeleteData`, 0x0069A8B0 `CGameCameraManager::PushCombatCameraMode`, 0x00662450
  `CThingCreatureBase::SetPItemToUnsheatheAfterCutscene`, 0x00C10140 `CItemDispatcher::FindItem` (was `CNamedSet::FindById`), and others.
- 0x004E8FCD `CPersistContext::TransferHelper` is descriptive (no source has the original name; NOTES).

**0x006C9B70 is held:** NF70 matched it by adding a new local `std::_Nonconst_traits`, which stlport/fable_containers.h already
defines. That is a new local model of a class that has a shared header, so the row stays as it was. The draft is in `held/`, and the
port onto the stlport headers is queued. NF70 flagged two similar rows, 0x004676EF and 0x006C31B0. They only fix declarations
inside STLport models those rows already had (template parameters, a `class` keyword, one member template), like the ~15
access / const fixes NF70 lists in NOTES, so they landed (`logs/recheck_extra.log`).

## Entries (APPLIED.tsv, 95): 61 done, 10 done-already, 24 blocked
q10747 (0x006C9B70) is back to blocked with the held row. The blocked list is in APPLIED.tsv. Among them: q10763 (CBasicString
`operator<` 0x00429950 needs its body as an opt-in in the root fable_string.h, exact change in `held/q10763_change.txt`), the libpk
sound / bank rows and the four CEventTriggerRef rows (the CNamedSet vs CItemDispatcher ruling).

## Rows and checks
- **Section 1: 45 self renames (52 file moves). Section 2: 100 respelled bindings.**
- **Coordinator corrections:** 0x00693250's module is the catalog's `` `anonymous namespace' `` (NF70 wrote `(anonymous namespace)`);
  the two `std::find` instances 0x0053C8F8 / 0x0053C91C carry their template arguments in the catalog name, as the other `std::find`
  rows do; 0x004676EF / 0x006C31B0 keep their old catalog names (leaf-only changes).
- **Asserts:** three overlays (CHeroSuitDef.h, CSpecialEffectsDef.h, CMap.h) moved a `FABLE_STATIC_ASSERT` to another line. The
  coordinator pinned each to its old line (`lane_merge/pin_asserts.py`: `FABLE_STATIC_ASSERT_AT(<old line>, ...)`), so its typedef
  name is unchanged. The landing prep script now reports moved asserts.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`, `logs/recheck_extra.log`):** all 87 rows graded from the repo after landing: 83
  RELOCATION_MATCH and 4 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 150 binding lines and added 119; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (13 opt-ins in 10 files; default path token-identical)
CHeroSuitDef, CSpecialEffectsDef, CTCHeroOnlineScoreboard (one each), C3DClothPrimitive and CParticleProgram (RemapBoneIndices),
CPersistContext (TransferHelper), CMap (3), CWorldMap (2), CTCCreatureModeManager (ALLOCATEMODE_BODY), libpk/CItem.h
(CITEMDISPATCHER_FINDBYKEY). Details: `naming/2026-11-17_agentNF70/hdr/README.md`.

## Coordinator items (NF70 NOTES)
- 0x00C265B0 / 0x00C265E0 need their callee bodies earlier in the same object so the compiler knows ecx is preserved (DIFFER 28v17).
- 0x00693250 and its caller 0x00694430 use an anonymous-namespace helper whose decorated name carries a per-file hash: as separate rows
  they cannot link.
- CreateBuilding 0x0072DF50's access is inferred (private, like its siblings), not proven.

## Name fixes queued (`land1117fu`, 14)
NF70's NAME_FIXES.tsv (12 callers outside its slice that still bind the old spellings), plus the 0x006C9B70 port and the q10763
fable_string.h change above.

## Rule slips
- A vland run with a relative log path wrote `work/vland_l.log` into the repo's ignored `work/` directory; the coordinator moved it
  out. Nothing was committed.
- NF70 wrote `/tmp/x1` outside its directory (removed). One Bash call that tried to overlay fable_string.h was refused by the
  permission check (nothing written). One Bash call exited 2.
