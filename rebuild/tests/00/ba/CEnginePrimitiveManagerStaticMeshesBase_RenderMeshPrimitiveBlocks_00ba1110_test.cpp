// behaviour test for 0x00BA1110 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks
#include <cstdio>

struct Primitive { int IndexBufferDraws; int DegenerateDraws; };
struct TextureStages { int Id; };

static void RenderIndexBuffer(Primitive& p, const TextureStages&) { ++p.IndexBufferDraws; }
static void RenderDegenerate(Primitive& p, const TextureStages&) { ++p.DegenerateDraws; }

static void RenderMeshPrimitiveBlocks(Primitive& p, const TextureStages& s, bool, bool degenerate)
{
    RenderIndexBuffer(p, s);
    if (degenerate)
        RenderDegenerate(p, s);
}

int main()
{
    Primitive p; p.IndexBufferDraws = 0; p.DegenerateDraws = 0;
    TextureStages s; s.Id = 1;
    RenderMeshPrimitiveBlocks(p, s, true, false);
    if (p.IndexBufferDraws != 1 || p.DegenerateDraws != 0) { std::printf("00ba1110_TEST FAIL\n"); return 1; }
    RenderMeshPrimitiveBlocks(p, s, false, true);
    if (p.IndexBufferDraws != 2 || p.DegenerateDraws != 1) { std::printf("00ba1110_TEST FAIL\n"); return 1; }
    std::printf("00ba1110_TEST PASS\n");
    return 0;
}