# Chunk 513 lessons (GOTCHAS one-liners)

- Most of this re-queue was parked on 2026-10-14 under the QFE-4035 lane; under RTM 13.10.3077 the def constructors, the tree `_M_create_node` / `_M_clone_node`, the ??_G and Init rows matched on the first or second grade -- regrade old "lane gap" parks before re-deriving them.
- A virtual that forwards to the same call on a child found three ways (`if (t) t->X(); else if ((t = B()) != 0) t->X(); else if ((t = C()) != 0) t->X();`) is three call sites VC7.1 tail-merges into one; a single `if (t) t->X()` after the lookups reloads the vptr into edx instead of eax (0x0053D000..0x0053D0C0).
- A CObserver-slot override (`this` at +4, `add esi,-4` before the primary-vtable call) gets retail's `mov ecx,esi ; push eax` order when the class chain is the real one (CEditBox : CChangingStateComponent : CComponent : CUIInterface, CObserver) with the virtuals in the primary base (0x00540120; the old `push eax` park).
- A by-value CCharString argument built with `CCharString(const char*)` into its stack slot needs a declared copy constructor; without one VC7.1 builds a temporary and pushes its dword (0x0053D000).
- A `CDefPointer` returned by value and dereferenced inside the argument list (`ChangeState(LoadAndReturnDef()->HoveredState)`) homes the temporary in the `push ecx` slot and releases it after the call (0x0055BAE0, full MATCH).
- A push_back argument that retail copies into the dead argument slot (`mov [ebp+8],eax ; lea eax,[ebp+8]`) is an rvalue: `push_back(child.operator->())`, `push_back(true)`, `push_back(Children.size() - 1)` (0x005383DD, first grade).
- A count clamped through a pointer (`lea eax,[esi+4] ; sub [eax],ecx ; cmp [eax],0 ; jge ; mov [eax],0 ; cmp ebx,ebx`) is an inline helper taking the member by reference; written inline the test folds into the `sub` flags (`jns`) (0x005BF00B).
- A string length compared signed (`cmp eax,2 ; jle`) while the inline getter returns unsigned is a `long length = s.GetLength();` local; it also gives retail's register choice for the later `length - 1` (0x005DB4C0).
- `X = CDefString(sub)` with an inline `CDefString(const CCharString&) { SetString(s); }` puts the 4-byte temporary in the dead argument slot and copies it out before the substring is destroyed; 0x009D5060 is SetString (no `mov eax,this`), not the constructor.
- A memberwise CItem copy / assignment (0x14 bytes, CCountedPointer at +8) matches as an explicit member-init / member-assign body under `optimize("s")` (0x005C08E6, 0x005C0C24).
- STLport `find_if(first, last, bind2nd(mem_fun_ref(&T::Pred), x))` calls the __fastcall find_if with the 8-byte binder pushed as two dwords (`push arg ; push offset Pred`) (0x005BDE96).
- Open: `push 0 ; call ??_G` for an explicit `_Destroy` of a CDefPointer list value is not produced by an inline ~CDefPointer, a same-TU `delete`, or __forceinline _Destroy under /Os (VC7.1 calls ??1); the gotcha's "implicit destructor" route suggests the list value is a wrapper with an implicit destructor (0x005BB190).
- Open: a CIVCountedPointer release on a frame temporary whose address escaped (`push_back(value_type(key, value))`) re-reads Object after `dec`; retail's single `dec [ecx+4] ; jne` was not reproduced by four spellings (0x005E38C0).
- Open: `long n = s.GetLength(); if (n < 0) n = 0;` -- VC7.1 threads the null-string arm onto the clamp's zero; retail keeps the diamond and re-tests (0x005DB5F0).
- Tests: with no headers (`extern "C" int printf(...)`), a placement `operator new(tsize, void*)` with `tsize` = `unsigned int` under _MSC_VER and `__SIZE_TYPE__` under g++ keeps tests standalone in both lanes; a by-value smart pointer in a test needs its own copy constructor (VC7.1 does not elide the return copy).

## Why earlier attempts stalled
- The 2026-10-14 batches (hand25/30/31/34/37/38/52/54/57/60) graded under QFE 4035 and recorded the parks only in batch READMEs, so they were never re-queued with their notes. Several "lane gaps" (register choice, `push eax` order, temporaries in the dead slot) were source-shape problems: invented base layouts, a missing CCharString copy constructor, a missing tail-merge form.
- Catalog labels for half the rows were propagated across byte-similar bodies (e.g. CEnginePrimitive2DText::operator= for CItem, CAnimationSet::Add for a CVectorMap::Insert, CNavigatorQueueManager::FrameUpdate for a list clear), which pointed earlier attempts at the wrong classes.

## Review at landing
- vland: no RELOC_CONFLICT / RELOC_DUPLICATE. Label-only RELOC_NAME warnings: 0x0040E170, 0x00519EF0, 0x005C13CD, 0x005C04BF.
- 0x004C41D0 (V allocator) vs the landed 0x004C4890 (U); 0x0040E170 / 0x004B74E0 __find spellings by caller element type.
