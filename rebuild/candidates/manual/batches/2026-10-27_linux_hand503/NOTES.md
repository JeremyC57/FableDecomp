# Chunk 503 notes (agent503)

The chunk has 30 rows. 26 are matched (1 MATCH, 25 RELOCATION_MATCH, no retail_asm), 2 are parked as DIFFER and
2 are skipped because they are not function entries. Every matched row passes its test under g++ (tall.sh) and VC7.1
(tall.py). The vland dry run reports WINS 26, with no RELOC_IDENTITY, RELOC_DUPLICATE or RELOC_CONFLICT.

## Proven
- 0x004BC670 NGraphicAppearance::CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject&, float) const
- 0x0053B717 NUISystem::CList::CList(const CList&)
- 0x006299C0 CWorld::EAExpressionWait(const CGameEvent*)
- 0x00634890 CPlayer::InitModeClickPastText
- 0x00634F60 CPlayer::InitModeNavigateInventoryMapScreen
- 0x006360A0 CPlayer::InitModeChargeUpWillSpell
- 0x006369B0 CPlayer::UninitModeLiveGUI
- 0x006BB860 CGameTimeManager::ConsoleSetTimeOfDay (parked)
- 0x007A75F0 CTCBattleCharge::FinalCleanup
- 0x007B74A0 CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector&, ulong, ulong, ushort, float, float, float, const C2DVector&, EEnginePrimitiveBlendMode, uchar x4, float, EEngineInterpolationSet, EEnginePrimitiveBlendOp)
- 0x007C4320 CTCHealLife::FinalCleanup
- 0x007C90A0 CTCFireballSpell::FinalCleanup
- 0x007CCD70 CTCMultiStrike::FinalCleanup
- 0x007F2120 CMockSpline::GetLength() const
- 0x00809650 std::__linear_insert<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>, (anon)::CDoNotUseBinaryPredicate>
- 0x00836C50 CThingTrackNode::ConstructFromParams(long, const C3DVector&, CCharString)
- 0x0090AF80 LHNetworkLib::LSocketServer::GetUDPPacket(std::vector<char>&, LAddress&)
- 0x00917E30 CAIStateGroup_Clean::GetBench (parked)
- 0x00926980 OrderByHeight(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)
- 0x00938CD0 CIsFreeSeat::operator()(const CIntelligentPointer<CThing>&) const
- 0x009E5360 CSymbolMap::RemoveSymbolsOfGroupType(const CCharString&)
- 0x009E6990 std::remove_copy_if<pair<CCharString, CSymbolMap::CSymbolInfo>*, ..., CPred_HasGroupName>
- 0x009ECB70 CConsole::ConsoleRunScript(CConsole&, const std::vector<CGenericVar>&)
- 0x00A4AC60 std::vector<CLipSyncPhonemeSample>::_M_insert_overflow

## Probable
- 0x0052B030 std::vector<T16>::_M_insert_overflow: the STLport body is certain, but the 16-byte element type is not proven.
- 0x008E97B0 NCreatureMode::CModeStrafe::LockStrafingCombinationAnimsToMainAnim
- 0x0099A140 std::_Rb_tree<CUnprovenKey_00999e50, ...>::_M_copy: the body is certain; the instantiation is the 0x00999E50 tree family.
- 0x00A49E00 std::__copy<CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, int>

## Descriptive
- 0x0052B030 element spelling CUnprovenElement_005269b0 (and CUnprovenElement16_005269f0 for 0x005269F0's binding)
- 0x0099A140 key and value placeholders CUnprovenKey_00999e50 / CUnprovenValue_00999e50

## Label corrections
- 0x00A0F6EA "Deque_InsertRange" and 0x00A0F98B "Deque_InsertRangeReverse" are not functions. Both are blocks inside the
  deque-insert body that starts at 0x00A0F4B0 ("RenderList_ProcessLargeInsert"). They are reached only by the jcc at
  0x00A0F5BC and the jcc at 0x00A0F85F, and they read esi/edi/ebx/ebp without setting them. They should be dropped
  from the function catalog. 0x00A0F7C6 is also mid-body.
- 0x006359C0 and 0x00633670 are landed as InitModeClickPastText and UninitModeClickPastText. They are really
  InitModeYesNoQuestion and UninitModeYesNoQuestion (mode 15, the +0x144 input process). The real
  InitModeClickPastText (mode 14) is 0x00634890.
- 0x008E97B0 is bound by the landed OnDeactivate as `ResetStrafeAnims_008e97b0`. Its real name is
  CModeStrafe::LockStrafingCombinationAnimsToMainAnim.
- The landed ability-TC stubs are vtable slot 14 (OnKill) and slot 2 (OnRemove) jumping to the private FinalCleanup.
  They are not FinalCleanup:
  - CTCBattleCharge 0x007A7BD0 / 0x007A7BE0
  - CTCHealLife 0x007C4410 / 0x007C4420
  - CTCFireballSpell 0x007C9160 / 0x007C9170
  - CTCMultiStrike 0x007CCE00 / 0x007CCE10
- 0x00A49BC0 / 0x00A49C10 / 0x00A4A2A0 are landed under stand-in names (UninitCopy_PairCharStringLongCountedPointer,
  UninitFillN_SPairCharStringLongCountedPointer, std::_Destroy_range). They are std::__uninitialized_copy /
  __uninitialized_fill_n / __destroy_aux over CLipSyncPhonemeSample. vland warns RELOC_NAME on all three.
- 0x004C3200 (catalog "~C3DClothPrimitive") is C3DMeshObject::~C3DMeshObject: it stores vtable 0x01239860, releases
  the counted mesh pointer and tail-jumps to ~CBaseClass. It is unbound; 0x004BC670 calls it as `??1C3DMeshObject@@UAE@XZ`.
- 0x007F21C0 (unnamed, no direct callers) is the lightning TU's CMockSpline::GetPositionAtIndex: it takes a float,
  clamps it to [0,1] and scales it by GetLength.
- 0x0099E480 (labelled CPersistContext::GetDefaultVal<CAnimationSet>), 0x00487150 (labelled ...CHTG_Executioner::
  OnPredicateFail) and 0x00A24270 (labelled CMeshDataBank::PeekMesh) are CCharString::ToWideString,
  CPlayer::EndSpecialMoveMode and CMeshDataBank::GetMeshInfo by their bindings and use (vland RELOC_NAME).
- 0x004B9D00 is labelled as a map<CCharString,float> _Lbound. 0x009E5360 calls it as
  _Rb_tree<CCharString, CCharString, _Identity...>::_M_lower_bound (set<CCharString>). The code is shared across
  identical instantiations.

## Header debt
- 0x004BC670: shared headers exist for CMeshDataBank, CWorld, CTCPhysicsBase and CTCBase, but the local retail-offset
  models stay (vland SHARED_HEADER_AVAILABLE).
- 0x006299C0: same for CCreatureActionBase, CGameEvent, CPlayer and CWorld.
- 0x007A75F0: same for CPlayer.
- 0x00809650: same for CWorld.
- 0x00836C50: same for CThingGameObject.
- 0x0053B717 reuses the landed CList(long) 0x0053B63E model verbatim. That model installs the three vtables through
  `SelectedChild(InstallListVirtualTables())` instead of a real polymorphic class. Fix both sources together.
- 0x0099A140 defines CUnprovenKey_00999e50 as one `char`. The landed family header (0x00999FE0 / 0x0099A0B0) says
  `long`, but this body copies the node value as one byte. Reconcile at landing.

## Review at landing
- 0x00487DC0 and 0x00487DD0 are byte-identical (`add ecx,2Ch ; jmp GetPItem`). The bindings name 0x00487DD0
  GetPControlledCreature, so 0x006299C0 / 0x006360A0 / 0x006369B0 call 0x00487DC0 as
  CPlayer::GetPPlayerCharacter() const. Both are Ego_r.pdb names.
- 0x005269B0 and 0x005269F0 use two different descriptive spellings for the same 16-byte element.
- The 0x004B9C10 / D00 / D50 tree helpers are bound under map<CCharString,long>, but 0x009E5360 calls them as the
  set<CCharString> instantiation.
- 0x0049D850 CWorld::GetPlayer has a stand-in binding; the PDB spelling ends `IAE...K`.
- Check the PDB spelling of CTCSoundPlayer::StopAttachedSound (0x0066ADD0).
- 0x0090AF80 calls vector<char>::resize as `?resize@?$vector@DV?$allocator@D@std@@@std@@QAEXIABD@Z`. The address
  0x004122D0 has only stand-in bindings (CArrayChar / CArray<char>).
- 0x00926980 calls CThing::GetPosition() const 0x004C73D0 by its plain name. That address has only stand-in bindings.

## Parked
- 0x006BB860 CGameTimeManager::ConsoleSetTimeOfDay DIFFER(173v181): under /Op ours forwards the parameter slot [esp] into the member clamps where retail reloads Time [ecx+8] and copies it through edx->eax | tried: inline SetTimeOfDay (21v181 until __forceinline), __forceinline + `PrevTime = Time = time`, with and without /Op (153v181 without) | next: clamp Time itself after storing it rather than the parameter, or confirm whether retail built this TU with /Op
- 0x00917E30 CAIStateGroup_Clean::GetBench DIFFER(140v134): ours reserves sub esp,8 (key/pred/result in [esp+8]) where retail uses a single push ecx slot; every other instruction is identical | tried: a user CIsFreeBench() ctor (removed a zero-store), result-in-local variants, a /FAs probe | next: find what owns the second slot (the empty predicate temporary or the find_if result iterator); try a named const predicate or returning find_if(...)._M_node directly
