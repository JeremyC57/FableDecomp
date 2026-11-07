# Chunk 842 notes

Result: 1 of 6 matched. 0x00D40CF0 is RELOCATION_MATCH (src/). Five rows are parked (differ/, each with a Grade / Next-idea
header and a test). All six tests print PASS under g++ (C++98) and under VC through tall.py. Every attempt is in ATTEMPTS_LOG.tsv,
with probe sources in probe/.
Diagnostic `/d2` flags were used only as meters, in tmp/diag copies. No row keeps one, and no row has a `#pragma inline_depth`
region.

## 0x00D40CF0 NScript::CQ_GuildTrainingScript::CMeleeApprentice::Main -- RELOCATION_MATCH (src/00d40cf0.cpp)
- Shape:
  - seh_me start loop, then sheathe(false), FACTION_HERO, killable(false, true), boss phases off and SetFriendsWithEverythingFlag(true)
    (CScriptThing slot 67, not SetAsUsable 66);
  - marker = "MeleeApprenticeMarker";
  - then `for (;; Game->NewScriptFrame()) { if (T) return; ... }`. This is the GOTCHAS 2376 form. It puts the expanded
    ~CScriptGameResourceObjectScriptedThingBase pad at the loop exit, as retail does (attempt l). A `__forceinline` row-local
    CScriptThing::GetPos expands the two marker.GetPos() sites (attempt m).
- Per-frame body:
  - MasterData->MeleeApprenticeNeededForCutscene block, then ParentClass.StartedMeleeTesting block: each releases, waits, then
    retakes control;
  - Q_GuildTrainingWoodsWill branch: run once to MK_GTM_WD_HEROWALK (3.0, ENTITY_MOVE_RUN), else the SCORPION moan when talked to;
  - otherwise the SKILL / WILL / departure (END for woods departure, else MELEE) moans when talked to, then a run to the marker;
  - finally, a run back when IsDistanceBetweenThingsOver(Me, marker, 4.0f) and idle.
- Callees, checked with bindchk / relpair (39 relocations, every one bound to retail's target; the only MULTI is NullCoord's
  three-word symbol):

  | Address | Callee |
  |---|---|
  | 0x007E72F0 | MoveToPosition |
  | 0x007E7360 | ClearCommands |
  | 0x007E7390 | Speak |
  | 0x007E7450 | IsPerformingScriptTask |
  | 0x007E7490 | GetScriptThing |
  | 0x007E74D0 | ~ScriptedThingBase |
  | 0x00CD23B9 | IsValid |
  | 0x00CD2770 | Release |
  | 0x00CBE3EA | IsDistanceBetweenThingsOver |
  | 0x00F35B30 | CEntityScriptBase::IsActiveThreadTerminating |
  | 0x006E7B60 / 0x006E7B80 | movie ctor / dtor |
  | 0x004AA840 | ~CScriptThing |
  | 0x0099A2E0 / 0x0099A380 / 0x0099A430 | CBaseClass / CBaseObject |

- String-literal order checked by hand. pending/..._hand838 bcheck.py (with the chunk 842 oracle) shows 0 reloc-masked byte
  differences.
- Header: engine/CQ_GuildTrainingScript.h opt-in MELEEAPPRENTICE (hdr/README.md).
- Names: all from Ego_r.pdb (members, class, Main) plus the vtable. The local `marker` and the macro names MA_T / MA_START /
  MA_MOAN are descriptive.

## 0x00EC4A00 NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main -- parked DIFFER(1524v1540), 15 lines
- Shape, in order:
  1. CSpawnedFunc1<CV_RockTrollFirstEncounterScript, CScriptThing>("ParentClass." + "WatchForRockTrollKilled", Me);
  2. the start loop;
  3. the two RockTrollReward items (AddedItemsToRockTroll);
  4. the exhume cut scene (seh_hero, map<CCharString, ScriptedThingBase> HERO / TROLL, movie, CWideScreenMagicPauseEntities,
     RunCutsceneMacro("CS_ROCKTROLL_EXHUME"), spawn WatchForRockTrollHit, PlayedExhumeCutScene);
  5. SPECIAL_BOAST / SPECIAL_IDLE with GDefaultToUsingPhysicsInCutsceneAnimations;
  6. the wait, release, EntityForceToLookAtThing / GiveThingBestEnemyTarget on the hero, then the idle loop.
- Residue: per-site CScriptThing copy split, the documented copy-inline blocker.
  - Retail expands the first spawn's by-value argument copy. It calls 0x004ABE90 for the second spawn's copy and for the
    CSpawnedFunc1 Par1 member copy.
  - With the copy ctor out of line (d), only the first site differs.
  - Every inline spelling expands all three sites: row-local inline (b), with CBaseClass init (e), explicit temporary (f), and
    FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (g). None of these spellings is in GOTCHAS for this case.
- Header: engine/CV_RockTrollFirstEncounterScript.h (NEW, under FABLE_RETAIL_NSCRIPT).
- Names: Ego_r.pdb names for the class, members and methods; GDefaultToUsingPhysicsInCutsceneAnimations from the chunk 838
  NAME_FIXES; the label is corrected (LABELS.tsv).

## 0x00E5E5A0 NScript::CV_BodyGuardScript::MakeHenchmanPerformIdleComment -- parked DIFFER(1594v1580), 116 lines
- Shape: as on line 1 of the differ file.
  - CTimer `operator=` / `==` and CCounter `operator>` are inline.
  - MakeHenchmanComment (the next function, 0x00E5EBD0) is called with HM_COMMENT_MULTIPLE_RANDOM / MULTIPLE / SINGLE.
  - The henchman scan copies each CScriptThing.
- Residue: per-exit ~CScriptThing inline split.
  - Retail expands the BORED temporary and the GOOD exit only. It calls 0x004AA840 at the QUESTION temporary, the EVIL exit
    and the loop end.
  - Ours is all-expanded (b) or all-called (a).
  - The diagnostic inline-threshold meter (T60) reproduces retail's split exactly, which makes this the brief's documented
    inline-budget park. T50 / T66 and S10 / S12 are non-monotonic.
  - The else-if (c) and nested-if (d) spellings, new and not in GOTCHAS, are byte-identical to b.
- Names: Ego_r.pdb `?MakeHenchmanPerformIdleComment@CV_BodyGuardScript@NScript@@QAE_NXZ`, cross-checked in engine_api.tsv and the
  BSim port audit. Member names come from the CV_BodyGuardScript header. Locals `start`, `henchman`, `i`, `j` are descriptive.

## 0x0092F310 CAIStateGroup_MinionReactToPlayerInteraction::GetGossipPhrase -- parked DIFFER(1994v1945), 288 lines
- Shape:
  - the village name: TCI_VILLAGE_MEMBER -> GetPVillage -> GetParentThing().PeekBaseDef().GetInstantiationName() through
    GDefStringTable;
  - the faction name: TCI_ENEMY -> CTCEnemy::PFaction (+0x18) -> CFaction::Name (+0);
  - the loop over CTCAIScratchpad::Gossip with four filters:
    - active (GetActiveGossipCategories);
    - for-player (GetIsGossipForPlayer);
    - villages (GetGossipVillages, std::find);
    - factions (GetGossipFactions);
  - `PeekTC(TCI_TALK)` with GetDialogueVoice() == "": return false;
  - the phrase scan: CBankFile::FindIndexBySymbolCRC(name.GetCRC()), then CDataBank::GetTextEntryFromGroup(index, voice, found)
    and again with voice + "_1_5", then push_back and break;
  - then GFRandom(size, GetPWorldSeed()) twice and phrase = FindIndexBySymbolCRC(...); return phrase > 0.
- Key spellings:
  - The Get* statics are the landed 0x008AE970 form, `iterator it = map.find(k); if (it == map.end()) return 0; return &map[k];`.
    This gives retail's `cmp eax,[map]`. `find(k) == end()` loads the header into esi first (b), and `end() == find(k)` reverses
    the operands (e).
  - GetGossipFactions is declaration-only, because retail calls 0x008AE970 at its one site. That brings the frame back to
    0x58 (g).
  - PeekBaseDef(), not GetPBaseDef(): GetPBaseDef copies the CDefPointer, which adds an inc / dec (a).
  - GFRandom is inline (`*seed = *seed*0x24A1+0x24DF; *seed = GFROR13(*seed); return range ? *seed%range : 0;`), Ego_r
    `?GFRandom@@YIKKPAK@Z`.
- Residue:
  - the Gossip iterator slot (0x48 v 0x3c);
  - categories._M_start is kept in ebx through the loop, so each continue reloads it and the villages continue is not merged
    into retail's shared pad at 0x0092F47B;
  - both GetDialogueVoice sites in the phrase loop are expanded, where retail calls 0x004FAEE0 and expands only the first test;
  - the std::__find tag is pushed as 0, where retail passes the dead phrase slot [esp+0x6c] (GOTCHAS chunk 149 lane gap);
  - the size()==0 return is not tail-merged.
  - Diagnostic /d2inlS15 and /d2inlS20 change nothing, so this is not the caller-size budget.
- Callees:

  | Address | Callee |
  |---|---|
  | 0x0040F020 / 0x004365B0 | CVectorMap::LowerBound (PeekTC / GetTC) |
  | 0x007EE540 | CTCVillageMember::GetPVillage |
  | 0x009D49B0 | CDefStringTable::GetString |
  | 0x007ACC90 / 0x008AC200 | _Rb_tree::_M_find, for map<CCharString,bool> and map<CCharString,vector> |
  | 0x008ADF10 / 0x007AEBE0 | map::operator[] |
  | 0x008AE510 / 0x008AE970 | CTCAIScratchpad::GetGossip / GetGossipFactions, landed |
  | 0x004B8FF0 | STLport __find<const CCharString*, CCharString> |
  | 0x004115A0 / 0x00411570 | CCharString compares |
  | 0x00404310 | CCharString::GetCRC |
  | 0x004014A0 | CCRC::Calc |
  | 0x009CC410 | FindIndexBySymbolCRC, PROBABLE (NAME_FIXES.tsv) |
  | 0x009C9280 | CDataBank::GetTextEntryFromGroup (FableWin.pdb) |
  | 0x004FAEE0 | CTCTalk::GetDialogueVoice (FableWin.pdb) |
  | 0x0099F600 | operator+(const CCharString&, const char*) |
  | 0x0044BFF0 | vector<CCharString>::push_back |
  | 0x00414EA0 | ~vector<CCharString> |
  | 0x008FCFD0 | CAIStateGroupBase::GetPWorldSeed |
  | 0x00497890 | GFROR13 |
  | 0x00BFEA14 | _free |

- Descriptive names:
  - the CTCAIScratchpad map member names (ActiveGossipCategories, IsGossipForPlayer, Gossip, GossipVillages, GossipFactions).
    The landed 0x008AE510 / 0x008AE970 rows use the same names and call them descriptive; Ego_r has only the Get* / Add*
    functions.
  - the locals village_name, faction_name, categories, category, active, for_player, villages, factions, talk, phrases, bank,
    index, found, chosen;
  - the GossipFind wrapper. The real STLport form is `std::find`. STLport std::find in c / d graded worse because of the tag
    temporary.
- Header debt:
  - CTCAIScratchpad.h is missing. It needs the five static maps at 0x013BAE2C..0x013BAE5C and the inline Get* bodies in the form
    above, with GetGossipFactions out of line.
  - CTCEnemy needs `CFaction* PFaction` at retail +0x18 plus FableWin GetPFaction; CFaction needs Name at +0 plus GetName.
  - NGameText::CDataBank should derive from CBankFile so that FindIndexBySymbolCRC is reachable. CBankFile.h is HOLD, so the
    differ uses a `reinterpret_cast<const CBankFile&>`.
  - A real STLport `std::__find` template instance is needed for 0x004B8FF0. The bound spellings `?__find@@YIPAV...` (0x004B00C0)
    and `?FindString@@...` (0x006E2CF0) are both non-template and non-std.

## 0x00EF9280 NScript::Cjoss_test_scriptsScript::Main -- parked DIFFER(1972v3158), 795 lines
- Shape:
  - ActivateQuest "guy_script" and "bandit_level_cutscene";
  - then `for (; !T; NSF)` with the fireh / summoner chain: StartScriptingEntity, then
    SummonerLightningOrbAttackTarget(fireh) when idle;
  - jim's MsgIsHitByWithFlourish / MsgIsHitByWithProjectileWeapon(distance);
  - the hero's MsgPerformedFlourish / MsgPerformedSuccessfulFlourish;
  - bob (EntitySetAsToAddToComboMultiplierWhenHit), my_generator (CreatureGeneratorIsDepleted), pinky / perky lookups;
  - the dan_bandit GiveThingBestEnemyTarget loop, once.
  - Retail calls CScriptBase::IsActiveThreadTerminating 0x00CB7940 18 times, ~vector<CScriptThing> 0x008AC970 once and
    CScriptThing::operator= 0x008AB980 5 times.
- Residue: per-site inline splits, all documented residue classes.
  - Retail expands ~CScriptThing at two exit pads only, operator= only at the fireh site, and IsAlive per site.
  - Ours calls everything (c) or expands everything (a / e: 3488 bytes, md ~1000).
  - The chunk 838 / 828 `for (;; NSF) { T; ... }` form (e / f) does not move the split.
- Header: engine/Cjoss_test_scriptsScript.h (NEW).
- Names: Ego_r.pdb `?Main@Cjoss_test_scriptsScript@NScript@@UAEXXZ` plus the TLC registration (CScriptInfoManager::RegisterScripts).
  The locals bob / jim / my_generator / dan / pinky / perky / summoner / fireh / bandits are descriptive, after the script names
  they look up.

## 0x00EF5FF0 NScript::CPersonalScript_EmptyScript::CCinemaMan::Main -- parked DIFFER(3409v3265), 47 lines
- Shape:
  - five GetThingWithScriptName (CinemaMarker1..3, LookAtMe1, LookAtMe2);
  - SetPredicate (0x00F35B10) of the six-way CIsThingAlive && chain (CPredicate_And<A,A> ctor 0x00E24760, ~CIsThingAlive
    0x00E241B0);
  - the start loop;
  - then per frame:
    - aggressive special-ability hit: "How very mean!";
    - else non-aggressive: "How very nice!";
    - else talked to: movie, pause, eight CM_SPEAK TEXT_QST_048_FATHER_INTRO_10..100, wait.
  - CScriptThing slots: 43 / 44 MsgIsHitBy*SpecialAbilityFrom, 27 MsgIsTalkedToBy.
- Residue: nested CPredicate_And temporary-dtor depth.
  - Retail expands one level of each temporary's dtor and calls the nested complete dtor (0x00EF6DE0 / 0x00EF6D90 / 0x00EF6D50 /
    0x00EF5B50).
  - Default header (a, md 1174): T5 one level plus a call, T4 called, T3..T1 expanded to the leaves.
  - Opt-in FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR (b, md 47): frame and registers equal retail's, but every level is expanded
    and there are no derived vtable stores.
  - Tried without effect: in-class dtor (c), explicit instantiations before / after Main (d, f), definition after Main (e),
    inline ctor / operator() / Clone (g), and diagnostic T40 / T60 / T80.
- Names: Ego_r.pdb Main and class names; the CCinemaMan entity labels come from the TLC registration "CinemaMan". The locals
  cinema_marker1..3 and look_at_me1 / 2 are descriptive, after their script names.

## Bindings and one-symbol-one-address
- Before binding each symbol, I checked that no other address binds it in reloc_bindings.tsv:
  - 0x004FAEE0: the landed 0x008A0CB0 binds it as `?GetBankHandle@CTCText@@...`;
  - 0x009CC410: seven spellings (CDataBank::LookupValue and others);
  - 0x00CB7940: eight wrong spellings;
  - 0x008AC970: nine spellings.
- All four conflicts are in NAME_FIXES.tsv with the PDB spelling.
- Row 6's own 39 symbols have no conflict.
