# Hand chunk 493 (CEngineClothSimulation2DAnimatedMesh .. CVtable_012b2f98): 35 of 40 byte-exact

18 MATCH and 17 RELOCATION_MATCH. There are no retail_asm rows. All 35 tests print `<addr>_TEST PASS` under both g++
(tall.sh) and VC7.1 (tall.py). The vland dry run gives 35 WINS and no RELOC_IDENTITY. Two rows are parked and three
are skipped.

## What matched (35)
- **CEngineClothSimulation2DAnimatedMesh / CEngineClothSimulationMeshBase** (0x00B80780, 0x00B80930)
  - CalcAndSetRunState stores run state 0 and reports that the state is not 1.
  - IsRunning returns whether ParticleSims is non-empty. A bool result needs `empty() ? false : true` to give a bare
    `setne al`. Writing `!empty()` widens through a full register (18v10).
- **CEngineOcclusionManager** (0x00B8D280, 0x00B8D2F0): GetFlareTestResult / GetPrevFrameFlareTestResult.
  - Uses the donor include of engine/CEngineOcclusionManager.h, and casts the opaque CArray to a typed STLport CArray.
  - The PC build folds the pixel count to the 0.0f pool constant (`fdivr [0.0f]`), then clamps to 1.0f.
  - The 0.0f default is loaded before the size test.
- **std::__lower_bound<pair<ulong,ulong>*, ulong, MapperPred, int>** (0x00B8F180)
  - Copied from the landed 0x00B8F1C0 model with an 8-byte pair, so the divide becomes a bare `sar 3`.
- **CArray<T>** (0x00BCA080 operator[], 0x00BE8B00 size)
  - Out-of-line COMDATs, emitted by explicitly instantiating only the member: `template T& CArray<T>::operator[](unsigned int);`.
- **CEnginePrimitiveManagerAnimatedMeshesBase::ApplyPalSkinVertexShaderSpotShadow** (0x00BCE900)
  - The two-ulong (AAEXKK) members end in a tail jump that reuses both argument slots. The second argument is
    unused, but it must exist, or VC emits `call ; ret 4` like the one-ulong siblings do.
- **CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType()** (0x00BDD6B0)
  - The non-copyable base ctor, three member inits, then the vtable store.
- **CEngineTreeFilerBase::CheckStructure** (0x00BF2E00): the release build keeps only the sibling/child walk.
  - To match, read `node->ChildListHead` twice and let CSE merge the reads.
  - A named `head` local swaps the eax/ecx roles of child and slot (69v69 twice).
- **zlib 1.2.1 `z_compressBound`** (0x00C03EB0) and **LZO 1.08 `lzo_assert`** (0x00C08530): library text,
  `extern "C" __cdecl`.
- **CString (MFC-style libpk string)**
  - `CString()` 0x00C08880: `m_pchData = g_pchNil`.
  - `Find(char,int)` 0x00C0BB70 and `Find(const char*,int)` 0x00C0BC50: the MFC text, with GetData() inlined
    carrying its LIB_ASSERT, `IsValidString` as an inline null test, and the tchar.h `_tcschr` / `_tcsstr` inline
    wrappers over the dllimport `_mbschr` / `_mbsstr`.
  - `Append(char)` 0x00C0DD30: `ConcatInPlace(1, &ch)`.
- **CRampedParam::SetTarget(float,float)** (0x00C0B880): a byte twin of the landed 0x00C3A950.
- **CScriptHost** (vtable 0x012B25A8)
  - Slot 4 `GetQueuedEvent` (0x00C0E6B0) uses the event list's keyed Find at slot 17, with the key passed by reference.
  - Slot 0 `Post` (0x00C0E970) is `m_pQueue->Process(this)`.
- **CVtable_012b2240::Visit** (0x00C0EFE0): the nth-key visitor. The landed visitor slot 0 takes `void**`.
- **std::logic_error::what** (0x00C109A0): a hand model of the VC7.1 `exception` and the Dinkumware `_Myptr()` (`_BUF_SIZE <= _Myres ? _Ptr : _Buf`).
- **CUnprovenPKInterface_012B2748::IsRunning** (0x00C10DB0): an `int` result of `!world->Paused`, giving `xor ; sete cl ; mov eax,ecx`.
- **CVtable_012b28d8 comparators** (0x00C155F0 / 0x00C15620 / 0x00C15640): Less, EqualToKey and LessThanKey, all int results.
  - In Less, the shared `sbb ; neg` tail comes from `ka == kb ? a < b : ka < kb`.
- **CVtable_012b2ab8 malloc allocator** (0x00C16700 / 0x00C16720): Allocate and Free over the dllimport CRT.
  - The live result decides the cleanup: Allocate pops its argument with `add esp,4`, while Free uses `pop ecx`.
- **CDebugBreak::CDebugBreak** (0x00C16750): `DebugBreak()`.
- **PKCTR red-black tree slots** (vtable 0x012B2F0C)
  - Find (slot 7) and FindByKey (slot 8).
  - ForEach (slot 13) and GetCount (slot 10), both on the landed callees' class names.
  - GetFirst (slot 11) and GetLast (slot 12). These two need the found path written first:
    `if (node != &Sentinel) return node->Key; return 0;`.
- **CVtable_012b2f98::Hash** (0x00C19970): the inline `strlen`, then a tail jump into the __fastcall JenkinsHashBuffer
  with seed 0 written over the argument slot.

## Parked (2)
- 0x00BF2980 CEngineTreeFilerBase::GetTreeMemSize DIFFER(113v96)
  - Residue: VC7.1 strength-reduces the child-table walk (`lea ebp,[node+1Ch] ; add ebp,4`) and spills `this`.
    Retail indexes `[edi+esi*4+1Ch]` and keeps `this` in ebp.
  - Tried: `(CNodeHeader**)(this+1)` accessor, a `Children[1]` member array, byte-pointer `GetChild(i)`, and a
    while-loop with a table cast. All four came out 113v96.
  - Next: try a `long` count recomputed through an out-of-line-looking inline, or index via a `CNodeHeader*` and a
    signed char loop variable. Compare with the other loops over the table in the landed AddNode 0x00BF2A10.
- 0x00C039F0 zlib 1.2.1 syncsearch DIFFER(92v82)
  - Residue: the LTCG private convention passes `have` in edi and `buf` in esi. The lane has no /GL link, so the
    function gets __cdecl stack arguments.
  - Tried: the zlib 1.2.1 text, graded once as `extern "C"`.
  - Next: a lane gap. It needs an LTCG-capable link step; the body itself is the library text.

## Skipped (3)
- 0x00BFEEA9: a CRT SEH filter fragment that runs on its parent's ebp frame. It is not a function.
- 0x00C088B0: two functions in one row: a thunk, int3 padding, then 0x00C088C0. The catalog must re-split it.
- 0x00C0BF9B ("CCharString_CopyRange_FromOffset"): a fragment from the middle of a routine. It uses flags at entry
  and pops an ebx it never pushed.

## Label corrections
- 0x00BDD6B0 "CXMVPlayerBuffers::CXMVPlayerBuffers" is `CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType()`.
  Evidence: it stores the RTTI vtable 0x012A7D94.
- 0x00C08880 "Object_SetVtable" is `CString::CString()`. It stores the g_pchNil data pointer, not a vtable.
- 0x00C16750 "list<CMessageEvent const*...>" is `CDebugBreak::CDebugBreak()`: a bare DebugBreak call.
- 0x00C0BF9B "CCharString_CopyRange_FromOffset" is a mid-routine fragment, not a function.
- 0x00C109A0 (gapscan) is `std::logic_error::what`. Evidence: slot 1 of the RTTI vtable 0x012B263C.
- The vland RELOC_NAME warnings come from propagated noise labels:
  - 0x00C0C270 "CCharString_InsertData" is CString::ConcatInPlace.
  - 0x00C08940 "_global::CTCHero_OnBlockParrySuccess" is the landed CEventQueue_00c08940::Process.

## Header debt
- engine/CVertexShader.h is now used by 0x00BCE900.
- engine/CEngineOcclusionManager.h is used via the donor include.
- These classes have no engine/ header, so local models were used:
  - CEnginePrimitiveManagerAnimatedMeshesBase
  - CLocalDetailObjectCollectionType
  - CEngineTreeFilerBase (+ CNodeHeader)
  - CBaseClass / CBaseClassNonCopyable
  - CShaderRenderManager
  - CEngineClothSimulation*
- The libpk classes (CString, CRampedParam, CScriptHost, the PKCTR tree, and the CVtable_* classes) have no PDB, so
  they get no header.

## Callee names
- 0x00988020 `?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z` (Ego_r), on g_pShaderRenderManager 0x01436E14.
- 0x0099A2F0 CBaseClassNonCopyable::CBaseClassNonCopyable.
- 0x00C0B570 `?SetTarget@CRampedParam@@QAEXMMM@Z`.
- 0x00C0C270 `?ConcatInPlace@CString@@QAEXHPBD@Z`.
- 0x00C08940 `?Process@CEventQueue_00c08940@@QAEXPAUCEvent_00c08940@@@Z`.
- 0x00C16750 `??0CDebugBreak@@QAE@XZ`, on the assertion hook g_pfnAssertHook 0x0143BB2C.
- 0x00C18D50 VisitInOrder.
- 0x00C18DF0 CountNodes.
- 0x00C1D500 JenkinsHashBuffer (__fastcall).
- Imports:
  - DebugBreak 0x0143FE64
  - malloc 0x01440158
  - free 0x01440154
  - _mbschr 0x0144001C
  - _mbsstr 0x0143FFDC

## Names
### Proven
0x00B80780
0x00B80930
0x00B8D280
0x00B8D2F0
0x00B8F180
0x00BF2980
0x00BF2E00
0x00C039F0
0x00C03EB0
0x00C08530
0x00C109A0
### Probable
0x00BCE900
0x00BDD6B0
0x00C08880
0x00C0B880
0x00C0BB70
0x00C0BC50
0x00C0DD30
0x00C16750
### Descriptive
0x00BCA080
0x00BE8B00
0x00C0E6B0
0x00C0E970
0x00C0EFE0
0x00C10DB0
0x00C155F0
0x00C15620
0x00C15640
0x00C16700
0x00C16720
0x00C19010
0x00C19080
0x00C19110
0x00C19130
0x00C19150
0x00C19180
0x00C19970
