#include <cstdio>
// Behaviour model of CTCLight::SetOverridden (0x00640DF0): leaving the overridden state releases the counted light
// info; entering it allocates a fresh one.
struct Info { unsigned long ref; };
static int g_deleted = 0;
struct Light {
    void* data; Info* info; bool overridden;
    void Release() { if (info) { if (--info->ref == 0) { g_deleted++; delete info; } } info = 0; data = 0; }
    void Reset(void* d) { Release(); data = d; info = new Info; info->ref = 1; }
    void SetOverridden(bool o)
    {
        if (overridden != o) {
            if (overridden) Release();
            overridden = o;
            if (o) Reset(this);
        }
    }
};
int main()
{
    Light l = { 0, 0, false };
    l.SetOverridden(true);
    if (!l.overridden || !l.info || l.data != &l) { std::printf("00640df0_TEST FAIL\n"); return 1; }
    l.SetOverridden(true);
    if (g_deleted != 0) { std::printf("00640df0_TEST FAIL\n"); return 1; }
    l.SetOverridden(false);
    if (l.overridden || l.info || l.data || g_deleted != 1) { std::printf("00640df0_TEST FAIL\n"); return 1; }
    std::printf("00640df0_TEST PASS\n");
    return 0;
}
