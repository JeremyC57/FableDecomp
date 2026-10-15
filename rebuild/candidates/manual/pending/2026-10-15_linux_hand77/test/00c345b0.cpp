#include <cstdio>
// Behaviour test for 00c345b0: an equal record is left alone and gives 0; a different one is copied and gives 1.
struct SUIStyleRecord_00c345b0 {
    unsigned long m_word0;
    unsigned long m_word4;
    unsigned long m_word8;
    unsigned long m_wordC;
    SUIStyleRecord_00c345b0& operator=(const SUIStyleRecord_00c345b0& other)
    {
        m_word0 = other.m_word0;
        m_word4 = other.m_word4;
        m_word8 = other.m_word8;
        m_wordC = other.m_wordC;
        return *this;
    }
    long Assign(const SUIStyleRecord_00c345b0& other);
};
static int g_compares = 0;
static bool UIStyleRecordEquals(const SUIStyleRecord_00c345b0* record, const SUIStyleRecord_00c345b0* other)
{
    ++g_compares;
    return (unsigned char)record->m_wordC == (unsigned char)other->m_wordC && record->m_word0 == other->m_word0
        && record->m_word4 == other->m_word4 && record->m_word8 == other->m_word8;
}
long SUIStyleRecord_00c345b0::Assign(const SUIStyleRecord_00c345b0& other)
{
    if (UIStyleRecordEquals(this, &other))
        return 0;
    *this = other;
    return 1;
}
int main()
{
    SUIStyleRecord_00c345b0 a; a.m_word0 = 1; a.m_word4 = 2; a.m_word8 = 3; a.m_wordC = 4;
    SUIStyleRecord_00c345b0 b = a;
    if (a.Assign(b) != 0 || g_compares != 1) { std::printf("00c345b0_TEST FAIL\n"); return 1; }
    b.m_word8 = 30; b.m_wordC = 5;
    if (a.Assign(b) != 1 || a.m_word8 != 30 || a.m_wordC != 5 || a.m_word0 != 1 || g_compares != 2) { std::printf("00c345b0_TEST FAIL\n"); return 1; }
    std::printf("00c345b0_TEST PASS\n");
    return 0;
}
