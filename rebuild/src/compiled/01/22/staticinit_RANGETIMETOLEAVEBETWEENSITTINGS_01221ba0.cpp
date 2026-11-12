// dynamic initializer for const long `anonymous namespace'::RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS @ 0x013BB700 = CMainGameComponent::GetConstantFPS() * 50
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221BA0, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; imul eax, eax, 0x32 ; mov dword ptr [0x13bb700], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 50. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS''
//   (0x010175E0/0x01017640) + FableWin.pdb (0x040001D0/0x04000270); Ego_r runs
//   SIT_ON_CHAIR/GET_UP_FROM_CHAIR/SIT_ON_BENCH/GET_UP_FROM_BENCH/SIT_ON_FLOOR/GET_UP_FROM_FLOOR_ANIM_NAME, MIN_TIME_TO_SIT,
//   RANGE_TIME_TO_SIT, MIN_/RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS = retail 0x01221AA0 .. 0x01221BA0 (MIN_TIME_TO_SIT landed at
//   0x01221B60; FPS*120, *10, *50). Anniv360 Test PDB (third source): S_LDATA32 const long RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS
//   (internal linkage). Was g_scaled_013bb700.
namespace {
const long RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS = CMainGameComponent::GetConstantFPS() * 50;    // 0x013BB700
}
