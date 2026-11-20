// dynamic initializer for long `anonymous namespace'::JACK_FADE_DOWN_TIME @ 0x013BAFE0 = g_01388a80
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7C0, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1388a80] ; mov dword ptr [0x13bafe0], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01388A80, defined elsewhere, so not folded). The object carries its PDB name (see Names below).
extern long g_01388a80;                   // 0x01388A80

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::JACK_FADE_DOWN_TIME'' (0x01011F40) +
//   FableWin.pdb (0x03FF7360); both PDBs run JACK_TOTAL_DURATION, JACK_GLOW_START_FRAME, JACK_FADE_DOWN_TIME = retail
//   0x0121F780, 0x0121F7A0 (landed), this (a copy of g_01388a80, the term JACK_GLOW_START_FRAME subtracts). Anniv360 Test PDB
//   (third source): S_LDATA32 long JACK_FADE_DOWN_TIME (internal linkage). Was g_scaled_013bafe0.
namespace {
long JACK_FADE_DOWN_TIME = g_01388a80;    // 0x013BAFE0
}
