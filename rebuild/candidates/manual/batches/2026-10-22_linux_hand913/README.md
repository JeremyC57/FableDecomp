# Hand chunk 913 (CRGBFloatColour .. CTCTavernGameCardBlackjack): 30 of 33 byte-exact

The template says "of 40"; this boundary chunk has 33 rows. 30 matched, 3 parked, 0 skipped. Every matched row has a behaviour test that passes under g++ and VC7.1. No row needed /GS or /EHsc. Leaf symbols are listed in STATUS / the sources.

## What matched (30)
- **CRGBFloatColour** (0043e470): the ctor from CRGBColour (B,G,R,A bytes) calls an inline `Set(r,g,b,a)` with `colour.X * (1.0f / 255.0f)` (pool 0x01231724 = 1/255).
- **CSoundEmitterLH** (10 rows: 00aab9c0, 00aaba00, 00aaba10, 00aaba20, 00aaba40, 00aaba60, 00aaba80, 00aabac0, 00aabb00, 00aabb60). Model: the manager at +4 has its system at +0x40; the position (C3DVector) is at +8; the VC6 list sentinel is at +0x14; the flags are at +0x18..0x1a.
  - The two forwarders take `CSoundSystem* system` as a named local.
  - vfunc_17 returns the position through a C3DVector with a user member-wise copy ctor.
  - vfunc_11 names `CActiveSound* sound` inside the walk.
  - vfunc_21 names the system and `long state`, then returns `state == 2 || state == 1`.
- **CSoundManagerLH** (4 rows: 00a3e330, 00a3e350, 00a3e380, 00a3e3c0): `if (m_pSoundSystem) m_pSoundSystem->GetPlayer()->...` with a direct member test.
  - vfunc_7: the player's slot 9 takes an `int`, which gives retail's `xor ecx,ecx ; test bl ; sete cl`.
- **CTCActivationReceptorBase::FrameUpdate** (007f7870):
  - Retail layout: Triggers is a 12-byte vector at +0x10 with an inline `empty()`.
  - The STLport `find_if` wrappers are `__forceinline` and call `__fastcall` `__find_if` instances, passing `IteratorCategory(first)`.
  - The empty predicate and tag classes need user ctors.
  - It compiles to a jump table.
- **CTCCreatureGeneratorCreator::OnSerialise** (007c9770): the CTCActivationTrigger model with pack(1). The saving path builds a local uid through an inline `GetUID()`; loading remaps when `GeneratorUID != 0`.
- **CTCHero::OnCreate** (006ca5a0): a chained GetWorldMap region lookup.
  - The region def is held by value with a null-tested `Release()` dtor.
  - `SetWeaponsAsUsable(!PreventHeroWeaponUse)`.
- **CTCInventoryItem::OnCreate** (005d9380): `GetThing()->GetDef()->GetSubDef(PDef)`.
- **CTCKraken::SetPhase** (007fc820): combat and thing are locals taken before the stores, then a switch (jump table).
  - `CThing* target = PTarget.GetPItem();` is a named local.
  - The inline `InitTentaclesAndHead` uses `CThingList::empty()`.
- **CTCKraken::OnInitialActivate** (007fc8e0): a block-scoped `thing` local for the GetInterface and GetSubDef pair.
  - `LeaveExperienceOrbsOverride` is bit 5 of a bitfield at +0x15c.
- **CTCQuestCompletionUI::IsHeroControlledByPlayer** (005e3ea0): a switch on the main player's mode, compiled to a byte-index table.
- **CTCSmashableBarrier / CTCSmashableWindow::Smash** (007eeb20 / 007f4e10, byte-identical): the base Smash, then `CreateEffectThing(..., &GetThing()->GetPosition(), false)`.
  - CreateEffectThing is `__fastcall`.
  - The inline `GetThing()` forces the early ParentThing load.
- **CTCSmashableBase::OnDie** (007eec50): `bool smashable = GetDef()->Smashable;` is a named local, then the virtual Smash (slot 28).
- **CTCSmashableBase::OnCreate** (007eec90): same-TU `auto_inline(off)` stand-ins for GetDef and GetSubDef, and a hand-written release.
- **CTCStockItem::OnCreate / OnSerialise** (007f4e80 / 007f4ed0).
  - OnCreate uses a GetSubDef stand-in and an unconditional release.
  - OnSerialise makes three Transfer calls ("ForSale", "Stealable", "Price").
- **CTCTavernGame::SetStateAfterOK** (005eb080):
  - The first quit test is written out in full.
  - The second comes from an inline `SetState(44)` helper (the SetStateAfterTime body).
  - VC7.1 then merges both exits into the FIRST block, as retail does.
  - The sound name is a block-scoped `CCharString`. The two `L""` arguments are implicit temporaries.
- **CTCTavernGameCardBlackjack::FrameUpdate** (007da360): `if (s >= 28) { if (s > 37) { if (s == 38) TurnPlayerCards(); } else UpdateGameState(); }`.

## Parked for Fable 5.1 (3)
- 00567d60 CNetString_Retail::Append(first, last, forward_iterator_tag) DIFFER(281v280): every byte matches; ours has one extra trailing `int3` after the noreturn ThrowLengthError (0x565A30) call, and q.py strips retail's trailing cc | tried: noreturn helper, forceinline CopyTrivial and UninitializedCopy | next: lane gap: verifier strips the trailing int3 (GOTCHAS chunk 249); land as matched if the lander compares unstripped bytes.
- 005aceda CTCInventoryTrade::OpenInventory DIFFER(1113v1067): retail homes `this` at [ebp-8] and pushes ebx/esi/edi only after the sound block. Also:
  - ours hoists `lea ecx,[ebp-4]` into the latch of the barber and tattoo loops;
  - the third special-shop tail is not cross-jumped to the ctor block;
  - the first normal loop is entry-tested (`cmp ; je`) instead of `jmp cond`;
  - the end block reloads `this` into ebx.

  | tried: out-param and HasTC/GetTC lookups, a forceinline body helper, optimize("gs"/"gsy"/"s"), forceinline CDefPointer release (fixed the def slots and the conditional-temporary flags), named CCharString locals (fixed the -0xC slot), for-loops, a reference local for PTop_Level_List | next: lane gap: callee-saved push placement / `this` homing (LTCG-style size TU). Worth one advisor pass on the loop-latch lea.
- 005e3f00 CTCQuestCompletionUI::DrawPreparePrimitivesForRendering DIFFER(295v295): retail emits `mov ecx,[esi+0BCh]` (PBlack) before `fstp qword [esi+48h]` (LastRenderTime); ours stores first | tried: plain form, inline GetBlack, const-ref local, `double now` local, a named PBlack local (it caches PBlack in edi), /Oa, an UpdateRenderTime helper | next: lane gap: store/load scheduling.

## Skipped
None.

## Label corrections
- 007f4e10 is CTCSmashableWindow::Smash; the manifest says CTCSmashableBarrier (the bodies are byte-identical).
- 005d9380 catalog `vfunc_4` is CTCInventoryItem::OnCreate.
- 007c9770 manifest `vfunc_1` is CTCCreatureGeneratorCreator::OnSerialise.
- 00567d60 is the STLport `basic_string<char>::append(first, last, forward_iterator_tag)` (CNetString_Retail), not CTCElectrocutedEffect::ReplaceString.
- 005ea950 is CTCTavernGame::PlaySound(const CCharString&) (manifest: CTCBetting PlaySoundW).
- 0059d9ad is CTCInventoryTrade's trade-list reset, not CMemoryAllocator::ClearPerformanceStats.
- 005bf482 is labelled CTCInventoryBase::GetAllItems; it fills a vector<pair<long,long>> (name kept).
- 005d9aa0 is GetTattooCardDef; the manifest says GetHairCardDef (that is 005d9a10).
- 00593139 is vector<pair<long,long>>::erase(iterator), not MedianCut::GetNewTreeNode.
- 00772510 / 00772590 carry the propagated label CTCShop::IsBarberShop; they are other shop-kind queries (0x00772490 is the barber one).
- Callee labels confirmed while authoring:
  - 0x686d30 GetWorldMap, 0x686a70 GetWorld, 0x4fb490 GetRegion, 0x6869d0 GetDefinitionManager, 0x7aed10 CTCBoss::GetCurrentPhase.
  - 0x7eeea0 / 0x59350c / 0x7fcd50 GetSubDef; 0x4045c0 / 0x410be0 / 0x44bb00 Transfer<bool / long / uint64>.
  - 0x4c73d0 CThing::GetPosition, 0x4b8050 list clear.
