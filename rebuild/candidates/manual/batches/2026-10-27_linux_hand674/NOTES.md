# Hand chunk 674 (CRendererInputPin .. CEngineLandscapeMemoryManager): 9 of 10 byte-exact

Rows: 10 (_gapscan 5, CTCInventoryMap 2, _global 3). 2 MATCH, 7 RELOCATION_MATCH, 1 parked (DIFFER), 0 skipped.
Every test passes under g++ (tall.sh) and VC7.1 (tall.py); the parked row's test (differ/00cbf7fe.test.cpp) passes under both too.

## What matched (9)
- **CRendererInputPin** (DirectShow renbase.cpp)
  - 0x00CA5200 `CheckMediaType`, MATCH. It is `return m_pRenderer->CheckMediaType(pmt);`, built on the landed CompleteConnect model (0x00CA5190), with the declaration added.
  - Its unlisted neighbours are 0x00CA5210 Active and 0x00CA5220 Inactive.
- **CBaseFilter** (DirectShow DECLARE_IUNKNOWN)
  - 0x00CA62D0 `QueryInterface`, MATCH. It is the `GetOwner()->QueryInterface` forwarder in `__stdcall` form.
  - Shape: VC7.1 emits it only once the owning CBaseRenderer vtable exists, so I defined the CBaseRenderer ctor with an empty body. The forwarder then comes out as `mov eax,[esp+4] ; mov eax,[eax-8] ; ... ; jmp [ecx]`.
- **CConversationAttitude** (`.text$yc` initializers)
  - 0x0121A4F0 `$E_ConversationAttitudeToAnimName` and 0x0121A580 `$E_ConversationAttitudeToAnimGroupName`, both RELOCATION_MATCH.
  - Shape: `std::vector<CCharString> X(12, "");` with the real STLport `vector(size_type, const T&, const allocator_type& = allocator_type())`, plus `CCharString(const char*, long = -1)`. The temporary `""` lives at `[esp+8]`, and the fill loop runs with `edi = 12`.
- **anonymous namespace (CrimeText)**
  - 0x0121A7CA `$E_CrimeText`, RELOCATION_MATCH.
  - Shape: `CCharString CrimeText[10] = { "", "TEXT_GUI_CRIME_WEAPON_OUT", ... };` in a `#pragma optimize("s", on)` TU. The /O2 build gave 182 bytes. The packed (unaligned) start address is the tell for /Os: `-1` sits in esi and each element is a `push esi ; push lit ; mov ecx ; call`.
- **CTCInventoryMap**
  - 0x005FD410 `GetRegionsToDisplay`, RELOCATION_MATCH (11 probes).
  - Model: STLport set/_Rb_tree (insert_unique 0x00512DA0), and a list with same-TU `_M_insert_dispatch`/`insert`/`_M_create_node`. The `_List_base` ctor uses the `_Node* __n` form, and the copy ctor is `insert(begin(), x.begin(), x.end())`.
  - Model, continued: a named `CThing* thing` local, and `__forceinline unsigned long HasTC` (gives `test ch,10h`).
  - The quest manager is reached through `CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass`.
- **CCutsceneDef**
  - 0x00F29DC0 `~CCutsceneDef`, RELOCATION_MATCH. This is the implicit destructor. It destroys eight `std::vector<CCharString>` at +0x3C..+0x90, then the packed CParentDefClassBase (CDefString +0x25, CVectorMap +0x29), then tail-jumps to ~CResource.
  - Shape: it is emitted by defining `CCutsceneDef::CCutsceneDef() {}`, and I used the real `_Vector_base`/`_STLP_alloc_proxy`/`_Destroy` structure.
  - The catalog name "CTCInventoryMap::ResetPrimitiveHandles" is wrong.
- **zlib 1.2.1 trees.c**
  - 0x00C057D0 `_tr_flush_block`, RELOCATION_MATCH, /GS.
  - It is the landed trees.c TU with the two 1.2.1 lines restored: `if (s->data_type == Z_UNKNOWN) set_data_type(s);` and `s->data_type = (Byte)(...)` in set_data_type.
  - The landed TU has zlib 1.2.2's `strm->data_type`; see LESSONS "Review at landing".
- **CEngineLandscapeMemoryManager**
  - 0x00BD9CE0 `FreeDisposalList`, RELOCATION_MATCH. It drains the texture, vertex-buffer and index-buffer disposal lists for each of the 3 generations.
  - Shape: same-TU `FreeVirtual` and `DeleteTexture`/`DeleteVertexBuffer`/`DeleteIndexBuffer` (`p->~T(); FreeVirtual(p);`). The same TU also reproduces retail 0x00BD9CC0 DeleteTexture byte for byte.
  - The catalog name "_global::TexturePool_Clear" is wrong.

## Parked for Fable 5.1 (1)
- 00cbf7fe PlayCutsceneSound DIFFER(465v480): the two independent prologue instructions are in the wrong order (retail `mov [ebp-14h],edx ; mov edi,ecx`, ours the reverse). The other 15 bytes of the 480-byte catalog span are the next function, GetScriptActiveStatus 0x00CBF9CF, and the parked source already reproduces those bytes exactly. | tried: 16 forms (probe/00cbf7fe.log): cast / functional-cast / `+0` / block-scope / function-scope key copies, const param, inline find helpers (by value, out-param), STLport two-level find, named end() iterator, direct assignment vs copy temporary, forceinline GetLength/IsEmpty, named ReadAsString locals | next: lane gap: scheduling (cl 3077 orders the fastcall register-home store after `mov edi,ecx` once the key is copied; without the copy the key home is not shared and the frame grows to 0x40); split the catalog row at 0x00CBF9CF.

## Skipped
- None.

## Label corrections
- 0x00F29DC0 is `??1CCutsceneDef@@UAE@XZ`, not CTCInventoryMap::ResetPrimitiveHandles. Its ??_G 0x00F29DA0 is landed as a CLandscapeBackgroundPatch stand-in; it is `??_GCCutsceneDef@@UAEPAXI@Z`.
- 0x00BD9CE0 is `?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ`, not _global::TexturePool_Clear.
- 0x01228ED8 is landed as "CDebugManager::ThrowException" (`FableThrow_1228ed8`). It is really the atexit destructor of CrimeText, registered by 0x0121A7CA.
- 0x00CBFAB8 is landed as "SetMiniMapRegionExitTextOffsetX" (fn_00cbfab8). It is really `?SetScriptActiveStatus@@YIXABVCCharString@@W4EScriptActiveStatus@@@Z`, over map 0x0143E910 = ScriptActiveStatuses.
- 0x013B8A38 / 0x013B8A44 are landed as g_ConversationAttitudeAnimNames / g_ConversationAttitudeAnimGroupNames. They are the PDB static members CConversationAttitude::ConversationAttitudeToAnimName / ConversationAttitudeToAnimGroupName.
- The CStringParser ctor 0x00404720 is landed with callback type `P6AXPAV1@@Z`. The Ego_r spelling is `P6IX0@Z` (fastcall, const CCharString&).
- The five _gapscan rows all get real names (see Proven).
- The 005FD410 and 00CBF7FE labels are right.
- Manifest labels flagged by vland RELOC_NAME: 0x009F2000 / 0x009F2010 are labelled `_Dest_val<...CFlash>` but are GFEnterAllocatorLock / GFLeaveAllocatorLock. 0x009F1F10 `GetMember_dword0x40c` is CMemoryAllocator::GetAllocationCount (the spelling 18 landed sources use). These are propagated-label errors; not respelled here.
- All rows are in NAME_FIXES.tsv.

## Proven
- 0x00CA5200 ?CheckMediaType@CRendererInputPin@@UAEJPBVCMediaType@@@Z -- slot 8 of primary vtable 0x012BF408 (xref 0x012BF428). It forwards to m_pRenderer vtable +0xB0 = CBaseRenderer slot 44 CheckMediaType. Same TU as the landed CompleteConnect 0x00CA5190. MATCH.
- 0x00CA62D0 ?QueryInterface@CBaseFilter@@UAGJABU_GUID@@PAPAX@Z -- slot 0 of CBaseRenderer's IBaseFilter vtable 0x012BF458. The -4 thunk 0x00CA6340 sits in the IAMovieSetup vtable 0x012BF444. AddRef/Release forwarders follow at 0x00CA62E0/0x00CA62F0. MATCH.
- 0x0121A4F0 _$E1 (`dynamic initializer for 'CConversationAttitude::ConversationAttitudeToAnimName'') -- FableWin.pdb initializer order (GeneralAttitudeToAnimName 0x0121A460 -> this -> GroupName -> MostRecentGroupQueried -> AttitudeSymbols). FableWin vector(n, val) ctor. Writes global 0x013B8A38, atexit 0x01228D40. RELOCATION_MATCH.
- 0x0121A580 _$E1 (`dynamic initializer for 'CConversationAttitude::ConversationAttitudeToAnimGroupName'') -- same order. Global 0x013B8A44, .CRT$XCU 0x013742EC, atexit 0x01228D80. RELOCATION_MATCH.
- 0x0121A7CA _$E1 (`anonymous namespace'::`dynamic initializer for 'CrimeText'') -- .CRT$XCU 0x01374324 points here, and the previous initializer ends with `ret` at 0x0121A7C9. Ego_r order DEFAULT_DEATH_RECOVERY_MARKER_NAME (0100DD76) -> CrimeText (0100DD93, length 0xB1 = 177 = this row) -> CrimeTotalText -> CrimeTimeLeftText. Array at 0x013B8B10, atexit 0x01228ED8. RELOCATION_MATCH.
- 0x005FD410 ?GetRegionsToDisplay@CTCInventoryMap@@QAEXAAV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z -- the catalog label is right. Leaf present in the Ego_r/FableWin signature sets (ghidra_out/naming_stragglers/*.signatures.tsv). RELOCATION_MATCH.
- 0x00F29DC0 ??1CCutsceneDef@@UAE@XZ -- the sole caller is ??_G 0x00F29DA0, slot 0 of vtable 0x012FB6E0, which the landed ctor 0x00F29D00 (Ego_r ??0CCutsceneDef@@QAE@XZ) stores. Eight vector<CCharString> members +0x3C..+0x90. RELOCATION_MATCH.
- 0x00F29DA0 ??_GCCutsceneDef@@UAEPAXI@Z -- slot 0 of 0x012FB6E0; calls 0x00F29DC0.
- 0x00CBF7FE ?PlayCutsceneSound@@YIXAAV?$map@HVCCharString@@U?$less@H@std@@V?$allocator@U?$pair@$$CBHVCCharString@@@std@@@3@@std@@H@Z -- Ego_r 00CA091F. 69 callers in the cutscene macro code. Literals "Play2DSound"/"PlayMusic"/"StopMusic". The catalog label is right. Parked DIFFER.
- 0x00CBF9CF ?GetScriptActiveStatus@@YI?AW4EScriptActiveStatus@@ABVCCharString@@@Z -- the 15-byte tail of the 0x00CBF7FE span: `ScriptActiveStatuses.find(name)->second` over 0x0143E910. Ego_r 00CA07FC. Bytes reproduced by the parked source.
- 0x00CBFAB8 ?SetScriptActiveStatus@@YIXABVCCharString@@W4EScriptActiveStatus@@@Z -- the writer of the same map, the Ego_r pair of GetScriptActiveStatus.
- 0x00C057D0 _tr_flush_block -- zlib 1.2.1 trees.c (decorated __tr_flush_block). RELOCATION_MATCH /GS.
- 0x00BD9CE0 ?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ -- Ego_r name set and layout (three `list<T*>[3]` disposal arrays at +0x10/+0x1C/+0x28). RELOCATION_MATCH.
- 0x00BD9CC0 ?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z -- emitted byte-identical by the 0x00BD9CE0 TU; unreferenced in retail.

## Header debt
- CRendererInputPin / CBaseRenderer / CBaseFilter are DirectShow base classes. They are modelled locally, following the landed 0x00CA5190 model; there is no engine header.
- CEngineLandscapeMemoryManager, CTexture, CVertexBufferWin32, CIndexBuffer: local models. There is an engine header for CEngineLandscapeMemoryManager, but the disposal-list STLport list bodies have to be in the same TU.
- CCutsceneDef uses engine/CDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE) plus a local packed CParentDefClassBase. The engine CParentDefClassBase header is not packed at +0x25.
- CTCInventoryMap / CThing / CQuestManager use local models with STLport set/list.
