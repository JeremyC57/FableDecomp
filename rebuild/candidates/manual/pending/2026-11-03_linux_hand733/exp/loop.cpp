struct E { long X, Y, B, R; };
struct V { E* f; E* l; E* e; unsigned long size() const { return l - f; } E& operator[](unsigned long n) { return *(f + n); } };
struct C { char pad[0x80]; V m;
  void f1(unsigned long& r, long w);
  void f2(unsigned long& r, long w);
  void f3(unsigned long& r, long w);
  void f4(unsigned long& r, long w);
  void f6(unsigned long& r, long w);
};
void C::f1(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (m[i].X > w) --i; r = i; } } }
void C::f2(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; for (;;) { if (m[i].X <= w) break; --i; } r = i; } } }
void C::f3(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; L: if (m[i].X > w) { --i; goto L; } r = i; } } }
void C::f4(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1 && m[r].X > w) { unsigned long i; for (i = r; m[i].X > w; --i) ; r = i; } }
void C::f6(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { long i = r; while (m[i].X > w) i--; r = i; } } }
