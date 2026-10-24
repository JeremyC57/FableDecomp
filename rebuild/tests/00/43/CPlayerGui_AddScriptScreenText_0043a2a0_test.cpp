// Behaviour test for CPlayerGui::AddScriptScreenText (0x0043A2A0): each call appends one entry at the END of the list holding the
// text, the position converted to virtual coordinates and snapped to the pixel grid, the colour channels, the font name and the
// current world frame.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
class CWideString { public: CWideString(const char* s) { std::strcpy(t, s); } CWideString(const CWideString& o) { std::strcpy(t, o.t); } char t[32]; };
class CCharString { public: CCharString(const char* s) { std::strcpy(t, s); } CCharString(const CCharString& o) { std::strcpy(t, o.t); } char t[32]; };
class C2DVector { public: float X, Y; };
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
static void GFACoordToVCoord(const C2DVector* a, C2DVector* v) { v->X = a->X / 640.0f; v->Y = a->Y / 480.0f; }
static float Snap(double v, double pixels) { return (float)((double)(long)(v * pixels + 0.5) / pixels); }
static float GFRoundVXToNearestPixel(double x) { return Snap(x, 640.0); }
static float GFRoundVYToNearestPixel(double y) { return Snap(y, 480.0); }
static long g_frame = 100;
class CWorld { public: static long GetFrame() { return g_frame; } };
namespace NPlayerGui {
class CScriptScreenText {
public:
    CScriptScreenText(const CWideString& text, const C2DVector& vpos, const CRGBColour& colour, const CCharString& fontName, unsigned long frameAdded)
        : Text(text), VPos(vpos), Colour(colour), FontName(fontName), FrameAdded(frameAdded) {}
    CWideString Text; C2DVector VPos; CRGBColour Colour; CCharString FontName; unsigned long FrameAdded;
};
}
struct NodeBase { NodeBase* next; NodeBase* prev; };
template <class T> struct Node : NodeBase { T data; };
template <class T> class List {
public:
    List() { sentinel = (Node<T>*)std::malloc(sizeof(Node<T>)); sentinel->next = sentinel->prev = sentinel; }
    void push_back(const T& x)
    {
        Node<T>* tmp = (Node<T>*)std::malloc(sizeof(Node<T>)); new (&tmp->data) T(x);
        NodeBase* n = sentinel; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    }
    Node<T>* sentinel;
};
class CPlayerGui {
public:
    List<NPlayerGui::CScriptScreenText> ScriptScreenText;
    void AddScriptScreenText(const CWideString& text, const C2DVector& pos, const CRGBColour& colour, const CCharString& fontName)
    {
        C2DVector vpos;
        GFACoordToVCoord(&pos, &vpos);
        vpos.X = GFRoundVXToNearestPixel(vpos.X);
        vpos.Y = GFRoundVYToNearestPixel(vpos.Y);
        ScriptScreenText.push_back(NPlayerGui::CScriptScreenText(text, vpos, colour, fontName, CWorld::GetFrame()));
    }
};
int main()
{
    int bad = 0;
    CPlayerGui gui;
    C2DVector p1 = { 320.4f, 240.0f }; C2DVector p2 = { 64.0f, 48.0f };
    gui.AddScriptScreenText(CWideString("Quest complete"), p1, CRGBColour(255, 128, 0, 200), CCharString("FONT_MAIN"));
    g_frame = 101;
    gui.AddScriptScreenText(CWideString("Gold +50"), p2, CRGBColour(1, 2, 3, 4), CCharString("FONT_SMALL"));
    NodeBase* s = gui.ScriptScreenText.sentinel;
    Node<NPlayerGui::CScriptScreenText>* a = (Node<NPlayerGui::CScriptScreenText>*)s->next;
    Node<NPlayerGui::CScriptScreenText>* b = (Node<NPlayerGui::CScriptScreenText>*)a->next;
    if (b->next != s || s->prev != b || b->prev != a || a->prev != s) bad = 1;
    if (std::strcmp(a->data.Text.t, "Quest complete") || std::strcmp(a->data.FontName.t, "FONT_MAIN") || a->data.FrameAdded != 100) bad = 1;
    if (a->data.VPos.X != 0.5f || a->data.VPos.Y != 0.5f) bad = 1;
    if (a->data.Colour.R != 255 || a->data.Colour.G != 128 || a->data.Colour.B != 0 || a->data.Colour.A != 200) bad = 1;
    if (std::strcmp(b->data.Text.t, "Gold +50") || b->data.FrameAdded != 101 || b->data.VPos.X != 0.1f || b->data.Colour.A != 4) bad = 1;
    if (bad) { std::printf("0043a2a0_TEST FAIL\n"); return 1; }
    std::printf("0043a2a0_TEST PASS\n");
    return 0;
}