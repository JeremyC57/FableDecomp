#include <cstdio>
struct M {
    virtual void v00() {}
    void* pool; long handle; unsigned char pad[0x2c]; char cmp; char* hp; unsigned long hn; bool hd; char cmp2; char* dob; unsigned long dn; bool dd; unsigned char pad2[0x1c]; char* names;
    virtual void Relocate(long h, void* o, void* n, long) {
        if (h != handle) return;
        if (names) names = names - (char*)o + (char*)n;
        if (hp) hp = hp - (char*)o + (char*)n;
        if (dob) dob = dob - (char*)o + (char*)n;
    }
};
int main()
{
    char a[64], b[64]; M m; m.handle = 3; m.names = a + 1; m.hp = a + 2; m.dob = 0;
    m.Relocate(4, a, b, 64);
    if (m.names != a + 1 || m.hp != a + 2) { std::printf("00a2e820_TEST FAIL\n"); return 1; }
    m.Relocate(3, a, b, 64);
    if (m.names != b + 1 || m.hp != b + 2 || m.dob != 0) { std::printf("00a2e820_TEST FAIL\n"); return 1; }
    std::printf("00a2e820_TEST PASS\n");
    return 0;
}