// $E_JACK_TOTAL_DURATION() -- dynamic initializer of the anonymous-namespace long JACK_TOTAL_DURATION of the Jack of Blades flash creature-mode TU: the glow and fade-down frame counts added
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F780, 20 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374680, plain `ret`: `mov eax,[0x01388A7C] ; mov ecx,[0x01388A80] ; add ecx,eax ; mov [0x013BAFD8],ecx`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x01011F00 / 0x01011F20 / 0x01011F40 and FableWin.pdb 0x03FF7320 / 0x03FF7340 /
//   0x03FF7360 run the anonymous-namespace initializers JACK_TOTAL_DURATION, JACK_GLOW_START_FRAME, JACK_FADE_DOWN_TIME, then the
//   three CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_* statics; retail runs 0x0121F780 (0x013BAFD8 = A + B),
//   0x0121F7A0 (0x013BAFDC = 0x013BAFD8 - B), 0x0121F7C0 (0x013BAFE0 = B), then fps * 6 / * 12 / * 3 -- the names fit the algebra
//   (total = glow + fade-down, glow start = total - fade-down, fade-down time = B) and Ego_r's GLOW / FADE copies are 17 / 11 bytes
//   like retail's; (2) NCreatureMode::CModeJackOfBladesFlash's activation 0x008D7F10 rewrites all of them from its def at run time
//   (`mov [0x01388A7C],edi` = fps * def+0x6C, `mov [0x01388A80],eax` = fps * def+0x70, then `lea ecx,[eax+edi] ; mov
//   [0x013BAFD8],ecx ; sub ecx,eax ; mov [0x013BAFDC],ecx ; mov [0x013BAFE0],eax`), the same three relations -- so the objects
//   are not const, and the anonymous namespace makes that activation code part of this TU.
//   Catalog row "_gapscan::sub_0121f780" carried no name.
// Data: 0x01388A7C / 0x01388A80 are two statically initialised longs of the same mode code (.data, 60 and 40; 0x01388A84 = 10
//   is their third neighbour), rewritten by the activation above. No PDB names them (no dynamic initializer): descriptive
//   g_01388a7c (new) and g_01388a80 (the spelling the landed 0x0121F7C0 already binds: one address, one name). Declared
//   extern because their real (possibly anonymous-namespace) spelling is unproven.
// Shape: the two-load add (`mov eax ; mov ecx ; add ecx,eax`) is what VC7.1 emits for a sum of two extern longs stored to a third.
extern long g_01388a7c;                                  // 0x01388A7C (descriptive; .data, initial 60)
extern long g_01388a80;                                  // 0x01388A80 (descriptive, as bound by the landed 0x0121F7C0; .data, initial 40)

namespace {
long JACK_TOTAL_DURATION = g_01388a7c + g_01388a80;      // 0x013BAFD8
}