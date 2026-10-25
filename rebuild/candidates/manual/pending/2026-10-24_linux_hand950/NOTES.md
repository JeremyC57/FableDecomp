# Hand chunk 950 (NScript::CQ_SunnyvaleMasterScript .. NScript::CQ_BreakSiegeScript::CDisplacedTrader): 613 of 613 byte-exact (family pass)

All 14 families done: 613 rows across 14 families (15 MATCH, 598 RELOCATION_MATCH), every behaviour test PASS under g++ (tall.sh)
and under VC7.1 (tall.py). The title names the real classes of the first (F001) and last (F014) representatives. The chunk's class list
(CGlobal_WatchForHeroDeathScript .. CTriangleList) is made of propagated manifest labels, and most of them are wrong (see Label corrections).

Grading caveat: gall.sh calls `$S/tools/q.py`, which has no chunk-950 oracle. It prints `no oracle for <addr>`, and its grep hides that line,
so it prints nothing. I graded every row through `gen/grade1.sh` instead. That script is the same q.py run with this agent's chunk950 oracle,
and it honours the `// Leaf:` and `// Flags:` lines; the object lands in `$S/wine/q/<a>/<a>.obj`.
- Final confirmation pass, run on the files exactly as they sit in src/ and test/ (`gen/final_run.sh`; no source change):
  - `gen/final_grades.txt`: 613 MATCH/RELOCATION_MATCH.
  - `gen/final_gpp.txt`: 613 PASS.
  - `gen/final_vc.txt`: 613 PASS.
- The per-family records are in `gen/fNNN_grades.txt`, `gen/fNNN_gpp.txt` and `gen/fNNN_vc.txt`. F004 and F006 share `gen/f046_*`.

Call-target check: `A=$S/w22/agent950 python3 $S/w22/calls.py` produced `gen/calls_final.txt`, and `gen/symcheck.py` ran over it.
- It found 421 distinct symbols and 1 problem.
- Every import goes to the import retail calls: scalar new 0xBFEA1A, malloc 0xBFEA0E, free 0xBFEA14 and memmove 0xBFEAE6.
- No callee name is bound to two targets inside the chunk, and existing catalog bindings are reused.
- The one problem is a catalog error in an already-landed row. The catalog binds `??_7?$CSpawnedFunc@VCV_ExposeMayorScript@NScript@@@@6B@` to
  0x012ECBA8, taken from landed 0x00E95F10. 0x012ECBA8 is really CV_HauntedHouseScript's CSpawnedFunc vtable: it is stored by 0x00E93DA0,
  the RegisterMain in the vtable 0x012ECB94 of the Alloc that RegisterScripts registers as "V_HauntedHouse". Our 0x00E756D0 (V_ExposeMayor,
  script vtable 0x012E96CC) stores 0x012E96E0. Fix the landed row; see Review at landing in LESSONS.md.

Generators (per family, in `gen/`):
- `gen_fNNN.py` reads the retail bytes of every instance from `$S/w22/chunk950.tsv` and works out the instance's identity from its own
  operands. Shared helpers: `gen/pe.py` (PE reader), `gen/xr.py` (callers verified by decoding the call/jmp, absolute references, function
  starts), `gen/scripts.py` (the `CScriptInfoManager::RegisterScripts` table and the script vtables).
- It stamps `src/<a>.cpp` + `test/<a>.cpp` from `fNNN_src.tmpl`/`fNNN_test.tmpl` (or from inline templates) and writes `fNNN_ids.tsv`, one
  identity row per instance with `proven` or `descriptive`.
- Generators that template a class name compile a toy object (`gen/toy.py`) and read back the exact mangled leaf symbol.
- Probes and stamped copies sit in `probe/fNNN/`; every src/ file is byte-identical to its probe copy.

## What matched (613)

### F001 -- 163 x `NScript::C<Name>Script::RegisterMain()` (161) / `::Main()` (2), 144 bytes, RELOCATION_MATCH
- **Function:** spawns the script's thread: `AddSpawnedFunction(new CSpawnedFunc<T>("Main", 0, this, &T::Main), "")`.
- **Representative:** 0x00CDD380 `CQ_SunnyvaleMasterScript::RegisterMain`, 4 grades.
- **Shape:**
  - The new-expression's CCharString temporary is conditional, so VC7.1 keeps a flag in ebx and destroys the temporary only if bit 0 is set.
  - The empty region name has to be built by an inline `SpawnFunction` helper after the new-expression. Written as an argument of
    AddSpawnedFunction, it is built first.
  - The member pointer is the ICF-folded vcall thunk `??_9@$B7AE` 0x00CDD440 (Main is virtual slot 2).
- **Recipe (`gen_f001.py` + `f001_scan.py`):**
  - The instance's .rdata slot (1 = RegisterMain, 2 = Main) gives the script vtable.
  - The Alloc that stores that vtable, directly or through its out-of-line ctor, is registered by `CScriptInfoManager::RegisterScripts`
    0x00CD52D0 under the script name. The class is `C<name>Script`, confirmed against the PDB name sets for all 163.
  - The thread name comes from the pushed literal.
  - 0x00ED0F50 (V_SingingStones) needed a manual override: its Alloc is reached through a jump table, registered at 0x00CD955C.
- **The two `Main` rows:** 0x00EE8F50 `CGlobal_WatchForHeroDeathScript::Main` spawns "WatchForHeroDeath", and 0x00EFF8A0
  `CShowTargetedThingHealthScript::Main` spawns "ShowTargetedThingsHealth". In both, the member pointer is a plain member, not the vcall thunk.
  The manifest label `CGlobal_WatchForHeroDeathScript::Main` is right for 0x00EE8F50 only.
- **Counts:** 163 matched, 0 parked, all names proven.

### F002 -- 85 x `CEntityScriptBinding<NScript::C<Name>Script>::CreateActiveEntityScript(const CScriptThing&, unsigned __int64)`, 272 bytes, RELOCATION_MATCH
- **Function:** with a class allocator, allocates the entity script (parent, the parent's master data, thing), calls Init() and wraps it in a
  new `CActiveEntityScript<T>`; without one, it wraps the parent's member function.
- **Representative:** 0x00CF66E0 (CQ_ArenaHoldingScriptScript), 4 grades.
- **Shape:**
  - `CCountedPointer<CEntityScriptBase>::~CCountedPointer` needs a same-TU `noinline` body. With only a declaration, VC7.1 swaps two frame
    slots and loses the ebp/ebx copies.
  - Both CActiveEntityScript ctors must be `__forceinline`.
- **Recipe (`gen_f002.py` + `f002_scan.py`):**
  - The binding vtable (slot 1 = this row) is stored by the script's Main (`new CEntityScriptBinding<T>(...)`).
  - That Main sits in the vtable of the script that RegisterScripts registers, which gives the class.
  - The CActiveEntityScript<T> vtable is read from the body.
- **Counts:** 85 matched, all in the PDB name sets.

### F003 -- 76 x `NScript::C<Name>Script::C<Name>Script(CScriptDataBase*, CGameScriptInterfaceBase&)`, 48 bytes, RELOCATION_MATCH
- **Function:** the script constructor: the CScriptBase base, then `Game = &game` (+0x40) and `MasterScriptData = data` (+0x44), then the
  script vtable.
- **Representative:** 0x00CDD330, 2 grades.
- **Shape:** VC7.1 drops the intermediate CLevelScriptBase vtable store as dead. The inline base ctor loads the second argument first and
  stores +0x40 before +0x44.
- **Recipe (`gen_f003.py` + `f003_scan.py`):** the stored vtable leads to the Alloc, then to the RegisterScripts name, then to the class.
- **Counts:** 76 matched, all in the PDB name sets. The out-of-line ctors have no retail callers (each Alloc inlines its own copy).

### F004 (57) / F006 (33) -- chunked-file sub-chunk getters on 11 C3DMeshFile*/C3DAnimFile* chunk classes, 144 / 224 bytes, RELOCATION_MATCH
- **F004:** `FindFirstX()`, `GetX()` and `PeekX()`, each in a non-const (QAE) and a const (QBE) twin. They return
  `CCountedPointer<C...Chunk>` from `FindFirstSubChunkOfType("<4CC>")`.
- **F006:** `FindNextX(const CCountedPointer<const C...Chunk>&)` from `FindNextSubChunkOfType(previous)`.
- **Representatives:** 0x00A98FA0 `C3DAnimFileRootChunk::FindFirstObject`, 3 grades; 0x00A99030 `C3DAnimFileRootChunk::FindNextObject`, 2 grades.
- **Shape:**
  - The body is `chunk; chunk.SetStaticCast(FindXSubChunkOfType(...)); return chunk;`, the landed `C3DMeshFile::GetRoot` 0x00AD1E80 shape.
  - The counted pointer's destructor zeroes both words. VC7.1 drops those stores as dead, except on F006's lookup temporary, where the
    converted argument's release follows.
- **Recipe (`gen_f046.py`):**
  - The pushed 4CC literal gives the sub-chunk type, and the sub-chunk class's constructor passes that same 4CC.
  - The TU address range gives the owning chunk class.
  - Inside each TU, the address order follows the Ego_r declaration order (FindFirstX, then FindNextX, per constness).
  - QAE vs QBE comes from the FindFirst/FindNextSubChunkOfType overload that is called: 0x00AA7F40 / 0x00AA7FF0 are const and
    0x00AA80E0 / 0x00AA8190 are non-const.
  - Where Ego_r splits a getter into Get and Peek, the non-const twin is `GetX` and the const one is `PeekX` (e.g. GetTriangles/PeekTriangles,
    GetBoneMask/PeekBoneMask, PeekVertices).
- **Counts:** 90 matched, all in the PDB name sets.
- **Classes:** C3DMeshFilePrimitiveChunk 19, C3DMeshFileSubMeshChunk 14, C3DMeshFileHelpersChunk 12, C3DMeshFileRootChunk 8,
  C3DMeshFileMaterialListChunk 7, C3DMeshFileMaterialChunk 6, C3DAnimFileRootChunk 6, C3DAnimFileObjectChunk 6, C3DAnimFileHelpersChunk 6,
  C3DMeshFileClothChunk 4, C3DAnimFileMovementVectorChunk 2.

### F005 (40) / F007 (32) -- STLport `vector<T>::_M_insert_overflow(pos, x, const __true_type&, fill_len, atend)` (POD), RELOCATION_MATCH
- **Function:** grows a vector of 4-byte PODs. It allocates `size + max(size, fill_len)`, memmoves the head, writes fill_len copies of x,
  memmoves the tail unless atend, frees the old block and adopts the new one.
- **Representatives:** 0x00436AC0 `vector<CThing*>` and 0x004408A0 `vector<float>`, 2 grades each.
- **Shape:**
  - `(max)` goes through its references.
  - new_start is homed in the dead fill_len slot.
  - The fill re-reads x through its reference.
  - A float fill copies through esi like any dword.
- **The F005/F007 split is padding only:** both families are the same 216-byte body, and only the int3 padding differs.
- **Recipe (`gen_f005.py`/`gen_f007.py`):** the element type of each COMDAT is proven from landed callers that name the container
  (push_back/insert/resize sites in landed sources), and the leaf is read back from a toy compile.
- **Descriptive element types** (no landed caller names the vector; the element is a 4-byte POD, modelled `CUnprovenElement_<addr>*`):
  - F005, 17 rows: 00514f40 0056d850 005e1500 006422b0 006f25a0 00765130 007a91a0 00841820 0084b6e0 0091ffe0 0098cde0 009ca950 00a17190
    00a44b40 00a80aa0 00b43e10 00be8f10.
  - F007, 14 rows: 0044be80 0048d0d0 004ac610 009b4bd0 009d2c10 009e3330 00a03780 00a17270 00a5e1b0 00a81590 00b291a0 00b389d0 00b96c60
    00bedbf0.
- **Counts:** 72 matched, 41 proven and 31 descriptive.

### F008 -- 25 x STLport `vector<T>::_M_insert_overflow(..., const __false_type&, ...)` for an 8-byte element, every callee inlined, RELOCATION_MATCH
- **Function:** for an element STLport does not know is POD, copy-constructs the head, x (once, or fill_len times) and the tail.
- **Representative:** 0x004D1250 `vector<pair<float, EConversationAttitude> >` (the CSubtitledMessage attitude CVectorMap), 2 grades.
- **Shape:**
  - len is homed in the dead fill_len slot and new_start sits in ebp.
  - When fill_len == 1 there is a single construct; otherwise the `__uninitialized_fill_n` loop runs.
  - The tail copy is strength-reduced to one offset register.
- **Element types:** 13 proven, including pair<unsigned long,float>, C2DCoordI, pair<EReactionSpeechType,long>, CComplexNumber,
  CVertexStreamFormat::CDataElement, C3DVertexBlend2 and CGPUScratchBufferTextureManager::CScratchPool.
- **Descriptive element types** (8-byte, `CUnprovenElement_<addr>`), 12 rows: 0066e260 00755350 0078b300 0081c160 0088cc20 008d61a0
  00ad3b20 00ad4e20 00ae6a70 00ae6d50 00b6c4c0 00b85ec0.
- **Counts:** 25 matched.

### F009 -- 20 x `CEngine::AddEngineComponent<T>(T* component, T*& globalPointer, unsigned char flag)`, RELOCATION_MATCH
- **Function:** reports load progress ("Engine: Add Engine Component"), publishes the component through its global pointer, tells the
  component where that pointer lives, push_backs it onto `EngineComponents` and calls `SetupEnableConsoleVariable`.
- **Representative:** 0x00B29930 (T = CEnginePrimitiveTypeInfo), 3 grades.
- **Shape:**
  - The progress text must be a named CCharString in its own block.
  - push_back is STLport's inline form, with the T*-to-base temporary homed in the dead `component` slot and the `__true_type` tag in the
    dead `flag` slot.
- **Recipe (`gen_f009.py`):** each row has exactly one call site in the CEngine ctor.
  - The size passed to new and the ctor called just before it give the component's vtable, whose RTTI names T.
  - The global pointer is the pushed address.
- **Counts:** 20 matched, all names in the Ego_r name set.
- **Components:** CEnginePrimitiveTypeInfo, CEngineResourceManager, CEngineOcclusionManager, CEnginePrimitiveOcclusionManager,
  CEngineEnvironment, CEngineCamera, CEngineStateBlocks, CEngineScreenEffectOutlineGlow, CEngineScreenEffectDisplacementRenderer,
  CEnginePrimitiveRenderer2D, CEngineShadowRenderer, CEngineWorldMap, CEngineWaterRenderer, CEngineLandscapeRenderer, CEngineSkyRenderer,
  CEngineWeatherRenderer, CEngineLocalDetailGenerator, CEngineScreenEffectColourFilter, CEngineScreenEffectGlowRenderer,
  CEngineScreenEffectRadialBlurRenderer.

### F010 -- 18 x `NUISystem::<Class>::AddAction*(CCountedPointer<NUISystem::CAction>&)` virtuals, RELOCATION_MATCH
- **Function:** appends a counted action to a lazily allocated STLport `list<CCountedPointer<CAction> >` member, allocating the list on
  first use. The rows differ only in the member offset.
- **Representative:** 0x00532D00 `CComponent::AddActionOnDesctruction` (Ego_r spelling), 3 grades.
- **Shape:** the dead `push ecx ; mov [esp+8],0` is the lifetime slot of STLport's default-allocator temporary. It appears only when
  `allocator` has its user `~allocator() {}`.
- **Recipe (`gen_f010.py`):**
  - The body's identity comes from the vtables that hold it (RTTI through the COL), the slot index and the member offset in the body.
  - The slot-to-method map is proven from the CComponent stub order and the override sets. The 15 empty base stubs, ordered by definition
    address, must be slots 66..71, 80, 72..79; this was checked, not assumed.
  - The owner is the one class, among the vtables that reference the body, whose Ego_r name set gives this method.
- **Members:** CMenuEntry AddAction/AddActionOnSelected/AddActionOnUnselected/AddActionOnBack/AddActionOnVisible/AddPreAction,
  CDraggableInto::AddAction, CScrollBarOutside::AddActionOnLeftClickedAbove/Under, CScrollBar::AddActionOnDraggedUp/Down,
  CDraggable::AddActionOnDropped/AddActionOnDroppedNowhere, and CClickable::AddActionOnLeftClicked/LeftUnclicked/LeftHeld/RightClicked.
- **Descriptive:** the list member names. No PDB layout exists for the NUISystem components.
- **Counts:** 18 matched.

### F011 -- 17 x STLport `vector<T>::_M_insert_overflow(..., const __false_type&, ...)` for 8-byte counted-pointer-like elements, RELOCATION_MATCH
- **Function:** the same growth insert as F008, but the range helpers are out of line: `__uninitialized_copy`, `__uninitialized_fill_n` and
  `__destroy_aux` are each element's own `__fastcall` COMDATs.
- **Representative:** 0x00449050 `vector<CCountedPointer<const CInputTypeBase> >` (CGamePlayerInterface::SetControlScheme button vectors),
  3 grades.
- **Shape:** every `__false_type` temporary lands on the atend argument slot, even while atend is still live. That needs same-TU noinline
  helper bodies; with bare declarations the first tag gets its own frame byte.
- **Element types:** 14 proven, including CCountedPointer over NUISystem::CComponent/CAction, CActionEventBase, CCombatSequenceBase,
  CSubdivisionTriangle, CLightningBranch, CShaderResource, C3DSkeletalMorphParameter, CJoystick, CBoneInfo, C3DAnimFileSequenceChunk and
  CEnginePrimitiveManager, plus the CCombatAction element.
- **Descriptive pointees** (`CCountedPointer<CUnprovenPointee_<addr>>`), 3 rows: 00ad68d0 00b83c30 00b97cb0.
- **Counts:** 17 matched.

### F012 -- 17 x STLport `_Rb_tree<CCharString, ...>::_M_find(const CCharString&) const`, 112 bytes, RELOCATION_MATCH
- **Function:** the exact-key lookup behind set/map/multimap `find` over CCharString keys.
- **Representative:** 0x004B9370 `set<CCharString>`, 2 grades.
- **Shape:**
  - The real `CBasicString<char>::operator<` body (landed 0x00429950) goes above the caller under `#pragma auto_inline(off)`.
  - The inline CCharString::operator< returns false for equal data pointers, true for a null this and false for a null other.
  - With that, VC7.1 keeps retail's register allocation: header in ebp, candidate in esi, node in edi, k in ebx.
  - map and multimap share one `_Rb_tree<K, pair<const K,V>, _Select1st<...>, less<K>, allocator<...> >` instantiation.
- **Recipe (`gen_f012.py`):** the container type is proven per row from the landed callers and the Ego_r member types they touch.
  The leaf is read back from a toy object, and the test covers a duplicate key for the multimap.

  | Row | Container | Evidence |
  |---|---|---|
  | 004b9370 | set<CCharString> | CFaction Allied/EnemyFactions, CQuestManager |
  | 005e04b0 | map<CCharString, CIVCountedPointer<CAnimComponentBase> > | global 0x013B8C1C filled by 0x005DD670 |
  | 00696340 | map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > | RegisterActionEventTypes global 0x013BA8BC |
  | 006bcf50 | map<CCharString, C2DVector> | |
  | 006c31b0 | map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > | |
  | 006f1360 | map<CCharString, TCombatSequenceCreator> | `CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)` |
  | 0071fac0 | map<CCharString, pair<long, CDateAndTime> > | |
  | 00756350 | map<CCharString, pair<long,long> > | |
  | 007acc90 | map<CCharString, bool> | |
  | 008ac200 | map<CCharString, vector<CCharString> > | |
  | 009ab4f0 | map<CCharString, NBankFileManager::CContainedBank> | |
  | 009ab560 | map<CCharString, CBankFileManager::CBankInfo> | |
  | 009ab5d0 | map<CCharString, CCharString> | BankAliasMap |
  | 009d2dd0 | map<CCharString, unsigned long> | |
  | 009eed20 | map<CCharString, CCountedPointer<CConsoleInputBase> > | |
  | 00a2b240 | map<CCharString, CArray<CMeshDataBank::CLipSyncEntry> > | |
  | 00a54310 | multimap<CCharString, float> | C3DAnimationStats TimingEvents |

- **Counts:** 17 matched, all proven.

### F013 -- 15 x STLport backward copy for an 8-byte element with a non-trivial (`__false_type`) assignment, 43 bytes, MATCH
- **Functions:**
  - 14 rows are `__copy_backward_ptrs(first, last, result, const __false_type&)`: `__fastcall`, `ret 8`, with
    `__copy_backward(..., random_access_iterator_tag(), (int*)0)` inlined.
  - 0x00B855C0 is the `ret 4` `copy_backward<CEngineSubPrimitiveRenderer::CSortElement*>`, which reaches the same loop through
    `__copy_backward_aux(__true_type)`.
- **Representatives:** 0x00527CA0 and 0x00B855C0, 3 grades each including one comment-only regeneration regrade. The other rows took 2.
- **Shape:** VC7.1 block-copies any trivially assignable 8-byte element as two dwords through edx, whatever the member types.
- **Recipe (`gen_f013.py`):** the element is proven from the callers (vector insert/erase sites) and their Ego_r member types.
- **Elements:** ThingLList (CThingManager::UpdateLists), NThingMap::CThingMapManager::CThingMapInfo, CRegionMinimap::RegionMinimapCellData,
  pair<EReactionSpeechType,long>, pair<long, COpinionDeed*>, CCrimeWaitingForBribe, CSkeletalMorphs::CEntry, pair<unsigned long,unsigned long>
  (CEngineOcclusionManager::HandleMapper), CFileInstaller::CBufferChunk, CComplexNumber, C3DIndexedBoneWeight,
  CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator, CEngineSubPrimitiveRenderer::CSortElement.
- **Descriptive:** 0x006E49D0 `CUnprovenElement_006e49d0` (a static vector at 0x013BABA4 of two floats) and 0x00AC4290
  `CUnprovenElement_00ac4290` (the ReadGroups table).
- **Counts:** 15 matched.

### F014 -- 15 x `NScript::<Script>::<Entity>::Alloc(<Script>& parent, CScriptDataBase* data, const CScriptThing& me)`, 153 bytes, RELOCATION_MATCH
- **Function:** the entity-script factory. A static `__fastcall` (hidden result in ecx, `ret 8`) builds
  `new <Entity>(parent, data, me)` and hands it to a fresh `CCountedPointer<CEntityScriptBase>`.
- **Representative:** 0x00D1FC30 `CQ_BreakSiegeScript::CDisplacedTrader`, 2 grades.
- **Shape:** the natural PDB constructor matches. `CEntityScriptBase(*parent.Game, me)` and `ParentClass(&parent)` go in the init list and
  `PMasterData = data` in the body. No root-class trick is needed.
- **Recipe (`gen_f014.py`):** the binding site is found by a raw dword search for the row address in .text. That site is
  `<Script>::Main` storing the factory of a `CEntityScriptBinding<Script>` named by the pushed literal, followed by
  `CScriptBase::AddEntityScriptBinding` 0x00CB8230. The script class comes from Main's vtable and the RegisterScripts name.
- **Per-row assertions against the retail bytes:** new size = PDB sizeof, member-ctor offset and target, base vtable 0x012C3224, and an
  Alloc symbol equal to the Ego_r PDB symbol (14 rows).
- **Rows:** CQ_BreakSiegeScript::CDisplacedTrader, CQ_HeroSoulsMotherScript::CHS_Screamer, CQ_HobbeCaveScript::CJames/CBanditSacrifice,
  CQ_MinionClifftopChaseScript::CMapKeeper, CQ_OrchardFarmRaidScript::CGuardTeamMember, CV_BordelloScript::CBordelloLady,
  CV_BanditTollScript::CTollBandit/CBanditLeader, CV_BookCollectingScript::CBS_Teacher, CV_FisticuffsClubScript::CTyler,
  CV_HiddenBootyScript::CM_HiddenBooty, CV_SickChildScript::CWomanToAttract, CV_SingingStonesScript::CSingingStone,
  CQ_AmbushTradersScript::CAT_BanditAlly.
- **Descriptive:** 0x00DCDE20 `CQ_OrchardFarmRaidScript::CGuardTeamMember` has no PDB entry.
  - The class name follows the `C<binding name>` convention that the other 14 rows prove, with binding "GuardTeamMember".
  - Size 0x3C; ScriptThing member at +0x30. The members at +0x1C..+0x2F are descriptive.
- **Stand-in callee:** 0x006E7B40 is `CScriptThing::CScriptThing()`, but it is called through the stand-in
  `CMember_006e7b40 : CScriptThing`. The catalog already binds `??0CScriptThing@@QAE@XZ` to 0x008CFF40, so the stand-in avoids a double
  binding (see Label corrections).
- **Counts:** 15 matched.

## Parked for Fable 5.1 (0)
None. Every instance of every family matched, and differ/ is empty.

## Skipped
None. No row's bytes hold a second function.

## Label corrections
- **F001:** the manifest labels all 163 rows `CGlobal_WatchForHeroDeathScript::Main` (propagated). It is right only for 0x00EE8F50. The
  others are `C<Name>Script::RegisterMain` per the RegisterScripts names, plus 0x00EFF8A0 `CShowTargetedThingHealthScript::Main`.
- **F002:** `_global::CScriptGameResourceObjectMonsterBase_CreateFromEntity` is propagated. All 85 rows are
  `CEntityScriptBinding<NScript::C<Name>Script>::CreateActiveEntityScript`.
- **F003:** `CEngineSubPrimitiveAnimatedMesh::CEngineSubPrimitiveAnimatedMesh` is wrong for all 76 rows. They are the script constructors.
- **F004/F006:** `C3DMeshFileSubMeshChunk::FindFirstBone` / `FindNextBone` is propagated to 90 getters on 11 classes. The real FindFirstBone
  is 0x00ABA530 (QAE) / 0x00ABA6A0 (QBE), and the real FindNextBone is 0x00ABA5C0 / 0x00ABA730.
- **F005/F007/F008/F011:** `Vector_InsertN`, `Std_Vector_Insert_Range`, `std::vector_Realloc_And_Copy`, `Vector_ReallocateAndFill` and the
  other behavioural labels (several claim `pair<u64>` elements that are not there) are all STLport `vector<T>::_M_insert_overflow`.
  Std_Vector_Insert_Range is plainly wrong: no range is inserted.
- **F009:** `CEngine::AddEngineComponent<CEngineShadowRenderer>` is propagated to all 20 rows. Only 0x00B2A3E0 is the ShadowRenderer one.
  The callee names should be `CEngineComponent::SetGlobalComponentPointer` 0x00B59750 and `SetupEnableConsoleVariable` 0x00B59760.
- **F010:** the labels are `vfunc_N` vtable-slot placeholders, and several sit on the wrong class (e.g. CSprite instead of CComponent,
  CMenuEntry/CScrollBar instead of CClickable/CDraggable).
- **F012:**
  - The behavioural labels are wrong; the real name is STLport `_M_find`:
    - `map_FindNode`, `BinaryTree_Search` and `lower_bound_string`.
    - The `lower_bound` callee name used in landed 0x007AE3D0.
  - Landed rows with wrong names:
    - 0x006B94C0 `CNavQuadTree::IsActionPoint` is CFaction::IsEnemyOfFaction (EnemyFactions +0x10).
    - 0x004BAA30 `CRegion::SetMiniMapRegionExitTextOffsetX` is a set<CCharString>::find wrapper (`ret 8`).
    - 0x009A8090 `CTCInventoryExperience::IsTutorialPlayed` is CBankFileManager::RegisterBank(const CCharString&, CBankFile*).
    - 0x00A4EAC0 `CNavQuadTree::IsActionPoint` is C3DAnimationStats::HasEvent.
    - 0x00A4EAE0 `CRegion::SetMiniMapRegionExitTextOffsetY` is C3DAnimationStats::GetEventTime.
    - 0x007ADB30 has a suspicious CRegion label.
    - 0x00756D00 `TransferMap<CObjectAugmentationParticleSet,...>` transfers a map<CCharString, pair<long,long> >.
    - 0x00755640 `CEngineSubPrimitive2DClockSprite_Constructor` is a CTCHeroRememberCrimes lookup.
  - Landed sources with wrong value types or conventions:
    - 0x009AB120 types the value as an invented `SBankRecord`; it is CBankFileManager::CBankInfo.
    - 0x009AAF70 `SRecord20` and 0x009A7F80 `SBankEntryInfo` are NBankFileManager::CContainedBank.
    - 0x009CCDC0's comment calls 0x009D2DD0 `__fastcall`; it is thiscall.
- **F013:** `_Copy_impl<std::pair<float,EConversationAttitude>*,...>` (a Dinkumware Ego_r spelling) is propagated to all 15 rows. None of them
  holds that pair: 14 are `__copy_backward_ptrs<E*,E*>` and 0x00B855C0 is `copy_backward<CSortElement*>`.
- **F014:**
  - `CTriangleList::CTriangleList` is wrong for all 15 rows.
  - The 557 landed `Create_CVtable_XXXX` rows of this shape (e.g. 0x00D62360) are really `NScript::<Script>::<Entity>::Alloc`.
  - Enclosing scripts:
    - 0x00DA34B0 `CQ_GuildTrainingScript::Main` is CQ_MinionClifftopChaseScript::Main.
    - 0x00F024C0 `CQ_HangingTreeGoodScript::Main` is CQ_AmbushTradersScript::Main.
    - 0x00DCC770 `CQ_GuildTrainingScript::Main` is CQ_OrchardFarmRaidScript::Main.
  - Callees:
    - 0x006E7B40 `CCarriedReadableDef::CCarriedReadableDef` is CScriptThing::CScriptThing().
    - 0x007E72A0 is CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase().
  - Catalog: `??0CScriptThing@@QAE@XZ` -> 0x008CFF40 (landed 0x00892220 "EntityGetStandingOnThing") is wrong. 0x008CFF40 returns the null
    script thing through GetNullScriptThing.
- **Earlier landed rows:**
  - Landed 0x00518850 and 0x005198B0 make conflicting vector<CThing*> claims.
  - Landed `CThingMask47` is an invented type.
  - Landed 0x00E95F10 `CSpawnedFunc<CV_ExposeMayorScript>` is CSpawnedFunc<CV_HauntedHouseScript> (vtable 0x012ECBA8; see the call-target
    check above).
