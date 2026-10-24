// NUISystem::CIME::Callback_DrawFans(const IMEUI_VERTEX* vertices, unsigned int count) -- ImeUi fan callback: draw the fan as screen triangles (centre, v[i], v[i+1], wrapping to v[1]) in each vertex's RGB with the composition alpha
// Leaf: ?Callback_DrawFans@CIME@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404E10, 225 bytes (static __fastcall, `ret`).
// Identity: FableWin.pdb ?Callback_DrawFans@CIME@NUISystem@@SIXPBUIMEUI_VERTEX@@I@Z (the ImeUi DrawFans hook; sibling of
// Callback_DrawRect). Callees: GFACoordToVCoord 0x009E21D0 (__fastcall in, out; converted in place), GFGetSystemManager
// 0x009A4EC0 (display manager +0x60 -> 2D render manager +8, as the landed CIMEFont::DrawTextW 0x00405020),
// CRenderManager2D::Draw2DTriangle 0x009DDFF0 (landed; centre, a, b, colour, flags 1).
// Layout: IMEUI_VERTEX is 32 bytes (Ego_r.pdb: sx, sy, sz, rhw, color +0x10, specular, tu, tv); the composition alpha
// byte 0x013750B0 as the landed CIME::Render 0x00404C00.
// Retail bytes: push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,0x20 (8-byte locals address-taken in the loop) ;
//   p[0] = v[0] ; GFACoordToVCoord(&p[0],&p[0]) ; edi = 1 ; cmp count,1 ; jbe ; L: next = (i == count-1) ? 1 : i+1 ;
//   p[1] = v[i], p[2] = v[next], both converted ; colour bytes B,G,R from v[i].color plus the alpha global ;
//   GFGetSystemManager()->+0x60 ; pushes ; ->+8 ; Draw2DTriangle(p[0], p[1], p[2], colour, 1) ; inc edi ; jb L
// Shape (RELOCATION_MATCH on attempt 3): the three points are one C2DVector array (separate locals take the reverse
//   frame order, as the landed Draw2DTriangle's vertex array) filled member by member (a float-pair constructor
//   copies one Y through the FPU); the colour bytes come from successive `>>= 8` of a dword local (retail's
//   shr/shr chain, the same as the uncovered dword-to-colour helper 0x00404CF0 and CIMEFont::SetColor 0x00404D20);
//   the display manager is a named local, so +0x60 loads before the pushes and +8 between them.
struct C2DVector {
    float X, Y;
    C2DVector() {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRenderManager2D {
    bool Draw2DTriangle(const C2DVector& p0, const C2DVector& p1, const C2DVector& p2, const CRGBColour& colour,
                        unsigned long flags);   // 0x009DDFF0
};
struct CDisplayManager {
    unsigned char     _pad00[8];
    CRenderManager2D* RenderManager2D;          // +8
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;            // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();                         // 0x009A4EC0
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0
extern unsigned char g_imeCompositionAlpha_013750b0;                     // 0x013750B0

struct IMEUI_VERTEX {
    float sx;
    float sy;
    float sz;
    float rhw;
    unsigned long color;      // +0x10
    unsigned long specular;
    float tu;
    float tv;
};

namespace NUISystem {
class CIME {
public:
    static void __fastcall Callback_DrawFans(const IMEUI_VERTEX* vertices, unsigned int count);
};

void __fastcall CIME::Callback_DrawFans(const IMEUI_VERTEX* vertices, unsigned int count)
{
    C2DVector points[3];
    points[0].X = vertices[0].sx;
    points[0].Y = vertices[0].sy;
    GFACoordToVCoord(&points[0], &points[0]);
    for (unsigned int i = 1; i < count; ++i) {
        unsigned int next = (i == count - 1) ? 1 : i + 1;
        points[1].X = vertices[i].sx;
        points[1].Y = vertices[i].sy;
        points[2].X = vertices[next].sx;
        points[2].Y = vertices[next].sy;
        GFACoordToVCoord(&points[1], &points[1]);
        GFACoordToVCoord(&points[2], &points[2]);
        unsigned long color = vertices[i].color;
        unsigned char b = (unsigned char)color;
        color >>= 8;
        unsigned char g = (unsigned char)color;
        color >>= 8;
        unsigned char r = (unsigned char)color;
        CRGBColour colour(r, g, b, g_imeCompositionAlpha_013750b0);
        CDisplayManager* displayManager = GFGetSystemManager()->DisplayManager;
        displayManager->RenderManager2D->Draw2DTriangle(points[0], points[1], points[2], colour, 1);
    }
}
}