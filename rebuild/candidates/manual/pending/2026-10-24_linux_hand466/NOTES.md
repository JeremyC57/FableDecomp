# Hand chunk 466 (std::vector .. std::list): 12 of 13 byte-exact

The chunk has 13 rows. 11 of the 13 catalog labels are wrong. They are propagated from byte-identical bodies, or they
are Dinkumware/Ego_r spellings of STLport code. The real names come from the FableWin.pdb name set, RTTI vtables and the
callers (see Label corrections).

Checks run on the 12 sources in src/:
- gall.sh grades all 12 MATCH/RELOCATION_MATCH (grades.txt).
- Each `// Leaf:` names exactly one defined symbol in its object (checked with objdump -t).
- calls.py pairs every call target with retail's. The imports match: free 0xBFEA14 and the EnterCriticalSection IAT
  0x0143FE0C.
- All 12 tests print PASS under g++ (tall.sh) and under the lane's VC7.1 (tall.py).

## What matched (12)
- **NScript::CPredicate_And destructors** (0x00EF5B50 two terms, 0x00EF6D50 three, 0x00EF6D90 four, 0x00EF6DE0 five).
  Each destroys the outer right CIsThingAlive term, then the nested Ands inline (each stores its vtable, then destroys
  its right and left terms), then resets the CPredicateBase vptr on every subobject.
  - One model covers all four depths. It is the FableWin.pdb one: `NScript::CPredicateBase` with `bool operator()()
    const`, `Clone() const` and an inline virtual destructor; `CIsThingAlive` with an out-of-line `~CIsThingAlive`
    (0x00E241B0); and `template <class P1, class P2> class CPredicate_And` with a user-declared `virtual
    ~CPredicate_And() { }`.
  - The source does `template class` explicit instantiation of the right depth, and the leaf is the full PDB mangled
    `??1` name (the nested levels' `??1` are emitted too).
  - With 3+ base resets VC7.1 merges them into `mov eax,0x012C2FF4` plus stores. With two it uses immediates. Both come
    out of the same source.
- **CGameCameraBase::~CGameCameraBase** (0x006FD9F0): own vtable, then the two 4-byte STLport lists in reverse order
  (out-of-line `std::list<C3DVector>::~list` 0x0048CA60), then the inline virtual `~CAIGameCameraBase` vtable store.
  - The class model is local: the Ego_r header has PDB-size lists.
- **CIndexBuffer::~CIndexBuffer** (0x00B564E0): `virtual ~CIndexBuffer() { Uninitialise(); }` over a polymorphic
  CMovableResource with an inline virtual destructor.
  - The call has no `mov ecx`: `this` is still in ecx after the vtable store.
- **CAutoLock::CAutoLock(CCritSec*)** (0x00CA48A0, DirectShow strmbase): the wxutil.h body `m_pLock = plock;
  m_pLock->Lock();`, defined out of line, with `EnterCriticalSection` as a dllimport.
- **std::uninitialized_fill_n<unsigned short*, unsigned int, unsigned short>** (0x005123A0, MATCH): STLport 4.6
  verbatim. The chain is uninitialized_fill_n -> __uninitialized_fill_n -> _aux(__true_type) -> fill_n, all
  `__fastcall` templates (/Gr TU).
  - fill_n's const-reference value is re-read on every pass (`mov si,[edx]` inside the loop).
  - The unsigned `__n > 0` test compiles to `test ; jbe`.
- **std::list<long>::pop_back** (0x005791B5, `optimize("s")`): STLport 4.6 verbatim `iterator __tmp = end();
  erase(--__tmp);`.
  - `erase` is declaration-only (0x00429B02).
  - The iterator has STLport's user copy constructor, so the by-value argument is built in its stack slot and the
    dropped result lands in the reserved frame word.
- **std::vector<NUISystem::CComponent*>::operator=** (0x005798C8, `optimize("s")`): the landed
  vector<unsigned short>::operator= model (0x00AADE80) with two changes:
  - `_M_allocate_and_copy` (member template, 0x005796D1) and `__fastcall __copy_trivial` (0x00405BA0) are
    declaration-only, as retail calls them.
  - The /Os ebp frame, the homed x._M_finish and the tail-merged copy call all come out unaided.
- **std::_Construct(pair<const CCharString,CShaderParserSymbol>*, const pair&)** (0x009E6220): `new (__p) _T1(__val)`
  in a `__fastcall` template.
  - The placement new's null test is `test edi,edi ; je`.
  - The STLport pair copy ctor and CShaderParserSymbol's implicit copy (two strings plus the EType) are inline.
- **std::_Construct(CVsyncCallbackTimer::CPendingQuery*, const CPendingQuery&)** (0x009F7890): the region is an
  unoptimised TU (args homed, placement new called out of line at 0x00405C10, a dead result temporary).
  - It needs `#pragma optimize("", off)` AND `#pragma inline_depth(0)`. The optimize pragma alone still expands the
    inline placement new (71v82).

## Parked for Fable 5.1 (1)
- 00bf32b0 CEngineDynamic3DResourceManager::ReleaseResources DIFFER(192v196): our inlined FirstKey sends its null exits straight to the epilogue (`je 0xbf336e`), where retail merges them into one `xor eax,eax` and then does `test eax,eax ; mov ebp,eax ; je` before `push esi ; push edi`, plus a second `test ebp,ebp ; je` at the loop top; this/key sit in ebp/ebx where retail has ebx/ebp | tried: plain same-TU FirstKey/NextKey (FirstKey stays a call; 147v196), FirstKey with its own walk (146v196), both `inline` (146v196), both `__forceinline` (192v196) | next: `__forceinline` NextKey with FirstKey written as `return NextKey((unsigned long)BaseNode);` so the nested expansion leaves the merged return value retail tests; then a `key = FirstKey(); while (key) { ... }` spelling. Behaviour test differ/00bf32b0.test.cpp passes under g++.

## Skipped
- None. No row holds two functions.

## Label corrections
- 0x005798C8 "vector<unsigned long>::operator=" -> `std::vector<NUISystem::CComponent*>::operator=`. Its out-of-line
  `_M_allocate_and_copy` is the landed 0x005796D1. The callers 0x0057740B/0x00577E49 assign CTCInventoryExperience's
  ExpSliders.
- 0x005123A0 "vector<unsigned short>::_Ufill" -> `std::uninitialized_fill_n<unsigned short*, unsigned int, unsigned
  short>` (callers: the landed basic_string<unsigned short> push_back/append and CWideString).
- 0x00B564E0 "~CActionParamCCountedPointer<NUISystem::CComponent>" -> `CIndexBuffer::~CIndexBuffer`.
  - Evidence: the vtables 0x0129EF2C (RTTI CIndexBuffer) and 0x0129B860 (CMovableResource), and FableWin.pdb
    `??1CIndexBuffer@@UAE@XZ`.
  - Its caller 0x00BF34B0 is the ??_G of CEngineDynamic3DResource<CIndexBuffer,3> (vtable 0x012A81B8). That class
    declares no destructor of its own.
- 0x00EF5B50 is the two-term And (catalog: four-term). 0x00EF6D50 is the three-term And (catalog: five-term).
  0x00EF6DE0 is the five-term And (catalog: four-term). 0x00EF6D90 (four-term) is right. Count the derived vtable stores
  / leaf calls to get the depth.
- 0x006FD9F0 "~CPredicate_And<...>" -> `CGameCameraBase::~CGameCameraBase` (RTTI vtables 0x01264A8C/0x0125C1F4, PDB
  `??1CGameCameraBase@@UAE@XZ`).
- 0x009F7890 / 0x009E6220 "_Cons_val<allocator<...>>" (Dinkumware) -> STLport `std::_Construct<...>`.
- 0x00CA48A0 "_List_unchecked_const_iterator<...CPendingQuery...>" -> `CAutoLock::CAutoLock(CCritSec*)` (strmbase;
  its caller 0x00CA7299 locks `m_pFilter->+0x94`).
- 0x005791B5 "list<CVsyncCallbackTimer::CPendingQuery>::pop_back" -> a folded POD-list COMDAT. The TU's own
  instantiation is `std::list<long>::pop_back` (SExpSpendUndo's four list<long>, callers 0x0056F572..0x0056F588).
- Callees seen here:
  - 0x00AC1EE0 (landed as "CVolume::ReleaseVolume") is `CIndexBuffer::Uninitialise` (FableWin.pdb
    `?Uninitialise@CIndexBuffer@@QAEXXZ`), possibly folded with a CVolume twin.
  - 0x00429B02 "LinkedList_Unlink_and_Free" is `std::list<POD>::erase(iterator)`.
  - 0x00405C10 (calls.py name "CFourierAnalysis") is the placement `operator new(unsigned int, void*)`.
  - 0x0048CA60 "CGuiControlTreePane::SortTreeRecursively" is a POD STLport `~list`/`~_List_base`.
  - 0x00AC20D0 "CThingFilter_IsValid::func_00ac20d0" is `CVertexBufferWin32::Sizeof` (as the landed 0x00A90860 says).
  - 0x00BF31D0 / 0x00BF3210 are `CEngineTreeFilerBase::NextKey(unsigned long)` / `FirstKey()`.
  - 0x00B564C0 "~CInputProcessManager" is `CVertexBufferWin32::~CVertexBufferWin32` (stores RTTI vtable 0x0129BA20, then
    tail-jumps to the base dtor).
