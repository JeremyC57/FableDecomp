# agentNF14 header overlay

Full copies of the current repo headers (`rebuild/include/engine/<name>.h`) with `#ifdef` opt-in additions only. With none of the
new macros defined, every header preprocesses to the repo's text (no repo line is removed or changed; checked with `diff`, zero `<`
lines). Graded with `FABLE_HDR_OVERLAY=$S/w22/agentNF14/hdr`.

| Header | Opt-in | Adds | Evidence (two sources each) | Row(s) |
|---|---|---|---|---|
| engine/CTCVillage.h | `FABLE_RETAIL_CTCVILLAGE_CRIMES` | `void RemoveCrime(long); // 0x0065C146`, `void RemoveCrimeIfApologisable(long); // 0x0065C1FC` | Ego_r.pdb `?RemoveCrime@CTCVillage@@QAEXJ@Z` / `?RemoveCrimeIfApologisable@CTCVillage@@QAEXJ@Z` (public); catalog names; the row's call to 0x0065C146 binds RemoveCrime | 0065c1fc |
| engine/CTCVillage.h | `FABLE_RETAIL_CTCVILLAGE_APOLOGISE` | `NotifyHasApologised() // 0x006585EE`, `CreateDummyVillagersIfNecessary() // 0x00658603`, `GetIsHeroEnemyOfGuards() // 0x00658604` | Ego_r.pdb names + Ego_r donor order; bodies store/compare the frame with LastFrameApologised / HeroEnemyOfGuardsUntil (retail +0xD8 / +0xD4) | 006585ee, 00658604 |
| engine/CTCCreatureNavigation.h | `FABLE_RETAIL_CTCCREATURENAVIGATION_ISNAVIGATING` | `bool IsCurrentlyNavigating() const; // 0x006CB8F0` | Ego_r.pdb `?IsCurrentlyNavigating@CTCCreatureNavigation@@QBE_NXZ`; catalog name; body reads FrameLastNavigated | 006cb8f0 |
| engine/CTCPhysicsStandard.h | `FABLE_RETAIL_CTCPHYSICSSTANDARD` | retail class `CTCPhysicsStandard : public CTCPhysicsBase` (PPhysicsMeshInfo, OffsetVector3D, RHSet, OldRHSet, RHSetLastFrameSet, CollidesWithThings, Impassable; 0x88 bytes) with `virtual void ResetInterpolation();` (the flat struct stays under `#else`) | Ego_r.pdb CTCPhysicsStandard layout; vtable 0x01265F84 slot 80 = 0x007236E0 (RTTI) | 007236e0 |
| engine/CTCPhysicsBase.h | `FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP` | protected `CWorldMap& GetWorldMap(); // 0x006AF620`, `const CWorldMap& GetWorldMap() const; // 0x006AF630` | Ego_r.pdb `?GetWorldMap@CTCPhysicsBase@@IAEAAVCWorldMap@@XZ` / `..IBEABV..`, adjacent 16-byte bodies in Ego_r donor order (0x006EC300 / 0x006EC310) as in retail; bodies `call GetWorld/PeekWorld ; jmp [vtbl+30h/34h]` (CWorld slots 12 / 13) | 006af620, 006af630 |
| engine/CTCEntityEvents.h | `FABLE_RETAIL_CTCENTITYEVENTS_FRAMEUPDATE` | `virtual void FrameUpdate(); // 0x00811450` | Ego_r.pdb / FableWin.pdb `?FrameUpdate@CTCEntityEvents@@UAEXXZ`; callee 0x00812030 is FableWin's `remove_if<NEntityEvents::CPred_EventExpired>` | 00811450 |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE` | `const CDisplayEngine& PeekDisplayEngine() const` (inline, PDisplayEngine word +0x28) + its forward declaration | FableWin.pdb `?PeekDisplayEngine@CMainGameComponent@@QBEABVCDisplayEngine@@XZ` (0x01A700DC); retail reads +0x28 in place at 0x007E279F | 007e2790 |
| engine/CDisplayEngine.h | `FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT` | types `ScreenFadeOutInfo` as `CFadeInFadeOutBase` (same 0x1C bytes at +0xBC) and adds `bool IsScreenFadeOut() const` (inline); pulls CFadeInFadeOutBase.h with `_ISACTIVE` | FableWin.pdb `?IsScreenFadeOut@CDisplayEngine@@QBE_NXZ` (0x01A70098); Ego_r layout ScreenFadeOutInfo CFadeInFadeOutBase; retail tests byte +0xBC at 0x007E27A2 | 007e2790 |
| engine/CFadeInFadeOutBase.h | `FABLE_RETAIL_CFADEINFADEOUTBASE_ISACTIVE` | `bool IsActive() const { return Active; }` | FableWin.pdb `?IsActive@CFadeInFadeOutBase@@QBE_NXZ` (0x01A65280); Active +0 (Ego_r layout) | 007e2790 |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER` | `const CGameDefinitionManager& PeekDefinitionManager() const` (same body as the existing `FABLE_RETAIL_CTCBOASTINGCROWDCONTROL` copy; guarded so both can be defined) | FableWin.pdb `?PeekDefinitionManager@CWorld@@QBEABVCGameDefinitionManager@@XZ` (0x018D4A60); retail reads +0x10 in place at 0x006C8820 | 006c8780 |
| engine/CThingManager.h | `FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD` | `const CWorld& PeekWorld() const { return World; }` | FableWin.pdb `?PeekWorld@CThingManager@@QBEABVCWorld@@XZ` (0x01CF6420; twin GetWorld 0x01CF63E0); retail reads the World reference +0x24 in place at 0x006C881D | 006c8780 |

Rows that also use existing repo opt-ins (no overlay change needed): 006ceae0 (CTCCreatureOpinionOfHero `_GREET`), 007e2790
(`FABLE_RETAIL_CTCBASE_COMPONENT`, `FABLE_RETAIL_CSUBTITLERENDERER(_INCUTSCENE)`, `FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI`), 006c8780
(`FABLE_RETAIL_CTHING_MANAGERACCESS`, `FABLE_RETAIL_CTHING_GETTC_EXPANDED`, `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN`).
