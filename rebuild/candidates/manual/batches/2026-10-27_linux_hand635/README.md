# Chunk 635 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 635` (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0):
- NScript::CQ_FireHeartScript::CFH_Scythe::Main 0x00D3AE40 -- a named `CQ_FireHeartScript* parent = ParentClass;` local before the
  IsValid/Release pair, on the retail NScript headers (CEntityScriptBase, CGameScriptInterface, CScriptThing).
- GetActionAnimSetList 0x00693140 (anonymous namespace, `__fastcall`, the TU sibling of the landed 0x008F4990) -- the STLport
  `_M_insert_overflow` body (landed 0x0066C3F0) in the same TU, noinline, before the caller.
- C3DVector::GetRandomPointWithinUnitSphere 0x00A146C0 (static `__fastcall` member, not a global) -- a named `double result = fmod(...)`
  inside the inline GFFloatRandom; the landed GFFloatRandom 0x004978A0 still matches with that local.
At landing the sibling caller 0x008F4990 was respelled to the const CThingCreatureBase::IsAnimCombinationAnim (QBE) and the protected
STLport `_M_insert_overflow` (IAE) so 0x00662820 and 0x0066C3F0 keep one spelling per landed pair; rechecked RELOCATION_MATCH, both
binding rows replaced. Seven rows stay DIFFER in `pending/2026-10-27_linux_hand635/differ/` (00a17690 HasBoundingBoxIntersectedFromTo
one x87 operand swap per lane, 0099ba70 CWideString::Sprintf two scratch rotations, 00a65480 CMemoryAreaChunk::AllocAreaFromArea
register choice, 008c7280 TrollGroundPound::Initialise `range` slot +0xc vs +0x10, 00a65720 NewChunk ebp/edi, 00b84470
CEngineSubPrimitiveRenderer ctor grouped RenderPass stores, 0055c6f0 NUISystem::CMouseCursor::ProcessEvent hidden-result temporaries
packed into one slot -- placeholder virtual slots, no engine/CMouseCursor.h). Lessons: same-TU STLport COMDAT bodies stop a local
vector/out-param escaping; named double for fmod; `regionSize = 0;` at loop top + named protect local in the VirtualAlloc retry;
member vs friend operator+ flips every x87 lane at once; check rtti-vtables.tsv before trusting a vfunc_N label.
17 NAME_FIXES + 4 review rows queued (prefix c635): CWideString::Sprintf/SprintfDynamic 0x0099BA70/0x0099BB80, the nested
CMemoryAllocatorVariableSize::CMemoryAreaChunk/CMemoryArea family, GetHealthPercentage 0x006A4E40, CEngineSubPrimitiveRenderer ctor,
CMouseCursor::ProcessEvent, CEngineComponent ctor 0x00B59710 (manifest label CNavigationLayer wrong), g_CurrentAlignment(Shift),
CVectorMap::LowerBound 0x0040F020 sweep, GetSquaredMagnitude -> GetLengthSquared x8. Labels: ghidra_out/labels_chunk635_2026-10-27.tsv.
