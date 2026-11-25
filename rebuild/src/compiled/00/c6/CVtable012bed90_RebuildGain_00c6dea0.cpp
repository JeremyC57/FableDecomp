// CSalRenderPan::UpdateFilters(const bool bNew) -- under the update lock record bNew in m_bUpdate and rebuild the new (bNew) or the current
//   parameter set's FilterPan 0 / 1 (and 2 / 3 with four or more output channels) from m_fGainPan and m_fGain (Params3DInit)
// Leaf: ?UpdateFilters@CSalRenderPan@@AAEX_N@Z
// Option S: thin row of the 0x00C6DE60 owner file (retail SalRenderPan.obj slice: Params3DInit, UpdateFilters). Graded from the owner's
//   object; no code of its own. Edits go to the owner file.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DEA0, 369 bytes (+ int3 padding), thiscall `ret 4`, one dword frame word.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderPan.obj CodeView class / method record + CODAApi.lib Release/SalRenderPan.obj body): private void UpdateFilters(const bool); (2) the Release body ties exactly (111 instructions /
//   `ret 4` = retail 111 / `ret 4`), and the Release SetGain / SetPosition call it where retail 0x00C6E020 / 0x00C6E050 call 0x00C6DEA0.
//   Was ?RebuildGain@CVtable_012bed90@@QAEX_N@Z (descriptive).
#include "../src/compiled/00/c6/CVtable012bed90_ComputeGainRamp_00c6de60.cpp"
