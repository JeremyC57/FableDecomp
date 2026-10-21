# Hand chunk 272 (CTCPhysicsControlled .. CTCVillage): 16 of 40 byte-exact

## What matched (16)
- CTCPhysicsControlled: 00725c40 Move -- the forced teleport `goto`s the shared SetPosition tail (retail `jmp`); the impulse length is a squared local.
- CTCPhysicsStandard / CTCPhysicsRigidBody: 00723c80 / 00759b40 GetRHSet -- interpolate the right-handed set between the old and current frames. Each axis is `(current - old) * t + old` through C3DVector operators that return `C3DVector(...)`. 00723c80 is CTCPhysicsStandard's copy (see the label corrections).
- CTCShop: 00774100 GetWantedItemsList -- the def test is a named bool over two by-value CDefPointer copies (`inc [def+4]` per copy).
- CTCShopKeeper: 00799140 CalcAreOtherPeopleInShop
  - The TC lookups use the out-parameter GetInterface (bit test, `long key`, map pointer, one end/hit ternary), so the village spill shares the key word.
  - An inline IsThingAlive(building) keeps the second inline `return false`.
  - A named `other` villager before `->HomeBuilding` gives `lea ecx,[eax+0x170]`.
- CTCSummonSpell: 0079b9d0 FrameUpdate
  - The carried-things loop reads `First` and then `Last` into named locals before the loop, which gives retail's ecx/eax order with the end in ebp.
  - The minion test is the nested `if (GetPItem()) { if (!Dead) { ... return; } }` form, with the RemoveTC tail last.
- CTCShotDefinitionBase: 007c04a0 UpdateCreatureAwarenessOfMe
  - A same-TU auto_inline(off) GetSubDef stand-in.
  - The ARROW_FLY_BY name is a named CCharString in its own block.
  - An inline GetOwnerDefClass accessor.
  - `CThingFilter_All() {}` is a user ctor, so there is no zero store.
  - The def is declared before GetInterface(0x10).
- CTCShotDefinitionNymphFun: 007bdb66 OnCollision
  - /Os frame-pointer TU: `optimize("y",off)` plus `("s",on)`.
  - C3DVector has a user copy ctor and the implicit operator=, so the `pos` copy goes through registers and the GetPosition assignment through movsd.
  - The empty name and the shooter's team are named locals in one block, so the team is read before the Create pushes.
- CTCSpotLight: 007320e0 OnSerialise (catalog "vfunc_1")
  - A named bool `saving = Mode==1||Mode==3` puts the save branch first.
  - CRGBColour members are B,G,R,A; its ctor assigns R,G,B,A.
  - The load arm does `PLightInfo.Reset(new CSpotLightInfo)` and then copies the colour into a local.
- CTCTavernBar:
  - 007ba000 GetPickupSlot -- the 0x2E component is left uninitialised when it is absent.
  - 007ba1d0 GetEmptyJugSlot -- `__forceinline` GetJug with an early-out `return CIntelligentPointer(0)`.
  - 007ba8c0 HasEmptyJugSlot -- the GetEmptyJugSlot body over local pos/rh/pointIndex.
  - In all three: `Food.size() <= (unsigned long)(chair-1)`, an unused default CCharString, and an 8-byte IP temp for the `and esp,-8` frame.
- CTCTavernGame: 005eb810 ChangeBet -- `changed = true` is written before the `+=`.
- CTCTavernGameCoinGolf: 007d3560 GetCamera -- C3DVector with a user copy ctor; the offset is added in place and then copied.
- CTCTavernGameSpotTheAddition: 007d83c0 OnInitialActivate
  - The GetSubDef stand-in keeps the def in edi and releases it without a re-read.
  - The const GetInterface uses LowerBound 0x4365B0 for TC 0x21.
  - The cover frame comes from vslot 72 of thing+0x60.
- CTCTurncoatSpell: 007c6760 ctor (leaf `??0CTCTurncoatSpell`)
  - The TC 0xA7 / 0x6F lookups are an inline HasTC (masked word) guarding an inline GetTC with no bit test of its own.
  - The def name is passed as `const char*`, so it is built before `push 1`.
  - The member IPs are polymorphic 8-byte CBaseIntelligentPointer subclasses with `SetPItem(0)` ctors.

## Parked for Fable 5.1 (24)
- 008e5d40 CTCPhysicsStandard::Move DIFFER(441v441): vector<CThing*> and old-position C3DVector swap frame slots ([esp+0x18]/[esp+0x24]) | tried: declaration order, scoped blocks, default ctor + assign, full STLport vector base, POD vector | next: lane gap: frame-slot order
- 008006d0 CTCPreCalculatedNavigationRoute::SaveRoute DIFFER(478v478): retail builds the "NavPosition"/"NavLayer"+GFIntToCharString(i) name before pushing the value/default addresses; frame +8 | tried: by-value name wrapper, temporaries as values, in-loop defaults, do/while guard | next: name as a by-value class argument with user copy ctor built in the argument slot
- 008008b0 CTCPreCalculatedNavigationRoute::LoadRoute DIFFER(432v420): same name-before-pushes gap; default C2DVector at unaligned [esp+0x37], push_back temp at [esp+0x47] (packed frame) | tried: SaveRoute model port | next: solve SaveRoute first, then pack(1) frame temps
- 006aeea0 CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions DIFFER(429v429): scratch rotation eax/ecx/edx vs ecx/edx/eax around GetDefName hidden ptr / def-name word / 0xBC key | tried: 16 probes (operator== forms, CDefString copy ctor/dtor, IsDefNamed helper, explicit temp, non-const creature) | next: lane gap: scratch-register rotation
- 00762ba0 CTCShapeManager::OnSerialise DIFFER(444v468): retail keeps three separate SelectedShape=end exits (ours merges two); offset zero stores via edi; ebx/edi map swap | tried: branch order (loading first fixed layout), named map, do/while explicit exits | next: each exit as a full inline call copy
- 005b48a2 CTCInventoryClothing::OnItemSelected DIFFER(488v460): this/zero edi/ebx swap, interface key in the dead itemType slot [ebp+8], frame +4 | tried: one first-pass shape | next: out-param GetInterface + arg-slot key reuse
- 007727e0 CTCShop::GetCurrentBasicPrice DIFFER(497v433): retail rounds the stock level with an inline-assembly `fadd 0.5 ; fistp ; fild ; fsub 1.0 ; cmp ; dec` sequence | tried: minimal C++ model | next: lane gap: inline-assembly rounding (oracle-only)
- 007735d0 CTCShop::GetAttitudeBuyPriceMultiplier DIFFER(443v448): register web: retail this=ebp, thing=edi, zero=ebx; ours this=edi, thing=ebp, zero=esi; both `sub esp,0x10` | tried: ~26 advisor probes (probe/advisor/) | next: lane gap: register-web priority tie-break (family of 0x0076C8A0)
- 00773790 CTCShop::GetAttitudeSellPriceMultiplier DIFFER(443v448): twin of 007735d0 with the Sell offsets; same register web | tried: as 007735d0 | next: lane gap: register-web priority tie-break
- 00774730 CTCShop::SellItem DIFFER(419v419): esi/edi swap between node and item pointer in the building-contents loop | tried: ref/ptr locals, accessors, iterator vs while, inline members (20 probes) | next: lane gap: register tie-break
- 007751a0 CTCShop::SetShopKeeper DIFFER(419v419): HasTC(0xab) on the reloaded ParentThing loads ecx/eax swapped (and the next lea edx/ecx) | tried: accessor, split-if, named locals (9 probes) | next: HasTC returning the masked word through a const accessor
- 007c25a0 CTCShotDefinitionBase::InitAttachedEffects DIFFER(452v452): frame +8 (`sub esp,0x50` v 0x48): retail shares one word between the uninit emitter, the IP temp and the GetDirection result | tried: named pointer block, inline getter, early declaration | next: emitter as an out-param of a noinline stand-in
- 007bbe34 CTCShotDefinitionSpreadShotProjectile::InitMovement DIFFER(439v464): target kept in edi with only a temp copy in [ebp-4] for IS_THING_ALIVE (ours re-reads the spill); per-vector frame slots `sub esp,0x5c` v 0x44 | tried: first pass, declared-up-front order + rvalue target (480v464) | next: IS_THING_ALIVE on a converted temporary (derived pointer) and named rh/velocity locals
- 00722590 CTCSmokeGenerator::CreatePositions DIFFER(434v425): frame +12; smoke def does not reuse the prefix string word; def release reloads the pointer | tried: if/else Format, Release member, HasTC+GetTC, declaration order | next: def in a block after the loop
- 007f53b0 CTCSoundAtmosphereVillage::FrameUpdate DIFFER(441v424): this/group register swap (retail this=ebx, group=edi), frame +4 | tried: one first-pass shape | next: named group iterator locals, def pointer in own block
- 0072c790 CTCSpecialAbilities::SetupEnflameSpell DIFFER(413v413): frame +8 (`sub esp,0x130` v 0x128): the IP temp shares the uninit inventory/key word in retail | tried: scoped temp, inline operator=, out-param lookups | next: lane gap: frame-slot sharing
- 0079a890 CTCSummonSpell::IsValidTeleportPos DIFFER(497v508): frame 0x48 v 0x34; retail reuses the arg1/arg2 slots for the blocked flag and the list node spill; this register and the early-false placement differ | tried: one first-pass shape | next: blocked flag / node as locals declared after the args die; early return written in full
- 006e3580 CTCTalk::PlaySpeech DIFFER(490v502): retail keeps a dead `player = 0` store in the key/player word, puts the __find tag temp in the lipSync slot, and has name/end in ebp/edi (ours swapped) | tried: named tag, thing local (fixed OnDialogueStart order), ternary lookup | next: tag as a by-value parameter of an inline wrapper
- 008f0eb0 CTCTavernGameCardBase::CardsTurning DIFFER(487v506): separate release+ret for the still-turning return, def in ebp, def slot reused for height | tried: 5 shapes | next: early return written with an explicit def release
- 008f10b0 CTCTavernGameCardBase::ShuffleCards DIFFER(466v410): def frame word reused for the height counter, the by-value layout sub-def copy kept in esi with an in-register refcount bump, frame 0x20 v 0x34 | tried: by-value layout temp + do/while (structure), first pass 427v410 | next: a dtor without the null test on the layout copy; height in a block-scoped local
- 007d75a0 CTCTavernGameCardPairs::GetRandomCard DIFFER(468v465): derived vptr store before the base Handle/flags init; mirror index folded to [edx+eax*4-4] (retail sub/dec); inner search exits via it!=last | tried: packed card hierarchy, forceinline operator=, name-field wrapper, goto/continue loop forms (8 probes) | next: base init in an out-of-line-free inline intermediate ctor; unsigned index through an operator[] taking size_type
- 007b8b10 CTCTavernTable::GetEatingPoint DIFFER(408v417): &pointIndex in ebp and start index in ebx swapped; map start loaded into eax first; loop-head pad | tried: search as member/free inline, startIndex scopes, named parent (6) | next: lane gap: register web
- 006de080 CTCTrophy::StartMinigame DIFFER(490v497): retail tests the TC 0x10 presence bit twice (`mov ecx,10000h ; test ecx,edx` x2); ours CSEs them into one `test [eax+20h],10000h` | tried: bool/word HasTC, bool lookup, commuted mask, nested if (6 probes) | next: lane gap: CSE of duplicate presence test
- 00659625 CTCVillage::UpdateTorchTasks DIFFER(466v475): `night` in bl and the building in edi with an [ebp-4] spill in retail (ours spills night to [ebp-1], building in ebx); fps load before the second GetWorldFrame | tried: one first-pass shape (control flow reproduced) | next: night as a named bool computed after the static init, building pointer named local spilled via address-taken temp

## Skipped
- none (every row holds one function)

## Label corrections
- 00723c80 is CTCPhysicsStandard::GetRHSet (the catalog says CTCPhysicsRigidBody; the byte-identical bodies were propagated).
- 00773790 is CTCShop::GetAttitudeSellPriceMultiplier (the catalog says GetAttitudeBuyPriceMultiplier; it reads the Sell offsets +0x70/+0x74).
- 005b48a2 is CTCInventoryClothing::OnItemSelected (the catalog says CTCShop::GetName; vtable 0x0124356C slot 33).
- 007f53b0 is CTCSoundAtmosphereVillage::FrameUpdate (the catalog says vfunc_9; vtable 0x0124111C slot 9).
- 007320e0 is CTCSpotLight::OnSerialise (the catalog says vfunc_1; CTCBase vtable slot 1).
- 007727e0's engine_api name is correct, but the row is an inline-assembly body and should be treated as oracle-only.
- Callee labels used here that are wrong in engine_api / forge labels:
  - 0x006A2220 is CTCCarrying::CollectCarrySlots, not RemapGroups.
  - 0x0049D870 is CWorld::GetWorldFrame.
  - 0x00686D30 / 0x00686A80 / 0x00686A70 are CTCBase GetWorldMap / GetWorld accessors, not GetDisplayEngine.
  - 0x0065FF5E is IsThingInTaskList<CVillageTaskTorch>, not CPersistTraits_IsModified.
  - 0x007D9A80 / 0x008F27A0 are GetSubDef bodies under unrelated template labels.
