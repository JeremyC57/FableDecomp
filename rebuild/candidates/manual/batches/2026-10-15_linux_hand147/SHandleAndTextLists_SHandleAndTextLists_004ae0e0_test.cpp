#include <cstdio>
#include <cstdlib>
#include <new>
static int g_text = 0, g_handles = 0, g_frees = 0;
struct Text { ~Text() { ++g_text; } };
struct Handle { virtual ~Handle() { ++g_handles; } void* item; };
struct Entry { long a, b; Text text; long c; };
template <class T> struct Vec { T* s; T* f; T* e;
    ~Vec() { for (T* p = s; p != f; ++p) p->~T(); if (s) { std::free(s); ++g_frees; } } };
struct Lists { long owner, id; Vec<Handle> handles; Vec<Entry> texts; };
int main()
{
    {
        Lists l; l.owner = 0; l.id = 0;
        l.handles.s = (Handle*)std::malloc(2 * sizeof(Handle)); new (&l.handles.s[0]) Handle; new (&l.handles.s[1]) Handle; l.handles.f = l.handles.e = l.handles.s + 2;
        l.texts.s = (Entry*)std::malloc(3 * sizeof(Entry)); l.texts.f = l.texts.e = l.texts.s + 3;
    }
    if (g_text != 3 || g_handles != 2 || g_frees != 2) { std::printf("004ae0e0_TEST FAIL\n"); return 1; }
    { Lists e; e.handles.s = e.handles.f = e.handles.e = 0; e.texts.s = e.texts.f = e.texts.e = 0; }
    if (g_frees != 2) { std::printf("004ae0e0_TEST FAIL\n"); return 1; }
    std::printf("004ae0e0_TEST PASS\n");
    return 0;
}
