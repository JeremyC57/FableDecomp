#include <cstdio>
class CCameraModeDef { public: int f0; void* vf; };
static int g_dtor_calls = 0;
static int g_del_calls = 0;
static void __fastcall CCameraModeDef_dtor(CCameraModeDef* mode){ (void)mode; ++g_dtor_calls; }
static void operator_delete_stub(void* p){ (void)p; ++g_del_calls; }
static void* __fastcall CCameraModeDef_scalar_deleting_destructor(CCameraModeDef* mode, int edx_unused, unsigned int flags)
{
    CCameraModeDef_dtor(mode);
    if (flags & 1)
        operator_delete_stub(mode);
    return mode;
}
int main(){
    CCameraModeDef obj; obj.f0 = 7; obj.vf = 0;
    g_dtor_calls = 0; g_del_calls = 0;
    void* r = CCameraModeDef_scalar_deleting_destructor(&obj, 0,0);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 0){ std::printf("FAIL noflag\n"); return 1; }
    g_dtor_calls = 0; g_del_calls = 0;
    r = CCameraModeDef_scalar_deleting_destructor(&obj, 0,1);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 1){ std::printf("FAIL flag\n"); return 1; }
    g_dtor_calls = 0; g_del_calls = 0;
    r = CCameraModeDef_scalar_deleting_destructor(&obj, 0,2);
    if (r != &obj || g_dtor_calls != 1 || g_del_calls != 0){ std::printf("FAIL evenflag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456779_TEST PASS\n");
    return 0;
}