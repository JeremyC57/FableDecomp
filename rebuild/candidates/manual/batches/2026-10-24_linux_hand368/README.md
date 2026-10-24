# Hand chunk 368 (_global .. _global): 35 of 40 byte-exact

All 40 rows were catalogued as `_global` with behavioural or propagated labels. Every row was identified from its
callers, vtables, the PDB layouts (`struct_layouts_egor.tsv`) and the Ego_r name set (used as names only, never by
address). Line 1 of each source carries the real name. gall.sh: 35/35 MATCH or RELOCATION_MATCH. tall.sh and tall.py:
35/35 PASS. Every `// Leaf:` was checked against `objdump -t` of the graded object and names exactly one defined symbol.

## What matched (35)

- **NPlayerGui CDraw\* destructors (0x00441870 ~CDrawStamina, 0x00441C20 ~CDrawPerceivers, 0x00443D70 ~CDrawQuestInfo,
  0x00444ED0 ~CDrawGuildSeal).** These are the implicit destructors that CPlayerGui's counted-pointer deleters call
  (0x00443520 / 0x00443820 / 0x00445370 / 0x004454E0).
  - Shape: no user destructor plus a `__fastcall` `delete p` helper. This gives an out-of-line `??1` with no vtable store.
  - Members are STLport `_Vector_base`/`vector`, with a `_Destroy` loop over elements that have virtual dtors.
  - A `list<long>` destructor that is called out of line next to an inlined one is declared as an explicit
    specialisation (`template <> _List_base<long>::~_List_base();`, 0x0042AB73).
  - `deque::~deque()` is declared out of line (0x004432B0).
- **0x00443350 DeleteRecordWithThreeVectors.** A `__fastcall` `delete p` of CRecordWithThreeVectors. Speed TU.
- **0x00440C00 std::__destroy_aux(deque<pair<CCharString,long>> iterators, const __false_type&).** Out-of-line
  `__fastcall`: the tag rides in ecx, `ret 20h`. It matched on the 4th attempt.
- **CGamePlayerInterface (0x00446B40 IsButtonForGameEvent, 0x00446B90 IsMouseButtonForGameEvent).**
  - Inline CVectorMap Find: `LowerBound`, then `(it == end || key < it->first) ? end : it`, compared against the
    re-read `end()`.
  - Then input-type virtual slot 6 / 7 (IsInputTypeWithButton / IsInputTypeWithMouseButton).
  - Uses `engine/CGamePlayerInterface.h` (Controls at +0x7a8 agrees with retail).
- **0x0044C26D COpinionPersonalityTraits::COpinionPersonalityTraits().** Size TU; the init list is 0.0f then eight 1.0f.
- **STLport vector<bool> (retail: a plain byte vector).**
  - 0x00450CCC `_Vector_base(n, a)`: size TU, needs `__forceinline` on allocate and the allocator/proxy ctors.
  - 0x00451583 `vector(n, const bool&, a)`: y-off + s, `fill_n`.
- **0x0045370A CInventoryItemDef::operator=.** Reached through the slot-19 thunk 0x00453705 of vtable 0x01231DBC.
  - The body is the base `CDefClassBase::operator=` 0x00431F10, then a memberwise copy.
  - The 14-byte CEngineGraphic is modelled as 14 bytes, so it gives `movsd x3 / movsw` and every member lands on the
    PDB offset with no `#pragma pack`.
- **STLport vector members in the def TUs.**
  - 0x0045BAAA `vector<CRGBColour>::operator=` (y-off + s).
  - 0x0045BB8F `_Construct<CRGBColour>` (`__fastcall`, y-off + s, copy through the inline `Set(r,g,b,a)`).
  - `_M_allocate_and_copy` (plain "s", allocate + `__uninitialized_copy` with `_Construct`): 0x0045BBC5 CRGBColour
    (out-of-line `_Construct`), 0x0045CD9B CAppearanceModifierGraphics::CEntry, 0x0045D125 pair<unsigned long,CSoundPair>,
    0x00460BEC CSoundBankEntry, 0x00461136 CReverbEnvironmentEntry, 0x0046656F NSpeechGainManager::CDialogueLayerDef.
  - vector<unsigned long>: 0x0046496F `erase(first,last)` and 0x00464ACE `_M_insert_overflow(..., const __true_type&, ...)`
    (both y-off + s).
  - 0x00465E44 `vector<EWeaponClass>::_M_range_initialize(set iterators)`.
- **CPersistContext binary transfers.** These are one-argument `Transfer<T>(T&)`: CheckCRC(""), then
  `CPersistTraits<T> traits;` and `switch (Mode) { case 2: In; case 3: Out; }`.
  - 0x0045C8D3 `Transfer<CSpecialEffectsStringMap>`: the traits local holds the default map. It is built by the
    out-of-line ctor 0x004533C5 and destroyed by 0x004518E1.
  - 0x0045E808 `Transfer<COpinionPersonalityTraits*>`: a 5-entry loop. It needs an unsigned index (`jb`) and
    `__forceinline` traits transfers (size TU).
- **0x004636CF _Rb_tree<CCharString, pair<const CCharString,long>, ...>::insert_equal.** STLport verbatim with an
  inline `CCharString::operator<`.
  - The key's data pointer is hoisted into ebx for the whole walk only when the callee `Compare` 0x00411570 has its
    BODY in the TU (VC then knows it only reads memory).
  - The equality test is written other-first.
- **list<CStringParserCommentBoundaries> (CStringParser::Comments).**
  - 0x00463871 copy constructor: the allocator needs STLport's user ctors, else `stosb`. Iterator has a user copy ctor
    and const_iterator a trivial one.
  - 0x004638C8 `_M_insert_dispatch`: `insert(pos, x)` needs `__forceinline` in the size TU.
- **GFSerialiseVectorBinaryOut/In (the landed 0x005C12A1 / 0x005C2AD8 templates).**
  - Out: 0x00464CED <float>, 0x004675C5 <bool>, 0x00466B93 <CBrainUpdateZone> (forceinline traits transfer),
    0x00467090 <CBrainBehaviour> (out-of-line traits ctor 0x004670F5 first).
  - In: 0x00465852 <CDefString>, 0x00466BEB <CBrainUpdateZone>, 0x0046712B <CBrainBehaviour>. Each uses the inline
    Read (`position += size`), STLport's one-argument `resize(n)` = `resize(n, T())` (force-inlined in the size TU),
    and `traits.TransferBinaryIn(&stream, &out[i])`.

## Parked for Fable 5.1 (5)

- 00458833 CPersistContext::Transfer<CPhysicalPrimitiveInit> DIFFER(79v76): retail hoists `lea ecx,[ebp-8]` (the traits) between `dec eax` and `je` so both arms and the default path share it and the save arm jumps back to a re-`lea`; ours forms the `lea` inside each arm after the value push and once more before the counted-pointer dtor | tried: switch 2/3, switch 3/2, if/else-if, traits pointer local, reference parameters, mode copied to a local, streams hoisted to locals, speed TU | next: lane gap: scheduling (common-subexpression hoist of `this` above a two-way branch)
- 00459af3 CPlayerInventoryDef::CPlayerInventoryDef DIFFER(96v96): the 4th vector ctor call's empty allocator temporary sits at [ebp-2] where retail overlays it on the live `false` temporary at [ebp-1] | tried: plain `IsUsable(6, false)`, the default-argument form (`IsUsable(6)` with `value = T()`) | next: lane gap: empty-temporary frame placement
- 00462e78 std::vector<bool>::operator= DIFFER(169v167): the tail-copy first argument `x._M_start + size()` is `mov ecx,[x] ; sub ecx,[esi] ; push edx ; add ecx,edx` where retail keeps finish in ecx: `push ecx ; sub ecx,[esi] ; add ecx,[x]` | tried: plain STLport, `__forceinline __copy_trivial`, `_Is_POD<T>::_Answer()` tag by value (gives the stosb + byte copy), `x.begin() + size()` | next: write the tail argument as `size() + x._M_start` or through a named `const_iterator mid` local
- 00464d23 GFSerialiseVectorBinaryIn<float> DIFFER(146v146): frame only. Retail puts the never-initialised CPersistTraits<float> local in [ebp-0Ch], the slot of the homed `out` argument (`sub esp,0Ch`, `lea ecx,[ebp-0Ch]`); ours gives it its own word (`sub esp,10h`, `lea ecx,[ebp-10h]`) | tried: DefaultVal float, an empty specialisation, traits declared after the resize, a per-iteration temporary, s without y-off | next: lane gap: stack-slot sharing between an uninitialised address-taken local and a spilled register parameter
- 004675f6 GFSerialiseVectorBinaryIn<bool> DIFFER(144v144): frame only, the float twin's gap: traits in [ebp-0Ch] shared with the homed `out` in retail; ours `sub esp,10h` / `lea ecx,[ebp-10h]` | tried: the float twin's template (one grade) | next: lane gap: stack-slot sharing (same as 0x00464D23)

## Skipped

None. No row holds padding followed by another function, and every oracle extent agrees with the retail function
bounds (the next function starts right after each row's trimmed bytes).

## Label corrections

Rows (catalog label -> real identity):

| Address | Catalog label | Real identity |
|---|---|---|
| 0x00440C00 | Vector_CopyPairRange | std::__destroy_aux over deque<pair<CCharString,long>> iterators |
| 0x00441870 | List_DestroyElements | NPlayerGui::CDrawStamina::~CDrawStamina |
| 0x00441C20 | TreePane_DestructorHelper | NPlayerGui::CDrawPerceivers::~CDrawPerceivers |
| 0x00443350 | GFCheckTreeNodes | `delete` of CRecordWithThreeVectors |
| 0x00443D70 | EventConversation_Destructor | NPlayerGui::CDrawQuestInfo::~CDrawQuestInfo |
| 0x00444ED0 | AsyncData_Destructor | NPlayerGui::CDrawGuildSeal::~CDrawGuildSeal |
| 0x00446B40 / 0x00446B90 | VectorMap_LookupValue | CGamePlayerInterface::IsButtonForGameEvent / IsMouseButtonForGameEvent |
| 0x0044C26D | InitializeFloatArray_Ones | COpinionPersonalityTraits::COpinionPersonalityTraits() |
| 0x00450CCC | Buffer_InitializeWithSize | std::_Vector_base<bool>::_Vector_base(n, a) |
| 0x00451583 | VertexListInsertVertices | std::vector<bool>::vector(n, const bool&, a) |
| 0x0045370A | CThingBuildingDef_CopyAssign_2 | CInventoryItemDef::operator= |
| 0x00458833 | CPhysicalPrimitiveInit_Transfer | CPersistContext::Transfer<CPhysicalPrimitiveInit>(T&) |
| 0x00459AF3 | CParentDefClassBase_ConstructorWithVectors | CPlayerInventoryDef::CPlayerInventoryDef() |
| 0x0045BAAA | CActiveFile_AssignFrom | std::vector<CRGBColour>::operator= |
| 0x0045BBC5 | Vector_AllocateAndCopyRGB | vector<CRGBColour>::_M_allocate_and_copy |
| 0x0045C8D3 | CSpecialEffectsDef_TransferMap | CPersistContext::Transfer<CSpecialEffectsStringMap>(T&) |
| 0x0045CD9B / 0x0045D125 / 0x00460BEC | CActiveFile_AssignVector24/12/20 | `_M_allocate_and_copy` of CAppearanceModifierGraphics::CEntry / pair<unsigned long,CSoundPair> / CSoundBankEntry |
| 0x00461136 | Vector_AllocateAndCopy_84 | vector<CReverbEnvironmentEntry>::_M_allocate_and_copy |
| 0x0046656F | CDialogueLayerDef_AllocateAndConstruct | vector<NSpeechGainManager::CDialogueLayerDef>::_M_allocate_and_copy |
| 0x0045E808 | COpinionPersonalityDef_TransferArray | CPersistContext::Transfer<COpinionPersonalityTraits*>(T&) |
| 0x00462E78 | CSimpleMemoryPool_AssignFrom | std::vector<bool>::operator= |
| 0x004636CF | BinarySearchTree_FindOrInsert | _Rb_tree<CCharString, pair<const CCharString,long>, ...>::insert_equal |
| 0x00463871 | CEngineSceneGrid_InitFromCell | list<CStringParserCommentBoundaries>::list(const list&) |
| 0x004638C8 | CEngineSceneGrid_InsertBoundingBoxNode | list<CStringParserCommentBoundaries>::_M_insert_dispatch |
| 0x0046496F | std_vector_push_copy_element | vector<unsigned long>::erase(first, last) |
| 0x00464ACE | Vector_InsertN_uint32 | vector<unsigned long>::_M_insert_overflow |
| 0x00464D23 / 0x004675F6 | GFSerialiseVectorBinaryIn<CRGBColour> | GFSerialiseVectorBinaryIn<float> / <bool> |
| 0x004675C5 | GFSerialiseVectorBinaryOut<C2DVector> | GFSerialiseVectorBinaryOut<bool> |
| 0x00465E44 | CMemoryAllocatorVariableSize_InitFromRange | vector<EWeaponClass>::_M_range_initialize(set iterators) |
| 0x00466B93 | GFSerialiseVectorBinaryOut<CActionInputControl> | GFSerialiseVectorBinaryOut<CBrainUpdateZone> |
| 0x00467090 | GFSerialiseVectorBinaryOut<CActionInputControl> | GFSerialiseVectorBinaryOut<CBrainBehaviour> |

Callees met on the way (manifest label -> what the call sites show):

| Address | Manifest label | What the call sites show |
|---|---|---|
| 0x004292C0 | GetNoAllocatedAreas | `_Rb_global<bool>::_M_increment` (`__cdecl` static) |
| 0x0042AB73 | CActiveFile::OnReadFinished | `_List_base<long>::~_List_base` |
| 0x004432B0 | CBankFileAsyncData::FreeBuffer | `deque<pair<CCharString,long>>::~deque` |
| 0x004316CF | BinaryTree_InsertDigScriptNode | `_Rb_tree<CCharString, pair<const CCharString,long>>::_M_insert(x, y, v, w = 0)` |
| 0x004533C5 | CDisplayManager::CopyBackBufferToTexture | CSpecialEffectsStringMap's default ctor (map<unsigned long,long> through _Rb_tree ctor 0x004518B7) |
| 0x0045C91D / 0x0045C95A | BinarySearchTree_WriteRangeToStream / GFSerialiseMapBinaryIn<long,long> | GFSerialiseMapBinaryOut / In<unsigned long, long> |
| 0x0044F3B2 | `_Dest_val<...COptimisedPrimitive>` | ~CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> (byte-identical) |
| 0x00404690 | std_list_node_Init | `_List_base<T>::_List_base(const allocator_type&)` (thiscall, one stack argument) |
| 0x004638A1 | CEngineSceneGrid::GetCellBoundingBox | `list<CStringParserCommentBoundaries>::insert<const_iterator>(pos, first, last)` |
| 0x00463901 | CEngineSceneGrid_AllocBoundingBoxNode | `list<CStringParserCommentBoundaries>::_M_create_node` |
| 0x004658DF | CArray<pair<long,long>>::push_back | `vector<CDefString>::resize(n, const CDefString&)` |
| 0x00466D9C / 0x0046728D | CArray<pair<unsigned long,CSoundPair>>::push_back | `vector<CBrainUpdateZone>` / `vector<CBrainBehaviour>::resize(n, const T&)` |
| 0x00467686 | vector<unsigned char>::resize | vector<bool>::resize (folded body) |
| 0x00442770 | vector<unsigned int>::resize | also vector<float>::resize (the 0x00464D23 caller passes `const float&`) |
| 0x00993E30 / 0x00993EE0 | WriteEBOOL / WriteCBYTE | CDataOutputStream::WriteBool / WriteFloat |
| 0x00453659 | (propagated) | vector<long>'s iterator-range template ctor taking (int, int), which dispatches to the fill path |
