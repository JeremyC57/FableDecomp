// Behaviour test for CAnimationEntry::TransferBinaryOut (0x005DB630): header words, component count
// (0 for a null map), then type id + component payload per element.
#include <cstdio>

static long g_out[32]; static int g_n = 0;
struct CDataOutputStream {
    void WriteLong(long v) { g_out[g_n++] = v; }
    void WriteULong(unsigned long v) { g_out[g_n++] = (long)v; }
};
struct CAnimComponentBase {
    long payload;
    virtual ~CAnimComponentBase() {}
    virtual void TransferBinaryOut(CDataOutputStream& s) const { s.WriteLong(payload); }
};
struct CAnimComponentEntry { int first; CAnimComponentBase* second; };
struct CAnimComponentMap { CAnimComponentEntry* m_pFirst; CAnimComponentEntry* m_pLast; long Size() const { return (long)(m_pLast - m_pFirst); } };
struct CAnimationEntry {
    long BankIndex, AnimName, GroupName; CAnimComponentMap* Components;
    void TransferBinaryOut(CDataOutputStream& stream) const
    {
        stream.WriteLong(BankIndex);
        stream.WriteLong(AnimName);
        stream.WriteLong(GroupName);
        unsigned long count = Components ? Components->Size() : 0;
        stream.WriteULong(count);
        if (count) {
            for (CAnimComponentEntry* it = Components->m_pFirst; it != Components->m_pLast; ++it) {
                CAnimComponentBase* component = it->second;
                stream.WriteLong(it->first);
                component->TransferBinaryOut(stream);
            }
        }
    }
};

int main()
{
    CDataOutputStream s;
    CAnimationEntry e = { 5, 6, 7, 0 };
    e.TransferBinaryOut(s);
    if (g_n != 4 || g_out[0] != 5 || g_out[3] != 0) { std::printf("005db630_TEST FAIL\n"); return 1; }
    CAnimComponentBase a, b; a.payload = 100; b.payload = 200;
    CAnimComponentEntry entries[2] = { { 3, &a }, { 9, &b } };
    CAnimComponentMap m = { entries, entries + 2 };
    e.Components = &m; g_n = 0;
    e.TransferBinaryOut(s);
    static const long expect[] = { 5, 6, 7, 2, 3, 100, 9, 200 };
    for (int i = 0; i < 8; ++i)
        if (g_out[i] != expect[i]) { std::printf("005db630_TEST FAIL\n"); return 1; }
    if (g_n != 8) { std::printf("005db630_TEST FAIL\n"); return 1; }
    std::printf("005db630_TEST PASS\n");
    return 0;
}