#include <cstdio>
static int g_copies = 0;
class CCharString { public: long id; CCharString(long i) : id(i) { } CCharString(const CCharString& o) : id(o.id) { ++g_copies; } };
class CWideString { public: long id; CWideString(long i) : id(i) { } CWideString(const CWideString& o) : id(o.id) { ++g_copies; } };
struct SLocalisedText {
    CWideString m_text0, m_text1, m_text2, m_text3; long m_value;
    SLocalisedText() : m_text0(1), m_text1(2), m_text2(3), m_text3(4), m_value(5) { }
    SLocalisedText(const SLocalisedText& o)
        : m_text0(o.m_text0), m_text1(o.m_text1), m_text2(o.m_text2), m_text3(o.m_text3), m_value(o.m_value) { }
};
struct SNamedTextEntry {
    CCharString first; SLocalisedText second;
    SNamedTextEntry() : first(9) { }
    SNamedTextEntry(const SNamedTextEntry& o) : first(o.first), second(o.second) { }
};
int main()
{
    SNamedTextEntry a;
    SNamedTextEntry b(a);
    bool ok = g_copies == 5 && b.first.id == 9 && b.second.m_text0.id == 1 && b.second.m_text3.id == 4 && b.second.m_value == 5;
    if (!ok) { std::printf("009a9ad0_TEST FAIL\n"); return 1; }
    std::printf("009a9ad0_TEST PASS\n");
    return 0;
}
