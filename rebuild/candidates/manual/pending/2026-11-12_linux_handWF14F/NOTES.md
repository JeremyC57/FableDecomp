## 0x009F20B0


## 0x009F20B0 CMemoryAllocator::CMemoryAllocator(void*) -- author attempt 1 (wf14), DIFFER(596v598)
Best source: differ/009f20b0.cpp (= v50.cpp). Leaf `??0CMemoryAllocator`. Still a local model (CMemoryAllocator.h is declaration-only;
siblings model locally too) -- porting onto headers not done yet. `static __forceinline` IsValidSize changed to `static inline` (rule).
Fixed this attempt: vslot 3 is `unsigned long GetAllocationSize(void*)` called with 0 (PDB); loop 2 reads
`maxSize = MaxFixedAllocationSizes[type]` BEFORE the `AllocatorExists[type]` test (retail `mov esi,[edi-0x3c]` precedes the je;
35 -> 34 hunks). With that the register-masked code is identical; residue is pure register choice:
retail ebx = constant 0 through loop 1 (heap re-read from its arg slot, i and the fixed-size bool in an 8-byte frame), then ebx =
type/a in loop 2 (edi row ptr, edx size); ours heap -> ebx in loop 1 (frame 4, bool spilled into the heap arg slot), loop 2 row ptr
ebx / size edi / a edx / type ecx.
Ideas tried (all leave the identical residue unless noted):
- GetAllocationSize(void*) / (void*)0 args; IsAllocatorFixedSize(i) const member (PDB name); inline static
  Init/Enter/LeaveSynchronisation; bool via ==true / explicit expr; no bool local; statement orders in loop 1; no `type` local.
- fixed tested twice (v2: no jump-threading, 626 bytes); nested `if (MaxAllocSize != 0)`; explicit info pointer; allocator temp;
  loop 1 fully indexed (v24/v25 worse).
- locals at top in 3 orders; int counters; one counter shared by loop 1 + loop 2 outer, or loop 1 + inner a (VC splits webs: no effect).
- `long type = 0` declared at top / just before loop 1 (thinking the ebx zero web is loop 2's counter): 610-614 bytes, worse.
- InitializeMemorySystem real landed body in the TU (with and without auto_inline(off)); inline helper for the two `new`s.
- loop 2 shapes: found flag, indexed, nested if, int64 local, entry/table references, ulong/int a, size<max+2 (worse), maxSize+1 local.
- flags /G7 /G5 /Ox /EHsc; pragmas optimize g off / w / a. Zero-fill loops instead of memset (not rep stos: wrong).
- PROBES: heap forced to memory (volatile read) -> ebx becomes the zero in the prologue but i takes ebx in loop 1, zero immediates
  stay immediates; extra zero stores in loop 1 -> local `xor eax,eax`, no zero web. So VC7.1 never forms the cross-loop zero web here
  while heap or i is a candidate; retail treats neither heap nor i as worth ebx.
Best next idea: find the source fact that makes heap/i lose ebx in loop 1 -- e.g. check whether the original passes heap through an
inline member that takes it by reference (address-taken param) AND has more zero-register uses; or compare a sibling VC7.1 function
with the same `xor ebx,ebx` prologue + arg-slot reload pattern; or port onto the shared headers (CMemoryAllocatorBase.h with
FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE, real 15-slot vtable) in case class shape changes weights.

## 0x009F20B0 -- author attempt 2 (wf14), still DIFFER(596v598); best source unchanged (differ/009f20b0.cpp = a1.cpp)
Residue unchanged (register choice only: retail ebx = zero through loop 1, heap / i / fixed-bool all in memory; ours heap in ebx).
NOTE: the model declares the ctor public (`??0CMemoryAllocator@@QAE@PAX@Z`); the PDB symbol is protected (`IAE`), so the leaf in
the file does not match the object symbol (q.py warns and grades the largest function). Make the ctor protected when porting.
Findings (probes, do not repeat):
- PROBE heap AND i moved to globals (ebx completely unused in loop 1): constant 0 STILL stays immediate in the loop body
  (`push 0`, `test eax,eax`). So in our IR the zero constant is never a loop-1 body candidate; the zero web only continues
  into a loop region when it already sits in the same register at the top AND that register is free there.
- e1.cpp (allocator temp, `info.PAllocator = allocator;` after the if/else, then a SECOND `if (fixedSize)` around the vcall):
  prologue becomes retail's (`xor ebx,ebx`, ebx/bl stores, i=0 from ebx), heap and i go to memory (heap re-read from the arg
  slot like retail), the zero continues into the loop HEAD (`cmp [esi-0x14],ebx`, latch re-zeroes ebx) -- but the bool takes ebx
  in the body and the second test is not jump-threaded (retail has two stores and no second test). Closest regime so far.
- heap removed (global) => frame becomes `sub esp,8` with the bool at [esp+0x13] (retail layout) but i takes ebx (coalesced with
  the zero). So retail needs BOTH heap and i to lose to the zero.
- No effect: TU flags /GS /Gr /G6 /GB /Ob1 /Op; QFE-4035 compiler (FABLE_VC71=$S/wine/vc71); g_AllocatorInfo sized / defined
  static in the TU; `a`/`size` declared =0 at top; named `void* base = 0` (top / before loop / in loop; constant-propagated);
  loop 1 as do-while / goto; explicit placement-new null checks (also with heap global); ternary new; const bool; `continue` on
  !fixed; ulong bool; `&&` form; bool / allocator declared at function scope; extra `MaxAllocSize != 0` test; loop-1 body as an
  inline member helper (not inlined by VC: 419 bytes -- would need __forceinline, not allowed). /Os 558, /Oa 603.
- `long type = 0` at top + type used as the loop-1 zero: constant-propagated, no register.
Best next idea: find what makes VC7.1 thread the second `if (fixedSize)` of e1.cpp (retail's dead bool spill + duplicated
`mov [esi+4],eax` stores fit a threaded second test) -- e.g. the second test inside an inline accessor, or a bool assigned in
each arm; or raise the zero-use count per VC71_CODEGEN_RESEARCH 2.3 (zero uses written per arm before tail merging). Then port
onto the shared headers (CMemoryAllocator.h has no fields: needs an opt-in with the PDB layout, protected ctor).

## 0x009F20B0 -- author attempt 3 (wf14), still DIFFER(596v598); best source differ/009f20b0.cpp = t1.cpp (a1 + protected ctor, leaf IAE)
KEY FINDING (c2prio.py, tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py, run with FABLE_HDR_OVERLAY=$W/hdr):
- c2 colouring order for t1: ... heap #4 prio -18 (allowed only ebx after esi=info ptr, edi=type, ebp=this) -> ebx;
  i -31 -> split, memory; const 0 #1 prio -162 -> split, all pieces immediate except the prologue piece (esi).
- const 0 gains only +36 (block 1: the init-list/MemoryAllocated/i=0 stores, cost 4 x K 9); in-loop compares/pushes are
  cost 0. Its -162 comes from being live through loop 2's depth-2/3 blocks (refs: size=0, a=0, minSize=0) and to the tail
  `MemoryAuditBuffer = 0` (cost-1 ref in block 42).
- PROBE pc.cpp (loop 2 + tail removed): const 0 = -13 > heap -18 > i -35 -> const 0 takes ebx whole, heap and i
  "split, memory", and loop 1 is retail BYTE FOR BYTE (sub esp,8, xor ebx,ebx, bl store, cmp [esi-0x14],ebx, cmp eax,ebx,
  push ebx, heap re-read from the arg slot, bool at [esp+0x13]). So the whole residue = const 0's priority vs heap/i.
- Removing only the tail store (pa.cpp) or only the inner zeros (pf.cpp, probe) leaves const 0 at -165 / -217: both the
  tail ref and the inner refs keep it live through loop 2.
- c2 candidates are WEBS: sharing one symbol between loop 1's i and loop 2's a (s1.cpp) gives two candidates; no effect.
- heap address-taken (probe pd: Probe(&heap)) gives retail's prologue (xor ebx,ebx, bl) but i then takes ebx; i
  address-taken (pe) kills strength reduction (imul) -> not retail.
Also tried (no effect / worse): allocator=0 dead init (t2/t3); inline Ref() on heap/i/bool (folded, t4-t6); loop-1 body
wrapped in if(MaxAllocSize!=0) / !MaxAllocSize (x1-x3); variable arm first (y1); e1 merge with per-arm constant bool
(w1, VC does not thread), re-tested IsFixedSize (w2), ulong &2 bool (w3, 592); e1 + continue (z2); non-literal zero var
for loop-2 type init (z1/z2: const-propagated); loop 2 ulong a (614), int vars, ternary minSize (598, worse layout);
inner IsFixedSize spelled >>1&1 / ==1 / &2 (592).
Retail logic: since retail's loop 2 uses ebx for type/a, const 0 cannot be a whole-function ebx candidate there; so either
(a) heap and i fail their whole colouring (ebx taken somewhere in their ranges by a higher-priority web -- none visible), or
(b) const 0's live range in retail does not cover loop 2 (loop-2 zeros a different candidate?). Neither found yet.
Best next idea: use c2prio --trace on variants to find a source fact that lowers heap (-18) and i (-31) below const 0 or
removes them from the list (e.g. heap's web extended into loop 2, a 1-ref heap, K growth in loop-1 blocks), or that splits
const 0's range at loop 2 (e.g. loop 2 + tail in a separately optimised region: an inline member with its own pragma,
or the loop-2 zeros typed differently); verify each with c2prio before grading.
- Loop 2 moved into an inline member `bool BuildSizeTables()` (b1/b2, made-up name, probe only): VC7.1 does not inline it at
  default budget (371 bytes, loop 1 then matches retail); with /d2inlS1000 it inlines and the output is identical to t1,
  so inlining does NOT give loop 2 its own constant candidates. Hypothesis (b) via inlining is dead.

## 0x009F20B0 -- author attempt 4 (wf14f, 2026-11-12): RELOCATION_MATCH (19 grades, 2 c2prio probes)
Final source: 009f20b0.cpp (= z2.cpp; also copied to differ/009f20b0.cpp), test 009f20b0_test.cpp (vt.py PASS), strict-header
regrade RELOCATION_MATCH, registered with addmatch.py (payload.json / oracle.tsv). No flags, no pragmas.
What made it match: in loop 2 the entry store + `break` is written out in BOTH arms (fixed-size range match / variable size)
instead of once after a `continue` chain. VC7.1 merges the two tails after register allocation, so the bytes are the same
shape, but before merging `size` has one more weighted reference block than `a`: size outranks a (estimated 112 v 96 from the t1 block table: one more K4/w4 block with size cost 4, a cost 2; not re-measured), size takes edx and
`a` (byte register needed) takes ebx. That empties the zero constant's allowed set early, so it is split BEFORE heap's turn and
its prologue + loop-1 piece outranks heap (-18) and i (-31): zero keeps ebx through loop 1, heap / i / the fixed-size bool stay
in memory (retail's dead `mov [esp+0x13],dl`), type's outer piece gets ebx in loop 2. The old "const 0 priority" residue was a
loop-2 source-structure fact (docs VC71_CODEGEN_RESEARCH 2.3: cross-jumped tails are counted per arm before merging).
How it was found: c2prio trace of a hack variant (extra `size` reference) showed loop 2 turning into retail's registers and the
const-0 piece landing at -26 next to heap's -18 -> the lever is the a/size order, i.e. the K / reference counts of loop 2.
Headers / flags it needs: overlay hdr/ (4 opt-ins, hdr/README.md): FABLE_RETAIL_CMEMORYALLOCATOR_CTOR (CMemoryAllocator.h: PDB
storage, CAllocatorInfo, CAllocSizeToAllocator, g_AllocatorInfo, protected ctor), FABLE_RETAIL_CMEMORYALLOCATORBASE_OPERATOR_NEW,
FABLE_RETAIL_CMEMORYALLOCATORFIXEDSIZE_CLASS, FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_CTOR, all on top of the existing
FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE. No local class models left in the row file.
Spelling facts (graded): the fixed-size bit must be a bool local `((info.InternalFlags >> 1) & 1) != 0` in loop 2 (a plain `if`
on the expression folds to `test byte [eax],2`). The PDB accessor `IsAllocatorFixedSize(unsigned long) const` written as
`g_AllocatorInfo[allocator]...` matches in loop 1 but NOT in loop 2 (strength-reduced pointer based at +0 instead of +0x14), so
the row does not use it and the overlay does not declare it. The 64-bit size check is written out (`__int64 tableBytes`; an
inline helper also matches).
Made-up names (found in no PDB / FSE / sibling source): `g_AllocatorInfo` (0x01396868) and `g_allocatorLock` (0x013CAA70) --
the spellings the landed CMemoryAllocator rows already use; locals (`fixedSize`, `maxSize`, `minSize`, `tableBytes`, `a`).
Landed names proved wrong / different from the PDB (see NAME_FIXES.tsv): 0x00A66730 is landed as `op_new`, PDB
`??2CMemoryAllocatorBase@@SIPAXI@Z` (Ego_r 0x00C085A0 + FableWin 0x033BEC90); 0x009F1D20 is landed as cdecl global
`InitializeMemorySystem`, PDB `?InitializeMemorySystem@?A0xcf4a15dd@@YIXXZ` (anonymous namespace, __fastcall; Ego_r 0x00B458B0 +
FableWin 0x030BCA50) -- this row declares it `void __fastcall InitializeMemorySystem()` (same name, PDB convention).
Not done: Init/Enter/LeaveSynchronisation are still raw KERNEL32 calls on g_allocatorLock (as in the sibling rows); the header's
Enter/LeaveSynchronisation are declared out of line, so using them would not inline.

