# Hand chunk 694 (CMouseDX .. NScript): 10 of 12 byte-exact

All ten new rows are RELOCATION_MATCH (none plain MATCH, because each has call or data relocations). The two PILOT rows did not match without
the pragma; they are parked in differ/ as a pilot RESULT, and their landed sources stay as they are.

## What matched (10)
- **CMouseDX::ConvertMouseEventToInputEvent 0x00AB5420** (740 incl. 3 jump tables + 3 byte index tables; ends 0x00AB5703).
  - Does: three switches.
    - Map a release type to the held input type: 4->5, 6->8, 5->0xB, 20..24->type+7.
    - Find the maintained event of that type and take its StartTime.
    - Dispatch to the CInputEvent setters at Pos (+0x3414).
  - Shape:
    - Case bodies in retail source order (Left, Middle, Right, Extra).
    - The jump tables are verified entry by entry. The first relocation-masked MATCH had 5/6 swapped.
- **libpk CVtable_012b8a78::Process 0x00C6E410 / CVtable_012b8ab4::Process 0x00C6F990** (533 each; retail_asm).
  - Does: vtable slot 2 (0x012B8A78 / 0x012B8AB4 + 8) of the SSE2 / MMX four-record gain mixer.
  - Shape:
    - The same-TU `static __declspec(noinline)` asm kernels (ecx convention).
    - The wrappers 0x00C6E380 / 3B0 / 3E0 and 0x00C6F900 / 930 / 960 are written above Process.
    - /GS.
  - Evidence lines: `0x00C6E334 pmulhw xmm0,xmm7` and `0x00C6F863 pmulhw mm0,mm7`.
- **CCmdQueue::GetCommandDueFor 0x00CAFA10** (395): the DirectShow ctlutil.cpp text verbatim, under /EHsc (CAutoLock).
- **CClassFactory::AddNewClassType 0x00A36F90** (47): uses one element-reference local; ends in a tail jmp to CCharString::operator= (returns const&).
- **CClassFactory::AllocateClass 0x00A371C0** (42): `ClassTypeInfo[type].AllocFunc` is indexed twice and is not held in a local.
- **CCombatActionBase::FinishAction 0x00859A20** (72): IS_THING_ALIVE inline __fastcall template, RemoveTC(0x100), ResetMeleeCombatAbilityEffects, base FinishAction.
- **NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0** (42): user inline virtual dtor (stores 3 vtables); the StatesToDo `std::list` `_List_base` dtor, then ~CComponent.
- **NCameraModes::CScriptModuleBase::~CScriptModuleBase 0x005ED120** (34): user inline virtual dtor, `POwner = 0; PCameraManager = 0;`, then the two intelligent-pointer member dtors.
- **NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase 0x00854310** (34): the IMPLICIT dtor of the intermediate class (no user `~`), so it holds only the inlined base body.

## Parked for Fable 5.1 (2) -- PILOT rows (landed RELOCATION_MATCH with `#pragma inline_depth`; keep them)
- 00ef5cb0 NScript::CPersonalScript_EmptyScript::CLookAtMe1::Main DIFFER(706v817): VC7.1 expands the plain inline ~CScriptThing at the first return and the inner wait-loop return only; retail also expands it at the outer-loop exit 0x00EF5EDF (crate + WalkToHere) and calls 0x004AA840 only after the wait (0x00EF5FC8) | tried: CVillager header set as is, outer while(!Term) loop, non-virtual predicate dtor, dtor body after Main, __forceinline, /Ob1, direct CPredicate_And, QFE-4035, nested block scopes, if(!Term){} first exit (13 probes) | next: none without a device -- keep the landed pragma source
- 00ef87d0 NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main DIFFER(1047v1003): the exits match; the functor's by-value CScriptThing argument differs -- retail copies it inline and calls the dtor 0x004AA840, we call the copy ctor 0x004ABE90 and inline the dtor (an inline copy ctor instead gives 1100v1003, expanded everywhere) | tried: inline / out-of-line copy ctor, plain-inline CSpawnedFunc1 ctor specialisation, named functor local, /Ob1, for(;;) final loop, explicit argument temporary, implicit "" conversion, QFE-4035, definition order (13 probes) | next: lane gap: per-site inline split of one copy ctor (the PDBs show only the const copy ctor ??0CScriptThing@@QAE@ABV0@@Z)

Pilot verdict: the pragma-free form matched NEITHER row. The CVillager rule (chunk 693, "inline at the first return, call at every later exit") is
specific to that row's exit layout. Do not propose a mass re-model of the 134 inline_depth rows without testing each row.

## Skipped
None.

## Names and evidence
- 0x00AB5420 ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z
  - Ego_r donor order (GetDeviceMouseEvent / Update).
  - Its one caller 0x00AB59B7 is in CMouseDX (vtable 0x0129EA14, RTTI CMouseDX : CMouse).
  - The callees match the Ego_r CInputEvent setter shapes.
- 0x00C6E410 / 0x00C6F990: class names CVtable_012b8a78 / CVtable_012b8ab4 and the wrapper names are **descriptive**. They follow the family convention of chunks 691/693; nothing proves a retail name.
- 0x00CAFA10 ?GetCommandDueFor@CCmdQueue@@UAEJ_JPAPAVCDeferredCommand@@@Z
  - The SDK source text byte-matches.
  - The landed CCmdQueue neighbours GetDueCommand 0x00CAF8F0 and CPosPassThru::QueryInterface 0x00CAFC40.
- 0x00A36F90 / 0x00A371C0 / 0x00A37060: Ego_r CClassFactory name set (AddNewClassType MAE, AllocateClass QAE, GetClassNumberFromName IBE), plus the landed neighbours 0x00A36FC0 / 0x00A17590.
- 0x00859A20 ?FinishAction@CCombatActionBase@@MAEXXZ
  - The catalog label and Ego_r agree.
  - The base call 0x00694430 is ?FinishAction@CCreatureActionBase@@UAEXXZ; 0x00859660 is ?ResetMeleeCombatAbilityEffects@CCombatActionBase@@IAEXXZ.
  - The RemoveTC enumerator 0x100 is unnamed.
- 0x0052CCF0, 0x005ED120, 0x00854310: each named from the vtable it stores plus its callers' RTTI base-class arrays. See NAME_FIXES.tsv.

## Label corrections
Every proven wrong spelling, with its file, is in NAME_FIXES.tsv. The catalog labels below are wrong:
- 0x0052CCF0, 0x005ED120, 0x00854310: "CTCIdleScheduler::Clear" (propagated across identical bodies).
- 0x00CAFA10: "CEditControlCentre::FindTrackNodeInTimeRange".
- 0x0052D5C0: "SortTreeRecursively".

## Header debt
- engine/CClassFactory.h flattens the vptr into a data member, so 0x00A36F90 uses a local model. Retail CClassTypeInfo is 0x14 (Used +8, Local +9, AllocFunc +0xC, Name +0x10). The AllocFunc type is `CClass* (__fastcall*)()` (FableWin CClassTypeInit ctor mangling P6IPAVCClass@@XZ).
- engine/CCreatureActionBase.h flattens the vptr. The retail base is 0xA8 bytes, so CCombatActionBase::PTarget is at +0xA8 (Ego_r +0xB4).
- engine/CMouse.h: the MaintainedEventsList is a one-word STLport list; CMouseDX::Pos is at +0x3414 (Ego_r +0x341C, from the Dinkumware list size).
- No engine headers exist for NCameraModes::CScriptModuleBase / SelfTerminateBase or for NUISystem::CComponent / CChangingStateComponent (retail CComponent is 0x134).
- vland WARN SHARED_HEADER_AVAILABLE CObserver at 0x0052CCF0: engine/CObserver.h has the right size (0x14). It flattens the vptr into `__vftable`, though, so it cannot be the polymorphic second base of CComponent. The local model stays until the header is regenerated with a real vptr.
- vland WARN SHARED_HEADER_AVAILABLE CBaseObject at 0x00CAFA10 is a false positive: that class is DirectShow strmbase's CBaseObject (the base of the CCmdQueue/CBaseList models), not the engine's CBaseObject.
