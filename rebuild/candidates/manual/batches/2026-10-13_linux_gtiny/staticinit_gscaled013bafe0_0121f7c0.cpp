// dynamic initializer for long g_scaled_013bafe0 @ 0x013BAFE0 = g_01388a80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7C0, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1388a80] ; mov dword ptr [0x13bafe0], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01388A80, defined elsewhere, so not folded). Both objects are named by address.
extern long g_01388a80;                   // 0x01388A80

long g_scaled_013bafe0 = g_01388a80;
