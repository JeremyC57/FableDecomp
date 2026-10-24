# Hand chunk 369 (std::vector<unsigned char> .. std::_Rb_tree<CCharString, pair<const CCharString, long>>): 39 of 40 byte-exact

All 39 sources grade MATCH (3) or RELOCATION_MATCH (36) in `grades.txt`, and every test prints `<addr>_TEST PASS` under
g++ (`tall.sh`) and the lane's VC7.1 (`tall.py`). The row extents all agreed with the bodies (no cut problems).

## What matched (39)

- **STLport vector<unsigned char> / vector<CRGBColour> growth** (4): 004676bb erase, 00467831 `_M_insert_overflow`
  (__true_type), 0046916b `_M_insert_overflow` (__false_type), 0046bf72 vector<CReverbEnvironmentEntry>::resize(n).
- **GFSerialise vector/map templates** (16): the CSoundDef / CTextureReplacementDef / CObjectFamilyDef element vectors:
  00468f15, 00469c6c, 00469cb2, 0046aa91, 0046ab33, 0046aff4, 0046b03a, 0046b422, 0046b4ab, 0046b958, 0046b9b0, 0046be4d,
  0046be89, 0046c31c (In/Out pairs per element), plus the map writers 00469309 (map<long, CBlendedParticleEffectSet>) and
  00472841 (map<long, CCharString>, CGlobalDef::ConversationCameras).
- **STLport copy / construct helpers** (8): 0046eb9e and 0047197a `_M_allocate_and_copy` (C2DVector; pair<unsigned long,
  CAnimationEntry>), 0046efb8 and 004719ce `__copy_ptrs` (CQuickAccessItemButtonGuiDef; pair<unsigned long,
  CAnimationEntry>), 004708ef `uninitialized_fill_n` (pair<unsigned long, CSoundPair>), 00471c40
  list<COpinionTransientOffset>::_M_insert_dispatch, 00471f97 vector<EOpinionAttitudeType> copy constructor, 00472d79
  vector<NSpeechGainManager::CDialogueLayerDef>::_M_insert_overflow.
- **STLport sort** (4): CSoundMap's two CVectorMap sorts: 00470e9e / 00471338 `__introsort_loop` and 00471055 / 004714ae
  `__adjust_heap` (pair<unsigned long, CSoundPair> 12 bytes; pair<unsigned long, CDefString> 8 bytes).
- **map<CCharString, long> tree** (7): 0046ece7 `_M_create_node`, 0047536c erase(key), 0047547d `_M_lower_bound`,
  004754c6 `_M_upper_bound`, 00475861 `_M_find`, 004758e3 insert_unique, 004759ad `_M_insert` (CPlayerGuiDef::ScriptSprites /
  MiniMapGraphics through TransferMap<CCharString, long> 0x00475026).

## Parked for Fable 5.1 (1)

- 00472bdd std::vector<NSpeechGainManager::CDialogueLayerDef>::erase DIFFER(77v77): `lea eax,[ebp+0Fh]` (ours) v `lea eax,[ebp+0Bh]` (retail): the address of the never-written `__false_type` tag passed to `__destroy_aux` 0x00450A14 lands in the dead `last` slot instead of the `first` slot (retail re-reads `first` from that slot for the return) | tried: tag without a constructor (83v77: it gets a `stosb`), tag with an empty constructor (77v77), the copy loop as a direct `__copy` call, the return spelled `last - (last - first)` (92v77) | next: give `_Destroy`/`__destroy` a different value-type argument shape so the tag is built at a different point, or `erase` through `copy(last, end(), first)`; else lane gap: VC7.1 stack packing of empty tag temporaries.

## Skipped

None.

## Label corrections

- 0x004676BB "VectorInsert_MoveBack" -> std::vector<unsigned char>::erase; 0x00467831 "VectorInsert_ExpandWithFill" ->
  vector<unsigned char>::_M_insert_overflow; 0x0046916B "Vector_Reallocate" -> vector<CRGBColour>::_M_insert_overflow.
- 0x00469309 "Serialization_WriteParticleEffect" -> GFSerialiseMapBinaryOut<CBlendedParticleEffectSet, long>.
- Element types of the GFSerialise rows (the catalog's are propagated): 0x00469C6C / 0x00469CB2 -> CTextureReplacementDef::CEntry;
  0x0046AA91 / 0x0046AB33 -> CSoundBankEntry; 0x0046B03A -> CAtmosBankEntry; 0x0046B4AB -> CMusicEntry; 0x0046B958 /
  0x0046B9B0 -> CMusicSetEntry; 0x0046BE89 -> CReverbEnvironmentEntry; 0x0046C31C -> CObjectFamilyEntry.
- 0x0046BF72 "$E1" -> vector<CReverbEnvironmentEntry>::resize(unsigned int). The "CArray<pair>::push_back" bodies 0x00468FA5,
  0x00469E00, 0x0046B144, 0x0046BB59, 0x0046BF98, 0x0046C430 and 0x004706F6 are vector::resize(n, const T&); 0x0046B65A /
  0x0046ACA8 ("_Umove<EWeaponClass*>") are vector::resize(n).
- Landed but mislabelled (review): 0x00469D47 (as CPersistTraits<CWeaponTrailGraphicSet>) is
  CPersistTraits<CTextureReplacementDef::CEntry>::TransferBinaryIn; 0x0046B0D6 (as the pair<CDefIndex, CDefString> traits) is
  CPersistTraits<CAtmosBankEntry>::TransferBinaryIn; 0x0046AADE / 0x0046ABC4 (as C3DGroup2 / CIndexTriple) are
  CSoundBankEntry::CSoundBankEntry() copies.
- 0x00993EB0 is CDataOutputStream::WriteSLONG and 0x00993EA0 WriteULONG (manifest "WriteCBYTE").
- 0x0046EB9E "CActiveFile_CopyConstruct" -> vector<C2DVector>::_M_allocate_and_copy; its caller 0x0046EABD is
  vector<C2DVector>::operator=, called three times by 0x0046CE2D ("CParentDefClassBase::operator=") = CPlayerGuiDef::operator=.
- 0x0046ECE7 "Alloc" -> _Rb_tree<CCharString, pair<const CCharString, long>>::_M_create_node; 0x0046EC57 (landed
  "CRetailTree_0046ece7::Copy", described as Dinkumware `_Tree::_Copy`) is STLport `_Rb_tree::_M_copy`.
- 0x0046EFB8 "Vector_CopyElements" -> __copy_ptrs<CQuickAccessItemButtonGuiDef*>; 0x0046ED15 "CActiveFile_AssignQuickAccessItems"
  -> vector<CQuickAccessItemButtonGuiDef>::operator=. 0x00431ED0 is CParentDefClassBase::operator= (the landed
  CQuickAccessItemButtonGuiDef::Copy 0x0046EF87 calls it "CParentDefClassBase::Copy").
- 0x004708EF "_Fill_n<unsigned long*, ...>" -> uninitialized_fill_n<pair<unsigned long, CSoundPair>*>; 0x00470778
  "vector_insert_12byte" -> vector::_M_fill_insert; 0x00470CD8 / 0x0047119A ("CGuiVarTransferBool::TransferToGui") ->
  CVectorMap<unsigned long, CSoundPair / CDefString>::Sort; 0x00470CF1 / 0x004711B3 -> sort(); 0x00470E9E / 0x00471338
  "Quicksort_Recursive" -> __introsort_loop; 0x00471055 / 0x004714AE "Heap_AdjustDown" -> __adjust_heap.
- 0x0047197A "AnimationVector_AllocCopy" / 0x004719CE "VectorInsert_CopyAnimations" -> vector<pair<unsigned long,
  CAnimationEntry>>::_M_allocate_and_copy / __copy_ptrs (CAnimationSet::Anims); 0x00471865 "CAnimationSet::AssignAnimations" is
  that vector's operator=.
- 0x00471C40 "LinkedList_InsertScriptData" -> list<COpinionTransientOffset>::_M_insert_dispatch; 0x00471C79 (landed "Alloc") is
  that list's _M_create_node; 0x00471C9A (landed as CNearestBuildingWithProperty's copy constructor) is the 24-byte element
  copy it constructs (COpinionTransientOffset); 0x00471A77 ("CParentDefClassBase::operator=") is COpinionDeedEffectsDef::operator=.
- 0x00471F97 "vector_insert_range_copy" -> vector<EOpinionAttitudeType>::vector(const vector&) (COpinionDeedReactionDef::Attitudes;
  also reached from 0x0067C70D / 0x0067CA37 / 0x0067CAB7 -- a folded identical instantiation); 0x00471FD9 is _Vector_base(n, a).
- 0x00472841 "ParticleMorphs_WriteToStream" -> GFSerialiseMapBinaryOut<CCharString, long> (map<long, CCharString>); 0x0047280C
  (landed "CEngineLightingManager::UpdateShadowScene") is the binary map dispatcher (mode 2 reader 0x00472896, mode 3 writer);
  0x0099EAE0 (manifest "_Cons_val<...>") is ~CCharString.
- 0x00472BDD "Vector_ShiftDialogueLayersRight" / 0x00472D79 "DialogueLayer_AllocCopyInsert" -> vector<NSpeechGainManager::
  CDialogueLayerDef>::erase / _M_insert_overflow; 0x00450A14 (landed "vector<CIntelligentPointer<CThing const>>::_Destroy") is
  __destroy_aux for that 64-byte polymorphic element; 0x0046638B (landed "CActiveFile::OnReadFinished") is its vector::_M_clear.
- 0x0047536C "RBTree_TraverseAndCompare" -> _Rb_tree::erase(const key&); 0x00475453 equal_range, 0x004753B1 distance,
  0x004753D4 erase(first, last). 0x0047547D / 0x004754C6 "RBTree_FindNode" -> _M_lower_bound / _M_upper_bound; 0x00475861
  "RBTree_FindNodeOrUpper" -> _M_find; 0x004758E3 "RBTree_InsertOrUpdate" -> insert_unique; 0x004759AD
  "RBTree_InsertOrUpdateScript" -> _M_insert.
