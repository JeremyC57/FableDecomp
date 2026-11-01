# Chunk 695 (NA038) -- 10 of 10 byte-exact; two catalog destructors and one "inventory" row renamed; CSymbolMap::UpdateDirty / UpdateDirtyQuick settled (2026-11-01)

All ten rows landed (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; bindings multi-check 0), none needed `__asm`, `volatile`, `__forceinline` or
`#pragma inline_depth`: the compiler-generated NCameraModes::CScriptModuleGoToLookInDirection::~ 0x005ED210 and CScriptModuleFOVZoom::~ 0x00854380
(byte twins, but two symbols: each named from its only caller, the derived class's scalar deleting dtor, and the RTTI base chain -- both were
"CTCIdleScheduler::Clear"), CTCScriptedControl::CleanupAllActions 0x00713630 and OnKill 0x00713700 (vtable 0x012658E4 slot 14),
~CThingGameObject 0x00838A00, CDataOutputStream::WriteString 0x009959F0 (the data pointer is read before the length: a named local), the
CMemoryDataOutputStream::~ 0x00996B30 (vtable 0x0129A788; was "~CCompressedMemoryDataInputStream"), CSymbolMap::UpdateDirty 0x009E5C30 (was
"CTCInventoryQuests::ProcessButtonBReleased"), CTCInventoryQuests::ProcessButtonAReleased 0x0061B590 (vtable 0x012585B4 slot 58, on a CTCInventoryBase
model with all 86 slots named) and ~CAInputProcess 0x00A0D3D0.

UpdateDirty vs UpdateDirtyQuick (coordinator check): 0x009E5C60 only sorts (33 bytes, the same body as the landed CCRCSymbolMap::UpdateDirtyQuick
0x00A38FE0), 0x009E5C30 sorts then runs DeallocateUnusedMemory, and FableWin orders UpdateDirty before UpdateDirtyQuick as retail does. The landed
0x009E5C60 (was ?UpdateDirty) and its caller TransferIn 0x009E2590 were respelled to ?UpdateDirtyQuick@CSymbolMap@@QBEXXZ in this landing so one
symbol keeps one address; both rechecked RELOCATION_MATCH + PASS (`recheck_009e5c60_009e2590.log`). The binders 0x009CC530 / 0x009AEF60 still spell
0x009E5C30 as an invented ?Finalise@CSymbolMap / ?Finalise@CSymbolTable (neither PDB has a CSymbolMap::Finalise): queued c695 for naming wave 27.

22 labels applied (`ghidra_out/labels_chunk695_2026-10-27.tsv`); NAME_FIXES.tsv 22 rows queued c695 (CThingManager::AddTo / RemoveFrom
PhysicalList / GameObjectList were "Nop_ret4"; ~CThingPhysical 0x006A5D50; CTCScriptedControl::OnDie 0x00713740; the STLport _List_base clear /
dtor 0x00719280 / 0x007196B0 -- the dtor spelling is hand-mangled, confirm from the compiler; CInputProcessManager::RemoveInputProcess protected,
by reference; CSymbolMap::DeallocateUnusedMemory 0x009E5350). Header debt (NOTES.md): engine/CTCBase.h, CThingPhysical.h, CThingGameObject.h,
CDataOutputStream.h, CMemoryDataOutputStream.h, CAInputProcess.h flatten the vptr (local polymorphic models with sizeof checks); no headers for
NCameraModes::CScriptModule*, CTCScriptedControl, CSymbolMap, NInventory::CTCInventoryBase, CTCInventoryQuests; engine/CThingGameObject.h claims
0xD0 with members ending at 0xC9.
