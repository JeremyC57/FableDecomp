// CStaticFontBank::Render(const CWideString& text, const C3DVector& pos, float scale, const CRGBColour* colour, unsigned long flags, bool additive) -- emit two textured triangles per glyph into the font's dynamic vertex buffer (glyph banks of 64, texel-centred UVs, scaled about the start position, honouring newline / right / centre / vertical-centre layout), flushing a draw call every 98 triangles and once at the end
// Grade: DIFFER(1783v1910) -- first draft: ours gets an 8-aligned ebp frame (0x1b4) vs retail esp frame 0x15c; retail builds each vertex through named position/UV temporaries copied with rep movsd (7 dwords), x87 scheduling differs
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB7C20, 1908 bytes, `ret 0x18`.
// Callees: GetEngine 0x009A4EC0, CDynamicVertexBufferRing::GetCurrent 0x009E2CE0, CDynamicVertexBuffer::Lock 0x00AC20A0 / Unlock 0x00AC20B0,
//   CWideString::CStr 0x0099AF00 / GetData 0x0099AF10, floor 0x00BFEB8A, CRenderer::DrawTriangleList 0x00A0ABE0;
//   own vtable +0x04 GetExtent(C2DVector*, const unsigned short*, bool); IDirect3DDevice9::SetStreamSource (vtable +0x190).
// Literals: 0.5f 0x0122F59C, 1.0f 0x0122DED8, 4294967296.0f 0x0122DCB4.
#include <math.h>

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned long Value; };
class CWideString {
public:
    void* PStringData;
    const unsigned short* CStr() const;                                  // 0x0099AF00
    const unsigned short* GetData() const;                               // 0x0099AF10
};
struct CFontVertex {
    float X, Y, Z, RHW;
    unsigned long Colour;
    float U, V;
};
struct CVertexPos { float X, Y, Z, W; CVertexPos(float x, float y, float z) : X(x), Y(y), Z(z), W(1.0f) { } };
struct CVertexUV { float U, V; CVertexUV(float u, float v) : U(u), V(v) { } };
inline CFontVertex MakeVertex(const CVertexPos& p, unsigned long colour, const CVertexUV& uv)
{
    CFontVertex v;
    v.X = p.X; v.Y = p.Y; v.Z = p.Z; v.RHW = p.W; v.Colour = colour; v.U = uv.U; v.V = uv.V;
    return v;
}
struct IDirect3DVertexBuffer9;
struct IDirect3DDevice9 {
    virtual void __stdcall s000(); virtual void __stdcall s004(); virtual void __stdcall s008(); virtual void __stdcall s00c();
    virtual void __stdcall s010(); virtual void __stdcall s014(); virtual void __stdcall s018(); virtual void __stdcall s01c();
    virtual void __stdcall s020(); virtual void __stdcall s024(); virtual void __stdcall s028(); virtual void __stdcall s02c();
    virtual void __stdcall s030(); virtual void __stdcall s034(); virtual void __stdcall s038(); virtual void __stdcall s03c();
    virtual void __stdcall s040(); virtual void __stdcall s044(); virtual void __stdcall s048(); virtual void __stdcall s04c();
    virtual void __stdcall s050(); virtual void __stdcall s054(); virtual void __stdcall s058(); virtual void __stdcall s05c();
    virtual void __stdcall s060(); virtual void __stdcall s064(); virtual void __stdcall s068(); virtual void __stdcall s06c();
    virtual void __stdcall s070(); virtual void __stdcall s074(); virtual void __stdcall s078(); virtual void __stdcall s07c();
    virtual void __stdcall s080(); virtual void __stdcall s084(); virtual void __stdcall s088(); virtual void __stdcall s08c();
    virtual void __stdcall s090(); virtual void __stdcall s094(); virtual void __stdcall s098(); virtual void __stdcall s09c();
    virtual void __stdcall s0a0(); virtual void __stdcall s0a4(); virtual void __stdcall s0a8(); virtual void __stdcall s0ac();
    virtual void __stdcall s0b0(); virtual void __stdcall s0b4(); virtual void __stdcall s0b8(); virtual void __stdcall s0bc();
    virtual void __stdcall s0c0(); virtual void __stdcall s0c4(); virtual void __stdcall s0c8(); virtual void __stdcall s0cc();
    virtual void __stdcall s0d0(); virtual void __stdcall s0d4(); virtual void __stdcall s0d8(); virtual void __stdcall s0dc();
    virtual void __stdcall s0e0(); virtual void __stdcall s0e4(); virtual void __stdcall s0e8(); virtual void __stdcall s0ec();
    virtual void __stdcall s0f0(); virtual void __stdcall s0f4(); virtual void __stdcall s0f8(); virtual void __stdcall s0fc();
    virtual void __stdcall s100(); virtual void __stdcall s104(); virtual void __stdcall s108(); virtual void __stdcall s10c();
    virtual void __stdcall s110(); virtual void __stdcall s114(); virtual void __stdcall s118(); virtual void __stdcall s11c();
    virtual void __stdcall s120(); virtual void __stdcall s124(); virtual void __stdcall s128(); virtual void __stdcall s12c();
    virtual void __stdcall s130(); virtual void __stdcall s134(); virtual void __stdcall s138(); virtual void __stdcall s13c();
    virtual void __stdcall s140(); virtual void __stdcall s144(); virtual void __stdcall s148(); virtual void __stdcall s14c();
    virtual void __stdcall s150(); virtual void __stdcall s154(); virtual void __stdcall s158(); virtual void __stdcall s15c();
    virtual void __stdcall s160(); virtual void __stdcall s164(); virtual void __stdcall s168(); virtual void __stdcall s16c();
    virtual void __stdcall s170(); virtual void __stdcall s174(); virtual void __stdcall s178(); virtual void __stdcall s17c();
    virtual void __stdcall s180(); virtual void __stdcall s184(); virtual void __stdcall s188(); virtual void __stdcall s18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, IDirect3DVertexBuffer9* vb, unsigned long offset, unsigned long stride);   // +0x190
};
struct CDynamicVertexBuffer {
    unsigned char _pad00[4];
    IDirect3DVertexBuffer9* Buffer;                                      // +0x04
    unsigned char _pad08[0x10 - 0x08];
    unsigned char Stride;                                                // +0x10
    CFontVertex* Lock(long a, long b, long c);                           // 0x00AC20A0
    void Unlock();                                                       // 0x00AC20B0
};
struct CDynamicVertexBufferRing { CDynamicVertexBuffer* GetCurrent(); };   // 0x009E2CE0
struct CTexture;
struct CTextureState { CTexture* Texture; long Additive; };
struct CRenderer {
    unsigned char _pad[0x3cf0];
    IDirect3DDevice9* Device;                                            // +0x3cf0
    void DrawTriangleList(const CTextureState* state, long blend, long first, long count);   // 0x00A0ABE0
};
struct CRenderHolder { unsigned char _pad[8]; CRenderer* Renderer; };
struct CEngine { unsigned char _pad[0x60]; CRenderHolder* Render; };
CEngine* GetEngine();                                                    // 0x009A4EC0
struct CGlyph { float U0, V0, U1, V1; short OffsetX, Width, Advance; short _pad; };
struct CGlyphBank { unsigned short First; unsigned short Count; CGlyph* Glyphs; };

class CStaticFontBank {
public:
    virtual void s00();
    virtual void GetExtent(C2DVector* out, const unsigned short* text, bool toEndOfLine);   // +0x04
    unsigned long LineHeight;                                            // +0x04
    unsigned char _pad08[4];
    CDynamicVertexBufferRing* VertexBuffers;                             // +0x0c
    unsigned char _pad10[0x1c - 0x10];
    long TextureSizeX;                                                   // +0x1c
    long TextureSizeY;                                                   // +0x20
    long MinChar;                                                        // +0x24
    long MaxChar;                                                        // +0x28
    long TotalNoGlyphs;                                                  // +0x2c
    CGlyphBank GlyphBanks[1024];                                         // +0x30
    CTexture* PTexture;                                                  // +0x2030
    void Render(const CWideString& text, const C3DVector& pos, float scale, const CRGBColour* colour, unsigned long flags, bool additive);
};

void CStaticFontBank::Render(const CWideString& text, const C3DVector& pos, float scale, const CRGBColour* colour, unsigned long flags, bool additive)
{
    float startX = pos.X;
    float startY = pos.Y;
    float x = startX;
    float y = startY;
    unsigned long col = colour->Value;
    CRenderer* renderer = GetEngine()->Render->Renderer;
    CTextureState state;
    state.Texture = PTexture;
    state.Additive = additive;
    long blend = (additive != 0) + 1;
    CDynamicVertexBuffer* vb = VertexBuffers->GetCurrent();
    if (flags & 8) {
        C2DVector extent;
        GetExtent(&extent, text.CStr(), false);
        y = (float)floor(y - extent.Y * 0.5f);
    }
    unsigned long triangles = 0;
    CFontVertex* out = vb->Lock(0, 0, 0);
    bool lineStart = true;
    const unsigned short* p = text.GetData();
    while (*p) {
        if (lineStart) {
            if (flags & 6) {
                C2DVector extent;
                GetExtent(&extent, p, true);
                if (flags & 2)
                    x = (float)floor(startX - extent.X);
                if (flags & 4)
                    x = (float)floor(startX - extent.X * 0.5f);
            }
            lineStart = false;
        }
        unsigned short c = *p++;
        const unsigned short* next = p;
        if (c == '\n') {
            x = startX;
            y += (float)LineHeight;
            lineStart = true;
        }
        if ((long)c < MinChar || (long)c > MaxChar)
            continue;
        CGlyphBank& bank = GlyphBanks[c >> 6];
        if (bank.Glyphs == 0)
            continue;
        unsigned long index = (c & 0x3f) - bank.First;
        if (index >= bank.Count)
            continue;
        CGlyph* glyph = &bank.Glyphs[index];
        if (glyph == 0)
            continue;
        x += (float)glyph->OffsetX;
        float left = (x - startX) * scale + startX - 0.5f;
        float right = ((float)glyph->Width + 1.0f) * scale + left;
        float top = (y - startY) * scale + startY - 0.5f;
        float bottom = ((float)LineHeight + 1.0f) * scale + top;
        x += (float)glyph->Advance;
        float texW = (float)TextureSizeX;
        float texH = (float)TextureSizeY;
        float u0 = (float)(TextureSizeX - 1) * glyph->U0 / texW;
        float v0 = (float)(TextureSizeY - 1) * glyph->V0 / texH;
        float u1 = (float)(TextureSizeX - 1) * glyph->U1 / texW;
        float v1 = (float)(TextureSizeY - 1) * glyph->V1 / texH;
        *out++ = MakeVertex(CVertexPos(left, top, pos.Z), col, CVertexUV(u0, v0));
        *out++ = MakeVertex(CVertexPos(right, top, pos.Z), col, CVertexUV(u1, v0));
        *out++ = MakeVertex(CVertexPos(left, bottom, pos.Z), col, CVertexUV(u0, v1));
        *out++ = MakeVertex(CVertexPos(right, top, pos.Z), col, CVertexUV(u1, v0));
        *out++ = MakeVertex(CVertexPos(right, bottom, pos.Z), col, CVertexUV(u1, v1));
        *out++ = MakeVertex(CVertexPos(left, bottom, pos.Z), col, CVertexUV(u0, v1));
        triangles += 2;
        if (triangles * 3 > 0x126) {
            vb->Unlock();
            if (vb)
                renderer->Device->SetStreamSource(0, vb->Buffer, 0, vb->Stride);
            else
                renderer->Device->SetStreamSource(0, 0, 0, 0);
            renderer->DrawTriangleList(&state, blend, 0, triangles);
            renderer->Device->SetStreamSource(0, 0, 0, 0);
            vb = VertexBuffers->GetCurrent();
            out = vb->Lock(0, 0, 0);
            triangles = 0;
        }
        p = next;
    }
    vb->Unlock();
    if (triangles > 0) {
        if (vb)
            renderer->Device->SetStreamSource(0, vb->Buffer, 0, vb->Stride);
        else
            renderer->Device->SetStreamSource(0, 0, 0, 0);
        renderer->DrawTriangleList(&state, blend, 0, triangles);
        renderer->Device->SetStreamSource(0, 0, 0, 0);
    }
}
