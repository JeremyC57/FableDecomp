# Hand chunk 695 (NCameraModes::CScriptModuleGoToLookInDirection .. CAInputProcess): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH under q.py / gall.sh, the read-only vland run (`tmp/vland.log`: WINS 10, behaviour PASS, no RELOC_IDENTITY;
one WARN RELOC_NAME = the catalog label of 0x0051DFF0, fixed in NAME_FIXES.tsv), tests PASS under g++ (tall.sh) and VC7.1 (tall.py).
Call targets checked with calls.py (every DISP32 / dir32 lands on retail's target). Four of the ten catalog labels were wrong.

## What matched (10)
- **NCameraModes::CScriptModuleGoToLookInDirection::~ 0x005ED210** and **NCameraModes::CScriptModuleFOVZoom::~ 0x00854380** (34 each,
  byte twins): both are compiler-generated destructors of intermediate classes (no vtable store of their own; the only store is the inlined
  ~CScriptModuleBase's 0x01255F74). Identity from each copy's sole caller: 0x005ED420 = ??_G of CScriptModuleTavernGame (RTTI COL: TavernGame ->
  GoToLookInDirection -> Base), 0x008544C0 = ??_G of CScriptModuleFOVZoomStayRelative (destroys its own +0x84 member, then calls the base;
  COL: StayRelative -> FOVZoom -> Base). Two different symbols, so the "one symbol binds one address" rule needs no hold: both land. Shape: the
  chunk 694 recipe (implicit derived dtor, qualified explicit call emits it out of line).
- **CTCScriptedControl::CleanupAllActions 0x00713630** (40): STLport list `empty()` / `front()` over `CCountedPointer<CActionBase>`
  (engine/CCountedPointer.h `Get()`), `IsActive()` then virtual slot 5 `OnCleanup`, then `PActionList.clear()` (list::clear forwarding to
  the out-of-line `_List_base::clear` 0x00719280).
- **CTCScriptedControl::OnKill 0x00713700** (51): vtable 0x012658E4 slot 14 (CTCBase order). `CleanupAllActions(); CTCBase::OnKill();`
  with CleanupAllActions a plain member defined above it: VC7.1 expands it by itself.
- **CThingGameObject::~CThingGameObject 0x00838A00** (40): user dtor `GetThingManager().RemoveFromGameObjectList(*this);` (protected inline
  PDB accessor over the private static CThing::PCurrentThingManager 0x013B8A1C), then the PThingStandingOn member dtor and ~CThingPhysical.
- **CDataOutputStream::WriteString 0x009959F0** (46): the inline PDB accessors `GetAsCharArray()` / `GetLength()`. The data pointer must be a
  named local read before the call: written inline as the first argument it is evaluated after the length (right-to-left) and VC splits the
  tails on the length test (52v46). 4 probes.
- **CMemoryDataOutputStream::~CMemoryDataOutputStream 0x00996B30** (47): the label said ~CCompressedMemoryDataInputStream; the stored vtable
  0x0129A788 is CMemoryDataOutputStream's. Empty user dtor over an STLport `vector<char>` (CArray) member (`_Vector_base` dtor: null test +
  free), CDataOutputStream with no dtor (no 0x0129A6E0 store), CDataStream with an inline user `{}` (the 0x0129A69C store), tail to
  ~CBaseClassNonCopyable.
- **CSymbolMap::UpdateDirty 0x009E5C30** (39): the label said CTCInventoryQuests::ProcessButtonBReleased. `SLONGMap.Sort();
  SLONGMap.Data.DeallocateUnusedMemory();` on a `mutable` CVectorMap in a const member; plain inline Sort (the PDB has an out-of-line copy, so
  no __forceinline needed).
- **CTCInventoryQuests::ProcessButtonAReleased 0x0061B590** (93): slot 58. `if (ActiveTestQuestMode) { ActivateSelectedTestQuest();
  AddCloseInventoryEvent(); } else if (!IsInventoryScreen() && InventorySelectionState == 1 && ProcessQuestActivateAction())
  AddCloseInventoryEvent(); NInventory::CTCInventoryBase::ProcessButtonAReleased();` matched first time on a fully PDB-named
  CTCInventoryBase vtable (86 slots) and the retail member layout (CTCInventoryBase 0x130, NHeroInformationScreens::CBase at +0x130).
- **CAInputProcess::~CAInputProcess 0x00A0D3D0** (46): `RemoveFromInterface();` with the real RemoveFromInterface (0x00A0D2D0 body) a plain
  member above the dtor; its virtual `Cleanup()` binds to the just-stored vtable (`call [0x0129CA1C]`). RemoveInputProcess is protected and
  by reference in the PDB (CInputProcessManager declares CAInputProcess a friend).

## Parked for Fable 5.1 (0)
None.

## Skipped
None (every extent checked: each row ends in its `ret`/`jmp` with int3 padding after it; no extent fixes).

## Names and evidence
- Row names: see each source's Identity block; summary: 0x005ED210 / 0x00854380 from the slot-0 ??_G callers' RTTI COL + Ego_r name set;
  0x00713630 Ego_r/FableWin + landed callers; 0x00713700 vtable slot 14 + PDB; 0x00838A00 / 0x00996B30 / 0x00A0D3D0 the stored vtable + PDB;
  0x009959F0 PDB + donor order = retail order; 0x009E5C30 PDB size ranking in both PDBs + FableWin order; 0x0061B590 vtable slot 58 + donor
  order.
- Virtual slot names: CActionBase (vtable 0x01265504): 1 Update (pure; CActionWait::Update 0x00712240), 2 OnActivate, 3 OnAdd, 4 GetActionName
  (pure; CActionWait 0x00903640), 5 OnCleanup. CDataStream / CDataOutputStream: 1 SetPosition, 2 GetPosition, 3 Close, 4 Write (from the
  CMemoryDataOutputStream overrides in 0x0129A788). CAInputProcess: 1 AddToInterface, 2 Init, 3 Cleanup. NInventory::CTCInventoryBase 28..85:
  aligned base bodies (packed sections 0x004E7D3B.., 0x0057A230.., 0x005BC4C0..0x005C0121) against the Ego_r / FableWin donor order and
  each body's first `ret N` (e.g. 81 `ret 18h` = AddItemToInventory(long,long,bool,bool,long,bool), 80 `ret 14h` = the CThing& overload, 73/74
  `ret` = Return/RemoveAllConfiscatedItems, 85 `ret 4` = ConfiscateItems); the CTCInventoryQuests overrides confirm 14, 38-43, 49, 61, 67,
  70-72, 75, 76. The order of the two AddItemToInventory overloads (80/81) is not byte-relevant (neither is called). NHeroInformationScreens::
  CBase: 0 GetScreenName, 1 GetPlayerMode (Quests returns 0xD), 2 GetScreenType (returns 1), 3 IsInventoryScreen (base `mov al,1`).
- Callee names: see NAME_FIXES.tsv (22 rows) -- every callee in these sources uses the PDB spelling the compiler emits.
- Descriptive: none. Unproven parameter names only (ctor declarations of GoToLookInDirection / FOVZoom carry the PDB types).
- EInventorySelectionState: no enumerator names exist in either PDB dump; the compare is written `InventorySelectionState == 1`.

## Label corrections
- 0x005ED210, 0x00854380: "CTCIdleScheduler::Clear" (propagated) -> ~CScriptModuleGoToLookInDirection / ~CScriptModuleFOVZoom.
- 0x00996B30: "~CCompressedMemoryDataInputStream" -> CMemoryDataOutputStream::~CMemoryDataOutputStream.
- 0x009E5C30: "CTCInventoryQuests::ProcessButtonBReleased" -> CSymbolMap::UpdateDirty (and the landed 0x009E5C60 "UpdateDirty" is
  UpdateDirtyQuick: coordinator decision below).
- Neighbours (NAME_FIXES.tsv): 0x0051DF60/70/E0/F0 (Nop_ret4 labels) are CThingManager Add/Remove...List; 0x006A5D50 (manifest
  ~CTCFishingSpot) is ~CThingPhysical; 0x00713740 (OnRemove / vfunc_13) is OnDie; 0x007196B0 (manifest CActiveFile::OnReadFinished) is
  ~_List_base of the action list; 0x009E5350 RemoveUnusedMemory is DeallocateUnusedMemory; 0x0061B5F0 is an unnamed __fastcall stand-in.

## Header debt
- engine/CTCBase.h, CThingPhysical.h, CThingGameObject.h, CDataOutputStream.h, CMemoryDataOutputStream.h, CAInputProcess.h flatten the vptr:
  each row uses a local polymorphic model sized from the header (`#define X XFields` include pattern + a sizeof check where the size is
  known: CThingPhysical 0xC0, CMemoryDataOutputStream 0x18, CAInputProcess 0x14).
- No engine headers for NCameraModes::CScriptModule*, CTCScriptedControl (+CActionBase), CSymbolMap, NInventory::CTCInventoryBase,
  CTCInventoryQuests, NHeroInformationScreens::CBase. The CTCInventoryBase model in 0x0061B590 (86 named slots, 0x130 retail layout) is a
  candidate for a generated header.
- engine/CThingGameObject.h gives sizeof 0xD0 but the members end at 0xC9 (Ego_r 232 with 8 bytes after the bitfield byte): unexplained
  tail; the source models only PThingStandingOn + the bitfield byte (no size claim).

## Decisions for the coordinator
- 0x009E5C30 takes `?UpdateDirty@CSymbolMap@@QBEXXZ`, which the landed 0x009E5C60 currently carries: respell 0x009E5C60 to
  `?UpdateDirtyQuick@CSymbolMap@@QBEXXZ` in the same landing (its caller 0x009E2590 binds the old spelling), or vland will see one symbol
  on two addresses.
- 0x007196B0's symbol in NAME_FIXES.tsv is hand-mangled (no compiler confirmation in this chunk).
