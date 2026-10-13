// dynamic initializer for long g_scaled_0143e8c0 @ 0x0143E8C0 = g_013ac74c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224C40, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x13ac74c] ; mov dword ptr [0x143e8c0], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x013AC74C, defined elsewhere, so not folded). Both objects are named by address.
extern long g_013ac74c;                   // 0x013AC74C

long g_scaled_0143e8c0 = g_013ac74c;
