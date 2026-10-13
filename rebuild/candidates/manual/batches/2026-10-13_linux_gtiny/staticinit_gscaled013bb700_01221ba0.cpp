// dynamic initializer for long g_scaled_013bb700 @ 0x013BB700 = g_01375550 * 50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01221BA0, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; imul eax, eax, 0x32 ; mov dword ptr [0x13bb700], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 50. Both objects are named by address.
extern long g_01375550;                   // 0x01375550

long g_scaled_013bb700 = g_01375550 * 50;
