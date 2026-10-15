// Behaviour test for NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(long, bool, float) @ 0x00656C3C
#include <cstdio>
#include <stddef.h>

static const wchar_t* g_lastText = 0;

class CWideString {
public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* text) : PStringData(text) { g_lastText = text; }
};

struct CQuestInfoElement {
    virtual ~CQuestInfoElement() { }
    long ID, ScriptID;
    float Scale;
    CQuestInfoElement(float scale) : ID(0), ScriptID(0), Scale(scale) { }
};

struct CQuestInfoElementTick : CQuestInfoElement {
    long        Graphic;
    CWideString Text;
    bool        Ticked;
    CQuestInfoElementTick(long graphic, bool ticked, float scale)
        : CQuestInfoElement(scale), Graphic(graphic), Text(L""), Ticked(ticked) { }
};

int main()
{
    CQuestInfoElementTick tick(77, true, 0.75f);
    if (tick.Graphic != 77 || !tick.Ticked || tick.Scale != 0.75f || tick.ID != 0) { std::printf("00656c3c_TEST FAIL\n"); return 1; }
    if (g_lastText == 0 || g_lastText[0] != 0 || tick.Text.PStringData != g_lastText) { std::printf("00656c3c_TEST FAIL\n"); return 1; }
    CQuestInfoElementTick off(3, false, 1.0f);
    if (off.Ticked || off.Graphic != 3) { std::printf("00656c3c_TEST FAIL\n"); return 1; }
    std::printf("00656c3c_TEST PASS\n");
    return 0;
}