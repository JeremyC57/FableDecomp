# Hand chunk 290 (CList .. CMenuEntry): 27 of 40 byte-exact

27 RELOCATION_MATCH (gall.sh), all 27 with behaviour tests passing under g++ (tall.sh) and VC7.1 (tall.py);
10 parked in differ/; 3 skipped (inline-asm lane gaps). Lane compiler: RTM cl 13.10.3077 throughout.

## What matched (27)

- **NUISystem::CList** (/Os + optimize("y", off) TU)
  - 0x0053AD07 AddScrollingChildAndSetOffsets -- writes the next scrolling position into the child's states 1/4/5 (0/3 on
    def request) and the clamped alpha byte. Shape: advisor A3's `c = child.Data` named local lets the map key store sink.
  - 0x0053B0BF ComputeScrollBarPosition -- counts visible / hidden-before children and places the thumb. Shape:
    `__forceinline ~CDefPointer`.
  - 0x0053B321 RemoveChild(long) -- shifts the following rows, clamps the selection, erases the row / state map /
    visibility entry, re-arms states, recomputes offsets. Shape: a cached `last` end local in the scrolling find but a
    fresh `end()` in the second find (keeps &vector, not &finish, in ebx); arrows cached in locals; a named
    CListDefPointer lands in the dead argument slot; `if (b) f(0); else f(6);` (a `?:` becomes sbb).
- **CLocalDetailObjectCollectionType** 0x00BE27B0 Load -- three resource ids (binary or bank-resolved names), 0x34-byte
  parameter block, id resolution.
- **CLocalDetailPrimitiveMeshZSpriteBatch** 0x00BFB210 Load (catalog vfunc_8) -- bounds, objects from the landscape
  memory manager, bulk z-sprite read; false on allocation failure.
- **CLookAtMe1** 0x00EF5CB0 Main -- under an alive filter, waits for the hero to use Me and puts OBJECT_HERO_NO_HAT on him.
- **CMC_Minion** 0x00D9F270 / **CMC_SpawnPoint** 0x00D9EE20 Main -- minion-camp targeting and spawning (rand() % 3 picks).
- **CMK_GHC_HERO3** 0x00D29DB0 Main -- Hook Coast hero marker: objective, distance wait, CS_GATEWAY_ABBEY cut-scene.
- **CM_BanditTrigger** 0x00E1E960 Main -- escort bandit trigger: spawn point from Me's data string, distance wait,
  off-screen wait with a 3-tick CTimer, random escort bandit type, optional container item. Shape: on the local
  CScriptThing the inline-defined virtual GetPos is devirtualised AND inlined; the declared virtual GetDataString becomes a
  direct call. `a != "" && b != "NULL"` gives the conditional-temporary ebx flag. Model: HobbeLeader (0x00D8FA30) GameCalls.
- **CMagicBarrierGraveyard** 0x00CE0600 / 0x00DA5490 Main -- re-create the force-field effects whenever both are dead.
- **CMainGameComponent** 0x00416953 PostInit (world / save load, static-map export) and 0x00417001 Render
  (interpolation info, forced catch-up frames). Shape for Render: write the fsub before the integer stores.
- **NUISystem::CManager** 0x0041DC9D AddActionsListenIgnoreOnList, 0x00427F31 CreateStaticSlider, 0x0042819A
  CreateStaticSliderBidirectional -- /Os y-off TU modelled on the landed CreateLevelSlider 0x00427DC9.
- **CMap** 0x0081F6C0 GetGroundFaceNormalAt -- triangle pick by diagonal parity, cross of edge vectors, fast normalise.
- **CMapInfo** 0x004FB4F0 LoadBinary -- names, flags, map area, Placed, 64-bit UID (low dword kept).
- **_Rb_global<bool>** 0x0042A34E _Rebalance_for_erase (catalog: CMemoryAllocatorVariableSize::Erase).
- **CMeetSisterMessenger** 0x00E292C0 Main -- ArenaExterior wait, cast Hero/Messenger, CS_ARENA_LEAVE_THERESA_01 in a paused
  movie, remove Me. Shape: a plain `inline` map dtor expands only at the first exit and calls the out-of-line copy
  0x00CDBFB0 at the rest; the map ctor with `head(0)` explains the double count store.
- **CMeleeThunder** 0x00D58080 / 0x00E386E0 Main -- stage-gated scripted control; jump-from-chest animation then follow.
- **CMemoryDataInputStream** 0x00995B10 ReadCompressed -- in-memory LZSS decompress with the 3-byte tail read.
- **NUISystem::CMenuEntry** 0x0053D720 GoForward, 0x0053DA30 InitializeRumbleActions, 0x0053E4E0 ~CMenuEntry.

## Parked for Fable 5.1 (10)

- 0x006F9BE0 NAnimationEvents::CManager::Save DIFFER(768v759): retail 0xa4 frame keeps the save context below the CSymbolMap temporary; ours 0x9c with it above, so every frame offset shifts | tried: the differ/ form (earlier probe history not itemised) | next: put the CPersistContext in an inner block opened after the CSymbolMap, or hoist the CDiskFileWin32 into its own scope
- 0x008203A0 CMap::ApplyScriptedMapBrushUpdate DIFFER(906v997): retail loads the area BRX, TLX, BRY, TLY, spills startX and {BRX,BRY} (0x48 frame) and re-tests the inner guard each row; ours hoists the guard and caches HeightMap in esi | tried: 7 grades (named area locals, re-read through this, guard as named bool, inner-loop restructuring) | next: lane gap: register allocation / LICM
- 0x00821850 CMap::InitCachedVolumes DIFFER(555v551): retail passes one shared empty allocator local (esp+0x13) to every _Destroy and the finder; ours makes a byte temporary per call; this in ebp vs edi | tried: 2 forms (iv_a, iv_b; STLport vector destroy loops in place) | next: a named allocator local in the outermost scope passed by reference to same-TU stand-ins
- 0x008E7040 CMeleeCombatAbilityDef::IsAbilityValid DIFFER(584v588): our entry esi/edi pushes are shrink-wrapped and the iterator-tag temporary gets its own slot (retail overlays it on the dead argument slot) | tried: 14 probes (find stand-in forms, named tag, const LowerBound route, exit order) | next: lane gap: locals in dead argument slots
- 0x009F20B0 CMemoryAllocator::CMemoryAllocator DIFFER(596v598): retail keeps a zero in ebx through the first loop and re-reads heap from its argument slot (frame 8); ours keeps heap in ebx | tried: 14 probes (named zero, loop forms, heap re-read) | next: lane gap: register allocation
- 0x00A22F80 CRightHandedSet::Rotate DIFFER(523v523): one commuted x87 fmul in the Up X row | tried: ~25 probes (operand order, Shoemake temporaries, inline matrix helper, advisor forms) | next: lane gap: x87 operand commutation
- 0x00A56270 CMatrix3x4::Orthonormalise DIFFER(706v693): retail copies the Y/Z words of both input rows into the frame with integer moves and keeps X on the stack; ours loads Y from the member and reorders the length sum | tried: 10 probes (member Normalise + CrossProduct, explicit copies, inline helpers) | next: C3DVector copies with a user copy ctor for both input rows before the cross
- 0x00A64910 CMemoryAllocatorVariableSize::RemoveFromTreeFixup DIFFER(756v757): register-only (retail this in ebp, info in edx, near nephew in edi, 4 pushes; ours 3 pushes + loop-align nop) | tried: 8 probes (inline_depth region, nephew locals, rotation forms) | next: lane gap: register allocation
- 0x00DC21F0 CM_RestartMarker::Main DIFFER(761v750): retail initialises the vector<CScriptThing> before the loop and runs its dtor 0x008AC970 per iteration and at late exits only; ours re-initialises it per iteration | tried: the differ/ form (vector scoped per iteration) | next: vector declared before the loop with an explicit per-iteration swap with an empty vector
- 0x00EA4DE0 CLostTraderTrader::MakeRandomComment DIFFER(528v692): row length only -- the 528-byte body + 5-entry jump table (0x00EA4DE0..0x00EA4FEF) is byte-identical modulo relocations; the catalog's 704-byte row runs into 0x00EA4FF0 | tried: 1 form | next: land with corrected length 0x210 (flag: switch jump table)

## Skipped

- 0x00417747 CMainGameComponent::UpdateForcedTickSpeed: inline-asm fnstcw/fldcw rounding helper (known lane gap).
- 0x00418289 CMainGameComponent::Update: inline-asm fnstcw/fldcw rounding + /GS (known lane gap).
- 0x00A5CB90 AlignedTransformTransposedAssumeW: SSE inline asm with an `and esp` stack realign (lane gap).

## Label corrections

- 0x0042A34E is `_Rb_global<bool>::_Rebalance_for_erase` (not CMemoryAllocatorVariableSize::Erase).
- 0x00BFB210 is CLocalDetailPrimitiveMeshZSpriteBatch::Load (catalog vfunc_8).
- 0x00A22F80 is CRightHandedSet::Rotate (not CMatrix3x3::MultiplyByQuaternionRotation).
- 0x006F9BE0 is NAnimationEvents::CManager::Save.
- 0x00EA4DE0 row length is 0x210, not 704.
- Callees: 0x0099A3B0 CBaseObject::operator=; 0x00704580 / 0x008AB1E0 shared bodies; 0x00CD23B9 / 0x00CD2770 IsScripting /
  Release; 0x0042D5B1 map operator[]; 0x00CBE87F AddLogbookStoryEntry; 0x007E72A0 the script-thing handle ctor;
  0x006E7AB0 the movie PImp dtor; 0x008AC970 vector<CScriptThing> dtor; 0x00533B50 AddChild; 0x00416258 time since the
  last render-frame start; 0x0053CD05 vector<state map>::erase (not CArray_BodyPartMesh::RemoveAt); 0x00533BC0
  CChangingStateComponent::RemoveChild (not CScriptBase_RemoveChildAt); 0x008274B0 / 0x008270E0 / 0x00825B40 STLport
  introsort loop / insertion sort / unguarded insertion sort (not Navigation quickselect / CVectorMap for_each);
  0x0042BED4 CAction::CAction.
