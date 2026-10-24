# Hand chunk 412 (NGameText::CDataBank .. GFXToY): 30 of 40 byte-exact

## What matched (30)

Bank files and STLport containers (FableWin bank code):
- 009c87c0 NGameText::CDataBank::~CDataBank -- calls CloseTextAndGroups, destroys the four arrays in reverse, then the secondary base (+0x164) and ~CBankFileAsync. SHAPE: same as the landed CParticleDataBank dtor 0x00AF1600. The two refcounted element arrays call their out-of-line `__destroy_aux` instantiations (declared `template <>` specialisations, `__fastcall`, tag temporary by reference). The CCharString array's generic `__destroy_aux` inlines to a call loop.
- 009ca500 _STL::__uninitialized_fill_n -- fills {counted pointer, long} records. SHAPE: `__fastcall` must be on the template DEFINITION. On the explicit instantiation alone it mangles as cdecl `YA`.
- 009cbe90 CBankFileEntryUpdateData::ReadInfo -- `memcpy` from InfoBuffer when offset + size is a valid 31-bit position. SHAPE: a `__forceinline` 64-bit range check `p >= 0 && p <= 0x7fffffff`.
- 009cd620 CBankFile::AddEntry -- wraps the filename in a one-element CArray<CCharString> and calls virtual AddEntryMultipleFiles (slot 6). SHAPE: STLport `vector(size_type n)` with the constant 1. `uninitialized_fill_n` unrolls to one placement copy with a null test. The by-value CCountedPointer has a user copy ctor and dtor, so the outgoing copy is built in its slot and the callee releases its own parameter at the end.
- 009d2160 CSmallVector<CCharString,8>::Allocate -- `Clear(); Start = new T[n]; Size = n;`. SHAPE: the shared CSmallVector model of the landed rows. `new T[]` writes the count cookie and an inline construct loop.
- 009d2a40 _STL::vector<unsigned char>::vector(n, val, alloc) -- fill constructor. SHAPE: `_M_end_of_storage` must be the real `_STLP_alloc_proxy` (allocator base + `_M_data`). Three plain pointers scheduled `mov esi,ecx` one slot late (101v101).
- 009d2d00 _STL::_Rb_tree::_M_copy -- STLport structural tree copy with 0x18-byte nodes {bool colour, parent, left, right, pair<const CCharString, 4-byte>}. SHAPE: verbatim STLport 4.6 `_M_copy`/`_M_clone_node`/`_M_create_node`/`_Construct` without /EHsc. The placement new tests the value address for null.
- 009d2ed0 _STL::__unguarded_partition and 009d3ed0 _STL::__final_insertion_sort -- the CBankFile entry sort by data offset (CompareDataOffset). SHAPE: verbatim STLport with `__fastcall`. They reuse the retail CBankFile model of the landed siblings 0x009D2FC0/0x009D3B90/0x009D3750 (RuntimeData +0x20, PackedDataOffset +0x38). `last - first > 16` compiles to `and eax,-4 ; cmp eax,40h`.
- 009d4e90 CDefStringTable::~CDefStringTable -- the atexit dtor of g_DefStringTable (0x013CA828). SHAPE: a body `StringMap.clear()` (inline STLport clear), then the members in reverse order, then the map's own `~_Rb_tree` clear and `~_Rb_tree_base` deallocate (`if (p) free(p)`).
- 009d5240 CBankFileAsync::SetEnableFailureHandling -- `s_allocationFlags = enable ? 0 : 2`. SHAPE: a static member under `__fastcall`.
- 009d6060 CBankFileAsyncEntry::BeginReadData -- a counted data object over a caller buffer, then SetupRead with skip 0. SHAPE: the landed sibling BeginReadDataAutoAlloc 0x009D6100 (a NAMED counted-pointer local returned by copy, so `inc ; dec` survive) without the alignment code.

MicroThread (FableWin):
- 009d86b0 MicroThread::SetStack -- writes the 0xCDCDCDCD guard word under the top, then the top, end and size globals.
- 009d8710 MicroThread::CreateMicroThread -- builds the initial stack-frame image into a new MicroThreadDetails. SHAPE: a full STLport vector model (tags with user ctors; inline `insert` -> `_M_insert_dispatch(__false_type)` -> out-of-line `_M_range_insert` 0x009B3630). The SInitialStackFrame fields are assigned in declaration order.

Render:
- 009d8b40 CScopedShadersEnable::~CScopedShadersEnable -- restores vertex/pixel shader enables through inline setters on g_ShaderRenderManager (+0x390/+0x391), old pointer 0.
- 009d8c30 CompareQuickDrawTriDepth and 009d8cb0 CompareTriangleAverageDepth (descriptive names) -- qsort comparators, descending depth (`a > b ? -1 : 1`). The second one inlines `(z2 + z1 + z0) * (1/3)`.
- 009de3f0 CRenderManager2D::Draw2DBox -- `return Draw2DBoxWithZ(box, 0.0f, colour, flags);`.

GF utilities (global):
- 009e18b0 / 009e19b0 GFStringToFloat(char / wchar_t) -- SHAPE: hoisted `c = *text` with `c = *(++i + text)` at the loop bottom, `divisor *= 10.0`, and the tail `fraction /= divisor; result = fraction + integer;` (gives retail's `fxch`).
- 009e1ad0 GFGet2DBoxDistance / 009e1b30 GFGet3DBoxDistance -- SHAPE: by-value `GFMax(T, T)` over named `(float)fabs(...)` locals (const-ref max homes both temps). The 3D version declares dz first.
- 009e1bc0 GFGetTime -- QueryPerformanceCounter or timeGetTime fallback. SHAPE: a named `__int64 elapsed` local (`-=` swaps the register pairs).
- 009e1e10 GFVXToAX / 009e1e40 GFVYToAY -- SHAPE: a named display-manager local before the hidden-result call (otherwise the result push is hoisted above GFGetSystemManager).
- 009e1e70 GFVBoxToABox(C2DBoxF) / 009e1f50 GFVBoxToABox(C2DBoxI) / 009e22c0 GFVCoordToACoord(C2DVector) / 009e2330 GFVCoordToACoord(C2DCoordI) -- inline float/long converters defined in the TU. The integer forms need inline LONG-parameter helpers `(long)((float)x * width / g)`. Width/height are converted before `BRX = TLX + width`.
- 009e23a0 GFXToY -- two named display locals first, then `x * width / height`.

## Parked for Fable 5.1 (10)
- 009d8650 MicroThread::MicroThreadSleep DIFFER(12v36): retail is a pure stack switch (`push ebx/ebp/esi/edi ; push fs:[0] ; mov eax,esp ; mov esp,[g_savedStack] ; mov [g_savedStack],eax ; pop fs:[0] ; pop ... ; ret`) | tried: a C++ reference body naming the effect (1 grade) | next: lane gap: retail_asm (the whole body writes esp; needs a retail_asm grade decision by the owner)
- 009d87f0 MicroThread::SwitchToMicroThread DIFFER(124v124): retail loads start/finish once (`mov ecx,[esi] ; mov edx,[esi+4] ; sub edx,ecx`) and sinks the active-thread store below the three memmove pushes; ours stores first and re-reads start (`sub edx,[esi]`) | tried: memmove before the store, store first with begin()/size() inline, named image pointer, image + size locals before the store | next: lane gap: scheduling (store sunk below argument pushes)
- 009e1960 GFStringToInt DIFFER(74v74): retail's zero-length guard `je` lands on the `cmp al,1` sign test; ours threads it past the known-false test to `mov eax,edi` | tried: plain top-read for loop, hoisted `text[i + 1]`, hoisted `*(++i + text)`, ternary negate | next: lane gap: jump threading (RTM and QFE both thread)
- 009e1a80 GFStringToInt(const wchar_t*) DIFFER(75v75): same single branch target as 0x009E1960 (`jbe` to `cmp bl,1` in retail, threaded to `mov eax,edi` in ours) | tried: hoisted read under RTM and QFE-4035, plain top-read `for`, explicit `if (i < length) do {...} while` guard | next: lane gap: jump threading
- 009e1c70 GFGetFastSubFrameTimer DIFFER(40v58): retail reads the counter with inline asm (`xor eax,eax ; xor edx,edx ; rdtsc ; mov [esp],eax ; mov [esp+4],edx`); the rest is `(double)(now - base) / g_CyclesPerSecond` | tried: a C++ reference body with an out-of-line counter read (1 grade) | next: lane gap: retail_asm candidate (minimal `rdtsc` block + `// Retail-asm:` line)
- 009e1dd0 GFAYToVY DIFFER(49v49): retail `fld qword [g_VirtualHeight] ; fmul dword [y]` with the `fstp` after `push ecx`; ours `fld dword [y] ; fmul qword [g]` with the `fstp` before it | tried: `y * g`, `g * y`, inline GetVirtualHeight() getter, `/Op` | next: lane gap: x87 commutative operand order (the landed GFAXToVX 0x009E1DA0 needed a cv-qualified global)
- 009e2020 GFABoxToVBox DIFFER(199v199): only the x87 operand order is left: `fld qword [g] ; fmul dword [x]` in retail vs `fld dword [x] ; fmul qword [g]` in ours, per axis, plus the Y `fstp` before the result-slot push | tried: one-expression GFAXToVX, const-ref dimension local, inline `ScaleToVirtual(double, x, const C2DExtentsI&)` helper (call-first order and `fidiv [eax]` now match) | next: lane gap: x87 commutative operand order
- 009e20f0 GFABoxToVBox(const C2DBoxI&, C2DBoxI&) DIFFER(219v219): only the x87 operand order is left: `fld qword [g] ; fimul [x]` in retail vs `fild [x] ; fmul qword [g]` in ours, plus the Y `fstp` scheduling | tried: inline long converters under RTM, `/G7`, inline `ScaleToVirtual(double, long, const C2DExtentsI&)` helper (235 -> 219 bytes) | next: lane gap: x87 commutative operand order
- 009e21d0 GFACoordToVCoord DIFFER(106v106): only the x87 operand order is left: `fld qword [g] ; fmul dword [x]` in retail vs `fld dword [x] ; fmul qword [g]` in ours, plus the Y `fstp` before the push | tried: one-expression GFAXToVX, const-ref dimension local, ScaleToVirtual helper (108 -> 106), `x * virtualSize` / `y * g` source order (canonicalised back) | next: lane gap: x87 commutative operand order
- 009e2240 GFACoordToVCoord(const C2DCoordI&, C2DCoordI&) DIFFER(116v116): only the x87 operand order is left, as 0x009E21D0 | tried: one-expression GFAXToVX, const-ref dimension local, ScaleToVirtual helper (118 -> 116) | next: lane gap: x87 commutative operand order

## Skipped
None. All 40 rows are single functions, and no row ends one 0xCC long after a noreturn call.

## Label corrections
- 009c87c0 CGameDataBank_DestructorHelper -> NGameText::CDataBank::~CDataBank (vtables 0x0129B4D4 / 0x0129B528; donor ??1CDataBank@NGameText@@UAE@XZ 0x00B17970). Callee 0x009C8660 "CommitChanges" -> CDataBank::CloseTextAndGroups.
- 009ca500 _Fill_n<unsigned_long*,unsigned_int,unsigned_long> -> _STL::__uninitialized_fill_n over {SIVCountedPointer, long}.
- 009cbe90 BankData_CopyRange -> CBankFileEntryUpdateData::ReadInfo.
- 009cd620 Map_InsertOrReplacePair -> CBankFile::AddEntry(const CCharString&, unsigned long, const CCharString&, CCountedPointer<CBankStateBlock>) (donor 0x00B1BF50).
- 009d2160 CSmallVector_AllocateHeap -> CSmallVector<CCharString,8>::Allocate(unsigned long).
- 009d2a40 Vector_InitFill -> _STL::vector<unsigned char>::vector(unsigned int, const unsigned char&, const allocator&).
- 009d2d00 BankEntry_CloneTree -> _STL::_Rb_tree<CCharString, pair<const CCharString, 4-byte>>::_M_copy.
- 009d2ed0 CBankFile_QuicksortPartition -> _STL::__unguarded_partition<unsigned long*, unsigned long, CBankFile::CompareDataOffset>.
- 009d3ed0 CBankFile_IntroSort -> _STL::__final_insertion_sort<unsigned long*, CBankFile::CompareDataOffset>. The callee 0x009D3B90 catalogued `_Insertion_sort1` is STLport `__insertion_sort` (the landed source already says so).
- 009d4e90 LTextTreeWalkThrough_Destructor -> CDefStringTable::~CDefStringTable. Its callee 0x0043020A "LTextTreeWalkThrough::BuildTreeArray" is the shared map<CCharString,long> `_M_erase`. 0x00996B30 "~CCompressedMemoryDataInputStream" is ~CMemoryDataOutputStream (vtable 0x0129A788).
- 009d6060 CBankFileAsyncEntry_SetupReadRequest -> CBankFileAsyncEntry::BeginReadData (donors 0x00B2A1B0 / 0x02FF7EC0). The manifest's 4-parameter signature for 0x009D5D30 is propagated from 0x009D5DF0. The retail ctor takes 3 (buffer, size, bank).
- 009d8b40 CShaderRenderManager_ApplyShaderStates -> CScopedShadersEnable::~CScopedShadersEnable.
- 009d8c30 Comparator_YPosition_Ascending / 009d8cb0 Comparator_DepthWeighted_Ascending -> descending (back-to-front) depth comparators. The descriptive names are CompareQuickDrawTriDepth / CompareTriangleAverageDepth.
- 009de3f0 ConstructString -> CRenderManager2D::Draw2DBox.
- 009e19b0 GFParseFloat -> GFStringToFloat(const wchar_t*).
- 009e1ad0 Math_MaxAbsDelta -> GFGet2DBoxDistance; 009e1b30 Vector3_MaxComponentAbsDifference -> GFGet3DBoxDistance.
- 009e1e10 GFAXToVX -> GFVXToAX; 009e1e40 GFAYToVY -> GFVYToAY (these convert virtual to actual).
- 009e1f50 GFABoxToVBox -> GFVBoxToABox(const C2DBoxI&, C2DBoxI&); 009e2240 GFABoxToVBox -> GFACoordToVCoord(const C2DCoordI&, C2DCoordI&); 009e2330 GFABoxToVBox -> GFVCoordToACoord(const C2DCoordI&, C2DCoordI&).
- Callees: 0x009B3630 "Vector_InsertGap" -> vector<unsigned char>::_M_range_insert. 0x00486189 is vector<unsigned char>::resize(n, const&).
