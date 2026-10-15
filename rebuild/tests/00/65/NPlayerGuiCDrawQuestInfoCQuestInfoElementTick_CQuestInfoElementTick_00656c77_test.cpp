#include <cstdio>
// Behaviour model of 0x00656C77: CQuestInfoElementTick(text, ticked, scale).
static float g_scale = 0.0f; static int g_copies = 0;
struct CWideString { const unsigned short* m_pData; CWideString(const unsigned short* p) : m_pData(p) {} CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_copies; } };
struct CQuestInfoElement { virtual ~CQuestInfoElement() {} float Scale; CQuestInfoElement(float scale) : Scale(scale) { g_scale = scale; } };
struct CQuestInfoElementTick : CQuestInfoElement {
    long Graphic; CWideString Text; bool Ticked;
    CQuestInfoElementTick(const CWideString& text, bool ticked, float scale) : CQuestInfoElement(scale), Graphic(0), Text(text), Ticked(ticked) {}
};
int main()
{
    static const unsigned short name[] = { 'Q', 0 };
    CWideString t(name);
    CQuestInfoElementTick e(t, true, 0.75f);
    if (e.Graphic != 0 || e.Text.m_pData != name || !e.Ticked || e.Scale != 0.75f || g_scale != 0.75f || g_copies != 1)
        { std::printf("00656c77_TEST FAIL\n"); return 1; }
    CQuestInfoElementTick f(t, false, 1.0f);
    if (f.Ticked || g_copies != 2) { std::printf("00656c77_TEST FAIL\n"); return 1; }
    std::printf("00656c77_TEST PASS\n");
    return 0;
}