# chunk 802 notes

Two rows matched (RELOCATION_MATCH), three parked in differ/ with residue, one not drafted. Names are Ego_r.pdb spellings unless marked
**descriptive**.

## 0x007ABBD0 CTCAIScratchpad::OnSerialise -- RELOCATION_MATCH (src/)
- Shape: only the hero's scratchpad (ParentThing->UsingComponents bit TCI_HERO_STATS, `test byte [thing+20h],10h`) persists the five
  statics: ActiveGossipCategories / IsGossipForPlayer through TransferMap<bool, CCharString> 0x007AE3D0, then Gossip / GossipVillages /
  GossipFactions as "<T>_num_vectors", "<T><i>" key and "_<T><i>" phrase vector. Each name is rebuilt from its literal at every use.
- Key spellings: one `long num` at the hero-if scope; `++it, ++i` in the for header; GetDefaultVal<CCharString> as a named local.
- Statics (Ego_r dynamic initializers): 0x013BAE2C / 38 / 44 / 50 / 5C.
- Label: catalog `vfunc_1` -> CTCAIScratchpad::OnSerialise (LABELS.tsv).

## 0x008EACB0 NCreatureMode::CModeHoverBase::OnActivate -- RELOCATION_MATCH (src/)
- Shape: base OnActivate 0x0073AAC0; the player re-initialises movement speeds and finishes its current animation; three IDLE sectors
  and per speed group (only when "<prefix>FORWARD_<SPEED>" exists, GetAnimationFromCreature 0x008618B0) FORWARD/RIGHT/BACK/LEFT
  sectors; the best state for heading (1,0,0); a random anim of CurrentName as the main anim unless performing an action; the
  special-effects def's effect spawns a particle emitter attached at the helper point.
- Key spellings: `CThing& thing = ParentThing` (no reload); GetRandomAnimFromName(CurrentName, -1); same-TU noinline
  GetSubDef<CSpecialEffectsDef>; sector bounds as N.0f/360.0f.
- Unbound callees given their PDB names: 0x008618B0 ?GetAnimationFromCreature@NCreatureMode@@YIPBVCAnimationEntry@@ABVCCharString@@ABVCThingCreatureBase@@@Z,
  0x008EA500 ?InitialiseMovementSpeeds@CModeHoverBase@NCreatureMode@@AAEXXZ. None of the new symbols is bound at another address.
- Local classes (no shared header yet; header debt): CModeHoverBase (CRadialArea, CHoverState, members HoverStates, CurrentName,
  PCurrentState, CurrentMovementType, AnimQueue, PParticleEffect), CAnimInit (Ego_r members; inline ctor -> Init 0x0070C050, respelled from an invented Set at hand-back, still RELOCATION_MATCH), CAnimateThing, CTCAnimationComplex,
  CTCDParticleEmitter. The source redeclares `operator+(const CCharString&, const char*)` that fable_string.h already has (harmless).
- Name fixes: 0x0073AAC0, 0x0073A680, 0x00662D80, 0x008EC1A0 (NAME_FIXES.tsv).

## 0x00684610 CTCCreatureOpinionOfHero::OnSerialise -- parked DIFFER(2826v2823), 122 difflines
- Identity: the catalog's COpinionReactionManager::TransferOpinionData is propagated; the only reference is slot 1 of vtable 0x0125BC3C
  (RTTI CTCCreatureOpinionOfHero, stored by the ctor 0x00684420); Ego_r donor 0x00592120.
- Shape: flags / hit counters / forced attitude (3-arg Transfer with defaults false, 0, 0.0f, -1.0f, 0x7fffffff, 0x12), OpinionValues
  vector on a full save, then (full save only) PendingDeeds as "count,deed,...", LastFrameAppliedDeedEffect[0..91], KnownDeeds as
  "(index:count:,deed...)" groups, TransientOffsets as a counted list, and the three flag masks through SerialiseFlagMask 0x00681BA0.
- Residue: same instruction stream; slot assignment (known / itemName strings in the dead argument slots, the cond-temp flag at
  +0x14), the two default temporaries hoisted in retail, and the per-site inline split of Transfer<CCharString> 2-arg 0x00411010 and
  list ~ 0x004046B0. Eight attempts logged in STATUS.tsv.

## 0x0066CF00 CRegionMinimap::PopulateRegionMinimap -- parked DIFFER(3252v3248), 671 difflines
- Shape: per cell, the strongest engine theme under the cell's world position (GetStrongestEngineThemeAt 0x0063ADF0, strength word
  kept across cells) is fetched through an inline CDefinitionManager::GetDef into a CDefPointer<const CEngineThemeDef> kept across the
  loop; if IsPosInMap, CEngineThemeDef::MinimapTheme (retail +0x90 = PDB +0x94, EMinimapThemeType) maps 1..8 -> 1,6,4,5,2,7,3,8; the
  RegionMinimapLightingMap table 0x0137E9D8 ({eTheme, useLighting}) decides IlluminateRegionMinimapTheme 0x0066C900 or 0.
  Then every CThingBuilding (GetAllThings<CThingBuilding, vector<CThingBuilding*>, CThingFilter_None> 0x0066E5A0 on
  PMainGameComponent->PeekWorld().PeekThingSearchTools()) with a physics mesh: the four box corners pushed to a vector<C3DVector> and a
  min/max box computed over them (dead in retail too: never read), the bounding circle's square clipped to the region box (C2DBoxF::ClipTo
  0x0066C560), converted to a TGA cell range grown by one, and each cell whose ground point (GetGroundSizeZAt + 100) hits the mesh
  (GetPointIntersectionFromTo) set to theme 8.
- **Descriptive**: the table's name g_RegionMinimapLightingMap (Ego_r has the struct RegionMinimapLightingMap, not the array's name),
  the RegionMinimapTheme values (written as casts; enumerators in neither PDB), the local names.
- Residue: x / def register swap (ebx / ebp), the GetDef assignment caches the object in edi (retail rereads the temporary: the
  template operator=<U> -> Assign shape), 8 frame bytes, corner-loop base. 0x00A11AE0 GetBoundingBoxCentre has no relocation binding
  (`call 0` in the diff) although its row is landed.
- Header debt: CEngineThemeDef has no shared header (local pad class; MinimapTheme +0x90). CRegionMinimap's cell list stays opaque
  (CellAt reads the first-pointer word).

## 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update -- parked DIFFER(2680v3080), 907 difflines (first compiled draft)
- Shape (from the listing): minion family + three fireball def indices by name; TCI_BOSS / TCI_COMBAT / TCI_TALK components; the
  battle def (GetSubDef<CJackOfBladesBattleDef> 0x008C33E0: BattleFloatHeight +0x50, BattleScale +0x54, ScaleUpTime +0x58 = PDB - 4);
  reset to the arena centre (physics slot 31 +0x7c), face the cutscene marker or the target (atan2(dx, dy)/2pi wrapped, slot 66
  +0x108), gravity off (slot 38 +0x98), JackFloatUp, scale up per frame, boss phase 2, wait one second (frames vs ConstantFPS
  0x01375550 as floats), SummonPillars / SummonPillarsOld by Pillars.size(), phase 3, will-response flags off, then the attack loop:
  counter from 6, `counter % 7` table 0x00975678 (0: ++rounds then 1-2 left fireball, 3-4 combo fireball, 5 flash then Summon when no
  helper lives and rounds > 1, 6 idle), ST_IDLE, taunts (table 0x00975694, SpeechCounter % 7, five TEXT_QST_039_JACK_BOSS_FINAL_TAUNT_*)
  and SPECIAL_BOAST on a knocked-down target, EnemyKnockDownFrame; finally KillHelpers, phase 4, Damageable / RespondToHits off.
- The AIAction_PerformActionAndWait `wait` bool is pushed before each action ctor's own arguments; with that the ctors match Ego_r:
  JackFloatUp(creature, float), JackShotFloat(creature&, target&, const CCharString&), JackFlash(creature&, target&),
  PlayCombatAnimation(creature&, target&, const CCharString&, bool).
- Unproven: the physics slot names (ResetPosition / SetPosition / SetFacingAngleXY / EnableGravity are the CTCPhysicsBase header's
  slots 30/31/66/38 by offset only); the EAIActionReturn / EAIActionRepeat enumerator names (casts).
- Residue: VC merges the two facing branches (retail keeps both copies), unbound callees, the action temporaries' slots.
- Header debt: CTCBoss and CTCWillResponse have no shared headers (local declarations); CAIStateGroup_JackOfBladesBase / Stage2 are
  flattened generated headers -- the row models them locally on the retail CAIStateGroupBase with the Ego_r member names.

## 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI -- not drafted
- Budget ran out after the other five rows. Shape for the next author (from the listing): clear the entry list (+0x10, list _M_erase
  0x00567980), ScoreboardState +0x50 = 0, CFrontEndManager::GetInstance 0x0041E5F2; the hero's GUI component (TC 0x11 via LowerBound,
  interface state +0x21c == 8) and +0x58 non-null; GUI +0x214 slots +0x98 (colour 0xffffffff, 1.0f, 0) and +0xc; +0x64 slot +0xc0(0);
  IsScoreboardEmpty 0x0055E7F0 -> "TEXT_GUI_MENU_CLAN_EMPTY" through the text bank (+0x14 of PMainGameComponent, GetTextBySymbol
  0x009C95E0) and DisplayErrorMessage 0x0055EF00, return; else build "PC_SUB_LIST" / "PC_UI_SCOREBOARD1_FRAME" / "PC_TITLE" components
  (CreateComponent 0x0041DB1D), a CAction (new 0x10, ctor 0x0042BE50, type 0x15) pushed to the list's action vector, then per clan entry
  (map find 0x00566FE0 / operator[] 0x0056A680 on +0x44) the menu-entry template (CreateMenuEntryComponent 0x00428494, GetEntryName
  0x0055E7C0), score text (GetScoreAsString 0x0055E6B0), score holder / button, subtitle, description (the catalog's GetLookDirection 0x00567A40
  is probably mislabelled: it is called like a map lookup on +0x40, unverified). Model on the landed ShowScoreboard 0x00563130 and engine/NUISystem_CComponent.h.

## Labels to correct
See LABELS.tsv (0x00684610, 0x007ABBD0, 0x0062E900 and the CRegionMinimap / Jack of Blades callees).
