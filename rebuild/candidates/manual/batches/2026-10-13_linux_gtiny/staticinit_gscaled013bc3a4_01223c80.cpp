// dynamic initializer for long g_scaled_013bc3a4 @ 0x013BC3A4 = g_01375550 * 5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223C80, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*4] ; mov dword ptr [0x13bc3a4], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 5. Both objects are named by address.
extern long g_01375550;                   // 0x01375550

long g_scaled_013bc3a4 = g_01375550 * 5;
