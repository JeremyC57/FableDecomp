// dynamic initializer for const long CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS @ 0x013BAFE4 = CMainGameComponent::GetConstantFPS() * 6
// Leaf: _$E1
// Naming wave 18: the global stand-in g_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7D0, oracle length 16 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*2] ; shl eax, 1 ; mov dword ptr [0x13bafe4], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 6. The object carries its PDB name (see Names below).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
#define FABLE_RETAIL_CMODEBASE_NOFRAMES
#include "engine/ETCInterfaceType.h"   // StatGetInterfaceType()'s enumerator
#include "engine/CTCCreatureModeManager.h"   // opt-in: the CModeBase NO_FRAMES_* statics (agentFAM09 hdr overlay)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for
//   'CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS'' (0x01011F50) + FableWin.pdb (0x03FF7380);
//   both PDBs run CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS, _IDLE_ANIMS, _WAVE_ANIMS =
//   retail 0x0121F7D0 (FPS*6), 0x0121F7E0 (FPS*12, landed), 0x0121F800 (FPS*3). Anniv360 Test PDB (third source):
//   ?NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@0JB. Was g_scaled_013bafe4.
const long CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS = CMainGameComponent::GetConstantFPS() * 6;    // 0x013BAFE4 ?NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@0JB
