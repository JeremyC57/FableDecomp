#include <cstdio>
// Model of the sprite update: copy fields, halve the sizes, map the descriptor flag word into the render bits
// (IsParticle forced on, ZBuffer = !NO_ZBUFFER), pick the offset factors from the alignment flags or the pixel offset.
struct Desc { float SizeX, SizeY; unsigned short Flags; float OffX, OffY; };
struct Out { float SizeX, SizeY, OffX, OffY; int Modulate2X, Lit, Sprite3D, RotCentre, RotZ, IsParticle, ZBuffer; };
static void Update(Out& o, const Desc& d, unsigned short width, unsigned short height)
{
    o.SizeX = d.SizeX * 0.5f; o.SizeY = d.SizeY * 0.5f;
    o.IsParticle = 1;
    o.Modulate2X = (d.Flags & 0x100) != 0; o.Lit = (d.Flags & 0x80) != 0; o.Sprite3D = (d.Flags & 1) != 0;
    o.RotCentre = (d.Flags & 0x20) != 0; o.RotZ = (d.Flags & 0x40) != 0; o.ZBuffer = (d.Flags & 0x200) == 0;
    if (d.Flags & 2) { o.OffX = 1.0f; o.OffY = 0.0f; }
    else if (d.Flags & 4) { o.OffX = -1.0f; o.OffY = 0.0f; }
    else if (d.Flags & 8) { o.OffX = 0.0f; o.OffY = -1.0f; }
    else if (d.Flags & 0x10) { o.OffX = 0.0f; o.OffY = 1.0f; }
    else { o.OffX = d.OffX / (float)width * 2.0f; o.OffY = d.OffY / (float)height * 2.0f; }
}
int main()
{
    int bad = 0;
    Desc d = { 4.0f, 6.0f, 0x1e1, 8.0f, 16.0f };
    Out o;
    Update(o, d, 32, 64);
    if (o.SizeX != 2.0f || o.SizeY != 3.0f) bad = 1;
    if (!o.Modulate2X || !o.Lit || !o.Sprite3D || !o.RotCentre || !o.RotZ || !o.IsParticle || !o.ZBuffer) bad = 1;
    if (o.OffX != 0.5f || o.OffY != 0.5f) bad = 1;
    d.Flags = 0x204;
    Update(o, d, 32, 64);
    if (o.ZBuffer || o.Lit || o.OffX != -1.0f || o.OffY != 0.0f) bad = 1;
    d.Flags = 0x0a;
    Update(o, d, 32, 64);
    if (o.OffX != 1.0f || o.OffY != 0.0f) bad = 1;
    d.Flags = 0x10;
    Update(o, d, 32, 64);
    if (o.OffX != 0.0f || o.OffY != 1.0f) bad = 1;
    if (bad) { std::printf("00bad670_TEST FAIL\n"); return 1; }
    std::printf("00bad670_TEST PASS\n");
    return 0;
}
