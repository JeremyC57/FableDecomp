struct E { long X, Y, B, R; };
struct V { E* f; E* l; E* e; unsigned long size() const { return l - f; } E& operator[](unsigned long n) { return *(f + n); } const E& operator[](unsigned long n) const { return *(f + n); } };
long abs(long);
struct C { char pad[0x80]; V m;
  void h1(unsigned long& r, long w);
  void h2(unsigned long& r, long w);
  void h3(unsigned long& r, long w);
  bool W3(unsigned long i, long w) { return m[i].X > w; }
  long GX(unsigned long i) { return m[i].X; }
  const V& GetM() const { return m; }
};
void C::h1(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (W3(i,w)) --i; r = i; } } }
void C::h2(unsigned long& r, long w) { if (r >= m.size()) r = 0; if (w != -1) { if (m[r].X > w) { unsigned long i = r; while (GX(i) > w) --i; r = i; } } }
void C::h3(unsigned long& r, long w) { if (r >= GetM().size()) r = 0; if (w != -1) { if (GetM()[r].X > w) { unsigned long i = r; while (GetM()[i].X > w) --i; r = i; } } }
