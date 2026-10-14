// _global::SetCurrentPair_00cb2ee0(void*, void*) -- when the global enable byte is set, store the two register arguments into a pair of globals
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2EE0, length 22 bytes (+ int3 padding).
// Callees: none. Globals: the enable byte at 0x0143E8AE, the two pointer slots at 0x0143CD54 and 0x0143CD60. The chunk label "CDrawMouseCursor::Draw"
// is propagated (see 0x009D81E0); this is a free `__fastcall` function of the 0x00CBxxxx module (both arguments arrive in ecx/edx, plain `ret`),
// between SetCurrentInterpolateInfo @ 0x00CB2E20 and ProcessAllMessages @ 0x00CB2F00, named for what it does.
extern bool  g_pairEnabled_0143e8ae;              // 0x0143E8AE
extern void* g_currentFirst_0143cd54;              // 0x0143CD54
extern void* g_currentSecond_0143cd60;             // 0x0143CD60

void __fastcall SetCurrentPair_00cb2ee0(void* first, void* second)
{
    if (g_pairEnabled_0143e8ae) {
        g_currentFirst_0143cd54 = first;
        g_currentSecond_0143cd60 = second;
    }
}
