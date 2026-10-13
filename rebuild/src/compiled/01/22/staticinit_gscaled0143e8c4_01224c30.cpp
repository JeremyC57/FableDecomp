// dynamic initializer for long g_scaled_0143e8c4 @ 0x0143E8C4 = g_014402f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224C30, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, dword ptr [0x14402f0] ; mov dword ptr [0x143e8c4], eax ; ret 
// Compiler-generated `??__E` initializer of a static-storage long computed from another global
// (0x014402F0, defined elsewhere, so not folded). Both objects are named by address.
extern long g_014402f0;                   // 0x014402F0

long g_scaled_0143e8c4 = g_014402f0;