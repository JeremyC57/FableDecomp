// $E_JACK_GLOW_START_FRAME() -- dynamic initializer of the anonymous-namespace long JACK_GLOW_START_FRAME of the Jack of Blades flash creature-mode TU: the total duration less the fade-down frames
// Leaf: _$E3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7A0, 17 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374684, plain `ret`: `mov eax,[0x013BAFD8] ; sub eax,[0x01388A80] ; mov [0x013BAFDC],eax`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x01011F20 (17 bytes, as retail) and FableWin.pdb 0x03FF7340 name
//   `anonymous namespace'::`dynamic initializer for 'JACK_GLOW_START_FRAME'', the second of JACK_TOTAL_DURATION /
//   JACK_GLOW_START_FRAME / JACK_FADE_DOWN_TIME; retail's second of the three is this 0x0121F7A0 (0x013BAFDC = JACK_TOTAL_DURATION
//   0x013BAFD8 - 0x01388A80) between 0x0121F780 (0x013BAFD8 = 0x01388A7C + 0x01388A80) and 0x0121F7C0 (0x013BAFE0 = 0x01388A80);
//   (2) NCreatureMode::CModeJackOfBladesFlash's activation 0x008D7F10 recomputes the same relation (`sub ecx,eax ; mov
//   [0x013BAFDC],ecx` after storing the total). Catalog row "_gapscan::sub_0121f7a0" carried no name.
// Model: JACK_TOTAL_DURATION is an anonymous-namespace object of the same retail TU (its own row is 0x0121F780), so it is
//   defined here, in retail order, before this one: VC7.1 then numbers the two initializers `_$E1` and `_$E3` (the even numbers
//   are reserved for atexit thunks), so this row's leaf is `_$E3`. Both objects are written at run time (above): not const.
// Data: 0x01388A7C / 0x01388A80 -- see 0x0121F780 (descriptive g_01388a7c / g_01388a80, the latter as the landed 0x0121F7C0 binds it).
extern long g_01388a7c;                                  // 0x01388A7C (descriptive; .data, initial 60)
extern long g_01388a80;                                  // 0x01388A80 (descriptive, as bound by the landed 0x0121F7C0; .data, initial 40)

namespace {
long JACK_TOTAL_DURATION = g_01388a7c + g_01388a80;      // 0x013BAFD8 (row 0x0121F780)
long JACK_GLOW_START_FRAME = JACK_TOTAL_DURATION - g_01388a80;   // 0x013BAFDC
}