// Behaviour test for CTCNymph::GetShouldSummon @ 0x0080FD30: ready, has the summon component, positive count.
#include <cstdio>
struct CTCAISummon { long count; long GetMaxCreatureCount() { return count; } };
struct SEntry { long type; void* component; };
struct CThing {
    unsigned long m_presentBits[9];
    SEntry* first; SEntry* last;
    SEntry* LowerBound(long type) { SEntry* p = first; while (p != last && p->type < type) ++p; return p; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            SEntry* it = LowerBound(interfaceType);
            if (it == last || interfaceType < it->type) it = last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCNymph {
    CThing* ParentThing;
    bool ReadyToSummon;
    bool GetShouldSummon()
    {
        if (ReadyToSummon) {
            CTCAISummon* summon;
            if (ParentThing->GetInterface(&summon, 0xfa)) return summon->GetMaxCreatureCount() > 0;
        }
        return false;
    }
};
int main()
{
    CTCAISummon s; s.count = 2;
    SEntry entries[2] = { { 0xfa, &s }, { 0x100, 0 } };
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.first = entries; t.last = entries + 2;
    CTCNymph n; n.ParentThing = &t; n.ReadyToSummon = true;
    if (n.GetShouldSummon()) { std::printf("0080fd30_TEST FAIL\n"); return 1; }      // no component bit
    t.m_presentBits[0xfa >> 5] = 1UL << (0xfa & 31);
    if (!n.GetShouldSummon()) { std::printf("0080fd30_TEST FAIL\n"); return 1; }
    s.count = 0;
    if (n.GetShouldSummon()) { std::printf("0080fd30_TEST FAIL\n"); return 1; }
    s.count = 1; n.ReadyToSummon = false;
    if (n.GetShouldSummon()) { std::printf("0080fd30_TEST FAIL\n"); return 1; }
    std::printf("0080fd30_TEST PASS\n");
    return 0;
}