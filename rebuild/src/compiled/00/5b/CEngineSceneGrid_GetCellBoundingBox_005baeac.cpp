// CEngineSceneGrid::GetCellBoundingBox @ 005baeac
// Thin forwarder: builds a 1-byte stack local and calls callee(a, b, &local),
// returning the callee's result. Retail keeps the EBP frame and pushes the
// argument stack slots directly (size-optimized codegen), so we disable
// frame-pointer omission and select size optimization to reproduce it.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)

extern "C" char __cdecl GetCellBoundingBox_callee_005ba9f9(unsigned long a, unsigned long b, void* out);

char GetCellBoundingBox(unsigned long a, unsigned long b)
{
    char local;
    return GetCellBoundingBox_callee_005ba9f9(a, b, &local);
}