// Behaviour test for 0x009D9C80 CRenderManager2D::DrawBufferedPolys: consecutive buffered primitives that share blend mode, texture,
// window and primitive type are drawn in one DrawPrimitiveUP call (lines: 2 vertices each, triangles: 3); a change of any of them
// flushes the pending run first; the tail run is flushed after the loop and the buffers are cleared.
#include <cstdio>
#include <vector>
struct Info { unsigned long BlendMode; int Texture; int Window; bool Line; };
struct Call { int type; long count; long first_vertex; };
static std::vector<Call> g_calls;
static void Flush(bool is_line, long pending, long& vertices_in)
{
    Call c; c.type = is_line ? 2 : 4; c.count = pending; c.first_vertex = vertices_in; g_calls.push_back(c);
    vertices_in += pending * (is_line ? 2 : 3);
}
static void DrawBufferedPolys(std::vector<Info>& tris)
{
    bool is_line = false, toggle_is_line = false;
    unsigned long current_blend_mode = 1; int current_texture = 0; int current_window = 0;
    long vertices_in = 0, c1 = 0, start_tri = 0;
    for (; c1 < (long)tris.size(); ++c1) {
        unsigned long blend_mode = tris[c1].BlendMode;
        bool change_window = current_window != tris[c1].Window;
        int texture = tris[c1].Texture;
        if (is_line != tris[c1].Line) toggle_is_line = true;
        if (blend_mode != current_blend_mode || texture != current_texture || change_window || toggle_is_line) {
            long pending = c1 - start_tri;
            if (pending > 0) { Flush(is_line, pending, vertices_in); start_tri = c1; }
            current_texture = texture;
            current_blend_mode = blend_mode;
            if (change_window) current_window = tris[c1].Window;
            if (toggle_is_line) { is_line = !is_line; toggle_is_line = false; }
        }
    }
    if (c1 - start_tri > 0) { long v = vertices_in; Flush(is_line, c1 - start_tri, v); }
    tris.clear();
}
int main()
{
    std::vector<Info> t;
    Info a = { 1, 0, 0, false }; Info b = { 1, 0, 0, true }; Info c = { 2, 7, 0, true }; Info d = { 2, 7, 3, false };
    t.push_back(a); t.push_back(a); t.push_back(b); t.push_back(b); t.push_back(b); t.push_back(c); t.push_back(d); t.push_back(d);
    DrawBufferedPolys(t);
    if (g_calls.size() != 4 || !t.empty()
        || g_calls[0].type != 4 || g_calls[0].count != 2 || g_calls[0].first_vertex != 0
        || g_calls[1].type != 2 || g_calls[1].count != 3 || g_calls[1].first_vertex != 6
        || g_calls[2].type != 2 || g_calls[2].count != 1 || g_calls[2].first_vertex != 12
        || g_calls[3].type != 4 || g_calls[3].count != 2 || g_calls[3].first_vertex != 14)
    { std::printf("009d9c80_TEST FAIL\n"); return 1; }
    g_calls.clear(); DrawBufferedPolys(t);
    if (!g_calls.empty()) { std::printf("009d9c80_TEST FAIL\n"); return 1; }
    std::printf("009d9c80_TEST PASS\n");
    return 0;
}
