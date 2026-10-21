#include <cstdio>
// Behaviour model of CRenderManager2D::Add2DLine (0x009DC2C0): with the viewport on screen, both vertices are
// converted to viewport space (x / width * 2 - 1, 1 - y / height * 2, z 0, rhw 1; other fields kept) and queued,
// then one Line record (window, blend mode, texture when given) is queued; offscreen, nothing is queued.
struct Box { float TLX, TLY, BRX, BRY; };
struct Vertex { float X, Y, Z, RHW; unsigned int Diffuse, Specular; float U, V; };
struct Texture { int id; };
struct Record { bool Line, IsText; Box Window; unsigned int BlendMode; int TextureId; };
struct Renderer {
    bool Offscreen;
    Box Viewport;
    Vertex Vertices[8]; int VertexCount;
    Record Records[4]; int RecordCount;
    float ToViewportX(const float& x) const { return x / (Viewport.BRX - Viewport.TLX) * 2.0f - 1.0f; }
    float ToViewportY(const float& y) const { return 1.0f - y / (Viewport.BRY - Viewport.TLY) * 2.0f; }
    void Add2DLine(const Vertex& v0, const Vertex& v1, unsigned int blendMode, const Texture* texture)
    {
        if (Offscreen)
            return;
        Vertex vertex = v0;
        vertex.X = ToViewportX(vertex.X);
        vertex.Y = ToViewportY(vertex.Y);
        vertex.Z = 0.0f;
        vertex.RHW = 1.0f;
        Vertices[VertexCount++] = vertex;
        vertex = v1;
        vertex.X = ToViewportX(vertex.X);
        vertex.Y = ToViewportY(vertex.Y);
        vertex.Z = 0.0f;
        vertex.RHW = 1.0f;
        Vertices[VertexCount++] = vertex;
        Record info;
        info.TextureId = 0;
        info.BlendMode = blendMode;
        if (texture)
            info.TextureId = texture->id;
        info.Window = Viewport;
        info.Line = true;
        info.IsText = false;
        Records[RecordCount++] = info;
    }
};
int main()
{
    Renderer r; r.Offscreen = false; r.VertexCount = 0; r.RecordCount = 0;
    r.Viewport.TLX = 0.0f; r.Viewport.TLY = 0.0f; r.Viewport.BRX = 640.0f; r.Viewport.BRY = 480.0f;
    Vertex a = { 320.0f, 120.0f, 5.0f, 7.0f, 0xff00ff00u, 1, 0.25f, 0.5f };
    Vertex b = { 640.0f, 480.0f, 9.0f, 9.0f, 0x80ffffffu, 2, 1.0f, 1.0f };
    Texture tex = { 42 };
    r.Add2DLine(a, b, 3, &tex);
    bool ok = r.VertexCount == 2 && r.RecordCount == 1
        && r.Vertices[0].X == 0.0f && r.Vertices[0].Y == 0.5f && r.Vertices[0].Z == 0.0f && r.Vertices[0].RHW == 1.0f
        && r.Vertices[0].Diffuse == 0xff00ff00u && r.Vertices[0].U == 0.25f
        && r.Vertices[1].X == 1.0f && r.Vertices[1].Y == -1.0f && r.Vertices[1].Specular == 2
        && r.Records[0].Line && !r.Records[0].IsText && r.Records[0].BlendMode == 3 && r.Records[0].TextureId == 42
        && r.Records[0].Window.BRX == 640.0f;
    r.Add2DLine(b, a, 1, 0);
    ok = ok && r.RecordCount == 2 && r.Records[1].TextureId == 0 && r.VertexCount == 4 && r.Vertices[3].X == 0.0f;
    r.Offscreen = true;
    r.Add2DLine(a, b, 1, 0);
    ok = ok && r.RecordCount == 2 && r.VertexCount == 4;
    if (!ok) { std::printf("009dc2c0_TEST FAIL\n"); return 1; }
    std::printf("009dc2c0_TEST PASS\n");
    return 0;
}
