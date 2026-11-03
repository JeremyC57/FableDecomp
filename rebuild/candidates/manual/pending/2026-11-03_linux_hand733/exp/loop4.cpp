struct E { long X, Y, B, R; };
struct V { E* f; E* l; E* e; unsigned long size() const { return l - f; } E& operator[](unsigned long n) { return *(f + n); } };
struct C { char pad[0x80]; V m;
  void k1(unsigned long& r, long w);
  void k2(unsigned long& r, long w);
  void k3(unsigned long& r, long w);
  void k4(unsigned long& r, long w);
  void k5(unsigned long& r, long w);
};
void C::k1(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1 && m[r].X > w) { while (m[r].X > w) --r; } }
void C::k2(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { while (m[r].X > w) r--; } } }
void C::k3(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (w != -1 && m[i].X > w) --i; r = i; } } }
void C::k4(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (i >= 0 && m[i].X > w) --i; r = i; } } }
void C::k5(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (m[i].X > w && i > 0) --i; r = i; } } }
