#include <cstdio>
#include <cstring>
// Behaviour test for 0x0057598A: a found name -> 1; a new name is appended and -> 0 (then found).
class CCharString { public: char m_text[16]; };
static CCharString* find(CCharString* first, CCharString* last, const CCharString& value)
{
    while (first != last && std::strcmp(first->m_text, value.m_text) != 0) ++first;
    return first;
}
struct SCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& value) { *_M_finish = value; ++_M_finish; }
};
class CTCInventoryExperience { public:
    SCharStringVector TutorialsPlayed;
    unsigned char IsTutorialPlayed(const CCharString& name);
};
unsigned char CTCInventoryExperience::IsTutorialPlayed(const CCharString& name)
{
    CCharString* last = TutorialsPlayed._M_finish;
    if (find(TutorialsPlayed._M_start, last, name) != last)
        return 1;
    TutorialsPlayed.push_back(name);
    return 0;
}
int main()
{
    CCharString store[8];
    CTCInventoryExperience e; e.TutorialsPlayed._M_start = e.TutorialsPlayed._M_finish = store; e.TutorialsPlayed._M_end_of_storage = store + 8;
    CCharString a; std::strcpy(a.m_text, "TUT_A");
    CCharString b; std::strcpy(b.m_text, "TUT_B");
    if (e.IsTutorialPlayed(a) != 0 || e.TutorialsPlayed._M_finish != store + 1) { std::printf("0057598a_TEST FAIL\n"); return 1; }
    if (e.IsTutorialPlayed(a) != 1 || e.TutorialsPlayed._M_finish != store + 1) { std::printf("0057598a_TEST FAIL\n"); return 1; }
    if (e.IsTutorialPlayed(b) != 0 || e.IsTutorialPlayed(b) != 1 || e.TutorialsPlayed._M_finish != store + 2) { std::printf("0057598a_TEST FAIL\n"); return 1; }
    std::printf("0057598a_TEST PASS\n");
    return 0;
}