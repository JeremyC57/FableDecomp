extern int (__cdecl *g_getCount)();
extern float g_scale_0129c81c;
float SetLungedThisFrame()
{
    return (float)g_getCount() * g_scale_0129c81c;
}