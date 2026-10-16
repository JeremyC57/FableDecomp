#include <cstdio>
// Behaviour model of CTCHeroMorph::Copy (0x0071CD90): counted-pointer reassignment and morph value copy.
struct Info { unsigned int refs; int deleted; };
struct Morph { void* data; Info* info; long frame; long idle; float w[8]; bool teen; bool changed;
    void Copy(const Morph& o)
    {
        void* d = o.data; Info* i = o.info;
        if (info != i) {
            if (info) { --info->refs; if (info->refs == 0) info->deleted = 1; }
            data = d; info = i; if (info) ++info->refs;
        }
        frame = o.frame;
        for (int k = 0; k < 8; ++k) w[k] = o.w[k];
        teen = o.teen; changed = true;
    } };
int main()
{
    Info a = { 1, 0 }, b = { 1, 0 };
    Morph x; x.data = 0; x.info = &a; x.frame = 0; x.idle = 7; x.teen = false; x.changed = false;
    Morph y; y.data = &y; y.info = &b; y.frame = 5; y.idle = 9; y.teen = true; y.changed = false;
    for (int k = 0; k < 8; ++k) { x.w[k] = 0; y.w[k] = (float)k; }
    x.Copy(y);
    if (!a.deleted || b.refs != 2 || x.info != &b || x.data != &y || x.frame != 5 || x.idle != 7 || x.w[7] != 7.0f || !x.teen || !x.changed) { std::printf("0071cd90_TEST FAIL\n"); return 1; }
    std::printf("0071cd90_TEST PASS\n");
    return 0;
}