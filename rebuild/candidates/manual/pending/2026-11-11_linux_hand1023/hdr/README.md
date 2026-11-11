# Header overlay, chunk 1023

Grade every row with `FABLE_HDR_OVERLAY=$A/hdr` exported. Each file is a copy of `rebuild/include/<same path>` at HEAD with the
changes below; every change sits behind its own `#ifdef FABLE_RETAIL_*` opt-in, so the default path is byte-identical.

| File | Opt-in | Row | What it adds |
|---|---|---|---|
| engine/NSpeechGainManager.h | (new header) | 0x006E3EC0 | `NSpeechGainManager::CDialogueLayerDef` (CDefClassBase, 0x40, attack +0x28 / release +0x2C), `CResponseTimes` (8 bytes; empty default ctor, FABLE_FORCEINLINE (float, float) ctor), statics g_LastUpdateFrame 0x013BAB8C, g_LayerSpeakerCounts 0x013BAB94 (vector<unsigned short>), g_ActiveDialogue 0x013BABA0, g_ResponseTimes 0x013BABA4, `Initialise` decl |
| engine/CGameDefinitionManager.h | FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF | 0x006E3EC0 | `const CGlobalDef* PGlobalDef` at +0xD0 and inline `PeekGlobalDef()` (both the HEROSTATSDEF-nested and the `#elif` branch, padding kept so sizeof is unchanged) |
| engine/_quarantine/CGlobalDef.h | FABLE_RETAIL_CGLOBALDEF_DIALOGUELAYERS | 0x006E3EC0 | `DialogueLayers` typed `std::vector<NSpeechGainManager::CDialogueLayerDef>` at retail +0x10C |
| engine/CTCHeroReceiveItems.h | FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS | 0x007CB8B0 | protected virtual `OnSerialise(CPersistContext&, bool)`, `Items` list +0x0C, `PickedUpItems` vector<long> +0x10 |
| engine/CTCInventoryQuests.h | FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS | 0x0061A6A0 | `virtual OnCreate()`, InventoryQuestCardSelectPrimitiveHandles +0x134, QuestCardItemList +0x140, IsGuildValid +0x14C, InventoryScreen +0x14D, QuestCardCount +0x150 |
| engine/CQuestCardItem.h | (new header) | 0x0061A6A0 | `CQuestCardItem { long ItemIndex; CWideString ItemText; }` (8 bytes, Ego_r.pdb) |
| engine/CCreatureDef.h | FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES (nested in the DIALOGUEVOICES branch) | 0x0061A6A0 | the screen inventory def indices +0x78..+0xA8 (QuestsScreenInventory +0xA4) |
| engine/NInventory_CTCInventoryBase.h | FABLE_RETAIL_CTCINVENTORYBASE_ONCREATE | 0x0061A6A0 | `virtual OnCreate()` 0x005C0121, protected `SetPDef(long)` 0x005BDB4D |
| engine/NHeroInformationScreens_CManager.h | FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER | 0x0061A6A0 | `RegisterScreen(const CBase*)` 0x0066EB11 |
| engine/CEngineLandscapeRenderer.h | FABLE_RETAIL_CENGINELANDSCAPERENDERER_BLENDTABLES | 0x00B67270 | private `BuildBlendingTables()` |
| engine/CEngineLandscapeMeshBuilder.h | FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND | 0x00B67270 | `enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION`, `float GetMappingDirectionBlend(LANDSCAPE_TEXTURE_MAPPING_DIRECTION, const C3DVector&)` 0x00BE2B70 |
| engine/CTexture.h | FABLE_RETAIL_CTEXTURE_LOCK | 0x00B67270 | `CSurfaceLockedRect Lock(unsigned long, unsigned long)` 0x009FA450, `Unlock(unsigned long)` 0x009F9DE0 |
| engine/CPixelFormat.h | FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE | 0x00B67270 | out-of-line `GetByteColourDepth() const` 0x009E3800 (only when FABLE_RETAIL_CPIXELFORMAT_CONVERT, which keeps the inline body, is off) |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE | 0x00B67270 | `void AccurateNormalise()` 0x00A14440 |
| libpk/LHAudioEventPosition.h | FABLE_RETAIL_LHAUDIO_LISTENER_SPACE | 0x00C1C460 (differ) | CSoundPosition::Position typed C3DVector; CQuadHolder (+GetValue 0x00C34600) declared; CSoundEvent PositionHolder +0x38 / PPlayingData +0x48; CPlaybackSlot (0x2C: PEvent +4, SourceHandle +0xC, Positional +0x25); CSoundEventSet Playbacks +0x1F8 (manager +0x224); CSoundEventManager ListenerPosition / Front / Top +0x08 / +0x14 / +0x20, EventSet public, ListenerValid +0x885. All size asserts unchanged |
| libpk/PKCFrallocStatic.h | (new header) | 0x00C1C460 (differ) | the pool template of the landed accessors 0x00C13700 / 0x00C13620 (m_count +4, m_pData +0x10, Get) |
| libpk/CUnprovenSoundWorld_00c1c460.h | (new header; defines FABLE_RETAIL_LHAUDIO_LISTENER_SPACE) | 0x00C1C460 (differ) | the unproven owner (PManager +4, Paused +0x18, SourcePositionsEnabled +0x19) and the callee declarations SwapYZ / InvertMatrix3x4 / CODA_SetSourcePosition / GlobalMethodTailThunk_00c33690, g_soundLock |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE | 0x00C1C460 (differ) | out-of-line Cross 0x00C25860 (first branch of the Cross chain), DotProduct 0x00C256A0, Copy 0x00C1BFF0 (declarations) |
