# Hand chunk 494 (CVtable_012b2f88 .. CVtable_012b4fec): 40 of 40 byte-exact

17 MATCH and 23 RELOCATION_MATCH. There are no retail_asm rows, no parked rows and no skipped rows. All 40 tests print
`<addr>_TEST PASS` under both g++ (tall.sh) and VC7.1 (tall.py). The vland dry run gives 40 WINS and no RELOC_IDENTITY;
its remaining warnings are RELOC_NAME label noise on callees (see "Callee names") and one SHARED_HEADER_AVAILABLE on the
libpk CHandleBasedMemoryPool (see "Header debt"). Every `// Leaf:` was checked against `objdump -t` of its object.

All rows are libpk (0x00C19E90..0x00C34C80): no PDB or RTTI. Most are slots of non-RTTI vtables, named by the vtable
(`CVtable_<vtable>`) unless a landed class already owns the vtable or the address.

## What matched (40)
- **Comparator vtables** (the libpk container comparator family of the landed CVtable_012b28d8 rows):
  - CVtable_012b4340 (also 0x012B4460) Equal / Less / EqualToKey / LessThanKey (0x00C2B750 / 0x00C2B780 / 0x00C2B7B0 /
    0x00C2B7E0) on a float key at +4. `==` gives `fucompp ; test ah,44h`, `<` gives `fcomp ; test ah,5`; int results.
  - CVtable_012b4788 (also 0x012B47D0) Equal / Less / EqualToKey / LessThanKey (0x00C2DF40 / 0x00C2DF60 / 0x00C2DF80 /
    0x00C2DFA0) on the CString at +8, through MFC's inline `operator==` / `operator<` over CString::Compare 0x00C0BCE0.
    The bool of `== 0` is `neg ; sbb al,al ; inc al`, then `movzx` to the int result.
- **Visitor functors**: CVtable_012b3840 / CVtable_012b3848::Visit (0x00C22D90 / 0x00C22DD0, find by the key at
  +0x20 / +0x40); CVtable_012b3858::Visit (0x00C21F00) calls `ref->~CLifetimeRef_00c26060()` explicitly.
- **CVtable_012b2f88::Compare** (0x00C19E90): the second key word must be a named local read straight after its call. The
  first word stays inline (see LESSONS).
- **PKCDefaultFileManager::ThreadProc** (0x00C1AC30): the static `__stdcall` the landed constructor declares.
- **CVtable_012b3224::Create** (0x00C1B8B0): `/EHsc`. `new CVtable_012b4f60(this, object)`, with the allocation's null test
  merged into the shared `return 0`.
- **CChickenSign::GetByteD** (0x00C1BAB0): an unsigned char member returned as unsigned short (`movzx ax`).
- **CScriptModuleTimer::DispatchTimes** (0x00C1D950): slot 2 of 0x012B32A0. The dispatcher is a one-word local copy of
  the target. `? 0 : -1` gives `neg ; sbb ; neg ; dec`.
- **CVtable_012b33a4::CopyData** (0x00C1E090) and **CVtable_012b4e08::Read** (0x00C31EB0): the inline `memcpy` intrinsic.
- **Reference count word +0x1E**: CScriptOwnerThing::IsUnreferenced (0x00C1EA20, bool `== 0`) and
  CParticle::IncrementLifetimer (0x00C1EA30).
- **CSoundEvent** (vtable 0x012B3674):
  - GetMode (0x00C20EE0).
  - ApplyPan, slot 7 (0x00C21360).
  - GetGain, slot 9 (0x00C21380).
  - GetMode and ApplyPan keep `this` in ecx across the call because CheckState (and GetMode) are compiled above them in the same TU.
- **PKCTimerManager::ThreadMain** (0x00C259E0): `while (!m_bShutdown) { tick; Sleep(period); }`, with the period computed
  once into a local (`_ftol2`).
- **CPooledResource** (vtable 0x012B3FDC): IsActive (0x00C265A0, a long `==`), SetRecord54 (0x00C26650, a 16-byte POD
  assignment), SetStateValues (0x00C26680).
- **CVtable_012b401c / CVtable_012b4030::PostRequest** (0x00C26A60 / 0x00C26EC0): an unsigned char flag stored as
  `flag != 0` into a short / bool member, then the worker queue push.
- **CHandleBasedMemoryPool** (the libpk copy):
  - ValidateLargestFreeArea (0x00C280E0): VC7.1 keeps the dead walk.
  - MoveResource (0x00C28220): memmove through the import.
  - GetSizeFromHandle (0x00C283A0): Dinkumware `vector::operator[]` = `*(begin() + _Pos)` gives retail's dead `lea`.
- **CVtable_012b4178::Invoke** (0x00C2A470): a byte member widened for a dword argument.
- **CAudioFormat::GetLoopEnd** (0x00C2D910): `m_loopEnd ? m_loopEnd : m_length`.
- **CVtable_012b4f4c** (the libpk file handle):
  - GetSize (0x00C32F40).
  - Read (0x00C32F60): six arguments. Four pass-through pushes go before the no-argument slot-2 call.
- **List vtables 0x012B4FBC / 0x012B4FEC**:
  - InsertAt (0x00C34A10, 0x00C34C40): the second inlines CPtrArray::Add.
  - IsNotEmpty (0x00C34A50): unsigned `> 0` as `cmp ecx,eax ; sbb ; neg`.
  - RemoveAt (0x00C34C80).

## Parked for Fable 5.1 (0)

## Skipped
None. Every row is a single function. Rows that start a vtable were checked against the vtable stores, not the slot
before them.

## Label corrections
- 0x00C1AC30 "FileManager_AsyncReadThreadWrapper" is `PKCDefaultFileManager::ThreadProc`.
  - Evidence: the declaration in the landed 0x00C1AC40 / 0x00C1A230, and the CreateThread `push offset`.
  - The reloc binding ?FileManagerThread_00c1ac30@@YGKPAX@Z is stale.
- 0x00C1BAB0 "CTCInventoryBase::GetCanItemBeQuickAssigned" is `CChickenSign::GetByteD() const`. Evidence: the binding from
  the landed CMaskedChickenSign rows.
- 0x00C1EA20 "CManager::GetKeyPressed" is `CScriptOwnerThing::IsUnreferenced() const`. Evidence: the binding from the
  landed 0x00C0EC90.
- 0x00C20EE0 "CCombatActionBase::GetAnimationTransitionInTime" is `CSoundEvent::GetMode()`.
  - The reloc binding ?GetValue48@CPlayer@@QBEJXZ is a propagated game name.
  - Every caller is a CSoundEvent member.
- 0x00C265A0 "CManager::GetKeyPressed" is `CPooledResource::IsActive()`. Evidence: the binding from the landed 0x00C265B0.
- 0x00C2D910 "CTCWeapon::SetAiming" is `CAudioFormat::GetLoopEnd()`. Evidence: its callers subtract the loop start
  0x00C2D900.
- 0x00C1EA40 is landed as `NParticleEngine::CParticle::IncrementLifetimer`, but it DECREMENTS the word. It has four bound
  spellings: DecRef, DecrementLifetimer, Release and the landed name.
- The 33 `_gapscan::sub_*` rows are named descriptively, as listed below.

## Header debt
- engine/CHandleBasedMemoryPool.h has the engine (Ego_r) layout. The libpk copy keeps its free lists at +0x3C and a
  Dinkumware handle vector at +0x43C (_Myfirst +0x440). 0x00C280E0, 0x00C28220 and 0x00C283A0 therefore model it
  locally (`// Layout:` line).
- engine/CManager.h and engine/CTCWeapon.h (named in the brief) are not used. Their rows (0x00C1EA20, 0x00C265A0,
  0x00C2D910) only carried propagated labels and are not those classes.

## Callee names
- Reused bound or landed spellings:
  - ?CheckState@CSoundEvent@@QAEXJ@Z 0x00C20720, given a same-TU stand-in body. The address has 7 spellings.
  - ?Set2DPosition@CSoundEvent@@QAEXM@Z 0x00C20EB0.
  - ?GetLevel@CSoundEvent@@QAEXPAUCSoundEventManager@@PAUSEventLevel@@@Z 0x00C20BE0.
  - CScriptModuleTimer::GetCurrentFrame / GetRemainingTimeSeconds / GetTotalTimeSeconds 0x00C1D810 / 0x00C1D820 /
    0x00C1D850.
  - ?InitializeAndDispatch@CAIStateGroup_00c1df70@@QAE_NPAX000@Z 0x00C1DF70.
  - ??0CVtable_012b4f60@@QAE@PAX0@Z 0x00C330A0.
  - ??1CLifetimeRef_00c26060@@QAE@XZ 0x00C26060.
  - ?Compare@CString@@QBEHPBD@Z 0x00C0BCE0.
  - ?GetFileSize@CFileManager_00c1a070@@QAE_NKPAK@Z 0x00C1A070.
  - State::Set2_00c20480 / Set3_00c204a0 0x00C20480 / 0x00C204A0. These are bound spellings. The catalog labels them
    CBrainOutput::SetAsPumpCombatActions, a propagated name.
  - ?Grow@?$CPtrArray@PAUCBlendEntry@@@@QAEXK@Z 0x00C1E640.
  - ?Remove@?$CPtrArray@PAUCBlendEntry@@@@QAEXPAUCBlendEntry@@@Z 0x00C1E900.
  - Imports: Sleep 0x0143FE1C, memmove 0x01440144, operator new 0x00BFEA1A.
- New descriptive spellings for unlanded callees:
  - PKCDefaultFileManager::AsyncReadThread 0x00C1A9C0.
  - CFileManager_00c1a070::QueueRead (6 args) 0x00C1A110.
  - CWorkerQueue_00c26cd0::Push 0x00C26CD0.
  - CWorkerQueue_00c27130::Push 0x00C27130.
  - CUnprovenTarget_00c371c0::Update(long) 0x00C371C0.
  - CPtrArray<CUnprovenItem_012b4fbc*>::Add 0x00C42B50.
  - CPtrArray<CUnprovenItem_012b4fbc*>::GetCount 0x00C42AC0. The landed candidate 0x00C34A40 spells this one
    CBaseIntelligentPointer::GetPItem_00c42ac0.

## Names
### Proven
0x00C280E0
0x00C28220
0x00C283A0

### Probable
0x00C1AC30
0x00C259E0

### Descriptive
0x00C19E90
0x00C1B8B0
0x00C1BAB0
0x00C1D950
0x00C1E090
0x00C1EA20
0x00C1EA30
0x00C20EE0
0x00C21360
0x00C21380
0x00C21F00
0x00C22D90
0x00C22DD0
0x00C265A0
0x00C26650
0x00C26680
0x00C26A60
0x00C26EC0
0x00C2A470
0x00C2B750
0x00C2B780
0x00C2B7B0
0x00C2B7E0
0x00C2D910
0x00C2DF40
0x00C2DF60
0x00C2DF80
0x00C2DFA0
0x00C31EB0
0x00C32F40
0x00C32F60
0x00C34A10
0x00C34A50
0x00C34C40
0x00C34C80

### Evidence for the proven and probable names
- **0x00C280E0 / 0x00C28220 / 0x00C283A0** come from the Ego_r.pdb CHandleBasedMemoryPool name set:
  - ValidateLargestFreeArea: AAEXXZ. The donor order is FindLargest, UpdateLargest, ValidateLargest, TestIfAreaIsGrowable,
    which matches the landed libpk neighbours 0x00C28050 / 0x00C28080 / 0x00C28120.
  - MoveResource: MAEXPAXJ00K. It has the same body as the engine's 0x00A02120.
  - GetSizeFromHandle: QAEKJ. It is the same accessor as the engine's 0x00A026D0, and the landed libpk Lock / Unlock
    0x00C283C0 / 0x00C283E0 follow it.
  - The class is the libpk copy of the engine pool.
- **0x00C1AC30**: the class comes from the ".\PKCDefaultFileManager.cpp" strings. The method name is the landed declaration.
- **0x00C259E0**: the class comes from the ".\PKCTimerManager.cpp" strings. The constructor 0x00C25B10 calls the vtable
  0x012B3FA0 constructor on `this`. The method name is descriptive.
