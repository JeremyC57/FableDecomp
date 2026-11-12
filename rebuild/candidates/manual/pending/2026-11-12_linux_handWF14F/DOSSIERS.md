## 0x009F20B0

# DOSSIER 0x009F20B0 CMemoryAllocator::CMemoryAllocator(void* heap)  (598 bytes, 0x009F20B0..0x009F2305)

## 1. Retail shape (xdis.py 009f20b0 0x266)
- __thiscall, `ret 4` (one arg: heap). this -> ebp (`mov ebp,ecx`), returned in eax (`mov eax,ebp`) = ctor.
- NO frame pointer (sub esp,8 ; push ebx,ebp,esi,edi). No SEH, no /GS cookie. Default /O2 shape, no /Oy-.
- Frame: 8 bytes of locals: [esp+0x14] = loop-1 counter i (dword), [esp+0x13] = BYTE spill of the fixed-size bit
  (`mov [esp+0x13],dl`) that is NEVER read again (dead store). Arg heap at [esp+0x1c], re-read from the arg slot at
  each sub-allocator new (`mov ecx,[esp+0x2c]` after 4 pushes). Loop 2's outer counter `type` lives IN THE DEAD heap
  arg slot [esp+0x1c] (compiler overlay, not source).
- ebx = 0 from entry through loop 1 (xor ebx,ebx; used for MemoryAllocated/NoAllocations/CurrentTrackingID stores,
  `cmp [esi-0x14],ebx`, `cmp eax,ebx` new-null tests, `push ebx` = the 0 `void* base` arg of the variable ctor,
  `push ebx` = 0 arg of the vcall). Loop 2 reuses ebx as outer type index then inner allocator index a.
- Order: init-list stores +0x400, +0x40c, +0x414(byte) BEFORE InitializeCriticalSection; then Enter; call
  InitializeMemorySystem; rep stosd 15 dw @+0x424; rep stosd 15 dw @+0x460; inline 15-byte zero @+0x415
  (dword,dword,dword,word,byte via lea eax,[ebp+0x415]); rep stosd 0x100 dw @+0; MemoryAllocated=0 again (+0x400);
  NoAllocators=0x1e (+0x410). Heap (+0x408) is NEVER stored.
- Loop 1: esi = &g_AllocatorInfo[0].InternalFlags (0x0139687C), stride 0x1c, `i < NoAllocators` re-read each iter
  (unsigned jb). Skip if MaxAllocSize==0. edx=InternalFlags>>1&1 (spilled), edi=Flags(type);
  AllocatorExists[type]=1 written BEFORE the branch. Fixed: operator new(0xc4c) -> ctor 0x00A65FF0(heap, MinChunkSize,
  MaxAllocSize, Alignment, type, InternalFlags); store PAllocator (+4 from esi); vcall slot 3 (+0xc) with arg 0;
  result -> MaxAllocSize; `if (MaxAllocSize > MaxFixedAllocationSizes[type])` signed (jle) then store.
  Variable: operator new(0xc38) -> ctor 0x00A65930(heap, MinChunkSize, 0, InitialSize, Alignment, type,
  InternalFlags); store PAllocator. Both new-null arms produce `xor eax,eax` (normal new-expression null check).
- Loop 2: type 0..14 (`cmp ebx,0xf ; jl` signed), edi walks AllocSizeToAllocator[type], [edi-0x3c] =
  MaxFixedAllocationSizes[type] loaded before the AllocatorExists test. __allmul((__int64)(max+2), 8 (64-bit)); 64-bit
  `>=0 && <=0x7fffffff` check; failure jumps to EPILOGUE 0x9F22FA (skips MemoryAuditBuffer=0 and LeaveCriticalSection).
  malloc(max*8+0x10) (lea edx,[esi*8+0x10]); `inc esi; js skip` => size loop `for size=0; size<=max+1`.
  Inner: for a < NoAllocators (re-read, unsigned): info.Flags != type -> next; if !(InternalFlags>>1&1) -> found; else
  minSize = (a>0 ? table[a-1].MaxAllocSize+1 : 0) -- the a>0 test is strength-reduced to a SIGNED pointer compare
  `cmp eax,0x139687c ; jle`; `size<minSize || size>info.MaxAllocSize` (signed) -> next. Found: [size].PAllocator=
  info.PAllocator, [size].ID=(uchar)a, break. Not found: nothing stored.
- Tail: MemoryAuditBuffer(+0x404)=0 with an immediate (`mov dword [ebp+0x404],0` -- ebx no longer 0 here), Leave.
- Loop alignment padding: `nop` at 0x9F213F, `lea esp,[esp]; mov edi,edi` at 0x9F2207 (jmp over) -- normal /O2.

## 2. Name / signature (manifest label is CORRECT)
- `??0CMemoryAllocator@@IAE@PAX@Z` = protected: __thiscall CMemoryAllocator::CMemoryAllocator(void*). Sources:
  ghidra_out/egor_pdb_names.tsv (Ego_r 0x00B45C30) + ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv
  (0x030BCD20, 0x0181B21C). manifest functions.tsv: __thiscall, (CMemoryAllocator*, void*).
- Single caller: 0x009F2897 inside CMemoryAllocator::Create 0x009F2870 (landed
  rebuild/src/compiled/00/9f/CMemoryAllocator_Create_009f2870.cpp, `new CMemoryAllocator(GetProcessHeap())` through
  inline class `??2CMemoryAllocator@@KIPAXI@Z` = VirtualAlloc). Ctor is protected in the PDB.

## 3. Callees
| target | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| IAT 0x0143FEE4/0x0143FE0C/0x0143FE10 | - | Initialize/Enter/LeaveCriticalSection(g_allocatorLock 0x013CAA70) | inline bodies of `?InitSynchronisation@CMemoryAllocator@@KIXXZ`, `?EnterSynchronisation@..KIXXZ`, `?LeaveSynchronisation@..KIXXZ` (protected static __fastcall; out of line in Ego_r 0xB45B60/0xB45B80, inlined in TLC) | raw API calls -- consider calling inline Init/Enter/LeaveSynchronisation (header CMemoryAllocator.h already declares Enter/Leave as non-inline statics) |
| 0x009F1D20 | 00/9f/global_InitializeMemorySystem_009f1d20.cpp | InitializeMemorySystem | `?InitializeMemorySystem@?A0xcf4a15dd@@YIXXZ` (anonymous-namespace, __fastcall, same TU = MemoryAllocator.cpp) | `void __fastcall InitializeMemorySystem()` global -- OK at link level, PDB says anon namespace |
| 0x00A66730 | 00/a6/global_operatornew_00a66730.cpp | `op_new` (__fastcall, VirtualAlloc body) | `??2CMemoryAllocatorBase@@SIPAXI@Z` (public static __fastcall; Ego_r 0x00C085A0) | draft `CMemoryAllocatorBase::operator new` static __fastcall = PDB-correct; landed name `op_new` is WRONG (NAME_FIXES candidate) |
| 0x00A65FF0 | 00/a6/CMemoryAllocatorFixedSize_CMemoryAllocatorFixedSize_00a65ff0.cpp | CMemoryAllocatorFixedSize ctor | `??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z` (void*, 5x unsigned long) | matches |
| 0x00A65930 | 00/a6/CMemoryAllocatorVariableSize_CMemoryAllocatorVariableSize_00a65930.cpp | ctor | `??0CMemoryAllocatorVariableSize@@QAE@PAXJ0JKKK@Z` (void*, long, void*, long, ulong, ulong, ulong) | matches |
| vslot 3 (+0xc) | FixedSize vtable 0x0129DB64 slot 3 = 0x00A65D40, 00/a6/CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40.cpp | GetAllocationSize | `?GetAllocationSize@CMemoryAllocatorFixedSize@@UAEKPAX@Z` -> virtual unsigned long GetAllocationSize(void*) | draft `GetMaxAllocationSize(long reserved)` WRONG: it is `GetAllocationSize(0)` (NULL void*), returns unsigned long |
| 0x00BFEB50 | 00/bf/global_allmul_00bfeb50.cpp | __allmul (toolchain_library) | CRT | compiler-generated from __int64 multiply |
| 0x00BFEA0E | 00/bf/global_malloc_00bfea0e.cpp | malloc import thunk (toolchain_library) | msvcrt _malloc | OK |
- Note: the vcall's result is stored into a `long` MaxAllocSize and compared signed with MaxFixedAllocationSizes (long[15]).

## 4. Members (PDB struct_layouts_egor.tsv)
CMemoryAllocator (0x49c, no vtable): MemoryAllocatedPerTrackingID ulong[256] +0; MemoryAllocated ulong +0x400;
MemoryAuditBuffer void** +0x404; Heap void* +0x408 (not touched); NoAllocations ulong +0x40c; NoAllocators ulong +0x410;
CurrentTrackingID uchar +0x414; AllocatorExists bool[15] +0x415; MaxFixedAllocationSizes long[15] +0x424;
AllocSizeToAllocator CAllocSizeToAllocator*[15] +0x460.  Draft layout = PDB exactly.
CAllocatorInfo (0x1c): MaxAllocSize long, MinChunkSize long, InitialSize long, Alignment long, Flags ulong, InternalFlags ulong,
PAllocator CMemoryAllocatorBase*.  CAllocSizeToAllocator (8): PAllocator CMemoryAllocatorBase*, ID uchar.
- rebuild/include/engine/CMemoryAllocator.h is a DECLARATION-ONLY projection (no fields; opt-ins
  FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE = Get()/OnFailedAllocation, FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED =
  AllocateAligned/DeallocateAligned). No opt-in offers the fields, the ctor, Init/LeaveSynchronisation or
  CAllocatorInfo/CAllocSizeToAllocator. Every landed CMemoryAllocator row (dtor 0x9F2310, GetAllocationSize 0x9F2390,
  GetHeaderOverhead 0x9F2610, GetNoFreeAreas 0x9F2500, InternalDeallocate 0x9F2920...) models class/CAllocatorInfo
  LOCALLY with the same spellings `g_allocatorLock` (unsigned char[24], 0x013CAA70) and `g_AllocatorInfo[]`
  (0x01396868). No PDB name found for either global. Including CMemoryAllocator.h would conflict with a local class --
  siblings don't include it.
- CMemoryAllocatorBase.h: with FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE it is the real polymorphic class (15 pure
  virtuals, slot 3 = GetAllocationSize(void*)), but declares NO ctor and NO operator new. CMemoryAllocatorFixedSize.h is
  a generated pad-only struct (no ctor). CMemoryAllocatorVariableSize.h (needs the VARIABLESIZE macro) declares no ctor.
  So the ctors + `operator new` must be declared locally or via a #define-rename trick like the fixed ctor row does.
- Draft local models with a shared header: CMemoryAllocatorBase, CMemoryAllocatorFixedSize,
  CMemoryAllocatorVariableSize, CMemoryAllocator (header exists but has no layout).

## 5. Visibility
- InitializeMemorySystem (0x9F1D20) is in the same TU (anon namespace, defined earlier) but large -> called, not inlined.
- Init/Enter/LeaveSynchronisation: inline in retail (IAT calls appear directly) -> original header had inline bodies.
- PDB has `?IsAllocatorFixedSize@CMemoryAllocator@@QBE_NK@Z` (const, takes allocator index) -- likely the original
  spelling of the `InternalFlags>>1&1` test (used in both loops); inline member.
- No STLport calls. __allmul is compiler-generated.

## 6. Siblings (same TU, 0x009F1D20..0x009F2BE0)
- 00/9f/CMemoryAllocator_CMemoryAllocator_009f2310.cpp (dtor): no pragmas, no Flags line, local class + g_AllocatorInfo.
- 00/9f/CMemoryAllocator_Create_009f2870.cpp: local `class CMemoryAllocator{protected: unsigned char _body[0x49c]; ...}`,
  inline class operator new.
- 00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp: #include "fable_compiler.h", calls
  `g_AllocatorInfo[id].PAllocator->GetAllocationSize(...)` (slot 3), raw Enter/LeaveCriticalSection(g_allocatorLock).
- 00/9f/CMemoryAllocator_InternalAllocate_009f2ac0.cpp: `// Flags: /GS` (only TU flag seen); others default.
- 00/a6/CMemoryAllocatorFixedSize_CMemoryAllocatorFixedSize_00a65ff0.cpp: `#define CMemoryAllocatorBase
  CMemoryAllocatorBaseFields` include-rename trick + local true polymorphic base.

## 7. Old draft model errors / suspects (vs disassembly)
- Draft DIFFER 596v598 (2 bytes short): retail keeps ebx=0 and re-reads heap from the arg slot; ours pins heap in ebx.
- The DEAD byte spill [esp+0x13] of the fixed-size bit is the strongest clue: the source reads `fixed` AFTER the
  sub-allocator new (live across calls) and the optimiser threaded that second test away. Try:
  `bool fixed = IsAllocatorFixedSize(i)` (or InternalFlags test); `if (fixed) PAllocator=new Fixed(...); else
  PAllocator=new Variable(...); if (fixed) { MaxAllocSize = PAllocator->GetAllocationSize(0); if (...>MaxFixed[type]) ...}`
  -- this extra live local raises pressure so heap goes to the stack slot and ebx stays the zero register.
- vslot 3 misnamed (GetMaxAllocationSize(long)); real is `unsigned long GetAllocationSize(void*)` with 0.
- Lock calls should probably be inline Init/Enter/LeaveSynchronisation statics (bytes identical, possibly scheduling).
- Second loop `a > 0` compiled as signed pointer compare: keep `a` as a signed long index (draft does).
- Failure of the size check returns from the ctor without LeaveCriticalSection (retail jumps to epilogue) -- draft right.

## 8. Scout re-check (wf14f, 2026-11-12) -- everything above re-verified; additions
- Disassembly re-run (xdis 009f20b0 0x266): body ends `ret 4` at 0x9F2303, int3 pad 0x9F2306..0x9F230F, next fn (dtor) 0x9F2310
  => retail length 598 (0x256); the draft header's "608 bytes" is the padded span. No frame pointer, no SEH, no /GS.
- xref.py: ONE caller, 0x009F2897 (CMemoryAllocator::Create 0x9F2870). PDB leaf `??0CMemoryAllocator@@IAE@PAX@Z` (Ego_r
  0x00B45C30, Ego_r size 0x250 = 592) + FableWin signatures -> 2 sources; label CORRECT; ctor PROTECTED (IAE).
- InitializeMemorySystem: PDB `?InitializeMemorySystem@?A0xcf4a15dd@@YIXXZ` (anon namespace, __fastcall, Ego_r 0xB458B0 +
  FableWin 0x030BCA50). LANDED row 00/9f/global_InitializeMemorySystem_009f1d20.cpp spells it `void InitializeMemorySystem()`
  (cdecl, leaf `?InitializeMemorySystem@@`) -- disagrees with PDB on namespace + convention (no-arg: call bytes identical).
  Draft spells `void __fastcall InitializeMemorySystem()` = PDB convention. Same anon namespace also holds GetBoolVal/GetSizeVal
  (landed global_GetSizeVal_009f1c80), SkipBlanks/SkipToChar/SkipToEOL/IsComment/SkipBlanksAtEnd (FableWin only) -- all earlier
  in the TU, all irrelevant to this body except as TU context.
- operator new 0x00A66730: PDB `??2CMemoryAllocatorBase@@SIPAXI@Z` (Ego_r 0xC085A0 + FableWin 0x033BEC90). Landed row
  00/a6/global_operatornew_00a66730.cpp names it `op_new` -> NAME_FIXES.tsv candidate (0x00A66730 -> CMemoryAllocatorBase::operator new).
- Globals g_allocatorLock (0x013CAA70) / g_AllocatorInfo (0x01396868): no PDB / FableWin name found (grep AllocatorInfo,
  anon-namespace symbols) -> keep the landed siblings' spellings (made-up, consistent).
- Sibling idiom for the polymorphic base: GetNoFreeAreas_009f2500 / GetTotalMemoryControlledByAllocator_009f2400 /
  GetHeaderOverhead_009f2610 do `#define CMemoryAllocatorBase CMemoryAllocatorBase_HeaderLayout` + `#include
  "engine/CMemoryAllocatorBase.h"` + `#undef`, then a local polymorphic CMemoryAllocatorBase with UnusedSlotNN virtuals. None of
  the 17 landed CMemoryAllocator_* rows use pragmas; only InternalAllocate_009f2ac0 has `// Flags: /GS`. Default /O2 TU.
- Residue status: docs/pipeline/REGISTER_SWAP_SUMMARY.md section 2 lists "zero constant in ebx vs immediates" as MOSTLY UNSOLVED
  lane-wide. NOTES attempt 3 pins it with c2prio: const-0 web priority (-162) loses to heap (-18) and i (-31) because its
  live range covers loop 2 + the tail `MemoryAuditBuffer = 0`; with loop 2 + tail removed loop 1 is byte-identical to retail.
  So the author must find a source fact that cuts the const-0 web at loop 2 (or demotes heap/i); do NOT re-try the ~60 spellings
  listed in NOTES.md attempts 1-3.

