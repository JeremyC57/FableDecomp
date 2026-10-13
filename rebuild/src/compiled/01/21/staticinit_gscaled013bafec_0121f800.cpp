// dynamic initializer for long g_scaled_013bafec @ 0x013BAFEC = g_01375550 * 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F800, oracle length 14 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x1375550] ; lea eax, [eax + eax*2] ; mov dword ptr [0x13bafec], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x01375550, defined elsewhere, so not folded) scaled by 3. Both objects are named by address.
extern long g_01375550;                   // 0x01375550

long g_scaled_013bafec = g_01375550 * 3;