// Behaviour test for 0086f680 CExpressionSet::TransferBinaryIn: count, resize with zero pairs, raw block, sort when dirty, marker.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct Stream {
    unsigned long m_position; const unsigned char* m_pData; long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* b, long n) { std::memset(b, 0xEE, n); }
    bool Valid(long size) { long long p = (long long)(unsigned long)(m_position + size); return p >= 0 && p <= 0x7fffffff; }
    void Read(void* b, long n) { if (!Valid(n)) return; if (m_remaining >= n) { std::memcpy(b, m_pData, n); m_pData += n; m_remaining -= n; m_position += n; } else ReadWithSrcChunkOverflow((unsigned char*)b, n); }
    void ReadBlock(void* b, long n) { if (n <= 0) return; if (!Valid(n)) return; if (n <= m_remaining) { std::memcpy(b, m_pData, n); m_pData += n; m_remaining -= n; m_position += n; } else ReadWithSrcChunkOverflow((unsigned char*)b, n); }
    unsigned long ReadULong() { unsigned long v = 0; Read(&v, 4); return v; }
};
struct Value { int w[2]; };
struct Pair { int first; Value second; Pair() : first(int()), second(Value()) {} };
struct Vec {
    Pair* b; Pair* e; Pair* c;
    unsigned long size() const { return (unsigned long)(e - b); }
    void resize(unsigned long n, const Pair& v) { Pair* nb = new Pair[n ? n : 1]; for (unsigned long i = 0; i < n; ++i) nb[i] = i < size() ? b[i] : v; delete[] b; b = nb; e = nb + n; c = e; }
};
static void Sort(Pair* f, Pair* l) { for (Pair* i = f; i < l; ++i) for (Pair* j = i + 1; j < l; ++j) if (j->first < i->first) { Pair t = *i; *i = *j; *j = t; } }
struct Set {
    Vec Pairs; unsigned char Compare; bool Dirty;
    void TransferBinaryIn(Stream& s)
    {
        unsigned long count = s.ReadULong();
        Pairs.resize(count, Pair());
        s.ReadBlock(Pairs.size() ? (void*)Pairs.b : (void*)&Pairs, Pairs.size() * sizeof(Pair));
        if (Dirty) { Sort(Pairs.b, Pairs.e); Dirty = false; }
        s.ReadULong();
    }
};
int main()
{
    int buf[2 + 3 * (1 + 2)]; int k = 0;
    buf[k++] = 3;
    for (int i = 0; i < 3; ++i) { buf[k++] = 30 - i * 10; for (int j = 0; j < 2; ++j) buf[k++] = i * 100 + j; }
    buf[k++] = 0x01AB6CBB;
    Stream s; s.m_position = 0; s.m_pData = (const unsigned char*)buf; s.m_remaining = sizeof(buf);
    Set set; set.Pairs.b = set.Pairs.e = set.Pairs.c = 0; set.Compare = 0; set.Dirty = true;
    set.TransferBinaryIn(s);
    if (set.Pairs.size() != 3 || set.Dirty || set.Pairs.b[0].first != 10 || set.Pairs.b[2].first != 30 || set.Pairs.b[0].second.w[0] != 200
        || s.m_remaining != 0 || s.m_position != sizeof(buf)) { std::printf("0086f680_TEST FAIL\n"); return 1; }
    std::printf("0086f680_TEST PASS\n");
    return 0;
}