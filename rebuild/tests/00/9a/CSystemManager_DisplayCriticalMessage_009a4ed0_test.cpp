#include <cstdio>
#include <cstring>
static int g_charAlive = 0, g_wideAlive = 0, g_boxes = 0, g_wideAliveInBox = -1;
static unsigned short g_text[64]; static unsigned short g_caption[64]; static void* g_window = (void*)1; static unsigned int g_type = 99;
static int MessageBoxW(void* window, const unsigned short* text, const unsigned short* caption, unsigned int type)
{
    ++g_boxes; g_window = window; g_type = type; g_wideAliveInBox = g_wideAlive;
    int i = 0; for (; text[i] && i < 63; ++i) g_text[i] = text[i]; g_text[i] = 0;
    for (i = 0; caption[i] && i < 63; ++i) g_caption[i] = caption[i]; g_caption[i] = 0;
    return 1;
}
class CWideString {
public:
    unsigned short m_data[64];
    CWideString(const char* s) { int i = 0; for (; s[i] && i < 63; ++i) m_data[i] = (unsigned short)s[i]; m_data[i] = 0; ++g_wideAlive; }
    CWideString(const CWideString& o) { std::memcpy(m_data, o.m_data, sizeof(m_data)); ++g_wideAlive; }
    ~CWideString() { --g_wideAlive; }
    operator const unsigned short*() const { return m_data; }
};
class CCharString {
public:
    const char* m_text;
    CCharString(const char* text, long = -1) : m_text(text) { ++g_charAlive; }
    ~CCharString() { --g_charAlive; }
    CWideString ToWideString() const { return CWideString(m_text); }
};
static const unsigned short g_caption_lit[] = { 'A','p','p','l','i','c','a','t','i','o','n',' ','E','r','r','o','r',0 };
class CSystemManager { public:
    void DisplayCriticalMessage(const char* message)
    {
        CCharString text(message);
        MessageBoxW(0, text.ToWideString(), g_caption_lit, 0);
    }
};
int main()
{
    CSystemManager s;
    s.DisplayCriticalMessage("Out of memory");
    if (g_boxes != 1 || g_window != 0 || g_type != 0 || g_text[0] != 'O' || g_text[12] != 'y' || g_text[13] != 0 || g_caption[0] != 'A' || g_caption[12] != 'E'
        || g_wideAliveInBox != 1 || g_wideAlive != 0 || g_charAlive != 0) { std::printf("009a4ed0_TEST FAIL\n"); return 1; }
    std::printf("009a4ed0_TEST PASS\n");
    return 0;
}