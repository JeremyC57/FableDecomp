// NPlayerGui::CDrawAbilityButtons::DrawButton(CDrawContext* context, const C2DVector* position, float alpha, long index, long count, long flags, bool secondary) -- draw one ability button sprite: pick the primary or secondary icon table, derive the alpha byte from the gui def's button alpha (clamped to 255 when the def says so), skip when the count is zero or the alpha is zero, build the sprite at the anchored position and submit it to the context's drawer
// Grade: DIFFER(343v324) -- best effort (x87 compare chain + __ftol2): the sprite constructor's argument list and the anchor helper's by-value struct are inferred
// Leaf: DrawButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645099, length 324 bytes (frame pointer), `ret 0x1c`.
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`), __ftol2 @ 0x00BFEA70, CDrawBase::AnchorPoint @ 0x00643D5D (12-byte by-value anchor, long
// tier, two 8-byte vectors by reference), CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(...) @ 0x00493AC2, the context drawer's
// virtual slot 0x5c (DrawPrimitive(const CDrawRequest*, const CEnginePrimitive2DSprite*, long)). Float pool: 0.0f @ 0x0122DEDC, 255.0f @
// 0x01230014, 160.0f @ 0x0125A29C.
// Layout: CDrawAbilityButtons def +4 (button alpha +0x2a0, flag +0x250), PrimaryIcons +0x1c, SecondaryIcons +0x3c (12-byte vectors of
// 8-byte entries); the context's drawer at +0x34.
// Shape: best-effort (fistp/ftol lane): the negative-alpha mirror flag, the clamped alpha byte, the x87 compare chain.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };
struct SAnchor { long A, B, C; };
struct CPlayerGuiDef {
    unsigned char _pad[0x250];
    bool ClampButtonAlpha;                 // +0x250
    unsigned char _pad2[0x2a0 - 0x251];
    float ButtonAlpha;                     // +0x2a0
};
CPlayerGuiDef* __fastcall PeekGuiDef();   // 0x00438570
struct CEnginePrimitive2DSprite {
    unsigned char _body[0x64];
    CEnginePrimitive2DSprite(long a, long b, const C2DVector* position, long texture, long count, long c, long mode, long d, float u, float v, long anchor);   // 0x00493AC2
};
struct CDrawer {
    virtual void v00();
    unsigned char _slots[0x5c - 4];
    virtual void DrawPrimitive(void* request, const CEnginePrimitive2DSprite* sprite, long order);   // slot 0x5c
};
struct CDrawContext { unsigned char _pad[0x34]; CDrawer* Drawer; };
struct SIconEntry { long Texture; long Extra; };
struct CIconVector { SIconEntry* first; SIconEntry* last; SIconEntry* end; };
struct CDrawAbilityButtons {
    unsigned char _pad0[4];
    CPlayerGuiDef* Def;                    // +0x04
    unsigned char _pad1[0x1c - 8];
    CIconVector PrimaryIcons;              // +0x1c
    unsigned char _pad2[0x3c - 0x28];
    CIconVector SecondaryIcons;            // +0x3c
    long AnchorPoint(long tier, SAnchor anchor, long flags, C2DVector* out, const C2DVector* in);   // 0x00643D5D
    void DrawButton(CDrawContext* context, const C2DVector* position, float alpha, long index, long count, long flags, bool secondary);
};

void CDrawAbilityButtons::DrawButton(CDrawContext* context, const C2DVector* position, float alpha, long index, long count, long flags, bool secondary)
{
    PeekGuiDef();
    CIconVector* icons = secondary ? &SecondaryIcons : &PrimaryIcons;
    bool mirrored = true;
    if (alpha < 0.0f) {
        mirrored = false;
        alpha = -alpha;
    }
    unsigned char a = (unsigned char)(long)(Def->ButtonAlpha * 255.0f);
    if (Def->ClampButtonAlpha && a < 255)
        a = 255;
    if (count > 0 && alpha > 0.0f) {
        float scaled = alpha * (float)a;
        float width;
        if (!mirrored)
            width = 160.0f;
        else if (Def->ClampButtonAlpha && a < 255)
            width = secondary ? 255.0f : 0.0f;
        else
            width = scaled;
        C2DVector out;
        C2DVector size = { 0.0f, 0.0f };
        C2DVector zero = { 0.0f, 0.0f };
        C2DVector pos = *position;
        SAnchor anchor;
        long tier = AnchorPoint((long)width, anchor, 0, &size, &zero);
        CEnginePrimitive2DSprite sprite(0, 0, &pos, flags, count, 0, 2, 0, 0.0f, 0.0f, tier);
        context->Drawer->DrawPrimitive(&icons->first[index], &sprite, 0xc0);
    }
}
