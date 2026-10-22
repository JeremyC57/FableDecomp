// CConsole::DrawHistoryScrollBar(const C2DBoxF&) -- at the right edge of the history window draw an up arrow while lines below the visible ones exist, a down arrow while lines above exist, and the scroll bar box spanning the visible part of the history
// Leaf: DrawHistoryScrollBar
// Shape: the four pixel/screen scales are IMPLICIT double->float conversions (`float bottomOffset = g_virtualPixelScaleY * -2.0;`); an explicit
//   (float) cast stores bottomOffset before `push ebp` and hoists the barWidth chain between the prologue pushes.
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CRenderManager2D {
    bool Draw2DTriangle(const C2DVector& v0, const C2DVector& v1, const C2DVector& v2, const CRGBColour& colour,
                        unsigned long flags);                               // 0x009DDFF0
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, long flags); // 0x009DE3F0
};
struct CDisplayManager {
    unsigned char     _pad[8];
    CRenderManager2D* PRenderManager2D;                 // +0x08
};
struct CSystemManager {
    unsigned char    _pad[0x60];
    CDisplayManager* PDisplayManager;                   // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0
extern double g_virtualPixelScaleX;                     // 0x013CA800
extern double g_virtualPixelScaleY;                     // 0x013CA808
extern double g_virtualScreenWidth;                     // 0x013961E8
extern double g_virtualScreenHeight;                    // 0x013961F0

struct CHistoryList {
    struct Node { Node* Next; Node* Prev; };
    Node* Sentinel;
    Node* begin() const { return Sentinel->Next; }
    Node* end() const { return Sentinel; }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (Node* p = Sentinel->Next; p != Sentinel; p = p->Next)
            ++n;
        return n;
    }
};
inline int Distance(CHistoryList::Node* first, CHistoryList::Node* last)
{
    int n = 0;
    CHistoryList::Node* it = first;
    for (; it != last; it = it->Next)
        ++n;
    return n;
}

struct CConsole {
    virtual void v00();
    unsigned char       _pad04[0x4c - 0x04];
    CHistoryList        CommandLineHistory;             // +0x4c
    CHistoryList::Node* VisibleHistoryStart;            // +0x50
    CHistoryList::Node* VisibleHistoryFinish;           // +0x54
    unsigned char       _pad58[0x8c - 0x58];
    CRGBColour          ScrollBarColour;                // +0x8c
    void DrawHistoryScrollBar(const C2DBoxF& window);
};

void CConsole::DrawHistoryScrollBar(const C2DBoxF& window)
{
    float bottomOffset = g_virtualPixelScaleY * -2.0;
    float barWidth = g_virtualPixelScaleX * 3.0;
    C2DVector arrowOffset(-(g_virtualPixelScaleX * 2.0 + barWidth), 3000.0f);
    float arrowHeight = g_virtualScreenHeight * 0.015f;
    float arrowWidth = g_virtualScreenWidth * 0.02f;
    float right = arrowOffset.X + window.BRX - 3000.0f;
    C2DVector up[3];
    float middle = right - arrowWidth * 0.5f;
    up[0].X = middle;
    float top = window.TLY + 3000.0f;
    up[0].Y = top;
    up[1].X = right;
    up[1].Y = top + arrowHeight;
    up[2].X = right - arrowWidth;
    up[2].Y = top + arrowHeight;
    C2DVector down[3];
    down[0].X = middle;
    float bottom = bottomOffset + window.BRY;
    down[0].Y = bottom;
    down[1].X = right - arrowWidth;
    down[1].Y = bottom - arrowHeight;
    down[2].X = right;
    down[2].Y = bottom - arrowHeight;
    CRenderManager2D* renderer = GFGetSystemManager()->PDisplayManager->PRenderManager2D;
    if (VisibleHistoryFinish != CommandLineHistory.end())
        renderer->Draw2DTriangle(up[0], up[1], up[2], ScrollBarColour, 0);
    if (VisibleHistoryStart != CommandLineHistory.begin())
        renderer->Draw2DTriangle(down[0], down[1], down[2], ScrollBarColour, 0);
    float barBottom = bottomOffset + window.BRY;
    float barHeight = barBottom - (window.TLY + 3000.0f);
    float startFraction = (float)Distance(CommandLineHistory.begin(), VisibleHistoryStart) / (float)CommandLineHistory.size();
    float finishFraction = (float)Distance(CommandLineHistory.begin(), VisibleHistoryFinish) / (float)CommandLineHistory.size();
    C2DBoxF bar;
    bar.TLX = window.BRX - 3000.0f - barWidth;
    bar.TLY = barBottom - finishFraction * barHeight;
    bar.BRX = window.BRX - 3000.0f;
    bar.BRY = barBottom - startFraction * barHeight;
    renderer->Draw2DBox(bar, ScrollBarColour, 1);
}
