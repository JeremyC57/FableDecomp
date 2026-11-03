struct S { unsigned long a:11; unsigned long b:14; unsigned long c:7; };
enum E { E0, E1 }; E ge(); unsigned short gs(); short gss(); float gf(); __int64 g64(); unsigned long g();
void f17(S& s) { s.c = ge(); }
void f18(S& s) { s.c = gs(); }
void f19(S& s) { s.c = gss(); }
void f20(S& s) { s.c = (unsigned long)gf(); }
void f21(S& s) { s.c = (unsigned long)g64(); }
void f22(S& s) { unsigned long v = g(); s.c = v + 0; }
void f23(S& s) { s.c = (unsigned char)g(); }
