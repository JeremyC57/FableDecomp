// Behaviour test for CSetTextInfo::CSetTextInfo(texts..., flags, time, bools) (0x0048DDF6): four string copies, then the scalars.
#include <cstdio>
static int g_copies = 0;
class CWideString { public:
    const unsigned short* m_text;
    explicit CWideString(const unsigned short* t) : m_text(t) {}
    CWideString(const CWideString& other) : m_text(other.m_text) { ++g_copies; }
};
struct CSetTextInfo {
    CWideString Text, Button1Text, Button2Text, Button3Text;
    unsigned long DisplayFlags;
    float DisplayTime;
    bool UpdateWhilePaused;
    bool ChangePlayerMode;
    CSetTextInfo(const CWideString& text, const CWideString& button1Text, const CWideString& button2Text,
                 const CWideString& button3Text, unsigned long displayFlags, float displayTime,
                 bool updateWhilePaused, bool changePlayerMode)
        : Text(text), Button1Text(button1Text), Button2Text(button2Text), Button3Text(button3Text),
          DisplayFlags(displayFlags), DisplayTime(displayTime), UpdateWhilePaused(updateWhilePaused), ChangePlayerMode(changePlayerMode) {}
    CSetTextInfo(const CSetTextInfo& other)
        : Text(other.Text), Button1Text(other.Button1Text), Button2Text(other.Button2Text), Button3Text(other.Button3Text),
          DisplayFlags(other.DisplayFlags), DisplayTime(other.DisplayTime), UpdateWhilePaused(other.UpdateWhilePaused), ChangePlayerMode(other.ChangePlayerMode) {}
};
static const unsigned short g_a[] = { 65, 0 }, g_b[] = { 66, 0 }, g_c[] = { 67, 0 }, g_d[] = { 68, 0 };
static bool Check(const CSetTextInfo& i)
{
    return i.Text.m_text == g_a && i.Button1Text.m_text == g_b && i.Button2Text.m_text == g_c && i.Button3Text.m_text == g_d &&
           i.DisplayFlags == 0x81 && i.DisplayTime == 2.5f && i.UpdateWhilePaused && !i.ChangePlayerMode;
}
int main()
{
    CWideString a(g_a), b(g_b), c(g_c), d(g_d);
    CSetTextInfo info(a, b, c, d, 0x81, 2.5f, true, false);
    if (!Check(info) || g_copies != 4) { std::printf("0048ddf6_TEST FAIL\n"); return 1; }
    std::printf("0048ddf6_TEST PASS\n");
    return 0;
}