// CSalRenderDisable::UpdateFilters(const bool bNew) -- under the update lock record bNew in m_bUpdate and rebuild the new (bNew) or the current
//   parameter set's FilterPan from the renderer gain (Params3DInit expanded)
// Leaf: ?UpdateFilters@CSalRenderDisable@@AAEX_N@Z
// Option S: thin row of the 0x00C6EBA0 owner file (retail SalRenderDisable.obj slice: Params3DInit, UpdateFilters). Graded from the owner's
//   object; no code of its own. Edits go to the owner file.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBE0, 106 bytes (+ int3 padding), thiscall `ret 4`, one dword frame word.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView class / method record + CODAApi.lib Release/SalRenderDisable.obj body): private void UpdateFilters(const bool); (2) the Release body ties exactly (35 instructions / `ret 4` =
//   retail 35 / `ret 4`), and the Release SetGain / SetPosition call it where retail 0x00C6EC6F / 0x00C6ECA7 call 0x00C6EBE0.
//   Was ?RebuildGain@CVtable_012bedb8@@QAEX_N@Z (descriptive; vendor naming V2).
#include "../src/compiled/00/c6/CVtable012bedb8_ComputeGainRamp_00c6eba0.cpp"
