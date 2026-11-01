# Chunk 694 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A RELOCATION_MATCH on a function with a jump table proves nothing about the case -> block mapping: the table entries are relocations and get masked. Read the `$L` label values from `objdump -t` of the object and compare them entry by entry with the retail table (0x00AB5420: the first MATCH had release types 5/6 swapped).
- A compiler-generated dtor of an intermediate class stores NO vtable of its own; it is just the inlined base dtor and stores only the base vtable. Model it as a class with no user `~` and emit it with an explicit qualified dtor call (0x00854310 ~CScriptModuleSelfTerminateBase is byte-identical to ~CScriptModuleBase 0x005ED120).
- Two byte-identical "Clear" dtor bodies with the same callee are different classes: name each from its callers' RTTI base-class arrays, never from the body (0x005ED120 vs 0x00854310).
- An indexed vector member read twice (`v[i].f` in the test and in the call) gives retail's `shl ; mov reg,[eax+esi+0xc]` load; a named element-reference or function-pointer local gives lea-scaled addressing instead (0x00A371C0). For four indexed writes to one element, use one element-reference local; writing `v[i].f` four times recomputes the address each time (0x00A36F90).
- `IS_THING_ALIVE(ptr)` is an inline __fastcall template (PDB ??$IS_THING_ALIVE@...@YI_N): GetPItem twice and the Dead bit test; a third GetPItem for the call that follows (0x00859A20).
- DirectShow strmbase rows: the SDK text verbatim, including its oddities (CCmdQueue::GetCommandDueFor AddRefs pPresent in the stream branch), under `// Flags: /EHsc` for the CAutoLock EH state (0x00CAFA10).
- libpk mixer pairs (SSE2/MMX): the same-TU `static __declspec(noinline)` asm kernel plus the out-of-line wrappers written above Process reproduce retail's private ecx-convention calls; the MMX twin is the SSE2 row with mm registers (0x00C6E410 / 0x00C6F990).
- PILOT result: the CVillager "plain inline ~CScriptThing inlines at the first return only" rule does not generalise. CLookAtMe1 (0x00EF5CB0) wants a third inline exit and CUndeadFellow (0x00EF87D0) wants a per-site split of the CScriptThing copy ctor; 13 pragma-free probes each (order, /Ob1, QFE-4035, __forceinline, scope reshaping) never reproduce them.
- vland's SHARED_HEADER_AVAILABLE warning matches class names only: DirectShow strmbase's CBaseObject (0x00CAFA10) is not engine/CBaseObject.h.

Review at landing:
- PILOTS: keep the landed `#pragma inline_depth` sources for 0x00EF5CB0 and 0x00EF87D0; do not propose a mass re-model of the 134 inline_depth rows without testing each row.
- Landed ??_G callers 0x00854500 / 0x00854540 bind 0x00854310 as `??1CScriptModuleBase@NCameraModes@@QAE@XZ`; respell to `??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ` (NAME_FIXES.tsv). 0x00549A00 binds 0x0052CCF0 as ~CDraggableInto; 0x0052D770 binds 0x0052D5C0 as ~CStateList.
- AllocFunc calling convention: FableWin's CClassTypeInit ctor mangling says `CClass* (__fastcall*)()` (P6IPAVCClass@@XZ); check any landed CClassTypeInit/CClassTypeInfo model that spells it __cdecl.
- Local base models next to existing engine headers: 0x00A36F90 (engine/CClassFactory.h flattens the vptr), 0x00859A20 (engine/CCreatureActionBase.h flattens the vptr; retail base is 0xA8 bytes).
- 0x00AB5420 needed 3 grades (one wrong-mapping MATCH); 0x00A371C0 needed 4.
