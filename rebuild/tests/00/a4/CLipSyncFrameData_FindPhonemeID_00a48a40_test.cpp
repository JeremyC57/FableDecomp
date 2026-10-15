// Behaviour test for 0x00A48A40 (CLipSyncFrameData::FindPhonemeID): copy of the matching record or an empty one.
#include <cstdio>
#include <cstring>
struct Str { char text[16]; Str() { text[0] = 0; } Str(const Str& o) { std::strcpy(text, o.text); } };
struct Phoneme { Str Name; unsigned int ID; int Value; Phoneme() : ID(0), Value(0) {} };
struct Frame {
    Phoneme* m_first; Phoneme* m_last;
    unsigned int size() const { return (unsigned int)(m_last - m_first); }
    Phoneme Find(unsigned int id) const
    {
        for (unsigned int i = 0; i < size(); ++i) if (m_first[i].ID == id) return m_first[i];
        return Phoneme();
    }
};
int main()
{
    Phoneme p[3];
    for (int i = 0; i < 3; ++i) { p[i].ID = 10 + i; p[i].Value = 100 + i; std::sprintf(p[i].Name.text, "ph%d", i); }
    Frame f; f.m_first = p; f.m_last = p + 3;
    Phoneme a = f.Find(11), b = f.Find(99);
    int ok = a.ID == 11 && a.Value == 101 && std::strcmp(a.Name.text, "ph1") == 0 && b.ID == 0 && b.Value == 0 && b.Name.text[0] == 0;
    f.m_last = p; Phoneme c = f.Find(10); ok &= c.ID == 0;
    if (!ok) { std::printf("00a48a40_TEST FAIL\n"); return 1; }
    std::printf("00a48a40_TEST PASS\n");
    return 0;
}