// CSalRenderPanDelay::UpdateFilters(const bool bNew) -- under m_mutexUpdate record whether a new parameter set is pending and rebuild that set
//   from the pan gains and delays (filters 2 and 3 only with four or more output channels)
// Leaf: ?UpdateFilters@CSalRenderPanDelay@@AAEX_N@Z
// Option S: thin row of unit 0x00C6CEF0 (retail SalRenderPanDelay.obj slice). The body is in the owner file, after Params3DInit, which VC7.1
//   expands at the first two sites of each branch and calls out of line at the rear two, as retail. Graded from the owner's object; no code of
//   its own. Name, evidence and retail-asm notes: the owner row 0x00C6CEF0.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CF40, 470 bytes, thiscall `ret 4`; called from SetGain 0x00C6D13C and SetPosition 0x00C6D341 only.
// Was ?RebuildGain@CVtable_012bed7c@@QAEX_N@Z (descriptive stand-in).
#include "../src/compiled/00/c6/CVtable012bed7c_ComputeTapGain_00c6cef0.cpp"
