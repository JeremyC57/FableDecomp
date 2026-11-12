// dynamic initializer for const long `anonymous namespace'::DEFAULT_UNARMED_COMBAT_IDLE_COUNT @ 0x013BAD40 = CMainGameComponent::GetConstantFPS() * 5
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121ED60, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*4] ; mov dword ptr [0x13bad40], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 5. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::DEFAULT_UNARMED_COMBAT_IDLE_COUNT''
//   (0x01011710) + FableWin.pdb (0x03FF3870); both PDBs run DEFAULT_UNARMED_COMBAT_IDLE_COUNT,
//   CTCCreatureModeManager::RegisteredModes, CModeBase::BaseFilter = retail this (FPS*5), 0x0121ED70, 0x0121ED80. Anniv360 Test
//   PDB (third source): S_LDATA32 const long DEFAULT_UNARMED_COMBAT_IDLE_COUNT (internal linkage). Was g_scaled_013bad40.
namespace {
const long DEFAULT_UNARMED_COMBAT_IDLE_COUNT = CMainGameComponent::GetConstantFPS() * 5;    // 0x013BAD40
}
