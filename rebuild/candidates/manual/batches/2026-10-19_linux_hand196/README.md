# Hand chunk 196 (CMinionEscort .. CMurderTwistSheriff): 31 of 40 byte-exact

All 31 src/ rows grade RELOCATION_MATCH in a final gall.sh run; all 31 tests print PASS under g++ (tall.sh) and VC7.1/Wine (tall.py).
The previous agent's 6 sources (00741ee0, 00743be0, 008c7c00, 008d8620, 008d86e0, 008e78c0) were re-graded (all match); their 4
missing tests were written, and the IceTroll CIcicleData stand-in was corrected to the PDB's 0x24 bytes (bytes unchanged).

## What matched (31)
- NCreatureMode mode constructors (00740450, 00740640, 008cc0b0, 008cc180, 008d7e60, 008d81f0, 008d8120, 00741fe0, 00741db0,
  00868220, 008e1580, plus the previous agent's six): the filter is built from the literal's implicit CCharString temporary;
  zero words share the register the CRC null test zeroed; vectors are the STLport `_Vector_base` + `_STLP_alloc_proxy` model;
  `PCreature = thing` stored last is a ctor-body assignment. CModeScorpionKingTailPlunge/Vulnerable put intelligent pointers at
  odd offsets (+0xa1, +0x7d): pack(1) on the class (TailPlunge) or on the pointer classes only (Vulnerable, whose vector stays
  4-aligned at +0x88). CModeIntangible's base is an inline CModeDragonBase(thing, const CCharString&) ctor, so the "" temporary
  is built before CModeBase(thing). The StrafeBurninate path ctor needs a CCountedPointer with a user copy ctor (built in its
  argument slot) and a C3DVector user `operator=` (plain `[esi+N]` stores instead of `lea` copies).
- CModeStrafe: ~CModeStrafe 007420e0 reuses the landed 0x743000 novtable/counted-pointer model; the ctor is a plain init list.
- CModeBase::OnActivate 0073aac0: the carried thing is a `const CThing*`, so its weapon lookup binds the const LowerBound copy
  0x4365B0 while the carrier lookup binds 0x40F020; the weapon arm calls SetGroup out of line (0x73DFF0), the "STAND" fallback is
  an inline copy of the same assignment + CRC.
- CModeBlock::OnActivate 008d8ce0: the landed BlockBase sound block; the CThingAnimatable root must carry the vptr (a
  non-polymorphic empty base under a polymorphic model shifts `this`).
- CModeCarryingStandBase::FrameUpdate 008d88a0: `if (type != 1) {...} else Remove(0x18);` puts the remove arm after the return.
- CModeCreatureIsProjectile::Recover 008e08c0: the recover combat action is an unnamed temporary of a derived class with an
  inline ctor and implicit dtor (out-of-line ~CCombatActionBase, no vptr re-store).
- CModeOutOfActionBase OnActivate 008d7270 (frame rate read into a local before the virtual call -> spilled + `fimul`) and
  OnDeactivate 008d8f30 (HasTC + GetTC whose end/hit choice is ONE ternary, plus the component's inline ClearFlag, gives the
  component straight in ecx).
- CModeIceTrollGroundSmashIcicleAssault::OnDeactivate 008d9650: begin() cached in a local, `while (it != end()) erase(it);` with
  the landed STLport vector::erase (0x84F570) and `__fastcall __copy`.
- CMouseDX ctor 00ab5d00: only InputManagerDX and the CursorGraphic pair are initialisers; DMouse/MouseStep/InputEnabled and the
  rest are body assignments around CMouse::ClearStoredEvents; ShowCursor is the USER32 import.
- CMirroredTextureHighlightsChunk::ReadFromFile 00ac5080: the landed bone-mask chunk model with 4-byte elements.
- NPlayerGui::CMultiStatBar::AddStatUpdate 00647457: size + `optimize("y", off)` TU; the new update aggregate is filled
  Change, Colour, Type (retail store order) before push_back; the merge arm is a `goto` to the shared mode tail.
- Entity scripts: CMinionEscort::Main 00da45c0 (landed CHTE_Prisoner::Main shape), CMurderTwistSheriff::Init 00eaee80 and
  CMurderTwistAssassinGuard::Init 00eb5500 (CGameScriptInterface slots from rtti-vtables.tsv; SetIsPushableByHero takes
  CScriptThing by value; slot 67 of CScriptThing is SetFriendsWithEverythingFlag).

## Parked for Fable 5.1 (9)
- 008ddfb0 NCreatureMode::CModeDragonStrafe::CModeDragonStrafe DIFFER(202v202): `push -1` issued before the three spline zero stores (retail after) | tried: init-list and body-assigned CArray/CCardinalSpline ctors, user dtors | next: lane gap: scheduling
- 008e72d0 NCreatureMode::CModeIntangible::CModeIntangible(CThing*, float, CCreatureActionBase*) DIFFER(186v186): inherited filter CRC null test in eax/ecx where retail uses ecx/eax | tried: GetPString/explicit guards, named timer local | next: lane gap: register allocation (the 0x741DB0 overload with the same base model matches)
- 008d8dd0 NCreatureMode::CModeBlock::AnimUpdate DIFFER(196v196): entry->AnimIndex loaded into ecx (retail edx), &init in edx (retail eax) | tried: pointer vs float zero pair in CAnimInit (the float pair fixed the immediate zero pushes) | next: lane gap: register allocation
- 008d6da0 NCreatureMode::CModeConversation::AnimUpdate DIFFER(222v222): parent thing via edx (retail eax), AnimIndex via eax (retail edx), &init via ecx (retail eax) | tried: as 008d8dd0 | next: lane gap: register allocation
- 008da7a0 NCreatureMode::CModeDrained::OnDeactivate DIFFER(227v227): `this` in edi and &PDrainer in ebx (retail ebx / edi, edi pushed after the state test) | tried: if/else-if, switch on the state byte | next: lane gap: register allocation
- 008da2e0 NCreatureMode::CModeScorpionKingExhausted::OnActivate DIFFER(200v195): frame 0xc v 8 (retail reuses the out CDefPointer slot for the interface key and loads the def straight into ebp) | tried: ternary HasTC/GetTC, `T* p = 0; GetInterface(&p)`, block-scoped out pointer, inline TakeBattleDef helper | next: a holder/out shape from a landed sibling that shares the out slot with a later key
- 008e7f80 NCreatureMode::CModeBriarRoseConcentrate::GetRandomImposter DIFFER(223v223): second loop index in edi, element address in ebp (retail ebp/edi) | tried: unsigned/signed index, named element reference | next: lane gap: register allocation
- 008e9950 NCreatureMode::CModeStrafe::UpdateWeaponState DIFFER(223v215): retail shares one SetChangeOfWeapon(weapon) block between the usable/unusable arms of the flip test, ours emits two | tried: ternary / `||` change test, out-param vs returned GetTC (one-ternary GetTC fixed the component-in-ecx), explicit if/else flip test | next: `if (usable != LastUsable)` combined with the one-ternary GetTC (not tried: attempt limit)
- 008ea220 NCreatureMode::CModeStrafe::FrameUpdate DIFFER(228v226): joystick device fetched into eax then `mov ecx,eax` (retail straight into ecx) and the parameter strength stored after the address push | tried: named device local, named parameter object, ctor-body CRumbleParams, entry reference | next: an inline input-manager accessor returning the device

## Skipped
None (no row holds two functions).

## Label corrections
- 0x008D81F0 is NCreatureMode::CModeScorpionKingTailPlunge::CModeScorpionKingTailPlunge (vtable 0x01285860), not CModeJackOfBladesFlash.
- 0x00655F36 is list<NPlayerGui::CMultiStatBar::CStatUpdate>::push_back (insert before the end node), not "ListPushFrontWrapper".
- 0x0070C050 fills a CAnimInit record (CAnimInit::Set, 7 dword arguments), not CInitRumble::CInitRumble; landed rows call it CInitRumble::Set and CRumbleEffectBase::Initialise (canonical-name sweep needed).
- 0x0073A6E0 CTCCreatureModeManager::GetCurrentMode (manifest "CPixelShader::CountReferences"); 0x0073DFF0 CAnimFilterOfGroup::SetGroup (manifest "CRCHashedString_SetAndHash"); 0x0073A680 a CModeBase world getter (`[game]+0x24`; manifest "CViewBase::GetDisplayEngine").
- 0x00A66F20 CMouse::CMouse (manifest "NHeroInformationScreens_Mouse_Constructor"); 0x00A66B10 CMouse::ClearStoredEvents (manifest "CGameEventPackage::Clear").
- 0x004AA840 ~CScriptThing (manifest "~C3DClothPrimitive"); 0x0070D8C0 CAnimateThing::RemoveCombinationAnimsOfType (manifest "CSpellContainerList::AddChild"); 0x005DC1F0 is a CAnimationEntry member (manifest class CCombatActionBase).
- 0x007EA270 the type-9 component's flag setter (manifest "CObservable::AddExclusiveObserver"); 0x006D5AB0 a creature brain-reset forwarder (`mov ecx,[ecx+1A8h] ; jmp 0x88BB60`; manifest "_Cons_val<...>").
- 0x008C5A50 GetSubDef for the scorpion-king battle def (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>"); 0x008E34C0 STLport __copy<CIcicleData*> (manifest "_Move<...>"); 0x008E8A10 the BriarRose imposter predicate (manifest "CAIStateGroup_VersusScorpionKing::FindChargeEvent"); 0x00789A70 a type-0x81 component's float getter (manifest "CMap::DrawGetSeaLevel").
- 0x013B8388 is the input manager (joystick vector at +0x2c); landed rows name it both g_pInputManager and g_Loader.
