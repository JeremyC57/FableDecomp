// NPlayerGui::CCrimeStatBar::DrawStatBarOnLeft(CDrawContext* context, const C2DVector* position, const C2DVector* size, const CRGBColour* colour, long a, long b) -- draw the stat bar: the bar width rounds the lerped crime fraction to the nearest pixel (the fistp/fild rounding helper), the bar itself goes through the context's virtual DrawBar with the rounded width, then the value text (an integer-formatted CWideString built from the wrapped crime value scaled by 0.5f) is drawn at the render-target-relative position
// Grade: DIFFER(366v399) -- parked at once: fistp/fild frame-slot rounding helper over a dead argument slot (lane gap); the rest is best effort
// Leaf: DrawStatBarOnLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646010, length 399 bytes (frame pointer), `ret 0x1c`.
// Callees: CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0 (on the global display manager 0x013B8390), __ftol2 @ 0x00BFEA70,
// CWideString::CWideString(long) @ 0x00405F00 (`__fastcall`-ish: this in ecx, value in edx), the drawer's virtual slot 8 (DrawBar),
// ~CWideString @ 0x0099B510, CCrimeStatBar @ 0x0048DA62 (a long getter on the stat object at +0x60), CRGBColour helper @ 0x009E21D0
// (`__fastcall` out, in), CTextDrawer::DrawText @ 0x00438010 on the global 0x013B8790. Float pool: 0.5f @ 0x0122F59C.
// Layout: CCrimeStatBar Def +4 (fraction +0x4bc, text colour +0x424), Scale +0x10, Width +0x2c, DrawRect +0x50..+0x58, Stat +0x60, Flags +0x68.
// Shape: best-effort -- parked at once: the `fistp [ebp+10h] ; fild [ebp+10h]` round-to-nearest helper over a dead argument slot is a
// lane gap (no C++ form reproduces the frame-slot fistp/fild pair under VC7.1 /O2).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CWideString {
    void* PStringData;
    CWideString(long value);               // 0x00405F00
    ~CWideString();                        // 0x0099B510
};
struct CDisplayManager { long* GetRenderTargetDimensions(long* out); };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager;   // 0x013B8390
struct CTextDrawer { void DrawText(const CRGBColour* colour, const CWideString& text, long flags, const C2DVector* position, long align, float scale); };   // 0x00438010
extern CTextDrawer* g_pTextDrawer;           // 0x013B8790
void __fastcall ConvertColour(C2DVector* out, const C2DVector* in);   // 0x009E21D0
struct CStat { long GetValue(long wrap); };   // 0x0048DA62
struct CDrawContext;
struct CDrawer {
    virtual void v00(); virtual void v01();
    virtual void DrawBar(CDrawContext* context, const C2DVector* position, const C2DVector* size, long a, long b, long c, const CWideString& text, long flags) = 0;   // slot 8
};
struct CDrawContext { CDrawer* Drawer; };
struct CCrimeStatBarDef { unsigned char _pad[0x424]; CRGBColour TextColour; unsigned char _pad2[0x4bc - 0x428]; float Fraction; };
struct CCrimeStatBar {
    unsigned char _pad0[4];
    CCrimeStatBarDef* Def;                 // +0x04
    unsigned char _pad1[0x10 - 8];
    float Scale;                           // +0x10
    unsigned char _pad2[0x2c - 0x14];
    float Width;                           // +0x2c
    unsigned char _pad3[0x50 - 0x30];
    long DrawRect[3];                      // +0x50
    long _pad5c;
    CStat* Stat;                           // +0x60
    long _pad64;
    long Flags;                            // +0x68
    void DrawStatBarOnLeft(CDrawContext* context, const C2DVector* position, const C2DVector* size, const CRGBColour* colour, long a, long b, long c);
};
static __forceinline long RoundToNearest(float value)
{
    long rounded = (long)(value - 0.5f);
    if ((float)rounded + 1.0f == value)
        ++rounded;
    return rounded;
}

void CCrimeStatBar::DrawStatBarOnLeft(CDrawContext* context, const C2DVector* position, const C2DVector* size, const CRGBColour* colour, long a, long b, long c)
{
    float fraction = -Def->Fraction;
    float width = (size->Y - fraction) * Width + fraction;
    long rounded = RoundToNearest(width);
    C2DVector barSize = { size->X, (float)rounded };
    long dimensions[2];
    g_pDisplayManager->GetRenderTargetDimensions(dimensions);
    C2DVector barPos = { position->X, (float)dimensions[1] - barSize.Y };
    CWideString text((long)Scale);
    context->Drawer->DrawBar(context, position, &barPos, DrawRect[0], DrawRect[1], DrawRect[2], text, 1L);
    CRGBColour textColour = *colour;
    if (Flags > 0)
        textColour = Def->TextColour;
    C2DVector textPos = { 0.0f, barPos.Y + size->Y };
    g_pDisplayManager->GetRenderTargetDimensions(dimensions);
    C2DVector textOut;
    CWideString value((long)Scale);
    textOut.X = ((float)dimensions[0] - (float)Stat->GetValue((long)Scale)) * 0.5f;
    ConvertColour(&textOut, &textPos);
    CWideString label((long)Scale);
    g_pTextDrawer->DrawText(&textColour, label, 0x43, &textOut, 0, 1.0f);
}
