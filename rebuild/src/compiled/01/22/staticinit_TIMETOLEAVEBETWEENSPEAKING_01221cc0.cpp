// dynamic initializer for const long `anonymous namespace'::TIME_TO_LEAVE_BETWEEN_SPEAKING @ 0x013BB730 = CMainGameComponent::GetConstantFPS() * 3
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221CC0, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*2] ; mov dword ptr [0x13bb730], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 3. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::TIME_TO_LEAVE_BETWEEN_SPEAKING'' (0x01017700)
//   + FableWin.pdb (0x040004B0); Ego_r runs NAME_OF_BITCHIN_BOWL, TIME_TO_LEAVE_BETWEEN_SPEAKING = retail 0x01221CA0
//   ("OBJECT_EATING_BOWL_NONSE"), 0x01221CC0 (FPS*3). Anniv360 Test PDB (third source): S_LDATA32 const long
//   TIME_TO_LEAVE_BETWEEN_SPEAKING (internal linkage). Was g_scaled_013bb730.
namespace {
const long TIME_TO_LEAVE_BETWEEN_SPEAKING = CMainGameComponent::GetConstantFPS() * 3;    // 0x013BB730
}
