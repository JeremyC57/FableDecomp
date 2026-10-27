// NPlayerGui::CCrimeStatBar::DrawStatBarOnLeft(CDrawContext* context, const C2DVector* position, const C2DVector* size, const CRGBColour* colour, long a, long b) -- draw the stat bar: the bar width rounds the lerped crime fraction to the nearest pixel (the fistp/fild rounding helper), the bar itself goes through the context's virtual DrawBar with the rounded width, then the value text (an integer-formatted CWideString built from the wrapped crime value scaled by 0.5f) is drawn at the render-target-relative position
// Leaf: DrawStatBarOnLeft
// Retail-asm: 0x00646044..0x00646064 `fld [ebp - 0xc] ; fsub [ebp - 8] ; fistp [ebp + 0x10] ; fild [ebp + 0x10] ; fadd [ebp - 4] ; fstp [ebp - 0x10] ; mov eax, [ebp - 0xc] ; cmp eax, [ebp - 0x10] ; jne 0x646061 ; inc [ebp + 0x10] ; fild [ebp + 0x10] ; mov eax, [ecx]` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00646010; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The bar width is the floor block.
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
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}


void CCrimeStatBar::DrawStatBarOnLeft(CDrawContext* context, const C2DVector* position, const C2DVector* size, const CRGBColour* colour, long a, long b, long c)
{
    float fraction = -Def->Fraction;
    float width = (size->Y - fraction) * Width + fraction;
    long rounded = FloorToLong(width);
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
