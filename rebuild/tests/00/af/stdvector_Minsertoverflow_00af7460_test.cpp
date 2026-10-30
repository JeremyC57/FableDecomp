// Behaviour test for vector<pair<EPSCType, CComponentInstance> >::_M_insert_overflow (0x00AF7460): growth insert with
// counted-pointer element copies (copies addref, destruction releases).
#include <cstdio>
#include <cstdlib>
#include <new>
struct Info { unsigned int RefCount; };
struct Inst {                                   // CComponentInstance: counted component, enabled flag, name
    void* Data; Info* Ref; bool Enabled; unsigned int Name;
    Inst(Info* r, unsigned int n) : Data(0), Ref(r), Enabled(true), Name(n) { if (Ref) ++Ref->RefCount; }
    Inst(const Inst& o) : Data(o.Data), Ref(o.Ref), Enabled(o.Enabled), Name(o.Name) { if (Ref) ++Ref->RefCount; }
    ~Inst() { if (Ref) --Ref->RefCount; }
};
struct Pair { int first; Inst second; Pair(int f, const Inst& s) : first(f), second(s) { } };
struct Vec {
    Pair* s; Pair* f; Pair* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    void insert_overflow(Pair* pos, const Pair& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        Pair* ns = len ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ns;
        for (Pair* q = s; q != pos; ++q, ++nf) new (nf) Pair(*q);
        if (fill == 1) { new (nf) Pair(x); ++nf; }
        else for (unsigned int k = fill; k > 0; --k, ++nf) new (nf) Pair(x);
        if (!atend) for (Pair* q = pos; q != f; ++q, ++nf) new (nf) Pair(*q);
        for (Pair* q = s; q != f; ++q) q->~Pair();
        if (s) std::free(s);
        s = ns; f = nf; e = ns + len;
    }
};
int main()
{
    Info a = { 0 }, b = { 0 };
    Vec v; v.s = (Pair*)std::malloc(2 * sizeof(Pair)); v.f = v.s; v.e = v.s + 2;
    new (v.f) Pair(1, Inst(&a, 10)); ++v.f;
    new (v.f) Pair(2, Inst(&a, 20)); ++v.f;
    Pair x(7, Inst(&b, 70));
    v.insert_overflow(v.s + 1, x, 1, false);    // 1 7 2
    bool ok = v.size() == 3 && v.e - v.s == 4 && v.s[0].first == 1 && v.s[1].first == 7 && v.s[2].first == 2 &&
              v.s[1].second.Name == 70 && a.RefCount == 2 && b.RefCount == 2;
    v.insert_overflow(v.f, x, 3, true);         // append three copies
    ok = ok && v.size() == 6 && v.e - v.s == 6 && v.s[5].first == 7 && a.RefCount == 2 && b.RefCount == 5;
    for (Pair* q = v.s; q != v.f; ++q) q->~Pair();
    std::free(v.s);
    ok = ok && a.RefCount == 0 && b.RefCount == 1;
    if (!ok) { std::printf("00af7460_TEST FAIL\n"); return 1; }
    std::printf("00af7460_TEST PASS\n");
    return 0;
}