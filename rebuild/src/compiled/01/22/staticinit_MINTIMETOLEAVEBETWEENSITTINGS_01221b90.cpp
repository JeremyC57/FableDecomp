// dynamic initializer for const long `anonymous namespace'::MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS @ 0x013BB6FC = CMainGameComponent::GetConstantFPS() * 10
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221B90, oracle length 16 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*4] ; shl eax, 1 ; mov dword ptr [0x13bb6fc], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 10. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS''
//   (0x010175D0/0x01017630) + FableWin.pdb (0x040001B0/0x04000250); Ego_r runs
//   SIT_ON_CHAIR/GET_UP_FROM_CHAIR/SIT_ON_BENCH/GET_UP_FROM_BENCH/SIT_ON_FLOOR/GET_UP_FROM_FLOOR_ANIM_NAME, MIN_TIME_TO_SIT,
//   RANGE_TIME_TO_SIT, MIN_/RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS = retail 0x01221AA0 .. 0x01221BA0 (MIN_TIME_TO_SIT landed at
//   0x01221B60; FPS*120, *10, *50). Anniv360 Test PDB (third source): S_LDATA32 const long MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS
//   (internal linkage). Was g_scaled_013bb6fc.
namespace {
const long MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS = CMainGameComponent::GetConstantFPS() * 10;    // 0x013BB6FC
}
