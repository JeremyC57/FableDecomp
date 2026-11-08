extern int gsrc, gdst;
#pragma auto_inline(off)
void rd(int* p) { gdst = *p; }                 // reads through the pointer only
void wr(int* p) { *p = gsrc; }                 // writes through the pointer
void keep(int* p); int* gkeep;                 // declared only
void rdref(int* const& p) { gdst = *p; }
struct C { int v; };
void ext(C* const& c) { gdst = c->v; }
#pragma auto_inline(on)
int v1_rd()  { int s = 0; { int a = 1; rd(&a); s += a; } { int b = 2; rd(&b); s += b * 2; } { int c = 3; rd(&c); s += c * 3; } return s; }
int v2_wr()  { int s = 0; { int a; wr(&a); s += a; } { int b; wr(&b); s += b * 2; } { int c; wr(&c); s += c * 3; } return s; }
int v3_keep(){ int s = 0; { int a = 1; keep(&a); s += a; } { int b = 2; keep(&b); s += b * 2; } { int c = 3; keep(&c); s += c * 3; } return s; }
int v4_ext() { int s = 0; { C a; a.v = 1; ext(&a); s += a.v; } { C b; b.v = 2; ext(&b); s += b.v * 2; } { C c; c.v = 3; ext(&c); s += c.v * 3; } return s; }
int v5_rd_noinit() { int s = 0; { int a; rd(&a); s += a; } { int b; rd(&b); s += b * 2; } return s; }
