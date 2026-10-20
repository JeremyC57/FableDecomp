# Hand chunk 212 (CTCTavernGame .. CTCWeapon): 35 of 40 byte-exact

All 35 graded MATCH / RELOCATION_MATCH under the RTM cl 13.10.3077 lane (gall.sh re-run after the lane change) and their
behaviour tests PASS under g++ (tall.sh) and VC7.1/Wine (tall.py). The five parked rows were re-graded once under RTM:
unchanged (none was a /GS-prologue diff).

## What matched (35)
- **CTCTavernGame** (0x005EA310, 0x005EC7C0, 0x005EC8A0, 0x005EC990): GetTotalWinnings (catalog "GetBet" is wrong; slot 49), RenderText, the constructor, DrawBox. Retail CTCTavernGame is 0xF0 bytes with 12-byte STLport vectors, so members sit 8..0x1c below engine/CTCTavernGame.h: local models. Shapes: a stand-in `GetSubDef_005ed4a0` body under `auto_inline(off)` keeps the def in a register with the short `dec [reg+4] ; jne` release; RenderText needs the inline CEnginePrimitive2DText ctor with Pos in the init list and the colour copied through Set; ctor = vectors / double / counted pointers in the init list, five scalar clears + three Resize(5) in the body; DrawBox repeats `(float)size.Y` (CSE temporary in the dead def slot).
- **CTCTavernGameCardBase** (0x008F1720, 0x008F17D0, 0x008F1D40, 0x008F2580): EndRound, TurnCardsBack, FrameUpdate, AddPointerParticleEffect. Preloaded-zero HasTC + one-ternary GetTC (component in ecx); STLport clear via __copy_trivial (memmove); FrameUpdate is a dense switch with its jump table, cases written in retail block order; AddPointerParticleEffect needs a __forceinline GetParticleEmitter.
- **CTCTavernGameCardBlackjack** (0x007DA900, 0x007DA9B0, 0x007DA290, 0x007DAB60): HasPlayerWonHand (first GetHandValue inlined, second out of line), HasPlayerDrawnHand (both inlined), TurnPlayerCards (for-walk re-reading end(); SetState tail-duplicated), UpdateValues (catalog vfunc_40; unchecked out-param GetInterface, a long local for the `fild`).
- **CTCTavernGameCardSorting** (0x007D5A00) NewRound: the order local is read before the def deref; inline STLport push_back.
- **CTCTavernGameCoinBase** (0x008EF950) ClearBoard: pack(1) intelligent pointer at Coin+0x25; inline STLport erase.
- **CTCTavernGameCoinGolf** (0x007D2920, 0x007D2B70): EndRound (an inline ParentThing accessor loads ecx first); GetCurrentScore (twin of 0x007CF690).
- **CTCTavernGameShoveHaPenny** (0x007CF690, 0x007CF5B0): GetCurrentScore (catalog says CardBase -- propagated), GetBestScore (stats out pointer zero-initialised).
- **CTCTavernGameSpotTheAddition** (0x007D7C80, 0x007D7F10, 0x007D7FD0): GetBestScore (stats out pointer left uninitialised), PositionCover (unrounded float locals on the x87, stand-in GetSubDef_007d9a80), EnterGame (stand-in GetSubDef, pack(1) vector classes at odd offsets).
- **CTCTrapBase** (0x007887BE) UpdateTriggerOnProximity: `optimize("y",off)` + `("s",on)`, __forceinline C3DVector copy, float delta locals.
- **CTCTrespasser** (0x007BB070) ForceTrespass: zeroed out-param GetInterface `&&` the handle test, two complete PostDeed arms.
- **CTCTurncoatSpell** (0x007C6DA0) StopTurncoatingEffects: out-param GetInterface inside the `if`, IS_THING_ALIVE twice.
- **CTCVillage** (0x0065A5BB, 0x0065A7A9, 0x0065AC83, 0x0065C08A, 0x0065C1FC, 0x0065C36E): all `y off` + `s on`. GetRegionLeavingDurationForCurrentCrimes (__forceinline temp-def dtor), RemoveTimedoutCrimes (separate "next" iterator in the key slot), HomeUsersAreAsleep (inline home accessor, unchecked GetInterface), UpdateSecuritySweepTasks (world + fps locals before the static frame call), RemoveCrimeIfApologisable (member-address local, forceinline def temp dtor), SetVillageLimbo (an inline persist setter gives the `or byte ptr` store). Retail lists are 4 bytes: local models.
- **CTCVillageMember** (0x007EE270, 0x007EE5A0): SetPVillage (intelligent-pointer temporaries with no user dtor; operator= takes GetPItem into a local first), OnInitialActivate (unchecked GetInterface, the debug-check search kept).
- **CTCWallMountEffects** (0x007DF6A0) DoEffect: a switch on the def's Effect (`dec eax ; je` / `dec eax ; jne`); the teleporter is `T* p = 0; if (HasTC) p = GetTC` (the one-ternary form emits `test dword ptr` without the `xor ecx,ecx` preload); healing calls CThingPhysical::ModifyHealth (slot 59, 0x006A68F0).
- **CTCInventoryWeapons** (0x005C3947, catalog row "CTCWallMountEffects ctor") constructor: `y off` + `s on` like the landed dtor 0x005C39F0; the def indices must be `CDefIndex` objects with an inline ctor (plain `long` members schedule the zero store one instruction later).
- **CTCWeapon** (0x005DAB10) IsUsableBy: over engine/CTCWeapon.h through the landed `CTCWeapon_Getters` extension (Leaf `?IsUsableBy@CTCWeapon_Getters`); one `return false` shared by the non-creature exit and the switch's missing default keeps all four pushes in the prologue; HasTC must be __forceinline inside the `||`.

## Parked for Fable 5.1 (5)
- 007da760 CTCTavernGameCardBlackjack::GetPosition DIFFER(164v164): retail hoists `fld [ebx]` (card offset X) above the size division, ours emits `fld [eax] ; fadd [ebx]` at the sum | tried: card+hand / hand+card through inline operator+, swapped component order in operator+, const-reference locals | next: lane gap: x87 load scheduling
- 0084a720 CCreatureAction_AlertAllNearbyToThing::AlertAllNearby DIFFER(188v190): search-box corner scheduling -- retail `mov ecx,edi ; sub ecx,eax ; mov edx,esi ; sub edx,eax ; add edi,eax ; add esi,eax` with TopLeft stored before the filter temporaries and BottomRight after the pushes, ours computes the corners in the other order | tried: box temporary with a 4-long ctor, named box local (4-long ctor), named box from two C2DCoordI temporaries; a stand-in GetAllTCsInRoughArea body fixed the esi-held vector storage | next: C2DBoxI built from a C2DCoordI centre +/- an extent (operator-/operator+ helpers), or corner members assigned in the body
- 00658fa2 CTCVillage::GetNextGuardTypeAndIncrement DIFFER(236v227): the people def lives in eax (index in ecx) where retail keeps it in ecx (index in eax), so the shared final release is `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` instead of `dec [ecx+4] ; jne` | tried: index declared before the def, a raw def-pointer local for every access, a pointer local in the CDefPointer dtor | next: lane gap: register allocation (try the GetSubDef stand-in body above under auto_inline(off), or a for loop over a quantity pointer local)
- 0065b349 CTCVillage::TryToRespawnDefNamed DIFFER(184v183): the IS_THING_DEAD argument temporary takes its own frame word (`sub esp,0Ch`, [ebp-8]) where retail reuses the dead `defName` slot [ebp+8] already shared by the IS_THING_ALIVE temporary and the GetTC key (`push ecx ; push ecx`) | tried: `&&` condition, nested ifs, a named villager-pointer local, IS_THING_DEAD over CBaseObject* const& | next: lane gap: dead-argument-slot sharing for a second temporary (QFE/RTM frame packing)
- 0065c146 CTCVillage::RemoveCrime DIFFER(182v182): register choice only -- retail `push 51h ; pop edi` and `push ebx ; mov ebx,[ebp+8]` after the lookup, ours `push ebx ; push 51h ; pop ebx` before it (edi then left to the witness) | tried: parameter used directly (this then stays in ebx), a handle copy after the lookup, the copy declared first (the register is right but its load moves above the lookup), declared first and assigned after | next: lane gap: callee-saved register colouring; try the deed loop in an inline helper taking the log and handle

## Skipped
- None.

## Label corrections
- 0x007CF690 / 0x007D2B70: CTCTavernGameShoveHaPenny / CTCTavernGameCoinGolf::GetCurrentScore, not CardBase (propagated).
- 0x005EA310: CTCTavernGame::GetTotalWinnings, not GetBet.
- 0x007DAB60: CTCTavernGameCardBlackjack::UpdateValues (catalog vfunc_40).
- 0x0084A720: CCreatureAction_AlertAllNearbyToThing::AlertAllNearby, not CTCTrapBase::UpdateTriggerThings.
- 0x005C3947: CTCInventoryWeapons::CTCInventoryWeapons(CThing*), not CTCWallMountEffects::CTCWallMountEffects (vtables 0x01253FF4 / 0x01253FE0).
- 0x005DA7F0: CTCWeapon::GetWeaponClass() const (returns WeaponDef->Class +0x2c), not NInventory::CCategory::GetCategoryIdentifier (byte twin).
- 0x006A98D0: a creature getter of the word at +0x1e8 (unidentified), not CWorld::DrawGetEnvironment (byte twin).
- 0x00686D60: CTCBase accessor (`[0x013B8A1C]+0x1c`), not NDisplayView::CViewBase::GetDisplayEngine; landed spelling GetGameSubsystem kept.
- 0x00485D30: vector<CEnginePrimitiveHandle>::vector(size_type), not map::operator[] (as landed 0x0082A460 already says).
- 0x008F17D0 / 0x008F1D40: CTCTavernGameCardBase methods (in the CardSorting vtable too); 0x005E9AA0 is CTCTavernGame::EndRound (landed as vfunc_33); 0x008EF190 is CTCTavernGameCoinBase::EndRound; 0x00787BF1 is CTCTrapBase::Reset.
- 0x0065C2D0: landed spellings are ProcessCrimeDeeds / OnAllFinesPaid; the PDB order suggests UpdateMirroredCrimeData -- check at landing.

## Callee names chosen (one symbol per address)
- 0x0049D870 `static long CWorld::GetWorldFrame()`; 0x00686D40 CTCBase::GetPlayerManager(); 0x00449970 CPlayerManager::GetMainPlayer() (non-const, returns CPlayer&); 0x00487DC0 CPlayer::GetHeroThing() (landed has several spellings).
- 0x00686800 CTCBase(CThing&) is the Ego_r.pdb form, called as CTCBase(*thing); the CTCTavernGame ctor itself keeps the landed CTCTavernGame(CThing*) spelling its derived ctors bind.
- FormatDouble kept (PDB name GFFloatToCharString); CTCHeroStats over the landed CHeroStats; PositionMesh as a static member (PDB; landed has 3 spellings); PostDeed with the PDB enum signature; the global DrawBox 0x0048E0A2 keeps the landed spelling (PDB CSubtitleRenderer::DrawBox).
- g_pDisplayDevice typed CDisplayManager* (landed CDisplayDevice* x2 vs CDisplayManager* x1); StopSample on CTCSoundPlayer; ClearMeshEffect in NGraphicAppearance (landed: global); GFHasScriptThing as landed.
- CTCVillageMember::GetPVillage() const is PDB-correct (landed mostly GFGetPVillageInterface); AddThingToVillage(CThing&) is the PDB form (landed CThing_Retail*); CThingManager::GetThingWithUID(unsigned __int64) (landed CThingManager_Retail).
- IS_THING_ALIVE(CBaseObject* const&) for 0x0040DCE0 (sibling form); IS_THING_ALIVE(const CBaseIntelligentPointer&) for 0x004AB8F0; a template IS_THING_ALIVE for 0x0065F8CA; the global flag g_bCheckVillageMembers_013b86d8.
- Stand-in GetSubDef bodies use the landed names GetSubDef_005ed4a0 / GetSubDef_007d9a80; GetSubDef_00512fe0 is landed.
- 0x007DFF90 CTCTeleporter::StartTeleporting(CThingPlayerCreature*); 0x006A68F0 CThingPhysical::ModifyHealth(float, bool) (slot 59); 0x00851B50 modelled as CExperienceRequirements::IsMetBy (label "CAbility_CheckRequirements", object unidentified).
