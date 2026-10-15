#include <cstdio>

static int g_calls = 0; static bool g_suspend = true; static long g_reason = -1;

struct CThing {
    unsigned char m_type;
    unsigned char Dead : 1;
    unsigned char _flags : 7;
    void SetSuspendAnimation(bool suspend, long reason) { ++g_calls; g_suspend = suspend; g_reason = reason; }
};

struct CTCForcePushed {
    CThing* ParentThing;
    void vfunc_2()
    {
        CThing* thing = ParentThing;
        if (thing && !thing->Dead && thing->m_type == 1)
            thing->SetSuspendAnimation(false, 0);
    }
};

int main()
{
    CThing t; t.m_type = 1; t.Dead = 0; t._flags = 0;
    CTCForcePushed c; c.ParentThing = 0;
    c.vfunc_2();
    if (g_calls != 0) { std::printf("00798540_TEST FAIL\n"); return 1; }
    c.ParentThing = &t; t.Dead = 1; c.vfunc_2();
    if (g_calls != 0) { std::printf("00798540_TEST FAIL\n"); return 1; }
    t.Dead = 0; t.m_type = 2; c.vfunc_2();
    if (g_calls != 0) { std::printf("00798540_TEST FAIL\n"); return 1; }
    t.m_type = 1; c.vfunc_2();
    if (g_calls != 1 || g_suspend || g_reason != 0) { std::printf("00798540_TEST FAIL\n"); return 1; }
    std::printf("00798540_TEST PASS\n");
    return 0;
}