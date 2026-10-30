// Behaviour test for std::vector<pair<EPSCType, CComponentInstance> >::insert(position, x) (0x00AF7730): the STLport 4.6
// single-element insert on a plain-array vector model -- at end() the element is copy-constructed at the finish; in the
// middle the last element is copied up one, [position, finish - 2) slides back and a copy of x is assigned at position;
// with no spare capacity the vector grows through _M_insert_overflow (old + max(old, 1)); the result is begin() + index.
// The counted component's add-ref on every copy is checked.
#include <cstdio>
#include <cstdlib>

struct SInfo { unsigned int RefCount; };
struct CComponentInstance {
    void* vptr; void* Data; SInfo* Info; bool Enabled; unsigned int InstanceName;
};
struct Pair { int first; CComponentInstance second; };
static void CopyConstruct(Pair* p, const Pair& o) { *p = o; if (p->second.Info) ++p->second.Info->RefCount; }
static void Assign(Pair& d, const Pair& s)
{
    d.first = s.first;
    if (d.second.Info != s.second.Info) {
        if (d.second.Info) --d.second.Info->RefCount;
        d.second.Data = s.second.Data; d.second.Info = s.second.Info;
        if (s.second.Info) ++s.second.Info->RefCount;
    }
    d.second.Enabled = s.second.Enabled; d.second.InstanceName = s.second.InstanceName;
}
static void Destroy(Pair& p) { if (p.second.Info) --p.second.Info->RefCount; }

struct Vec { Pair* _M_start; Pair* _M_finish; Pair* _M_eos; };
static void InsertOverflow(Vec& v, Pair* pos, const Pair& x)
{
    unsigned int old = (unsigned int)(v._M_finish - v._M_start), len = old + (old < 1 ? 1 : old);
    Pair* ns = (Pair*)std::malloc(len * sizeof(Pair)); Pair* nf = ns;
    for (Pair* p = v._M_start; p != pos; ++p) CopyConstruct(nf++, *p);
    CopyConstruct(nf++, x);
    for (Pair* p = pos; p != v._M_finish; ++p) CopyConstruct(nf++, *p);
    for (Pair* p = v._M_start; p != v._M_finish; ++p) Destroy(*p);
    std::free(v._M_start); v._M_start = ns; v._M_finish = nf; v._M_eos = ns + len;
}
static Pair* Insert(Vec& v, Pair* position, const Pair& x)
{
    unsigned int n = (unsigned int)(position - v._M_start);
    if (v._M_finish != v._M_eos) {
        if (position == v._M_finish) {
            CopyConstruct(v._M_finish, x);
            ++v._M_finish;
        } else {
            CopyConstruct(v._M_finish, *(v._M_finish - 1));
            ++v._M_finish;
            Pair x_copy; CopyConstruct(&x_copy, x);
            Pair* last = v._M_finish - 2; Pair* result = v._M_finish - 1;
            for (int k = (int)(last - position); k > 0; --k) Assign(*--result, *--last);
            Assign(*position, x_copy);
            Destroy(x_copy);
        }
    } else
        InsertOverflow(v, position, x);
    return v._M_start + n;
}

int main()
{
    int ok = 1;
    SInfo shared; shared.RefCount = 1;
    Pair x; x.first = 7; x.second.vptr = 0; x.second.Data = 0; x.second.Info = &shared; x.second.Enabled = true; x.second.InstanceName = 70;
    Vec v; v._M_start = (Pair*)std::malloc(4 * sizeof(Pair)); v._M_finish = v._M_start; v._M_eos = v._M_start + 4;
    for (int i = 0; i < 3; ++i) { Pair e; e.first = i; e.second.vptr = 0; e.second.Data = 0; e.second.Info = 0; e.second.Enabled = false; e.second.InstanceName = i; *v._M_finish++ = e; }
    Pair* r = Insert(v, v._M_start + 1, x);                   // middle, spare room: {0, 7, 1, 2}
    ok &= r == v._M_start + 1 && v._M_finish - v._M_start == 4;
    ok &= v._M_start[0].first == 0 && v._M_start[1].first == 7 && v._M_start[2].first == 1 && v._M_start[3].first == 2;
    ok &= shared.RefCount == 2;                                // one stored copy (x_copy came and went)
    r = Insert(v, v._M_finish, x);                             // full: grow to 8, append
    ok &= r == v._M_start + 4 && v._M_eos - v._M_start == 8 && v._M_start[4].first == 7 && shared.RefCount == 3;
    r = Insert(v, v._M_finish, x);                             // end(), spare room
    ok &= r == v._M_start + 5 && v._M_start[5].second.InstanceName == 70 && shared.RefCount == 4;
    std::free(v._M_start);
    if (!ok) { std::printf("00af7730_TEST FAIL\n"); return 1; }
    std::printf("00af7730_TEST PASS\n");
    return 0;
}