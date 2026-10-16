// Behaviour test for 005dc070: CAnimationSet::TransferBinaryOut -- count, (key, entry) pairs, then four defaults.
#include <cstdio>
struct TStream { long out[32]; int n; TStream() : n(0) {} void WriteULong(unsigned long v) { out[n++] = (long)v; } void WriteLong(long v) { out[n++] = v; } };
struct TEntry { long BankIndex, AnimName, GroupName; void* Components; void TransferBinaryOut(TStream& s) { s.WriteLong(1000 + BankIndex); } };
struct TPair { unsigned long first; TEntry second; };
struct TSet {
    TPair* m_first; TPair* m_last; long DefaultFlags, DefaultTransitionTime, DefaultDelay, DefaultGroup;
    unsigned long size() const { return m_last - m_first; }
    void TransferBinaryOut(TStream& s)
    {
        s.WriteULong(size());
        for (unsigned long i = 0; i < size(); ++i) { s.WriteULong(m_first[i].first); m_first[i].second.TransferBinaryOut(s); }
        s.WriteLong(DefaultFlags); s.WriteLong(DefaultTransitionTime); s.WriteLong(DefaultDelay); s.WriteLong(DefaultGroup);
    }
};
int main()
{
    TPair p[2]; p[0].first = 11; p[0].second.BankIndex = 1; p[1].first = 22; p[1].second.BankIndex = 2;
    TSet set; set.m_first = p; set.m_last = p + 2; set.DefaultFlags = 5; set.DefaultTransitionTime = 6; set.DefaultDelay = 7; set.DefaultGroup = -1;
    TStream s; set.TransferBinaryOut(s);
    long expect[] = { 2, 11, 1001, 22, 1002, 5, 6, 7, -1 };
    if (s.n != 9) { std::printf("005dc070_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (s.out[i] != expect[i]) { std::printf("005dc070_TEST FAIL\n"); return 1; }
    std::printf("005dc070_TEST PASS\n");
    return 0;
}