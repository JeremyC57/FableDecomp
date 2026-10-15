#include <cstdio>
#include <cstring>
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct CPrimitive { int id; };
struct CPrimitiveVector {
    CPrimitive** _M_start; CPrimitive** _M_finish; CPrimitive** _M_end_of_storage;
    void clear() { CPrimitive** i = (CPrimitive**)CopyTrivial(_M_finish, _M_finish, _M_start); _M_finish = i; }
};
struct CAppearanceListener { virtual ~CAppearanceListener() { } virtual void OnAppearanceClear() = 0; };
struct CTCBase { virtual ~CTCBase() { } int m_x; };
struct CTCLight : CTCBase, CAppearanceListener {
    CPrimitiveVector FlareSprites, Lights;
    virtual void OnAppearanceClear() { Lights.clear(); FlareSprites.clear(); }
};
int main()
{
    CPrimitive a, b, c; CPrimitive* store1[4] = { &a, &b, 0, 0 }; CPrimitive* store2[4] = { &c, 0, 0, 0 };
    CTCLight l;
    l.FlareSprites._M_start = store1; l.FlareSprites._M_finish = store1 + 2; l.FlareSprites._M_end_of_storage = store1 + 4;
    l.Lights._M_start = store2; l.Lights._M_finish = store2 + 1; l.Lights._M_end_of_storage = store2 + 4;
    CAppearanceListener* listener = &l;
    listener->OnAppearanceClear();
    if (l.FlareSprites._M_finish != store1 || l.Lights._M_finish != store2 || l.Lights._M_end_of_storage != store2 + 4) { std::printf("00641210_TEST FAIL\n"); return 1; }
    listener->OnAppearanceClear();
    if (l.FlareSprites._M_finish != store1) { std::printf("00641210_TEST FAIL\n"); return 1; }
    std::printf("00641210_TEST PASS\n");
    return 0;
}
