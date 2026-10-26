#include <cstdio>
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRetailLongVector { long* m_pFirst; long* m_pLast; long* m_pEnd; CRetailLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
enum EMiniGameType { MINI_GAME_NONE = 0, MINI_GAME_3 = 3 };
struct CQuestInfoElement {
    virtual void v00() { }
    virtual ~CQuestInfoElement() { }
    long ID; long ScriptID; float Scale;
    CQuestInfoElement(float scale) : ID(0), ScriptID(0), Scale(scale) { }
};
struct CQuestInfoElementMiniGameBar : CQuestInfoElement {
    EMiniGameType Type; float Percentage; CRGBColour Colour; CRGBColour FadeColour; bool Updatable; long Graphic; CRetailLongVector ExtraElements;
    virtual void v00() { }
    CQuestInfoElementMiniGameBar(EMiniGameType type)
        : CQuestInfoElement(2.0f), Type(type), Percentage(0.0f), Colour(255, 255, 255, 255), FadeColour(255, 255, 255, 255), Updatable(true), Graphic(0) { }
};
int main()
{
    CQuestInfoElementMiniGameBar b(MINI_GAME_3);
    if (b.Scale != 2.0f || b.Type != MINI_GAME_3 || b.Percentage != 0.0f || b.Colour.R != 255 || b.Colour.G != 255 || b.Colour.B != 255 || b.Colour.A != 255
        || b.FadeColour.A != 255 || !b.Updatable || b.Graphic != 0 || b.ExtraElements.m_pFirst != 0 || b.ExtraElements.m_pEnd != 0 || b.ID != 0)
    { std::printf("00656caa_TEST FAIL\n"); return 1; }
    std::printf("00656caa_TEST PASS\n");
    return 0;
}