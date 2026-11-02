// $E_MIN_TIME_TO_SIT() -- dynamic initializer of the anonymous-namespace long MIN_TIME_TO_SIT of the seat state-group TU: twenty seconds in frames (CMainGameComponent::GetConstantFPS() * 20)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221B60, 17 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x0137470C, plain `ret`: `mov eax,[ConstantFPS] ; lea eax,[eax+eax*4] ; shl eax,2 ; mov [0x013BB6F4],eax`.
// Identity (proven, two sources): Ego_r.pdb 0x010174D0..0x01017650 and FableWin.pdb 0x04000050..0x040001D0 run, in one TU, the
//   anonymous-namespace initializers SIT_ON_CHAIR_ANIM_NAME, GET_UP_FROM_CHAIR_ANIM_NAME, SIT_ON_BENCH_ANIM_NAME,
//   GET_UP_FROM_BENCH_ANIM_NAME, SIT_ON_FLOOR_ANIM_NAME, GET_UP_FROM_FLOOR_ANIM_NAME, MIN_TIME_TO_SIT, RANGE_TIME_TO_SIT,
//   MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS, RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS, then the next TU's SIT_ON_FLOOR / GET_UP_FROM_FLOOR /
//   MIN_ / RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS / TIME_TO_LEAVE_SHOUT_MESSAGE; retail runs the six CCharStrings "ST_SIT_ON_SEAT",
//   "ST_STAND_UP_FROM_SEAT", "ST_SIT_ON_BENCH", "ST_STAND_UP_FROM_BENCH", "ST_SIT_ON_FLOOR", "ST_STAND_UP_FROM_FLOOR"
//   (0x01221AA0..0x01221B40), then this (fps * 20), 0x01221B80 (fps * 120), 0x01221B90 (fps * 10), 0x01221BA0 (fps * 50), then
//   "ST_SIT_ON_FLOOR", "ST_STAND_UP_FROM_FLOOR", fps * 10, fps * 50, fps * 5 -- literal for literal and the Ego_r sizes of the
//   leave-between-sittings pair (16 / 14) and the shout delay (14) equal retail's. Its one reader 0x00939540 (`mov ecx,[0x013BB6F4]`) adds it
//   to a random range and the world frame (0x0049D870) for a deadline the loop compares with `jae`. Catalog row "_gapscan::sub_01221b60" carried no name.
// Type: long (the multiplied GetConstantFPS() long; no reader narrows it). `const` is the upper-case constant convention (retail
//   never writes it); no PDB data-symbol dump proves the qualifier. A const anonymous-namespace object compiles to the internal
//   symbol `_MIN_TIME_TO_SIT` (the reader at 0x00939540 lives in the same TU).
// Data: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA) through the inline GetConstantFPS() (naming wave 18).
// Shape: `* 20` is strength-reduced to `lea eax,[eax+eax*4] ; shl eax,2`.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace {
const long MIN_TIME_TO_SIT = CMainGameComponent::GetConstantFPS() * 20;   // 0x013BB6F4
}