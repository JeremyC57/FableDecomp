struct S { unsigned long a:11; unsigned long b:14; unsigned long c:7; };
unsigned long g(); unsigned long* p;
void f1(S& s) { s.c = g(); }
void f2(S& s) { unsigned long v = g(); s.c = v; }
void f3(S& s) { s.c = *p; }
void f4(S& s, unsigned long v) { s.c = v; }
void f5(S& s) { s.b = g(); s.c = g(); }
struct T { unsigned long a:11; unsigned long b:14; signed long c:7; };
void f6(T& s) { s.c = g(); }
struct U { unsigned long a:25; unsigned long c:7; };
void f7(U& s) { s.c = g(); }
