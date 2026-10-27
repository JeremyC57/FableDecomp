// behaviour test for 0x00431DF0: a new definition has no manager, cleared flags, no name, an empty map and an empty vector
#include <cstdio>
#include <cstring>
#pragma pack(push, 1)
struct D { unsigned int vt; unsigned char res[0x18]; unsigned int mgr; unsigned int gi; unsigned char flags; int name; unsigned int m[3]; unsigned char cmp, dirty; unsigned char pad[5]; unsigned int v[3];
    D() { std::memset(this, 0xAB, sizeof(*this)); mgr = 0; flags &= 0xF8; name = -1; m[0] = m[1] = m[2] = 0; dirty = 0; v[0] = v[1] = v[2] = 0; } };
#pragma pack(pop)
int main()
{
    D d;
    if ((char*)&d.v[0] - (char*)&d != 0x3c || d.mgr || (d.flags & 7) || d.name != -1 || d.m[2] || d.dirty || d.v[0] || d.v[2] || d.gi != 0xABABABAB) { std::printf("00431df0_TEST FAIL\n"); return 1; }
    std::printf("00431df0_TEST PASS\n");
    return 0;
}