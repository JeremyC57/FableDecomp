// MSVC5 claim (byte-tactics c2-regalloc.md): expression temporaries take eax, ecx, edx round-robin from a pointer reset
// once per function. Does VC7.1 c2 do the same?
extern int G[16];
void r1() { G[0] = G[1] + 3; G[2] = G[3] + 5; G[4] = G[5] + 7; G[6] = G[7] + 9; }
void r2(int* p) { p[0] = p[1] + 3; p[2] = p[3] + 5; p[4] = p[5] + 7; p[6] = p[7] + 9; }   // p holds a register
void r3() { G[0] = G[1] + 3; G[2] = G[3] + 5; G[4] = G[5] + 7; G[6] = G[7] + 9; G[8] = G[9] + 11; G[10] = G[11] + 13; }
void r4() { G[2] = G[3] + 5; G[0] = G[1] + 3; G[4] = G[5] + 7; }     // reordered statements
void r5() { G[0] = G[1] * G[2]; G[3] = G[4] + 5; }                     // first temp needs a different shape
int r6(int a, int b) { G[0] = a + 3; G[1] = b + 5; G[2] = a * b; return G[2]; }
