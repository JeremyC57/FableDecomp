// Behaviour test for 0x00A7E440 for_each(CSaveNavigationPositionToFile): each position is written as 12 bytes
// (position - MapPos, layer) with size 12 and async false; the returned functor keeps the file and origin.
#include <cstdio>
#include <cstring>
struct C2DVector { float X, Y; };
struct CNavigationPosition { C2DVector Position; int Layer; };
class CFile {
public:
    unsigned char buffer[64]; unsigned int used; int calls; bool lastAsync;
    CFile() : used(0), calls(0), lastAsync(true) { }
    virtual void Write(const void* data, unsigned int size, bool async)
    {
        std::memcpy(buffer + used, data, size); used += size; ++calls; lastAsync = async;
    }
};
struct CSave {
    CFile* File; C2DVector MapPos;
    void operator()(const CNavigationPosition& pos) const
    {
        CNavigationPosition relative;
        relative.Position.X = pos.Position.X - MapPos.X;
        relative.Position.Y = pos.Position.Y - MapPos.Y;
        relative.Layer = pos.Layer;
        File->Write(&relative, sizeof(relative), false);
    }
};
template <class It, class F> F ForEach(It first, It last, F f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
int main()
{
    CFile file;
    CNavigationPosition p[2] = { { { 10.0f, 20.0f }, 3 }, { { -4.0f, 1.5f }, 0 } };
    CSave s; s.File = &file; s.MapPos.X = 2.0f; s.MapPos.Y = 0.5f;
    CSave r = ForEach((const CNavigationPosition*)p, (const CNavigationPosition*)p + 2, s);
    CNavigationPosition out[2];
    std::memcpy(out, file.buffer, sizeof(out));
    if (file.calls != 2 || file.used != 24 || file.lastAsync || r.File != &file || r.MapPos.X != 2.0f) { std::printf("00a7e440_TEST FAIL\n"); return 1; }
    if (out[0].Position.X != 8.0f || out[0].Position.Y != 19.5f || out[0].Layer != 3 || out[1].Position.X != -6.0f || out[1].Position.Y != 1.0f) {
        std::printf("00a7e440_TEST FAIL\n"); return 1;
    }
    std::printf("00a7e440_TEST PASS\n");
    return 0;
}