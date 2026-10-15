#include <cstdio>
// Behaviour test for 0064dae0: a change text starting at anchor A (flag set) or anchor B (flag clear) is pushed to the
// front of the bar's list with the colour, text, value and flag; the temporary's text is destroyed once per call.
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
static int g_wideDtors = 0;
struct CWideString {
    int m_id;
    CWideString() : m_id(0) {}
    CWideString(const CWideString& o) : m_id(o.m_id) {}
    ~CWideString() { ++g_wideDtors; }
};
struct CChangeText {
    C2DVector    m_startPos;
    C2DVector    m_pos;
    CWideString  m_text;
    long         m_value;
    CRGBColour   m_colour;
    bool         m_flag;
    float        m_age;
    CChangeText(const C2DVector& pos, const CRGBColour& colour, const CWideString& text, long value, bool flag)
        : m_startPos(pos), m_pos(pos), m_text(text), m_value(value), m_colour(colour), m_flag(flag), m_age(0.0f) {}
};
struct SPushed { float x, y; int textId; long value; unsigned char r; bool flag; };
static SPushed g_pushed[4]; static int g_nPushed = 0;
struct CChangeTextList {
    void push_front(const CChangeText& t)
    {
        SPushed p; p.x = t.m_startPos.X; p.y = t.m_startPos.Y; p.textId = t.m_text.m_id; p.value = t.m_value; p.r = t.m_colour.R; p.flag = t.m_flag;
        g_pushed[g_nPushed++] = p;
    }
};
struct CStatBarOwner {
    C2DVector m_changeTextAnchorA;
    C2DVector m_changeTextAnchorB;
};
struct CStatBar {
    CStatBarOwner*  m_pOwner;
    CChangeTextList m_changeTexts;
    void AddChangeText(const CRGBColour& colour, const CWideString& text, long value, bool flag);
};
void CStatBar::AddChangeText(const CRGBColour& colour, const CWideString& text, long value, bool flag)
{
    if (flag)
        m_changeTexts.push_front(CChangeText(m_pOwner->m_changeTextAnchorA, colour, text, value, flag));
    else
        m_changeTexts.push_front(CChangeText(m_pOwner->m_changeTextAnchorB, colour, text, value, flag));
}
int main()
{
    CStatBarOwner owner; owner.m_changeTextAnchorA.X = 1.0f; owner.m_changeTextAnchorA.Y = 2.0f;
    owner.m_changeTextAnchorB.X = 3.0f; owner.m_changeTextAnchorB.Y = 4.0f;
    CStatBar bar; bar.m_pOwner = &owner;
    CRGBColour colour; colour.R = 9; colour.G = 0; colour.B = 0; colour.A = 255;
    CWideString text; text.m_id = 42;
    bar.AddChangeText(colour, text, 15, true);
    bar.AddChangeText(colour, text, -3, false);
    if (g_nPushed != 2 || g_pushed[0].x != 1.0f || g_pushed[0].y != 2.0f || !g_pushed[0].flag || g_pushed[0].value != 15
        || g_pushed[1].x != 3.0f || g_pushed[1].y != 4.0f || g_pushed[1].flag || g_pushed[1].value != -3
        || g_pushed[0].textId != 42 || g_pushed[1].r != 9 || g_wideDtors != 2) {
        std::printf("0064dae0_TEST FAIL\n"); return 1;
    }
    std::printf("0064dae0_TEST PASS\n");
    return 0;
}