# Hand chunk 482 (std::vector .. CDefinitionManager): 26 of 40 byte-exact

All 26 matched rows grade RELOCATION_MATCH through gall.sh (Leaf/Flags honoured), and their behaviour tests PASS under both
g++ (tall.sh) and VC7.1 (tall.py). calls.py shows no import mismatch: list/vector nodes allocate with malloc 0xBFEA0E,
scalar new/delete go to 0xBFEA1A / 0xBFE9BC as in retail. All STLport code is modelled in namespace `std`.

## What matched (26)

- **STLport containers and algorithms (12).**
  - 008f6c90 `vector<CHeightMapCell>::_M_fill_insert` (/GS) and 0098d360 `vector<C4DVector>::_M_fill_insert`. Both are
    STLport 4.6 verbatim: `_Tp __x_copy = __x` is the only frame local. The helpers have same-TU noinline bodies, so the
    tags and `n - elems_after` are homed in the dead `x` slot.
  - 00913d60 `deque<CBezierCurve>::_M_push_back_aux_v`. `_Construct` must be `__forceinline`; a plain inline stays an
    out-of-line call.
  - 00927590 `__unguarded_partition<CIntelligentPointer<const CThing>>`. iter_swap is inlined, and the by-value
    comparator copies are built in the outgoing area, second argument first.
  - 00979760 / 0097b4e0 `__introsort_loop<CThing*>`. Both reuse the landed sort model, with operator() as a same-TU
    noinline stand-in.
  - 00993860 `__introsort_loop<CSoundBankEntry>` (/Gr). `__median` is inlined, and the pivot is copy-constructed straight
    into the partition's by-value argument slot.
  - 009934c0 `__partial_sort<CSoundBankEntry>` (/Gr). The `i < last` loop becomes a counted loop homed in the dead `last`
    slot.
  - 00993310 `vector<pair<long,CSoundBankRecord>>::_M_insert_overflow`. `(max)` returns a reference, and the new start is
    homed in the dead fill_len slot.
  - 00990900 / 00990dc0 `_Rb_tree<CPendingObjectSortKey,...>::insert_unique` (plain and hinted). Real STLport 4.6
    `_tree.c`. In the hinted one, operator++/-- and the key compare have same-TU noinline bodies; without them VC7.1
    duplicates the `insert_unique(v).first` tail and swaps ebx/ebp.
- **LHNetworkLib (3).**
  - 008fa120 `LSocket::ClearData`. `resize(0)` then `clear()` versus `clear()` then `resize(0)` over a `const _Tp&`
    resize; the char() temporary lands at [esp+13h].
  - 0090aa40 `LSocketServer::~LSocketServer`. The delete loops re-read `begin()`, because the sockets unlink themselves.
  - 0090b160 `LSocketServer::Think`. The resize fill char is a named local zeroed once before the UDP loop; /Oa is worse.
- **NCameraModes::CPhysicsForceModelBase (1).**
  - 008fbb90 `PotentialFieldBufferPoints`. This is the 0x00876E10 model with the step as a literal 0.1f; VC7.1 folds the
    division into a multiply by 10.
- **AI (1).**
  - 00910570 `CAIStateGroup_MinionListenToStory::StandUp`. It reuses the landed GetUpFromSitting 0x0085FF10 model, and
    matched first try.
- **CAFile (7).**
  - 00997bb0 `GetCopyFilename` (/Oa). Uses the float pow overload.
  - 00998010 `ParserReadPastComponent` (/Oa). A named bool result.
  - 00998250 `GetAbsoluteFilePath` (/Oa). The implicit `const char*` -> CCharString conversions build the temporary
    before any push.
  - 00998a30 `IsFilePathSyntaxValid` (/Oa). The PDB `ParserIs*Next` helpers are inline.
  - 00998c90 `ExpandPath`. A named last-index local frees ebp.
  - 00998f60 `CompareFiles`. The two `vector<unsigned char>(0x1000)` constructors are `__forceinline`.
  - 00999ab0 `IsRoomForFile` (/GS). Named `L""` locals in the forced-inline MakePath helper.
- **Startup / system (2).**
  - 0099e260 `InitExternalStrings` (@InitExternalStrings@0, /GS). HKEY, type, langID and size are declared at function
    scope, which gives retail's local order. `g_LanguageID = (WORD)(((v & 0xFC00) == 0) ? (v | 0x400) : v)` avoids a
    store-back to the address-taken local.
  - 009a5020 `CSystemManager::ExceptionHandler`. `IsValid() == true && ExclusiveMode == true`, then one switch; the FLT
    overflow and divide-by-zero cases share a body. Every case/string pairing was checked by hand against the jump table.
- **CDefinitionManager (1).**
  - 009af1e0 `CompileString`. The CStringParser ctor/dtor are inline, and the marker strings go through the implicit
    `const char*` conversion. `CBasicStringChar::operator==` 0x004115A0 has a same-TU noinline body under
    `auto_inline(off)`, so defType's block stays in ebx across the call. The engine header is used through a `_Methods`
    wrapper.

## Parked for Fable 5.1 (14)

- 008f8540 LHNetworkLib::LSocket::CheckForActivity DIFFER(344v344): exceptSet's zero is stored from ebx (readSet) where retail uses ebp (writeSet), and `mov eax,edx` (the switch value) sits before `xor ebp,ebp` | tried: four declaration orders of the three fd_set pointers | next: model the three sets as one array or a struct indexed by mode
- 008f8a80 LHNetworkLib::LSocket::LSocket DIFFER(302v302): only the /GS epilogue (retail checks the cookie before popping edi/esi/ebx; ours pops edi early) | tried: inline vs __forceinline InitWS, STLport list link order, QFE 4035 toolset (identical) | next: lane gap: /GS cookie-check-before-pops (GOTCHAS chunk 323)
- 008fa800 NCameraModes::CPhysicsForceModelBase::DoAttractToPoint DIFFER(496v496): the frame is 0x5C instead of 0x50 (one extra 12-byte vector temporary), and the falloff exp is stored twice (`fst` + `fstp [esp+8]`) | tried: float locals with casts (560v496), user C3DVector copy ctor, a named pull vector, scalar-on-left operator* | next: compute the Gaussian weight as a float member function of the attractor pair (retail may pass it through one call-free helper)
- 008fc050 NCameraModes::CPhysicsForceModelBase::UpdateScan<list<CPhysicsMeshObject>> DIFFER(935v984): frame 0xA8 v 0xC0, and this is spilled to [esp+5Ch] (retail keeps it in esi unspilled, the frame number in ebx, the ring in edi) | tried: the 876v876 body of 0x008FC430 with the inlined list GetPointIntersection (1 grade) | next: keep the intersection data and `point` in non-overlapping slots (declare `CMeshIntersectionData` at the template's scope), then revisit the register choice
- 008fc430 NCameraModes::CPhysicsForceModelBase::UpdateScan<const CWorldMap> DIFFER(876v876): scheduling only (retail hoists `mov esi,[esi] ; push 14h`, i.e. ring.end() and the malloc size, above the ycs.Z `fmul st(1) ; fstp`; ours places it two adds later, so three [esp+x] offsets differ by 4) | tried: push_back vs insert(end()), `point` declared before `end` (909v876) | next: lane gap: x87 list-scheduling order (every instruction is identical)
- 00927910 std::__partial_sort<CIntelligentPointer<const CThing>> DIFFER(276v276): esi/edi swapped throughout | tried: __forceinline / inline __pop_heap, non-forceinline copy ctor | next: a same-TU noinline __adjust_heap stand-in that never touches the copy (frees the comparator pointer from edi)
- 0096a3c0 CAIStateGroup_WhisperPotionCombat::GetValidJumpPosition DIFFER(823v872): register allocation (retail: this in ebp, the navigation pointer in a frame word reloaded per try, start/finish in ebx/esi; ours: this in edi, navigation in ebx) | tried: size()==0 vs empty(), end() captured after the empty test, void vs bool GetInterface | next: read the navigation interface into a named local inside the try loop (retail reloads it)
- 0098e7d0 CThreadedFileManager::CloseAllStreamsForFile DIFFER(883v840): retail keeps this in ebx (homed at [esp+14h]) and &CriticalSection in edi with a memory `inc [ebx+16Ch]`, and leaves the third Objects.find (after the callback) out of line (call 0x009902D0, hidden iterator result); VC7.1 here inlines it | tried: index loop, pointer/end loop, /Oa, named data pointer + count (gives retail's down-counter shape) | next: route the post-callback lookup through a same-TU noinline `_Rb_tree::find` stand-in (retail's 0x009902D0 copy) and keep the first two inline
- 009919c0 CASoundManager::RegisterSoundBank DIFFER(500v579): retail stores NeedsSort = 1 and re-reads it before the sort; the DisplayProgress temporaries are ordered differently and CVectorMap::Add is out of line | tried: 1 grade (uncacheable-flag candidate rule) | next: lane gap: uncacheable flag (volatile member, banned in sources); descriptive name
- 00991c10 CASoundManager::RegisterAtmosSoundBank DIFFER(233v338): the same NeedsSort store + re-read, DisplayProgress arguments built before its string temporary, CVectorMap::Add out of line | tried: 1 grade (uncacheable-flag candidate rule) | next: lane gap: uncacheable flag (volatile member, banned in sources); descriptive name
- 00996610 CDataOutputStream::WriteCompressed DIFFER(451v449): only the inlined work-memory vector(0x70000) ctor (retail: `xor eax` before the start store, p kept in edx across `rep stosd`, finish `lea eax,[edx+70000h]`; ours: `add edx,70000h`) | tried: memset return + n (451), `memset; finish = p + n` (443: CSEs the end-of-storage sum), an `if (size > 3)` split | next: an STLport `uninitialized_fill_n` returning `__first + __n` from a same-TU noinline `__uninitialized_fill_n` stand-in
- 0099fb60 GFIntVectorToCharString DIFFER(390v428): retail inlines UnassignString only into the result's final destructor (result's block in edi); ours calls it everywhere, and the visible-body variant inlines it everywhere (620v428) | tried: declaration-only Unassign, visible body, QFE 4035 | next: lane gap: per-call-site inline budget (VC7.1 inline order)
- 009a23e0 GFUpdateFontTextureCache DIFFER(473v495): register/slot choice (retail: dest in ebp, pixel size in edx, unroll counter edi, palette index through dl; ours swaps them) | tried: y as a pure counter with ++row, char cache memory, dest before the size local | next: a named `unsigned short* dst` per row inside the y loop
- 009a3460 ReadTextFileIntoArray DIFFER(424v424): register choice only (retail takes eax/ecx for the resize temporary and count + 1 in the big-endian and one-byte branches; ours takes edx/eax in all three) | tried: named count, per-loop index, unsigned-char buffer | next: lane gap: register allocation in three identical branches

## Skipped

None. No row covers two function bodies.

## Label corrections

The catalog name is on the left; the evidence (PDB name sets plus callers) is on the right.

- **Rows in this chunk.**
  - 008f6c90 "std_deque_insert_range<CSoundPair>" -> std::vector<CHeightMapCell>::_M_fill_insert
  - 008f8540 "CheckForActivity" -> LHNetworkLib::LSocket::CheckForActivity
  - 008f8a80 "Socket_Constructor" -> LHNetworkLib::LSocket::LSocket
  - 008fa120 "Socket_ReceiveDataAndBuffer" -> LHNetworkLib::LSocket::ClearData
  - 008fa800 "DoAttractToPoint_ComputeAcceleration" -> NCameraModes::CPhysicsForceModelBase::DoAttractToPoint (PDB, virtual, C3DVector by value)
  - 008fbb90 "Light_ComputeContributionKernel" -> NCameraModes::CPhysicsForceModelBase::PotentialFieldBufferPoints
  - 008fc050 "DoThingEvasion_BuildParticleTrail" -> CPhysicsForceModelBase::UpdateScan<list<CPhysicsMeshObject>> (PDB)
  - 008fc430 "ComputeEvasionParticleTrajectory" -> CPhysicsForceModelBase::UpdateScan<const CWorldMap> (PDB)
  - 0090aa40 "CGuiControlTreePane_Destructor" -> LHNetworkLib::LSocketServer::~LSocketServer
  - 0090b160 "LSocket_ProcessNetworkEvents" -> LHNetworkLib::LSocketServer::Think
  - 00910570 "CTCLightningOrb_Shutdown" -> CAIStateGroup_MinionListenToStory::StandUp (PDB)
  - 00913d60 "PathBuilder_AppendBezierCurveNode" -> std::deque<CBezierCurve>::_M_push_back_aux_v
  - 00927590 "Sort_Partition_Predicate" -> std::__unguarded_partition<CIntelligentPointer<const CThing>, ConstThingCompare>
  - 00927910 "Sort_MergeHeap" -> std::__partial_sort<CIntelligentPointer<const CThing>, ConstThingCompare>
  - 0096a3c0 "FindValidCombatPositions" -> CAIStateGroup_WhisperPotionCombat::GetValidJumpPosition (PDB)
  - 00979760 / 0097b4e0 "std_sort_QuickSort" -> std::__introsort_loop<CThing**, CThingCompare_InDirectionOfAndNearestToHeuristicBitch> (two instantiations)
  - 0098d360 "Vector_InsertRange_Overflow" -> std::vector<C4DVector>::_M_fill_insert
  - 0098e7d0 "CThreadedFileManager_ProcessPendingRequests" -> CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile*) (PDB; the only CThreadedFile* member)
  - 00990900 "RBTree_InsertOrFind" -> std::_Rb_tree<CPendingObjectSortKey,...>::insert_unique(const value_type&)
  - 00990dc0 "map_operator_square_bracket" -> std::_Rb_tree<CPendingObjectSortKey,...>::insert_unique(iterator, const value_type&)
  - 009919c0 "RegisterSoundBanks" -> CASoundManager::RegisterSoundBank (descriptive name)
  - 00991c10 "CMainGameComponent_RegisterSoundBank" -> CASoundManager::RegisterAtmosSoundBank (descriptive name)
  - 00993310 "Vector_InsertRange_Filter" -> std::vector<pair<long,CSoundBankRecord>>::_M_insert_overflow
  - 009934c0 "Heap_BuildFrom" -> std::__partial_sort<CSoundBankEntry>
  - 00993860 "Heap_Sift_Down_Recursive" -> std::__introsort_loop<CSoundBankEntry>
  - 00996610 "PersistData_CompressAndVerify" -> CDataOutputStream::WriteCompressed (PDB ?WriteCompressed@CDataOutputStream@@QAEXPBXJ@Z)
  - 00997bb0 / 00998250 / 00998a30 -> CAFile::GetCopyFilename / GetAbsoluteFilePath / IsFilePathSyntaxValid (PDB)
  - 00998010 -> CAFile::ParserReadPastComponent
  - 00998c90 -> CAFile::ExpandPath
  - 00998f60 "IsInvalidCustomTattoo" -> CAFile::CompareFiles
  - 00999ab0 -> CAFile::IsRoomForFile
  - 0099fb60 "GFIntToCharString_JoinList" -> GFIntVectorToCharString (PDB)
  - 009a23e0 -> anonymous GFUpdateFontTextureCache (PDB)
  - 009a3460 -> anonymous ReadTextFileIntoArray
  - 009a5020 "CTCInventoryBase_ExceptionHandler" -> CSystemManager::ExceptionHandler (0x0099A330 is CInitBaseClass::IsValid)
  - 009af1e0 "CDefinitionManager_LoadDefinitions" -> CDefinitionManager::CompileString (PDB)
- **Callees and neighbours.**
  - 0x009B9BA0 "SerialiseGameState" -> CStringParser::SkipPastString
  - 0x009B9C00 "CThingCreatureBase::SerialiseGameState" -> CStringParser::SkipPastWholeString
  - 0x009B9880 / 0x009B9870 -> CStringParser::GetString / GetSymbol (the landed 0x00998170 comment has them the other way round)
  - 0x0099E620 / 0x0099E7F0 -> GFIsCharStringNumber / GFCharStringToInt
  - 0x0099D470 "Utility_LoadStringByLanguageWithFallback" -> RetrieveString (@RetrieveString@16, extern "C" __fastcall)
  - 0x00997E90 -> CAFile::ParserReadPastSeparator
  - 0x0073A6E0 "CPixelShader::CountReferences" -> CTCCreatureModeManager::GetCurrentMode
  - 0x0088B8F0 "CQuestManager::RemoveAvailableQuestCardType" -> CAIBrain::SetOutput
  - 0x0062E900 "~CTCLightningOrb" -> CCombatActionBase::~CCombatActionBase
  - 0x008574D0 "CTCInGameMenu::SetPDef" -> CCombatActionBase::SetCombatAnimation
  - 0x0096BA30 `_Move<...>` -> std::remove_copy_if (already noted in landed code)
  - 0x009902D0 / 0x009905D0 / 0x0098FE70 / 0x0098FCA0 / 0x009901D0: the Objects map find, the pending map erase(first,
    last), the pending _M_find, _M_upper_bound and _M_lower_bound (landed under generated stand-in names).
    0x0042A34E is `_Rb_global<bool>::_Rebalance_for_erase`, and 0x00464ACE is `vector<unsigned long>::_M_insert_overflow`
    (catalog "Vector_InsertN_uint32").
  - 0x00990CD0 is the pending map's erase(const key&), with distance out of line at 0x0098FF50.
  - 0x008FC810 / 0x008FC7E0 -> CPhysicsForceModelBase::GetCollisionPoints<const CWorldMap> / <list<CPhysicsMeshObject>>
    (UpdateScan, then 0x008FA9F0).
  - The ~LSocket label travels across byte-identical bodies at 0x0099AD80 / 0x00401630 / 0x008F9780 (CWound::Kill).
    Check them with label_trust.py before attributing any of them to LSocket.
