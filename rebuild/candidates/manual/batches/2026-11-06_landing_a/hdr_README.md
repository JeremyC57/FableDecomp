# chunk 800 header overlay

Every edit is an opt-in branch (or a new header): each repo default path is unchanged, so landed includers cannot change bytes.
Each file was copied fresh from rebuild/include when edited and diffed against the repo copy again at hand-back (additions only, no
repo change since). Includers rechecked read-only with this overlay: SIBLINGS.tsv.

| header | change | macro | used by |
|---|---|---|---|
| engine/CQ_EndGameFocalSitesScript.h | NEW: NScript::CQ_EndGameFocalSitesScript (CLevelScriptBase<CQ_SunnyvaleMasterData>, 0x94: CurrentFocalSiteRegion +0x48 .. JackReached +0x90 = Ego_r.pdb - 0x14) and the nested CEndGameScythe (0x1C; vtable 0x012CC33C, Alloc 0x00D2C650). No landed includer (the quest's 45 landed rows carry local models). | guarded by FABLE_RETAIL_NSCRIPT | differ/00d2ed20.cpp |
| engine/CQ_BanditCampBossBattleScript.h | opt-in class CBanditKing (Ego_r.pdb 32 bytes: ParentClass +0x14, PMasterData +0x18, HealthBarIndex +0x1C; vtable 0x012C9474 slots 0..6) | FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_BANDITKING | differ/00d0a830.cpp |
| engine/CQ_GuildTrainingScript.h | opt-in member declarations Main calls (RunArrivalCutscene 0x00D44CB0, RunTutorials 0x00D45DD0, CheckFriendlyAttacks 0x00D45060, KeepTabsOnWhisper 0x00D3CBD0, WatchForSparrowKilled 0x00D3C840, KeepBookcaseExitRemoved 0x00D3C8F0), the 13 nested entity-script class names and CWillDummy::Alloc 0x00D43360. Declarations only. | FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN | src/00d3bc60.cpp |
| engine/CSubtitleRenderer.h | opt-in `void Clear(bool);` (Ego_r.pdb / FableWin.pdb ?Clear@CSubtitleRenderer@@QAEX_N@Z, out of line 0x00492BAB). Declaration only. Reached through engine/CPlayerGui.h too; the overlay copy is the one used (the row does not compile without it). | FABLE_RETAIL_CSUBTITLERENDERER_CLEAR | differ/007d8e20.cpp |
| engine/NUISystem_CSpellContainer.h | opt-in new virtuals of the primary vtable 0x0124786C, slots 142..148 in order (AddChildDynamically(CCountedPointer&, unsigned long), AssignSpell, UnAssignSpell, SetEmptySlotDef, AcceptAssignments, AddChildAndSetOffsets, GetBaseSpells); names Ego_r.pdb / FableWin.pdb, slots rtti-vtables.tsv. Declarations only, no layout change. | FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS | src/00544d20.cpp |
| engine/CScriptGameResourceObjectMovieBase.h | opt-in ctor-only __forceinline (`call 0x0099A380; mov [esi],0x01260EF4; zero +8/+C` at every site of both rows), the dtor left compiler-generated so its expansions carry no MovieBase vtable reset (the existing FABLE_..._FORCEINLINE pair adds one per site: chunk 797 0x00D18DF0 residue). Exclusive with FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE. | FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR | differ/00d2ed20.cpp, differ/00d0a830.cpp |

Not handed back: an exploratory `FABLE_RETAIL_CVECTORMAP_LOWERBOUND_INLINE` (plain inline CVectorMap::LowerBound) tried for row
0x0094AB00 (a5): VC7.1 expands 4 of the 9 lookups there, retail 1, so it was dropped.
