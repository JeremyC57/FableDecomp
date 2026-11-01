# Chunk 694 (NA037 + a two-row pragma-free pilot) -- 10 of 10 new rows landed; the pilot failed (2026-11-01)

Ten rows landed (`verify.log`, every row RELOCATION_MATCH + PASS, vland's RELOC_JUMPTABLE check clean; bindings multi-check 0): CMouseDX::
ConvertMouseEventToInputEvent 0x00AB5420 (740, three jump tables and three byte index tables inside the row), the libpk SSE2 / MMX gain mixers
CVtable_012b8a78::Process 0x00C6E410 and CVtable_012b8ab4::Process 0x00C6F990 (grade `retail_asm`: each TU repeats its landed retail_asm ramp kernel,
the chunk 691 / 693 precedent; Process itself is C++), DirectShow ctlutil.cpp CCmdQueue::GetCommandDueFor 0x00CAFA10 (/EHsc; the label said
CEditControlCentre::FindTrackNodeInTimeRange), CClassFactory::AddNewClassType 0x00A36F90 and AllocateClass 0x00A371C0, CCombatActionBase::FinishAction
0x00859A20, and three destructors the catalog had all labelled "CTCIdleScheduler::Clear": NUISystem::CChangingStateComponent::~ 0x0052CCF0,
NCameraModes::CScriptModuleBase::~ 0x005ED120 and the compiler-generated NCameraModes::CScriptModuleSelfTerminateBase::~ 0x00854310 (same bytes as
0x005ED120; named from its callers' RTTI). 5 labels applied (`ghidra_out/labels_chunk694_2026-10-27.tsv`); NAME_FIXES.tsv 6 rows queued c694 (the ??_G
callers 00854500 / 00854540 / 00549a00 / 0052d770 bind the wrong dtor spellings).

Pilot (decision, coordinator): the landed CLookAtMe1::Main 0x00EF5CB0 and CUndeadFellow::Main 0x00EF87D0 were re-authored without `#pragma
inline_depth` (13 probes each) and matched neither (706v817: VC7.1 does not inline ~CScriptThing at the outer-loop exit retail inlines; 1047v1003: the
functor's by-value CScriptThing copy is inline in retail where VC calls the copy ctor). The landed pragma sources stay. The chunk 693 CVillager shape
does not generalise, so no mass re-model of the 134 inline_depth rows is proposed; the question goes to the owner with this result. Pilot sources
in `../../pending/2026-10-27_linux_hand694/differ/`.

Note on verification: the agent's scratch grader first reported MATCH on 0x00AB5420 with two jump-table cases swapped (relocation masking hides it);
the landing verifier's RELOC_JUMPTABLE check (reloc_identity.py) compares every table entry with the retail label and would have refused it, so
landed rows are not exposed. Header debt: engine/CClassFactory.h / CCreatureActionBase.h / CObserver.h model the vtable pointer as a data member (local
models); no engine headers for NCameraModes::CScriptModuleBase / SelfTerminateBase or NUISystem::CComponent / CChangingStateComponent.
