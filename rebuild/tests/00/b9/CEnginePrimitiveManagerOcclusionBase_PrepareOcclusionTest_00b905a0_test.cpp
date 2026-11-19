// Behaviour test for CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest (0x00B905A0), a portable model of the row. With occlusion
// enabled, the combined pass (2), a current layer and an occlusion record for it, the row allocates a query (index = used count, at
// most 0x2000, else -1). It writes the handle out and, if valid, records it in the info. The info's handle list is cleared first when
// it was filled in an earlier frame; the info's test frame then becomes the current frame. Otherwise the handle is -1.
#include <stdio.h>
#include <vector>
struct Info { std::vector<unsigned long> handles; unsigned long test_frame; };
struct Settings { bool enable; long layer; } g_settings;
struct Pool { unsigned long frame, used; } g_pool;
struct Prim { Info* info[2]; };
static void PrepareOcclusionTest(Prim& primitive, int render_pass, unsigned long& out_occlusion_handle)
{
    if (g_settings.enable && render_pass == 2 && g_settings.layer != -1 && primitive.info[g_settings.layer])
    {
        Info& occlusion_info = *primitive.info[g_settings.layer];
        out_occlusion_handle = g_pool.used < 0x2000 ? g_pool.used++ : 0xffffffff;
        if (out_occlusion_handle != 0xffffffff)
        {
            if (g_pool.frame != occlusion_info.test_frame) occlusion_info.handles.clear();
            occlusion_info.handles.push_back(out_occlusion_handle);
            occlusion_info.test_frame = g_pool.frame;
        }
    }
    else
        out_occlusion_handle = 0xffffffff;
}
int main()
{
    int bad = 0; unsigned long h = 7;
    Info info; info.test_frame = 0; info.handles.push_back(99);
    Prim p = { { &info, 0 } };
    g_settings.enable = false; g_settings.layer = 0; g_pool.frame = 5; g_pool.used = 3;
    PrepareOcclusionTest(p, 2, h); if (h != 0xffffffff) ++bad;          // disabled
    g_settings.enable = true;
    PrepareOcclusionTest(p, 1, h); if (h != 0xffffffff) ++bad;          // wrong pass
    g_settings.layer = 1;
    PrepareOcclusionTest(p, 2, h); if (h != 0xffffffff) ++bad;          // no record on that layer
    g_settings.layer = 0;
    PrepareOcclusionTest(p, 2, h);                                         // new frame: old handle cleared
    if (h != 3 || g_pool.used != 4 || info.handles.size() != 1 || info.handles[0] != 3 || info.test_frame != 5) ++bad;
    PrepareOcclusionTest(p, 2, h);                                         // same frame: appended
    if (h != 4 || info.handles.size() != 2) ++bad;
    g_pool.used = 0x2000;
    PrepareOcclusionTest(p, 2, h); if (h != 0xffffffff || info.handles.size() != 2) ++bad;   // pool full
    printf(bad ? "00b905a0_TEST FAIL\n" : "00b905a0_TEST PASS\n");
    return bad ? 1 : 0;
}