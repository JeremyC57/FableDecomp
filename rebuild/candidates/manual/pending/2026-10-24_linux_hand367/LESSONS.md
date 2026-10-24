# Chunk 367 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- CEnginePrimitiveHandle::Clear() inlined into a loop (std::fill / __copy / __copy_backward, 0x00440100 / 0x00440260 / 0x00440390): retail
  keeps the pointee in ecx (`mov ecx,[esi+4] ; cmp ecx,edi ; je ; cmp [ecx+4],1 ; jne ; mov eax,[ecx] ; push esi ; call [eax+0Ch]`).
  That needs the pointee member form `if (Data.Object) Data.Object->NotifyIfLastHandle(this);` (the member of the landed 0x0041C500 model). The
  `Data.GetRefCount() == 1` and `Data.IsUnique()` forms load the pointee into eax and add `mov ecx,eax` (+2 bytes). In the out-of-line
  operator= 0x0043F9B0 retail DOES have the eax + `mov ecx,eax` shape, so the right form depends on the context: try both.
- An STLport `__destroy_aux(first, last, const __false_type&)` over DEQUE iterators (0x00440B40, sibling 0x00440C00) is `__fastcall` with
  the tag reference in ecx and the two 16-byte iterators on the stack (`ret 20h`). The callers pass `lea ecx,[esp+N]` (an odd address = the
  empty tag). The block size is `128 / sizeof(T)` (28-byte element -> 4, `lea ebx,[esi+70h]`).
- A `bool`-typed PDB free function whose retail body ends `xor eax,eax ; inc eax` / `xor eax,eax` (0x0042E3CE InstallationComplete) is an
  `int` return under `optimize("s")`. The bool form gives `mov al,1` / `xor al,al` (graded 29v32).
- An STLport `pair<A, B>` copied by an inlined `_Construct` gets `movsd` for a 12-byte POD pair. Retail's member-by-member copy needs an
  explicit `pair(const pair& o) : first(o.first), second(o.second) {}` (0x0043212B).
- Size-TU STLport helpers (allocate, _Construct, __uninitialized_copy, copy, resize(n), the traits ctor, TransferBinaryIn, _M_clone_node)
  are emitted out of line unless `__forceinline` (0x0043212B 38v80 -> match, 0x00433273 153v185 -> match, 0x00432BAB).
- A class with a `double` member and its own vptr pads the vptr to 8 bytes (all members shifted by +4, 0x004350D0 CDisplayEngine).
  Deriving from a vptr-only root (`struct CBaseClass { virtual void v00(); ~CBaseClass(); }`) keeps the members at +4 (same as the
  chunk 166/167 __int64 lesson).
- A by-value C2DBoxF return copied through a USER copy constructor (member init list) moves the first float through the FPU
  (`fld [esp] ; fstp [eax]`) and the other three through integer registers. The implicit copy is four integer moves (0x00434B50, 52v50).
  The PDB name set confirms `??0C2DBoxF@@QAE@ABV0@@Z`.
- A two-branch virtual (show/hide observer) under /O2 tail-duplicates the base call into the first branch and threads the second test
  away. Retail re-tests and calls the base once, which is `optimize("s", on)` (0x0042D604, 0x0042D65B).
- Implicit `const char*` -> CCharString argument conversions are built BEFORE the explicit argument temporaries of the same new-expression
  (0x00419D90). An explicit `CCharString("x", -1)` argument evaluates right to left after them.
- VC7.1 `wchar_t` / `L""` in a lane source needs `#include <stddef.h>` (0x0042DED5 SRC_FAIL otherwise).
- Behaviour tests: a 64-bit `unsigned long` changes struct sizes and counts. Use `int` for a stream count read (0x00433273 segfaulted)
  and never assert retail `sizeof` values in a test (0x00440B40).

Review at landing:
- 0x00431606 (parked after 4 grades): the graded stand-in models the key as `CBasicString<char>*` under CComparePointer (the landed
  0x007AD060 set model). The real tree is map<CCharString, long>: its _M_insert 0x004316CF (this chunk) and _M_create_node 0x00431773 use
  CCharString keys. Re-model it with those types before more grading.
- Local models next to existing engine headers (each source has a `Model: local --` line that gives the reason). The generated headers are
  data-only (pack(1), no member functions, vptr and counted pointers flattened), so a body that IS a member or calls virtuals cannot use
  them: 0x0041641F, 0x00417568 (CDisplayEngine, CWorld, CSurface, CPixelFormat); 0x00419D90 (CConsoleCommandBase); 0x0041C090 (CTexture);
  0x0041C850 (CPixelFormat); 0x0041CEB3, 0x0041D0A8, 0x0042DED5 (CManager, CDefinitionManager, CIVCountedPointeeBase); 0x00434B50,
  0x004350D0 (CDisplayEngine); 0x00436BA0 (CDisplayViewManager); 0x00436FB0 (CIEngine, forward only); 0x0043A2A0 (CPlayerGui, CWorld);
  0x00440100 / 0x00440260 / 0x00440390 (CEngineInternalPrimitiveBase).
- engine/CObserver.h is the donor 0x18-byte layout; retail CObserver is 0x14 (CChangingStateComponent: CObserver +4, CComponentBase3
  +0x18; 0x0042D604 / 0x0042D65B). engine/CParentDefClassBase.h is 0x3D, retail 0x39 (landed 0x00431ED0). Both headers need a retail
  reconciliation.
- The handle pointee has three names in landed code: `CEngineInternalPrimitiveBase` / m_handleCount (here and in 0x00B19C20),
  `CIVCountedPointeeBase` / m_refCount (0x0041C500, which has the NotifyIfLastHandle member used here) and
  `CEnginePersistentPrimitiveBase` / RefCount (0x0043F9B0, 0x00BBC310). Ego_r.pdb types the handle's pointer as
  `CIVCountedPointer<CEnginePersistentPrimitiveBase>`. Unify the names when the handle gets a shared header.
- The landed CEnginePrimitiveHandle sources disagree on Clear() spelling (GetRefCount / IsUnique / NotifyIfLastHandle). Each graded in
  its own context, so do not "normalise" them without re-grading.
- 0x0041CFB2 is an LTCG custom-convention body (hidden result in edi). It cannot be graded in the standard lane.
