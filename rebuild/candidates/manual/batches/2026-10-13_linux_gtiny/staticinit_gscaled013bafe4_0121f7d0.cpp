// dynamic initializer for long g_scaled_013bafe4 @ 0x013BAFE4 = g_01375550 * 6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F7D0, oracle length 16 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*2] ; shl eax, 1 ; mov dword ptr [0x13bafe4], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 6. Both objects are named by address.
extern long g_01375550;                   // 0x01375550

long g_scaled_013bafe4 = g_01375550 * 6;
