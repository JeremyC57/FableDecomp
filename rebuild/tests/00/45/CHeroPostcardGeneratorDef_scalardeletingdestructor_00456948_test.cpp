#include <cstdio>

struct CHeroPostcardGeneratorDef {
    int f0;
    int Parent;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CHeroPostcardGeneratorDef_dtor(CHeroPostcardGeneratorDef* def) { (void)def; g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CHeroPostcardGeneratorDef_scalar_deleting_destructor(CHeroPostcardGeneratorDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CHeroPostcardGeneratorDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}

int main()
{
    CHeroPostcardGeneratorDef obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CHeroPostcardGeneratorDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    r = CHeroPostcardGeneratorDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL delete\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456948_TEST PASS\n");
    return 0;
}