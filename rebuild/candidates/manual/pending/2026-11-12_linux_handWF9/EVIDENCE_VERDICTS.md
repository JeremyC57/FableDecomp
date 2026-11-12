# Workflow wf9 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00867b40 -- DIFFER, author attempts 3

- What made it match: Not a registered match. Against the true 468 bytes, attempt 1's port already matched: CThing::GetTC(&modes) from the shared header under FABLE_RETAIL_CTHING_GETTC_EXPANDED; a named `const C3DAnimationInfo*` local before GetAnimationStats(); LeaveDeadCreature through the inline CThing::PeekBaseDef() (FABLE_RETAIL_CTHING); real class sizes for the three temporaries (Die 0xAC, BeStunned 0xAC, Recover 0x118). No compiler flag, no pragma.

## 00762ba0 -- VERIFIED, author attempts 1

- What made it match: Moving the row onto the shared engine headers with the scout's corrected identities fixed everything except arm order. The last step was writing the load arm first, `if (!context.IsSaving()) { load } else { save }`, followed by a single `SelectedShape = PersistantShapes.end();` (VC7.1 duplicates that statement plus the epilogue into the four retail exits by itself), with the bounds as four plain stores in the order BRX, BRY, TLX, TLY. One part is inferred: the first IsSaving if/else carries the same Transfer statement in both arms, because retail keeps two separate call set-ups. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00762ba0/00762ba0.cpp (copy in differ/00762ba0.cpp); test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00762ba0/00762ba0_test.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory.

## 00a12ab0 -- VERIFIED, author attempts 1

- What made it match: Twin-row body on the shared headers plus one float-specific fact: the callee's float result passes through a double-precision expression that folds away: `return (float)(mesh->PeekRoughProportionOfSphereContained(name, local, scaled_radius) * 1.0);`. That leaves a conversion tuple between the call and the result store, so VC7.1 emits `test esi,esi` before `fst [esp+4Ch]` as retail does. Final source /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00a12ab0/00a12ab0.cpp, test /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00a12ab0/00a12ab0_test.cpp (vt.py PASS); payload.json and oracle.tsv written by addmatch.py; notes in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00a12ab0/NOTES.md.

## 00726160 -- VERIFIED, author attempts 1

- What made it match: Porting onto the shared engine headers and writing both solid-to-thing tests as `if (X.GetTC(&solid) && !solid->IsSolidToThing(Y)) return;` with one address-taken `CTCSolidToThing* solid` local (the forced-inline CThing::GetTC<T>(T**), default non-expanded branch). That single change fixed the radius-sum load order (Player in ecx before thing->PhysicsTC in edx), which no spelling of the sum itself moved. Also needed: `const C3DVector& player_pos = Player->GetPosition()` declared before `thing->GetPos()`; a C2DVector delta; `delta.X*delta.X + delta.Y*delta.Y < radii`; nested ifs rather than early returns; the sibling's sign/angle/turns idiom with `CMainGameComponent::Get()->PeekWorld()` read before `CWorld::GetFrame()`; `C3DVector push = -Player->GetFacingVector()`. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00726160/00726160.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/00726160/00726160_test.cpp (vt.py PASS); row registered by addmatch.py in payload.json / oracle.tsv in the same directory.

## 00a90900 -- VERIFIED, author attempts 1

- What made it match: The original port onto the shared headers plus FABLE_RETAIL_STLP_VECTOR. The repair only changed the type of the Groups member. Its elements are 8 bytes either way, and the member is not used in this function.

## 008dc720 -- VERIFIED, author attempts 1

- What made it match: The landed twin's shape without its early return, moved onto the shared headers with no stand-in bodies. GetSubDef<CSpecialEffectsDef> comes from engine/CDefClassBase_GetSubDef.h (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, real body out of line) and GetEffectFromString from engine/CSpecialEffectsDef.h (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, real body). The counted def temporary is ParentThing.GetPBaseDef() by value; GetTC(TCI_PARTICLE_EMITTER, &emitter) uses FABLE_RETAIL_CTHING_GETTC_EXPANDED; operator= uses FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN. LastExplodeFrame = CWorld::GetFrame() sits before CTCDParticleEmitter::Create, and FlameActive = true right after the assignment. The dragon mode chain (CModeDragonFlyingBase / CModeDragonFlying / CModeDragonStrafeBurninate) is a local model because no shared header exists for it; it derives from the shared CTCCreatureModeManager::CModeBase. No made-up names.

## 006f3a30 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Only comments changed (source header and test). The code and the overlay are the same as the matched version: STLport list sort in the shared header, the _S_merge body visible to the compiler, and the comparator's operator() declared only.
- OPEN at hand-back: NOTES.md says landed 0x006F2BB0 `CRetailList_node0xc::Initialise` is wrong (the list<const CThingCreatureBase*> default-constructor closure), but neither NOTES.md nor NAME_FIXES.tsv gives the correct symbol (e.g. the constructed `??_F?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@QAEXXZ`) or two sources for it, so the claim cannot be acted on as check 6 requires.
- OPEN at hand-back: NOTES.md and DOSSIER.md say the landed caller 0x006EBA60 (rebuild/src/compiled/00/6e/CCombatManager_CCombatManagerRebuildCombatLists_006eba60.cpp, lines 8/21/49/67-68) wrongly spells the comparator `CCompareByDistance` and the callee `list::sort`, yet NAME_FIXES.tsv has no row for 0x006EBA60, so the coordinator, who respells from NAME_FIXES.tsv, will not fix that caller.

## 0085d180 -- VERIFIED, author attempts 1

- What made it match: Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/0085d180/src/0085d180.cpp (leaf ?InitMapwhoLevels@CThingMap@NThingMap@@IAEXXZ, Flags: none).
1. Shared headers instead of the draft's local models: engine/NThingMap_CThingMap.h (MAP_VALIDATE branch), C2DBoxI::GetExtents(), the STLport vector, and the real globals NThingMap::MaxLayers / NThingMap::LayerInfo[]. All four resizes are the one-argument resize(n).
2. #define FABLE_RETAIL_STLP_VECTOR (visible _M_fill_insert body, as in the retail TU): c2 then drops the destructor of the vector<CThingMapCell>() temporary after Cells.resize, and the two leading resizes stay calls.
3. The inline split is a site-frequency fact, not a device: the second arm is written `else if (size != -1)`. The test folds away to no code, but c2 rates that arm colder, so resize<CThingMapCell> (cost about 80 at frequency 1) stays a call at 0x0085D317 while the constant-count resize(1) in the first arm is expanded with erase's copy loop. A plain `else` expands both (531v481).
4. `long size = LayerInfo[i].Size;` is declared before `MapwhoLevelInfo[i].NoThings = 0;` (retail reads the size first and stores -1 as an immediate).
No flags, no pragmas, no inline device.

## 005be8f3 -- VERIFIED, author attempts 1

- What made it match: Moving the row onto the shared headers fixed all the register and frame residue of the old draft; the last 2 bytes were the min: the maximum slot count is a local evaluated before the vector size (`long max = GetMaximumNumberOfSlots(); count = std::min(max, (long)Augmentations.size())`), not a direct argument (VC7.1 evaluates arguments right to left, which computed the size first). Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf9/005be8f3/005be8f3.cpp (also copied to differ/005be8f3.cpp); payload.json and oracle.tsv written by addmatch.py.

## 008c6860 -- VERIFIED, author attempts 1

- What made it match: The original match still holds after the repairs. I made the overlay GetShotAccuracyPercentage const (QBEJXZ, per Ego_r 0x0053E950 and FableWin 0x017CE205 / 0x01D8C0B0) and fixed the citations in the overlay comment and hdr/README.md. I also made the test model's method const. NAME_FIXES.tsv now cites Ego_r 0x005192A0 and FableWin 0x017BA372 / 0x01DB6460 for 0x00692940, and has a new row for 0x00663B10 (the landed leaf QAEJXZ should be QBEJXZ). The GDefStringTable note in NOTES.md now gives address 0x013CA828 and two sources.

