struct E { long X, Y, B, R; };
struct V { E* f; E* l; E* e; unsigned long size() const { return l - f; } E& operator[](unsigned long n) { return *(f + n); } };
struct C { char pad[0x80]; V m;
  void g1(unsigned long& r, long w);
  void g2(unsigned long& r, long w);
  void g3(unsigned long& r, long w);
  void g5(unsigned long& r, long w);
  void g6(unsigned long& r, long w);
  __forceinline bool W1(unsigned long i, long w) { if (m[i].X > w) return true; return false; }
  inline bool W2(unsigned long i, long w) { if (m[i].X > w) return true; return false; }
};
void C::g1(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (W1(i,w)) --i; r = i; } } }
void C::g2(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (W2(i,w)) --i; r = i; } } }
void C::g3(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (!(m[i].X <= w)) --i; r = i; } } }
void C::g5(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; bool b = m[i].X > w; while (b) { --i; b = m[i].X > w; } r = i; } } }
void C::g6(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (m[i].X > w) { --i; continue; } r = i; } } }
