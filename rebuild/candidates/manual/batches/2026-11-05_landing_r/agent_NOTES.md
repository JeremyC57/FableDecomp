# Chunk 791 notes

Four rows RELOCATION_MATCH (src/ + test/), two parked (differ/). Every function and callee name below is in the Ego_r.pdb and
FableWin.pdb name sets plus a second source (RTTI slot, caller, literals, layout); names marked (descriptive) have no retail symbol.

## 0x009A1720 CStreamingFontBank::PrepareToRender(bool) const -- RELOCATION_MATCH (1 attempt)
- Shape: GFGetSystemManager()->GetDisplayManager()->GetRenderManager() (the landed CSurfaceCompositor::SetBlendMode 0x00F38A10 chain),
  then 22 inline CRenderStateManager setters in retail order; the textured block under `if (flag)`.
- Spellings: ?PrepareToRender@CStreamingFontBank@@UBEX_N@Z (vtable 0x0129AB54 slot 3); GFGetSystemManager 0x009A4EC0
  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ. Opt-in FABLE_RETAIL_CSTREAMINGFONTBANK (hdr/).
- Descriptive: the parameter name `textured`.

## 0x00AB83A0 CStaticFontBank::PrepareToRender(bool) const -- RELOCATION_MATCH (1 attempt)
- Shape: the streaming sibling + TextureUVModeU/V wrap, CRenderManagerCore::SetVertexFormat(0x144) 0x00A0AA30 (out of line),
  TextureCoordIndex(0,0), modulate colour stage. ?PrepareToRender@CStaticFontBank@@UBEX_N@Z (vtable 0x0129EAD4 slot 3).
  Opt-in FABLE_RETAIL_CSTATICFONTBANK (hdr/).
- Descriptive: the parameter name `textured`.
- Label note: vland warns RELOC_NAME for 0x01396DC8: the header symbol ?D3DTextureUVModeTable@CRenderStateManager@@0PAKA vs the
  descriptive label g_textureUVModeToD3D; proposed relabel in LABELS.tsv (the sibling D3DTextureFilterModeTable label comes from a PDB
  DATA citation; confirm this one with cvdump first).

## 0x004963A0 CFrontendGameComponent::Input() -- RELOCATION_MATCH (12 attempts)
- Shape: `if (EventScanStart()==true) do { if (handled==true) break; switch (Mode) {...} } while (EventScanNext()==true);` then
  EventScanEnd. Same-TU bodies decide the bytes: GetMaxVisible{Profile,World,Save}Number visible and not inlined (ecx survives),
  SetMode 0x00495FE0 over ClearString 0x004944C0 (expanded at the two mode-10 sites, called for mode 11), and GFWrapLimit<long,int,int>
  as a `__declspec(noinline)` same-TU body (retail's own instantiation at 0x00496EB0) so the 1/7 temporaries pack into one slot pair.
- Callees: see the source header (InputManager Update slot 2, EventScanStart/Next/End, CInputEvent accessors, the CFrontendGameComponent
  privates, SetQuit vtable slot 4, GFWrapLimit 0x00496EB0, g_KeyToNumber 0x013B87B0 = established spelling, no PDB data names).
- Descriptive: locals `scanInfo`, `event`, `unusedEvent`, `handled`; enum values written as numbers (no enumerator names in the PDB extracts).
- Labels to correct: 0x004944C0 is ClearString (catalog ClearTextInput); 0x00496EB0 is ??$GFWrapLimit@JHH@@YIXAAJABH1@Z (landed
  as GFWrapLimit_00496eb0 with pointer params); landed GetMaxVisible* Leaf lines use QBE, the PDB has ABE (private); landed SetMode
  0x00495FE0 models GetSaveNames 0x00495830 / GetProfileNames 0x004959E0 as Refresh members and +0x1E44 as m_ModePointerEnabled
  (it is CursorHighlight). All in NAME_FIXES.tsv.
- Header debt: CFrontendGameComponent has no compilable retail-layout header (STLport vectors); the class is modelled locally in
  the source with the retail offsets ProfileNames +0x1E0C, SaveNames +0x1E1C, SaveToLoad +0x1E2C, WorldNames +0x1E30,
  WorldToLoad +0x1E40, CursorHighlight +0x1E44. Opt-ins FABLE_RETAIL_CINPUTEVENT_ACCESSORS / FABLE_RETAIL_CINPUTMANAGER_EVENTSCANEND (hdr/).

## 0x00E49020 NScript::CV_BanditTollScript::CTauntingBandit::Main() -- parked DIFFER(2424v2084), 11 attempts
- Shape: SetPredicate(CIsThingAlive), loop with terminate checks; BanditsFlee: MoveToPosition("M_BanditEscapePoint") wait loop until
  IsDistanceFromThingToPositionOver(..., TollBanditCloseToExit + 2) fails, FadeOutAndKillEntity(TollBanditTimeToFade); BanditsAngered:
  EntitySetInFaction("FACTION_BANDITS") + FollowThing(hero); TollPaid: CTimer taunt timer, IsPlayerThreateningEntity ->
  WARNING_10/20/30/ANGERED_10 by AngerCount (4-entry jump table), else NO_LONGER_THREATENING_10 and rand()%300 random taunt when
  IsDistanceBetweenThingsUnder(TollBanditPayDistance).
- Residue: the per-exit inline split of ~CTimer 0x00CD4470 / ~CScriptThing 0x004AA840 / handle dtor 0x007E74D0. Best (f) matches the
  final exit and every TollPaid exit; 16 of 48 exit pads differ in the Flee/Angered blocks (one notch too hot); TimerIndex cached in ebp.
- Labels to correct: pGlobalScriptDef +0x43C / +0x440 are TollBanditCloseToExit / TollBanditTimeToFade (landed CSafeTrader::Main
  0x00E4A280 calls them SafeTraderEscapeDistance / SafeTraderFadeTime).
- Descriptive: the local CScriptDef model (only the four TollBandit fields), locals `escapePoint`, `tauntTimer`, `escapePos`.
- Header debt: no compilable CScriptDef retail header (the def is 0x34 lower than Ego_r in this region); modelled locally.

## 0x00EC32F0 NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs() -- parked DIFFER(2040v2016), 10 attempts
- Shape: CCharString bandit; std::vector<CCharString> defs; switch (Game->GetHeroRoughExperienceLevel()) (slot 175) with `default:` written
  first (level 3 grunts/archers/leader), cases 0..8; grunt strings from ChooseRandomBanditGruntString 0x00EC3AD0; then
  bandit = defs[BanditCreatureGeneratorIndex % defs.size()]; ++BanditCreatureGeneratorIndex (unsigned, +0x6C).
- Residue: VC7.1 keeps the zeroed vector words across the virtual call (first default site folds, ebx = 0, edi = _M_finish, extra
  push ebp); retail reloads the vector words at every site.
- Descriptive: locals `bandit`, `defs`; the local class model (Game +0x40, BanditCreatureGeneratorIndex +0x6C).
- Header debt: CV_RandomPopulationSimScript has no retail-layout header (modelled locally, Ego_r offsets - 0x14).

## 0x00E91F20 NScript::CV_GuildMasterScript::GetGuildMasterSpeech() -- RELOCATION_MATCH (5 attempts)
- Shape: switch (MasterScriptData->PostSavePosition) over EGP_ enumerators; sub-choices IsQuestCompleted (slot 286),
  IsQuestActive (slot 284), SeenAbbeyMotherAtGuild, the CurrentHeroSoulsPosition if-chain; each sub-body starts with
  `if (IsActiveThreadTerminating()) return nothing;`; `default: return "";`; tail: repeat check against LastDialogueSaid, rand()%4,
  GuildMasterDialogue[2..4] info lines, then GetHeroTitle() (slot 205) == 19 && ![1] -> INFO_HERO_TITLE.
- Key trick: `CCharString nothing = CopyString(CCharString("", -1));` -- an inline by-value pass-through of a const& keeps retail's
  temporary + copy + dtor; every direct spelling is elided by VC7.1.
- Descriptive: CopyString (expanded, no retail symbol), locals `nothing`, `speech`, the `(EHeroTitle)19` cast (no enumerator name proven).
- Data: CQ_SunnyvaleMasterData PostSavePosition +0x04, SeenAbbeyMotherAtGuild +0x60, CurrentHeroSoulsPosition +0x70 (shared header).
