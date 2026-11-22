#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
#define FABLE_RETAIL_FONT2DVERTEX_PASS28
#include "engine/FONT2DVERTEX.h"

typedef wchar_t FableFontWideChar;
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned long GetD3DCOLOR() const { return 0x01020408UL; } };
struct CTexture {};
struct CWideString {
    const wchar_t* str;
    operator const wchar_t*() const { return str; }
    const unsigned short* GetAsCharArray() const { return reinterpret_cast<const unsigned short*>(str); }
};
struct C2DExtentsF { float X, Y; };
struct CVertexBufferWin32 {
    FONT2DVERTEX storage[306];
    bool locked;
    CVertexBufferWin32() : locked(false) {}
    void* Lock(unsigned long, unsigned long, unsigned long) {
        assert(!locked); locked=true; return storage;
    }
    void Unlock() { assert(locked); locked=false; }
};
struct CFontManager {
    CVertexBufferWin32 buffer;
    CVertexBufferWin32& GetFreeVertexBuffer() { return buffer; }
};
struct CRenderManager {
    CVertexBufferWin32* current;
    std::vector<FONT2DVERTEX> submitted;
    std::vector<unsigned long> batches;
    unsigned long textures;
    CRenderManager() : current(0), textures(0) {}
    void SelectVertexBuffer(unsigned long stream,CVertexBufferWin32* p) {
        assert(stream==0); current=p;
    }
    void DrawCurrentBuffersAsTris(const CTexture** h,unsigned long count,unsigned long first,unsigned long n) {
        assert(current && !current->locked && h[0] && first==0);
        assert(count==(h[1] ? 2UL : 1UL)); textures=count;
        submitted.insert(submitted.end(),current->storage,current->storage+3*n);
        batches.push_back(n);
    }
};
struct CDisplayManager {
    CRenderManager render;
    CRenderManager& GetRenderManager() { return render; }
};
struct CSystemManager {
    CDisplayManager display;
    CDisplayManager* GetDisplayManager() { return &display; }
};
static CSystemManager system_manager;
static CSystemManager* GFGetSystemManager() { return &system_manager; }
struct CGlyph { float Left, Top, Right, Bottom; short Offset, Width, Advance; };
struct CStaticFontBank {
    CFontManager* PManager;
    const CTexture* PTexture;
    unsigned long FontHeight;
    unsigned short MinChar, MaxChar;
    struct { int X, Y; } TextureSize;
    CGlyph glyph;
    const CGlyph* FindGlyph(unsigned short c) const { return c=='A' ? &glyph : 0; }
    C2DExtentsF GetStringExtents(const wchar_t*,bool) const { C2DExtentsF e={8,10};return e; }
    void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*) const;
};
inline FONT2DVERTEX __fastcall InitFont2DVertex(const D3DXVECTOR4& p, unsigned long color, float tu, float tv)
{
    FONT2DVERTEX vertex;
    vertex.p = p;
    vertex.color = color;
    vertex.tu = tu;
    vertex.tv = tv;
    return vertex;
}

void CStaticFontBank::Render(const CWideString& str, const C3DVector& pos, float scale,
                             const CRGBColour& colour, unsigned long flags, const CTexture* mask) const
{
    const float originx = pos.X;
    const float originy = pos.Y;
    float sx = originx;
    float sy = originy;
    const float rhw = 1.0f;
    const unsigned long d3dcolor = colour.GetD3DCOLOR();
    CRenderManager& render_manager = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    const CTexture* handles[2] = {PTexture, mask};
    const unsigned long texture_count = mask ? 2 : 1;
    CVertexBufferWin32* pvb = &PManager->GetFreeVertexBuffer();
    if (flags & 8) {
        C2DExtentsF extents = GetStringExtents(reinterpret_cast<const FableFontWideChar*>(static_cast<const wchar_t*>(str)), false);
        sy = (float)floor(sy - extents.Y * 0.5f);
    }
    unsigned long no_tris = 0;
    FONT2DVERTEX* pvertices = reinterpret_cast<FONT2DVERTEX*>(pvb->Lock(0, 0, 0));
    int starting_new_line = 1;
    const unsigned short* pstr = str.GetAsCharArray();
    while (*pstr) {
        if (starting_new_line) {
            if (flags & 6) {
                C2DExtentsF extents = GetStringExtents(reinterpret_cast<const FableFontWideChar*>(pstr), true);
                if (flags & 2)
                    sx = (float)floor(originx - extents.X);
                if (flags & 4)
                    sx = (float)floor(originx - extents.X * 0.5f);
            }
            starting_new_line = 0;
        }
        unsigned short letter = *pstr++;
        if (letter == '\n') {
            sx = originx;
            sy += (float)FontHeight;
            starting_new_line = 1;
        }
        if (letter < MinChar || letter > MaxChar)
            continue;
        const CGlyph* pglyph = FindGlyph(letter);
        if (!pglyph)
            continue;
        sx += (float)pglyph->Offset;
        const float sx1 = (sx - originx) * scale + originx - 0.5f;
        const float sx2 = ((sx - originx) * scale + originx - 0.5f) + ((float)pglyph->Width + 1.0f) * scale;
        const float sy1 = (sy - originy) * scale + originy - 0.5f;
        const float sy2 = ((sy - originy) * scale + originy - 0.5f) + ((float)FontHeight + 1.0f) * scale;
        sx += (float)pglyph->Advance;
        const float tx1 = ((TextureSize.X - 1) * pglyph->Left) / TextureSize.X;
        const float ty1 = ((TextureSize.Y - 1) * pglyph->Top) / TextureSize.Y;
        const float tx2 = ((TextureSize.X - 1) * pglyph->Right) / TextureSize.X;
        const float ty2 = ((TextureSize.Y - 1) * pglyph->Bottom) / TextureSize.Y;
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy2, pos.Z, rhw), d3dcolor, tx1, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy1, pos.Z, rhw), d3dcolor, tx1, ty1);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy2, pos.Z, rhw), d3dcolor, tx2, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy1, pos.Z, rhw), d3dcolor, tx2, ty1);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx2, sy2, pos.Z, rhw), d3dcolor, tx2, ty2);
        *pvertices++ = InitFont2DVertex(D3DXVECTOR4(sx1, sy1, pos.Z, rhw), d3dcolor, tx1, ty1);
        no_tris += 2;
        if (no_tris * 3 > 0x126) {
            pvb->Unlock();
            render_manager.SelectVertexBuffer(0, pvb);
            render_manager.DrawCurrentBuffersAsTris(handles, texture_count, 0, no_tris);
            render_manager.SelectVertexBuffer(0, 0);
            pvb = &PManager->GetFreeVertexBuffer();
            pvertices = reinterpret_cast<FONT2DVERTEX*>(pvb->Lock(0, 0, 0));
            no_tris = 0;
        }
    }
    pvb->Unlock();
    if (no_tris > 0) {
        render_manager.SelectVertexBuffer(0, pvb);
        render_manager.DrawCurrentBuffersAsTris(handles, texture_count, 0, no_tris);
        render_manager.SelectVertexBuffer(0, 0);
    }
}

#include <string>
static CRenderManager DrawText(const std::wstring& text,unsigned flags,bool masked)
{
    CFontManager manager;
    CTexture tex,mask;
    CStaticFontBank bank;
    bank.PManager=&manager; bank.PTexture=&tex; bank.FontHeight=10;
    bank.MinChar=32; bank.MaxChar=191; bank.TextureSize.X=32; bank.TextureSize.Y=16;
    CGlyph glyph={0.25f,0.5f,0.5f,1.0f,1,4,5}; bank.glyph=glyph;
    CWideString str={text.c_str()}; C3DVector pos={100,50,0.25f}; CRGBColour colour;
    system_manager.display.render=CRenderManager();
    bank.Render(str,pos,2.0f,colour,flags,masked?&mask:0);
    assert(!manager.buffer.locked && !system_manager.display.render.current);
    return system_manager.display.render;
}
int main()
{
    CRenderManager a=DrawText(L"A",0,false);
    assert(a.textures==1 && a.batches.size()==1 && a.batches[0]==2 && a.submitted.size()==6);
    const float xs[]={101.5f,101.5f,111.5f,111.5f,111.5f,101.5f};
    const float ys[]={71.5f,49.5f,71.5f,49.5f,71.5f,49.5f};
    for(unsigned i=0;i<6;++i) {
        const FONT2DVERTEX& v=a.submitted[i];
        assert(v.p.x==xs[i] && v.p.y==ys[i] && v.p.z==0.25f && v.p.w==1.0f);
        assert(v.color==0x01020408UL);
        assert(v.tu==(xs[i]==xs[0]?31.0f/128:31.0f/64));
        assert(v.tv==(ys[i]==ys[0]?15.0f/16:15.0f/32));
    }
    for(unsigned flags=0;flags<16;flags+=2) {
        CRenderManager b=DrawText(L"AA\nBA",flags,true);
        assert(b.textures==2 && b.submitted.size()==18);
        float expected=101.5f-((flags&4)?8.0f:(flags&2)?16.0f:0.0f);
        assert(b.submitted[0].p.x==expected && b.submitted[12].p.x==expected);
        assert(b.submitted[0].p.y==71.5f-((flags&8)?10.0f:0.0f));
        assert(b.submitted[12].p.y==b.submitted[0].p.y+20.0f);
    }
    assert(DrawText(L"",0,false).batches.empty());
    assert(DrawText(L"\x0001\x00ff",0,false).submitted.empty());
    CRenderManager c=DrawText(std::wstring(51,L'A'),0,false);
    assert(c.batches.size()==2 && c.batches[0]==100 && c.batches[1]==2);
    assert(DrawText(std::wstring(50,L'A'),0,false).batches.size()==1);
    puts("00ab7c20_TEST PASS");
    return 0;
}