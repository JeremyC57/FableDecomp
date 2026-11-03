struct S { unsigned long a:11; unsigned long b:14; unsigned long c:7; };
unsigned long g(); long gl(); unsigned char gc();
void f8(S& s) { s.c = (g() & 0x7f); }
void f9(S& s) { unsigned long v=g(); unsigned long w = *(unsigned long*)&s; *(unsigned long*)&s = (v<<25) | (w & 0x1ffffff); }
void f10(S& s) { unsigned long v=g(); unsigned long w = *(unsigned long*)&s; *(unsigned long*)&s = (w & 0x1ffffff) | (v<<25); }
void f12(S& s) { s.c = gl(); }
void f13(S& s) { s.c = gc(); }
void f14(S& s) { s.c = (unsigned long)(long)g(); }
void f15(S& s) { s.c = g() << 0; }
void f16(S& s) { s.c = 0x7f & g(); }
