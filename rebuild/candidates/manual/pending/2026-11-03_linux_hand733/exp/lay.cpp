struct Def { int a[20]; };
struct P { Def* o; };
P GetP();
void F(float& out, float a, float b, float c, float d);
extern long g1, g2, g3;
struct C { char pad[0x80]; long* m; long* e; unsigned long R; float x, y;
  void t1(); void t2(); void t3();
};
void C::t1() {
  P p = GetP();
  if (p.o == 0) { F(x, (float)g1, -1.0f, (float)g2, -1.0f); F(y, (float)g1, -1.0f, (float)g2, -1.0f); }
  else { F(x, (float)g1, (float)p.o->a[1], (float)g2, (float)p.o->a[2]); F(y, (float)g1, (float)p.o->a[3], (float)g2, (float)p.o->a[4]); }
  unsigned long i = R;
  while (i >= 0 && m[i] > g3) --i;
  R = i;
  F(x, 1, 2, 3, 4);
}
void C::t2() {
  P p = GetP();
  if (p.o == 0) { F(x, (float)g1, -1.0f, (float)g2, -1.0f); F(y, (float)g1, -1.0f, (float)g2, -1.0f); }
  else { F(x, (float)g1, (float)p.o->a[1], (float)g2, (float)p.o->a[2]); F(y, (float)g1, (float)p.o->a[3], (float)g2, (float)p.o->a[4]); }
  unsigned long i = R;
  while (m[i] > g3) --i;
  R = i;
  F(x, 1, 2, 3, 4);
}
