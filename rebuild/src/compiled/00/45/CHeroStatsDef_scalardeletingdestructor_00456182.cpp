#pragma optimize("s",on)
#include <new>
struct CHeroStatsDef { int f0; };
extern void __fastcall CHeroStatsDef_dtor(CHeroStatsDef* def);

void* __fastcall CHeroStatsDef_scalar_deleting_destructor(CHeroStatsDef* def, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CHeroStatsDef_dtor(def);
    if (flags & 1)
        ::operator delete((void*)def);
    return def;
}