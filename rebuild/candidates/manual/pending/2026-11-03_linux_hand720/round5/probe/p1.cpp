#pragma optimize("s", on)
struct O { virtual unsigned long GetProperties() const; };
void f1(); void f2(); void f3(); void f4();
inline bool HasBit(unsigned long v, long bit) { return ((v >> bit) & 1) != 0; }
template<unsigned N> struct BS { unsigned long w; BS(unsigned long v):w(v){} bool test(unsigned n) const { return ((w >> n) & 1) != 0; } };
void probe(O& o)
{
    if (HasBit(o.GetProperties(), 9)) f3();
    if ((bool)((o.GetProperties() >> 9) & 1)) f4();
    if (BS<32>(o.GetProperties()).test(9)) f1();
    unsigned long p = o.GetProperties(); if ((p & (1 << 9)) != 0) f2();
}
