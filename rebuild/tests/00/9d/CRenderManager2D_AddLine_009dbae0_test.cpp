#include <cstdio>

// Model of CRenderManager2D::AddLine: the two vertices are offset by the viewport origin, converted to viewport space with RHW 1 and queued with a line record, unless the viewport is offscreen.
struct CTVertex { float X, Y, Z, RHW; unsigned long Diffuse, Specular; float U, V; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CTexture { int id; };
struct CInfo { bool Line, IsText; C2DBoxF Window; CTexture UploadedTexture; long BlendMode; float ZAverage; };

struct CRenderManager2D {
    bool offscreen;
    C2DBoxF viewport;
    CTVertex verts[8]; int nVerts;
    CInfo infos[4]; int nInfos;
    const C2DBoxF& GetViewportBoxFloat() const { return viewport; }
    float ToViewportX(float x) const
    {
        const C2DBoxF& box = GetViewportBoxFloat();
        return (box.TLX + x) / (box.BRX - box.TLX) * 2.0f - 1.0f;
    }
    float ToViewportY(float y) const
    {
        const C2DBoxF& box = GetViewportBoxFloat();
        return 1.0f - (y + box.TLY) / (box.BRY - box.TLY) * 2.0f;
    }
    void Add(const CTVertex* v, int n, long blendMode, CTexture* texture)
    {
        if (offscreen)
            return;
        for (int i = 0; i < n; ++i) {
            CTVertex vertex = v[i];
            vertex.X = ToViewportX(vertex.X);
            vertex.Y = ToViewportY(vertex.Y);
            vertex.RHW = 1.0f;
            verts[nVerts++] = vertex;
        }
        CInfo info;
        info.ZAverage = -1.0f;
        info.UploadedTexture.id = 0;
        info.BlendMode = blendMode;
        if (texture)
            info.UploadedTexture = *texture;
        info.Window = GetViewportBoxFloat();
        info.Line = true;
        info.IsText = false;
        infos[nInfos++] = info;
    }
};

int main()
{
    CRenderManager2D m;
    m.offscreen = false; m.nVerts = 0; m.nInfos = 0;
    m.viewport.TLX = 100; m.viewport.TLY = 50; m.viewport.BRX = 300; m.viewport.BRY = 150;
    CTVertex v[3];
    for (int i = 0; i < 3; ++i) { v[i].X = 50.0f * i; v[i].Y = 25.0f * i; v[i].Z = 0.5f + i; v[i].RHW = 9.0f; v[i].Diffuse = 0xff00ff00u + i; v[i].Specular = 7; v[i].U = 0.25f; v[i].V = 0.75f; }
    CTexture tex; tex.id = 42;
    m.Add(v, 2, 3, &tex);
    if (m.nVerts != 2 || m.nInfos != 1) { std::printf("009dbae0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 2; ++i) {
        float ex = (100.0f + 50.0f * i) / 200.0f * 2.0f - 1.0f, ey = 1.0f - (25.0f * i + 50.0f) / 100.0f * 2.0f;
        const CTVertex& o = m.verts[i];
        if (o.X != ex || o.Y != ey || o.Z != 0.5f + i || o.RHW != 1.0f || o.Diffuse != 0xff00ff00u + i || o.U != 0.25f || o.V != 0.75f)
            { std::printf("009dbae0_TEST FAIL\n"); return 1; }
    }
    const CInfo& info = m.infos[0];
    if (info.BlendMode != 3 || info.UploadedTexture.id != 42 || info.Window.BRX != 300.0f || info.Line != true || info.IsText)
        { std::printf("009dbae0_TEST FAIL\n"); return 1; }
    m.Add(v, 2, 1, 0);
    if (m.infos[1].UploadedTexture.id != 0 || m.infos[1].BlendMode != 1) { std::printf("009dbae0_TEST FAIL\n"); return 1; }
    m.offscreen = true;
    m.Add(v, 2, 1, &tex);
    if (m.nInfos != 2 || m.nVerts != 2 * 2) { std::printf("009dbae0_TEST FAIL\n"); return 1; }
    std::printf("009dbae0_TEST PASS\n");
    return 0;
}