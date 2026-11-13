// Behaviour test for 0x00B9E5B0 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer: a host model of the row
// with recording stand-ins for the render core (cached per-stage texture, device SetTexture only on a change, stage high-water mark)
// and the shader manager's six-argument DrawBuffersIndexed (SetIndices, PT_NULL -> buffer type, DrawIndexedPrimitive). Checks: blocks
// flagged DegenerateTriangles are skipped; change flag 1 binds the diffuse texture on the diffuse stage (that stage is NOT tested
// against -1); change flag 2 binds the bump texture only when the bump stage is in use; each draw passes (type strip/list, 0, 0,
// vertex count, start index, primitive count); an unchanged texture makes no device call.
#include <cstdio>
#include <cstring>
struct Tex { int id; };
struct Block { unsigned long prim_count, start; bool strip, degenerate; unsigned char change; const Tex* diffuse; const Tex* bump; };
struct Prim { unsigned long vertex_count; int index_buffer; unsigned long count; Block* blocks; };
struct Stages { long diffuse, bump; };
struct Log { int set_texture_calls; int last_stage[16], last_tex[16]; long used; int bound[8];
             int draws; unsigned long draw[16][6]; int indices[16]; };
static void Attach(Log& g, long stage, const Tex* t)
{
    int id = t ? t->id : 0;
    if (g.bound[stage] != id) { g.bound[stage] = id; g.last_stage[g.set_texture_calls] = (int)stage; g.last_tex[g.set_texture_calls++] = id; }
    if (g.used < stage + 1) g.used = stage + 1;
}
static void Draw(Log& g, unsigned long vc, int ib, unsigned long start, unsigned long pc, unsigned long type)
{
    g.indices[g.draws] = ib;
    unsigned long* d = g.draw[g.draws++]; d[0] = type; d[1] = 0; d[2] = 0; d[3] = vc; d[4] = start; d[5] = pc;
}
static void Row(Log& g, Prim& p, const Stages& s)
{
    unsigned long n = p.count;
    for (unsigned long i = 0; i < n; ++i) {
        Block& b = p.blocks[i];
        if (!b.degenerate) {
            if (b.change & 1) Attach(g, s.diffuse, b.diffuse);
            if ((b.change & 2) && s.bump != -1) Attach(g, s.bump, b.bump);
            Draw(g, p.vertex_count, p.index_buffer, b.start, b.prim_count, b.strip ? 5 : 4);
        }
    }
}
int main()
{
    Tex t1 = {11}, t2 = {22}, t3 = {33};
    Block blocks[4] = {
        {10, 0, true, false, 1, &t1, &t2},      // diffuse only (flag 1)
        {20, 30, false, true, 3, &t3, &t3},     // degenerate: skipped
        {5, 90, false, false, 3, &t1, &t3},     // diffuse unchanged (no call), bump bound when the bump stage is used
        {7, 120, true, false, 2, &t2, &t2},     // bump only
    };
    Prim p = {64, 7, 4, blocks};
    bool ok = true;
    {
        Log g; memset(&g, 0, sizeof g);
        Stages s = {0, 1};
        Row(g, p, s);
        ok = ok && g.draws == 3 && g.draw[0][0] == 5 && g.draw[0][3] == 64 && g.draw[0][4] == 0 && g.draw[0][5] == 10;
        ok = ok && g.draw[1][0] == 4 && g.draw[1][4] == 90 && g.draw[1][5] == 5 && g.draw[2][0] == 5 && g.draw[2][5] == 7;
        ok = ok && g.indices[2] == 7;
        ok = ok && g.set_texture_calls == 3 && g.last_stage[0] == 0 && g.last_tex[0] == 11 && g.last_stage[1] == 1 && g.last_tex[1] == 33
                && g.last_stage[2] == 1 && g.last_tex[2] == 22 && g.used == 2;
    }
    {
        Log g; memset(&g, 0, sizeof g);
        Stages s = {0, -1};                    // no bump stage: flag 2 binds nothing
        Row(g, p, s);
        ok = ok && g.draws == 3 && g.set_texture_calls == 1 && g.used == 1;
    }
    {
        Log g; memset(&g, 0, sizeof g);
        Stages s = {2, -1};                    // the diffuse stage is used as given
        Row(g, p, s);
        ok = ok && g.set_texture_calls == 1 && g.last_stage[0] == 2 && g.used == 3;
    }
    printf(ok ? "00b9e5b0_TEST PASS\n" : "00b9e5b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
