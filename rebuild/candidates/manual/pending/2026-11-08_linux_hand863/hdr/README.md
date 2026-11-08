# hdr/ overlay (chunk 863)

All changes are opt-in `#ifdef FABLE_RETAIL_*` branches or a new header. The default paths are byte-for-byte unchanged, so the
existing landed includers (84 sources include one of the changed headers) cannot change. They were not rechecked: no default
text changed.

| Header | Change | Opt-in macro | Evidence | Used by |
|---|---|---|---|---|
| engine/CPlayerGui.h | the first five members (World +4, PlayerManager +8, InputInterface +0xC, Component +0x10, Graphics +0x14) with Ego_r.pdb names/types replacing the pad, plus the ctor declaration and forward declarations | FABLE_RETAIL_CPLAYERGUI_CTOR | struct_layouts_egor.tsv; retail 0x0043B570 `ret 14h` stores | 0x0043B570 (src, RELOCATION_MATCH) |
| engine/CPlayerGuiDef.h | AbilityButtons typed vector<CAbilityButtonGuiDef> (0x5C elements) at +0x7FC | FABLE_RETAIL_CPLAYERGUIDEF_ABILITYBUTTONS | ctor sizes the button vectors with `imul 0B21642C9h; sar 6` (/0x5C) at 0x0043B95F; Ego_r.pdb CAbilityButtonGuiDef members | 0x0043B570 |
| engine/CSubtitleRenderer.h | ctor declaration CSubtitleRenderer(const CGraphicDataBank&) 0x0049166E | FABLE_RETAIL_CSUBTITLERENDERER_CTOR | Ego_r/FableWin ??0CSubtitleRenderer@@QAE@ABVCGraphicDataBank@@@Z; call at the +0x18 member | 0x0043B570 |
| engine/NHeroInformationScreens_CManager.h | ctor declaration CManager(CWorld&) 0x0066EAB7 and tail pad to 0x54 | FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_CTOR | Ego_r ??0CManager@NHeroInformationScreens@@QAE@AAVCWorld@@@Z; `new(0x54)` at 0x0043BA68 | 0x0043B570 |
| engine/NPlayerGui_CDrawBase.h | ctor declaration CDrawBase(CPlayerGui&, vector<CDrawBase*>&) 0x0064CC65 | FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR | Ego_r/FableWin mangled name in the header comment | 0x0043B570 |
| engine/NPlayerGui_CDrawNPCName.h | inline ctor (CDrawBase ctor, vptr 0x01231770, Name, ScreenShotMessageCounter = -1) | FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_CTOR | expanded in 0x0043B570 at 0x0043C32D..0x0043C340 | 0x0043B570 |
| engine/CQ_BanditCampBossBattleScript.h | CAssassinMarker nested class declaration (Alloc 0x00D048C0, GetParentScript 0x00D04960, Main 0x00D11930) | FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER | vtable 0x012C8B80 slots; Ego_r 28-byte CAssassinMarker. Includer: engine/CQ_EndGameFocalSitesScript.h (default path untouched) | 0x00D11930 (differ) |
| engine/CQ_OrchardFarmRaidScript.h | NEW header: NScript::CQ_OrchardFarmRaidScript retail layout (DoneIntroduction +0x50, HeroAtWrongEntrance +0x51, HeroTeam +0x78, Teams +0x8C, sizeof 0x10C) | new file, needs FABLE_RETAIL_NSCRIPT | Ego_r layout - 0x14 (- 4 after the STLport vector); checked against the landed ctor 0x00DCC040 and 0x00DCFA60 | 0x00DCFA60 (differ) |
