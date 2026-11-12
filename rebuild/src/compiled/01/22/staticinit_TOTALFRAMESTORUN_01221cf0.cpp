// dynamic initializer for const long `anonymous namespace'::TOTAL_FRAMES_TO_RUN @ 0x013BB738 = CMainGameComponent::GetConstantFPS() * 60
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221CF0, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; imul eax, eax, 0x3c ; mov dword ptr [0x13bb738], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 60. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::TOTAL_FRAMES_TO_RUN'' (0x01017730) +
//   FableWin.pdb (0x04000560); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME -> TOTAL_FRAMES_TO_RUN = retail 0x01221CD0 ->
//   this (FPS*60, the frame budget the landed CAIStateGroupReactToDeadLeader rows read). Anniv360 Test PDB (third source):
//   S_LDATA32 const long TOTAL_FRAMES_TO_RUN (internal linkage). Was g_scaled_013bb738.
namespace {
const long TOTAL_FRAMES_TO_RUN = CMainGameComponent::GetConstantFPS() * 60;    // 0x013BB738
}
