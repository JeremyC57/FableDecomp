# Chunk 1024 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every change is behind its own opt-in; with the opt-in undefined each modified header is byte-identical in effect to the repo copy.
New headers are only reached by the rows that include them.

| Header | Opt-in / status | What it adds | Row |
|---|---|---|---|
| engine/CAIStateGroup_TavernServer.h | `FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_METHODS` | Ego_r members WanderWithinTavern (EAIActionReturn), IsCarryingJug, GetServingPoint, FreeServingPoint, GetJug, GetOldJugPoint, PickUpJug, PutDownJug, FillTankard, WalkToBar, WalkToServingPoint | 0x00966DA0, 0x00967D40 |
| engine/CAIStateGroupBase.h | `FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER` | `AIAction_MinionWander(EMovementType, CThingBuilding*)` 0x0097F870 | 0x00967D40 |
| engine/CTCTavernTable.h | new | TCI_TAVERN_TABLE, GetServingPoint 0x007B8CC0, FreeServingPoint(long) 0x007B9090; ServingMode SERVING_MODE_0..2 (descriptive enumerators) | 0x00966DA0, 0x00967D40 |
| engine/CTCTavernJug.h | new | pack(1) Fullness +0x0C, OnBar +0x10, RHSet +0x11, Pos +0x29; TCI_TAVERN_JUG | 0x00967D40 |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_LIGHTINGFLASH` | TransitionToThemeAllChannels declaration, private LightingFlash() | 0x006B3B80 |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_WEATHERSCALERS` | GameTickState split: SnowStrength +0x1C0, RainStrength +0x1C4, MistStrength +0x1C8, LightningFrequency +0x1CC | 0x006B3B80 |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_SAMPLEBANK_WORLDSEED` | GetSampleBank() const, GetWorldSeed() const (Ego_r names; +0x10 / +0x1613C) | 0x006B3B80 |
| engine/CThingSoundEmitter.h | `FABLE_RETAIL_CTHINGSOUNDEMITTER_CREATE` | static __fastcall Create(const C3DVector&, const CThing*, bool) 0x008369F0 | 0x006B3B80 |
| engine/CThingSoundEmitter.h | `FABLE_RETAIL_CTHINGSOUNDEMITTER_TALKFRIEND` | `friend class CTCTalk` (access unproven) | 0x006E2ED0 |
| engine/CTCSoundPlayer.h | `FABLE_RETAIL_CTCSOUNDPLAYER_TALKFRIEND` | `friend class CTCTalk` | 0x006E2ED0 |
| engine/CTCTalk.h | `FABLE_RETAIL_CTCTALK_LIPSYNC` | private inline GetSpeechBank(), GetPLipSyncBank() 0x006E2860, GetCurrentLipSyncFrameData | 0x006E2ED0 |
| engine/CTCBase.h | `FABLE_RETAIL_CTCBASE_PEEKSAMPLEBANK` | PeekSampleBank() const 0x00686D70 | 0x006E2ED0 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE` | GetGameScriptInterface() const (inline, +0x38) | 0x006E2ED0 |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | `FABLE_RETAIL_CTCGRAPHICAPPEARANCE_WASRENDERED` | WasRenderedLastFrame() 0x004BD200 | 0x006E2ED0 |
| engine/CASoundEmitter.h | `FABLE_RETAIL_CASOUNDEMITTER_ACTIVESAMPLEINFO_ORDER` | slot 24 GetActiveSampleInfo(const CASoundBank&, long, vector<CActiveSoundInfo>&) | 0x006E2ED0 |
| engine/CLipSyncDataBank.h | `FABLE_RETAIL_CLIPSYNCDATABANK_LIPSYNCDATA` | PeekLipSyncInfo 0x00A42830, GetLipSyncData(unsigned long) const 0x00A42FC0 | 0x006E2ED0 |
| engine/CLipSyncSampleData.h | new | GetFrameInterpolate(CLipSyncFrameData&, float) const 0x00A48F10 | 0x006E2ED0 |
| engine/CActiveSoundInfo.h | new | CActiveSoundInfo : CBaseClass (RTTI vtable 0x0129E674), size 0x10, descriptive members SampleIndex / PBank / SampleID | 0x006E2ED0 |
| engine/CTCMultiStrike.h | `FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS` | ctor 0x007CD390 and the Ego_r members at retail offsets (PDef +0x0C .. ParticleEffects +0x4C, sizeof 0x58) | 0x007CD390 |
| engine/CTCInventoryWeapons.h | `FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE` | StatGetInterfaceType (TCI_INVENTORY_WEAPONS 19), inline GetActiveMeleeWeapon() const (FableWin) | 0x007CD390 |
| engine/CSpecialAbilitiesMultiStrikeDef.h | new | Ego_r members at retail offsets (Sounds +0x3C .. AttachmentPoint +0x90, 12-byte STLport vectors) | 0x007CD390 |
| engine/CTCDParticleEmitter.h | new | TCI_PARTICLE_EMITTER, SetAlwaysUpdate 0x006E0590, AttachToThing 0x006E0BE0 (Ego_r / FableWin spellings) | 0x007CD390 |
