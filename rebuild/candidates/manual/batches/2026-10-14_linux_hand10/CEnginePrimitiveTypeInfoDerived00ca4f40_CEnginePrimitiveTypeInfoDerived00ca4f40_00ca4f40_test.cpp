#include <cstdio>
// Model of the primitive type-info derived constructor @ 0x00CA4F40: Base(0, owner, &owner->block, b, c), m_pOwner = owner.
struct COwnerBlock { unsigned char _pad[0x7c]; unsigned char m_block[4]; };
static long g_zero, g_b, g_c; static COwnerBlock* g_owner; static void* g_block;
struct Base { Base(long z, COwnerBlock* o, void* blk, long b, long c) { g_zero = z; g_owner = o; g_block = blk; g_b = b; g_c = c; } virtual void f0() {} };
struct Mid : Base { COwnerBlock* m_pOwner; Mid(COwnerBlock* o, long b, long c) : Base(0, o, o->m_block, b, c), m_pOwner(o) {} };
struct Derived : Mid { virtual void f0() {} Derived(COwnerBlock* o, long b, long c) : Mid(o, b, c) {} };
int main()
{
    COwnerBlock owner; Derived d(&owner, 5, 6);
    if (g_zero != 0 || g_owner != &owner || g_block != owner.m_block || g_b != 5 || g_c != 6 || d.m_pOwner != &owner) { std::printf("00ca4f40_TEST FAIL\n"); return 1; }
    std::printf("00ca4f40_TEST PASS\n"); return 0;
}
