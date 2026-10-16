#include <cstdio>
struct CStr { unsigned int bytes; unsigned int Sizeof() const { return 4 + bytes; } };
struct CData { unsigned int Sizeof() const { return 100; } };
struct CTextEntry {
    CStr a, b, c, t; unsigned int cap; CData* data;
    unsigned int Sizeof() const {
        unsigned int size = 0x44;
        size += a.Sizeof() - 4; size += b.Sizeof() - 4; size += c.Sizeof() - 4; size += t.Sizeof() - 4;
        size += cap * 8;
        size += (data ? data->Sizeof() + 8 : 8) - 8;
        return size;
    }
};
int main()
{
    CData d;
    CTextEntry e = { {1}, {2}, {3}, {4}, 5, 0 };
    if (e.Sizeof() != 0x44 + 10 + 40) { std::printf("009c7d90_TEST FAIL\n"); return 1; }
    e.data = &d;
    if (e.Sizeof() != 0x44 + 10 + 40 + 100) { std::printf("009c7d90_TEST FAIL\n"); return 1; }
    std::printf("009c7d90_TEST PASS\n");
    return 0;
}