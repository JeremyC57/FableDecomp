# agent55 — hand-authored chunk 55 of the class-labelled 17–96-byte pool (CLocalDetailPrimitiveRepeatedMesh .. CMapFilenames)

Linux VC7.1 lane (Wine + DDK cl.exe). Chunk 55 (`chunk55.tsv`, 40 rows): **32 of 40** graded `RELOCATION_MATCH`/`MATCH` with
behaviour tests (all 32 PASS under g++ and under the lane's cl.exe via `tcl.sh`); eight parked (sources in `differ/`, one
`// Grade:` line each). Sources from `gen.py` (+ the per-attempt edits applied in place), tests from `gen_tests.py`,
`payload.json` / `oracle.tsv` from `finish.py`. 25 rows matched on the first grade, 4 on the second, 1 on the third, 2 on the
fourth; no flags or pragmas were needed by any matched row except the two NUISystem::CManager rows (`optimize("s")` with
`("y", off)` for GetLayer, `optimize("s")` for SetGraphicsBank).

## The chunk's shape
- Landscape: 0x00BF9B40 is CLocalDetailPrimitiveRepeatedMesh::OnPostRelocation (vtable 0x012A82AC slot 7): five rebinds through the
  landscape memory manager (0x01436EA8 -> +0x6b0), +0x50/+0x48 with 0x00BD9AB0 and +0x54/+0x44/+0x4c with 0x00BD9AE0 (the second empty
  `ret 8` member, landed as E2::M). The two constructors 0x00BDFB50 (CLocalDetailPrimitiveMesh, vtable 0x012A7E04, CEnginePrimitiveHandle
  record at +0x6c) and 0x00BFAA80 (CLocalDetailPrimitiveRepeatedMesh, record at +0x58, eight block pointers cleared in the body) are the
  agent54 00bfac70 model and hit the same lane gap (below).
- Script modules: CScriptThing is {vptr, CCountedPointer<CThing> PImp} (12 bytes); `Target = target` (0x00D9ECF0 SetTarget) is the
  implicit CScriptThing assignment = the counted pointer's operator= with both source fields read before the compare (agent52's shape);
  0x00E93020 is the nested form `PQuest->Slot281(Thing.Slot1(0))` (the quest vtable fetched before the inner call); the Main bodies
  (0x00DD1EB0 / 0x00DD1AC0) test the parent's byte +0x88 (and pointer +0x78), call the abort test 0x00F35B30 and tail-jump to the parent's
  0x00DD1AF0 (manifest "CQ_OrchardFarmRaidScript::DoMultiplierCutscene"); the Init bodies are plain stores around the quest slot calls.
- CMainGameComponent::BeginInputLoading (0x00C087C0) forwards (window, flag, this, 0) to the `__fastcall` 0x00C0AEE0 (window in ecx, flag in
  dl, two stack words; it forwards (owner, 0, 0x10) to 0x00C0AE70 -- the manifest's "ConstructString(CWideString*, long)" signature is
  wrong). CMainGameComponentInit (0x00D15140) is four CCharStrings at +0, +0xc, +0x10, +0x14 with two uninitialised words.
- CManagedTexture::Sizeof (0x00A69E60): `8 + (data ? 0x4c + AllocSize + (Texture ? ByteLength + 0xc : 8))` written as `size = 8; if (data)
  size = AllocSize + (tex ? ByteLength + 0xc : 8) + 0x54;` (the ternary inside the add duplicates the `lea` tail, the hoisted 8 serves the
  early return). engine/CManagedTextureData.h and engine/CTexture.h layouts hold (ByteLength is the 28-bit field at +4).
- NUISystem::CManager (size-optimised TU, frame pointers): GetLayer (0x0041E3B2) clamps the key to [-3, 13], finds it in the VC6-style tree
  at +0x70 (node value at +0x14, head at +0) and falls back to `key = 0; return tree[key]` (the slot reuse `and [ebp+8],0` reproduces
  from the assignment); SetGraphicsBank (0x0042A9B7) takes the counted pointer BY VALUE and assigns through the out-of-line ShareData
  (the by-value argument's destructor @ 0x00419108 runs on the slot). The retail layout differs from engine/CManager.h (definition
  manager +0x64, bank +0x10, layer tree +0x70).
- The "CManager::GetEmbeddedEventNames"/"AnimHasEmbeddedEvent" rows are an animation-event query object (bank pointer at +4, flag byte
  +0x3d): `CCountedPointer<const C3DAnimationStats> stats = m_pBank->GetAnimation(a)->GetAnimationStats(); stats->X(...)` with the hidden
  result pointer pushed before the inner call's argument and the `__forceinline` release; AnimHasEvent (0x006F9720) is
  `found = Table(); if (!found && m_flag) found = Embedded(); return found;` (the callee's al carried straight out).
- 0x0088BB60 is a CAIBrain method: a stack CBrainOutput (0x00835620, the landed layout), ForceReset (0x0088BA40) and the inlined
  ~CBrainOutput (CBaseIntelligentPointer dtor @ 0x00A01C10 on +0x40 -- CIntelligentPointer has no destructor of its own -- then the
  PAction counted pointer released).
- CMap: the grid model of the landed GetEngineThemeAt (0x0081F090): game grid at +0x1a7c (cells +0x10, 12 bytes = engine/CHeightMapCell.h,
  width +0x204, maxY +0x208, index `(maxY - y) * (width + 1) + x`), quarter grid at +0x1a78 (7-byte entries at +4, width +0x1c, height
  +0x20), palette at +0x1a6c (8-byte entries). IsCameraPassableAt returns the +9 bitfield's bit 3 after a virtual slot-9 call on the
  0x013B86A0 engine globals; GetThemeSizeZAt computes the cell pointer before the 0x0081FA20 call (named locals) and adds after;
  AllocateMapThingUID is `return m_nextThingUID++` on an unsigned __int64 at +0x1d30; InitGameMap builds the new CGameMap (0x24 bytes,
  0x006B7760 with (world +0x20, this, a stack copy of the 8-byte record at +0x1d1c)) BEFORE reading the old pointer, stores, then deletes
  the old one through slot 0; GetScriptedMapBrush walks the sentinel list at +0xc comparing the block's name (0x0082E250 is a no-argument
  `lea eax,[ecx+48h]` accessor, not "GetAnimFilter(long)") with the inlined CCharString == (same block, or both non-null with equal
  lengths and `Compare(a, b) == 0` through the `__fastcall` 0x00411570) and ends with `throw;` (= _CxxThrowException(0, 0), no /EHsc needed).
- The two "CMapFilenames" constructors: 0x00529B00 is CThingSwitch (CThing(9) @ 0x005296B0, the CThingGameObject vtable 0x01244F7C and its
  CIntelligentPointer at +0xc0, then the CThingSwitch vtable 0x012453FC and two CCharStrings at +0xd4/+0xe0); 0x00ABEF60 is
  C3DMeshMaterial::CFilenames (vtable 0x0129EDB4): the reference count at +4 set to 1 by a `__declspec(novtable)` base's inline
  constructor (its store precedes the derived vtable store and the `lea` of the first string; a non-polymorphic base puts the `lea` first),
  then four CCharStrings.
- 0x00BFC370 "CManagedTextureData ctor" is CPatchTesselationEdgeStrip (vtable 0x012A8318, 0x2c bytes): CBase @ 0x0099A2F0, then a member
  record with an inline constructor (the stores follow the vtable store; body or init-list stores on the class itself precede it).

## Label corrections
- 0x00BDFB50 is CLocalDetailPrimitiveMesh's constructor (vtable 0x012A7E04), not CLocalDetailPrimitiveRepeatedMesh's (0x00BFAA80).
- 0x00529B00 "CMapFilenames::CMapFilenames" is CThingSwitch::CThingSwitch; 0x00ABEF60 is C3DMeshMaterial::CFilenames::CFilenames.
- 0x00BFC370 "CManagedTextureData::CManagedTextureData" is CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip.
- 0x0088BB60 "CManager::GetEmbeddedEventNames" is a CAIBrain method (ForceReset with a stack CBrainOutput).
- 0x00C59BC0 "CManager::GetCurrentScreen" is the render-pass Begin of agent27's 0x00C59720 family.
- 0x006F90D0 / 0x006F91E0 / 0x006F9240 / 0x006F9720 are one animation-event query class (bank at +4), not NUISystem::CManager.
- 0x00C0AEE0 "ConstructString(CWideString*, long)" is a `__fastcall` with two register and two stack arguments (`ret 8`).
- 0x0082E250 "CModeBlock::GetAnimFilter(long)" takes no argument (`lea eax,[ecx+48h] ; ret`): the block's name string.
- 0x00A242C0 "CTCInventoryAbilities::GetAbilityLevel" is the animation bank's `m_pAnimations[i]` lookup (+0x380 table, `ret 4`).
- 0x00BD9AE0 ($E2) is the landscape memory manager's second empty (block, owner) member, called like 0x00BD9AB0.

## Lessons
- A `__fastcall` free function with stack arguments: `void __fastcall F(void* a, bool b, T* owner, long ctx)` gives `mov dl ; push 0 ;
  push ecx ; mov ecx,[esp+0Ch]` with no tail call (different `ret` sizes).
- `throw;` (a rethrow) is `push 0 ; push 0 ; call _CxxThrowException` and compiles without /EHsc in a function with no try block.
- A found/else result that leaves the callee's al untouched is a bool local assigned by each call (`found = A(); if (!found && f) found = B();`).
- `new` then swap: `T* map = new T(...); T* old = m_p; m_p = map; if (old) delete old;` reads the old pointer AFTER the constructor;
  the local-before-new form hoists the read and holds it in a register across the call.
- A ternary inside an add (`AllocSize + (p ? x + 0xc : 8) + 0x54`) duplicates the `lea` tail into both arms; the hoisted `mov eax,8 ; je`
  early return needs the `size = 8; if (data) size = ...; return size;` form (a separate `return 8` gets its own block).
- A novtable intermediate base's inline constructor store (`mov [esi+4],1`) precedes the derived vtable store AND the `lea` for the first
  member constructor; the same store from a non-polymorphic base comes after the `lea`.
- The counted-pointer assignment inlined from the implicit CScriptThing operator= (vptr not copied) is the agent52 shape (both source
  fields read before the compare).
- A list-search `operator==` on CCharString: `Compare(this->data, other->data) == 0` (this first; `neg ; sbb ; inc` for the == 0).

## Parked (8; sources in differ/)
- 0x0041DB1D NUISystem::CManager::CreateComponent (71 v 71): retail pushes `name` before the inline manager selection
  (`global ? GFGetPDefinitionManager() : (m_p ? m_p : &CGameDefinitionManager::Get())`); the lane evaluates the branches first and
  pushes after (ternary, plain inline and __forceinline helper forms; the out-of-line helper form pushes first but keeps the call).
- 0x006F90D0 AnimHasEmbeddedEvent (92 v 92): register choice only -- retail `mov ecx,[event] ; mov eax,[stats] ; push ecx ; mov ecx,eax`,
  the lane `mov eax,[event] ; mov ecx,[stats] ; push eax` (operator->, direct member, const long& and optimize("s") forms); its two
  void siblings 0x006F91E0 / 0x006F9240 match with the same model.
- 0x0081F590 CMap::GetSoundAt (63 v 65): retail divides pos.X by 4 first (into esi), the lane evaluates pos.Y / 4 first in every form when
  the coordinates come through the reference (the landed 0x0081F090 twin with plain parameters matches the same expression).
- 0x00A69DA0 CManagedTextureData::Uninitialise (65 v 65): retail re-reads Texture_Data (+0x20) right after clearing it (a null `this` for
  the tail-jumped CTexture::Uninitialise @ 0x009F9F70 -- a retail bug); the lane folds the load to `xor ecx,ecx`; a volatile view keeps
  the load but moves the +0x20 store before the +0x24 store.
- 0x00BDFB50 / 0x00BFAA80 the two CLocalDetailPrimitive*Mesh constructors (69 v 69, 93 v 93): the agent54 00bfac70 gap -- the lane hoists
  `mov [esi+34h],ecx ; lea ebx/edi,[esi+record] ; mov ecx,...` (the member record's constructor `this`) above the novtable base's five zero
  stores; retail keeps the zero stores first and `push edi` late (init-list, base-body and optimize("s") forms; the latter out-lines the
  constructors).
- 0x00C59BC0 render-pass Begin (89 v 89): the wave-24b gap -- the lane narrows `(flags >> 8) & 0xFFFFFF01` to `0x00FFFF01` (agent27
  tried four forms on the sibling 0x00C59720); the rest of the body (handle select, 8-byte record copy, two virtual calls) matches.
- 0x00D9F160 CMC_Minion::Init (56 v 56): `push edi` placement only (retail pushes it before the `inc dword ptr [eax+50h]`, the lane after
  the quest vtable load); direct increment, inline AddMinion helper and optimize("s") forms.
