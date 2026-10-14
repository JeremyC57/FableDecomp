// Behaviour test for CAnimateThing::IsAnimationFinished @ 0x0070c0e0: 1 when no entries, first entry has no frames left, or flag bit 1 set; else 0.
#include <cstdio>
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) { } T& operator*() const { return *p; } };
    iterator begin() const { return iterator(first); } bool empty() const { return first == last; }
};
struct SAnimFrameEntry { short m_framesLeft; short m_pad; };
struct CAnimateThing {
    long m_value0; CRetailVector<SAnimFrameEntry> m_entries; unsigned char _pad[0x41]; unsigned char m_flags;
    long IsAnimationFinished() const
    { if (!m_entries.empty()) { if ((*m_entries.begin()).m_framesLeft > 0) { if (!(m_flags & 2)) return 0; } } return 1; }
};
int main()
{
    SAnimFrameEntry entries[2]; entries[0].m_framesLeft = 5; entries[1].m_framesLeft = 0;
    CAnimateThing t; t.m_entries.first = entries; t.m_entries.last = entries + 2; t.m_flags = 0;
    bool ok = t.IsAnimationFinished() == 0;
    t.m_flags = 2; if (t.IsAnimationFinished() != 1) ok = false;
    t.m_flags = 0xFD; if (t.IsAnimationFinished() != 0) ok = false;
    entries[0].m_framesLeft = 0; if (t.IsAnimationFinished() != 1) ok = false;
    entries[0].m_framesLeft = -1; if (t.IsAnimationFinished() != 1) ok = false;
    entries[0].m_framesLeft = 1; t.m_entries.last = entries; if (t.IsAnimationFinished() != 1) ok = false;
    if (!ok) { std::printf("0070c0e0_TEST FAIL\n"); return 1; }
    std::printf("0070c0e0_TEST PASS\n");
    return 0;
}