# 2026-11-12 landing dm -- naming agent NF26: queue dh/di and FAM09's alias respellings, 135 landed rows changed, 11 renamed

Source: `naming/2026-11-12_agentNF26/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, FAM09_FIXES, hdr README).

**Part 1: queue dh + di, 8 entries (6 done, 2 partial, 0 rejected)**
- Self rows renamed: 0x005DA4C0 CTCWeapon::SetAimingTimeOverride, 0x005DA490 CTCWeapon::IsAimedForLongEnough, 0x00658A35
  CTCVillage::IsSchoolTime and 0x00658A80 CTCVillage::IsStoryTime (both were IsCookTime), 0x0079C740
  CTCBulletTime::GetTimeMultiplier (was `_unattributed::GetFloatThrough_...`). Their binders are respelled.
- 0x004FB150 CWorldMap::GetLoadedRegion: binder 0x0088E300 moves onto engine/CWorld.h + CWorldMap.h.
- 0x00662410 IsInMeleeAsAttacker (partial): 6 binders are respelled; 5 sit on stand-in classes and 2 were busy.
- 0x008780D0: record only (the row is unlanded).
- Found on the way: 0x004BBC00 is CMainGameComponent::InitLUA (self row and caller 0x00416953).

**Part 2: FAM09's 97 aliases.** Of the 155 wrong bindings in 117 rows, all but one now bind the PDB name: CWideString::NullString,
CScriptThing::NullString, DAY_ONLY_SUFFIX / NIGHT_ONLY_SUFFIX, STRIKE_EVENT_*, NFile::CurrentPath and the rest. The exception is
0x012295F0, an atexit thunk whose type is not modelled. Also fixed: 0x009974A0 NFile::CalcCurrentPath, 0x0099EC30 (the CCharString
copy ctor, which two rows bound as the char* ctor) and 0x0099AF00 (CWideString `operator const wchar_t*`).
- Catalog renames of five initializers: 0x0121AC70/ACA0/AD80/ADC0/ADF0 are RIGHT_ / LEFT_ / BACKWARDS_ / BACKWARDS_RIGHT_ /
  BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION. NF26 spelled them `` `anonymous namespace'::`dynamic initializer for '...'' ``; the
  coordinator respelled them to FAM09's `_static_init::<NAME>` convention. Their sibling rows 0x0121AC30 / ACD0 / AD20 / AD50 still use
  the older `_global::$E...` spelling.
- Verified but not applied: 0x0121A670 is NThingManager::UIDsToReplaceWithUID and the real CThing::TCsToBeRemoved is 0x0121A440;
  both need tree and list element types (REJECTED.tsv).

**Headers** (opt-ins only, all additive, the macros new): CTCWeapon.h AIMING, CTCVillage.h SCHOOLSTORYTIME, CScriptThing.h NULLSTRING,
CMainGameComponent.h INITLUA, CLevelLoader.h GETGLOBALSECTION. NF26's overlay started from FAM09's, so FAM09's seven headers were
merged against that copy.

**Verification:** all 140 touched rows (135 changed plus the 5 catalog-only renames, whose files moved) recheck 3 MATCH and 137
RELOCATION_MATCH, every test PASS (`logs/recheck.log`). naming/rebind.py: removed 363, added 303. bindings_multi_check stays at its
baseline, and funclet_includes_check finds 1317 checked, 0 broken.
**Open (NOTES):** 0x00416056's `// Leaf:` line is stale (PB_W vs the object's PBG); FAM10 took it with the five stand-in binders.
