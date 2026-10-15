#include <cstdio>
#include <cstdlib>
// Behaviour test for CRetailVector20::_M_clear (0x005C1F1F): destroys [start, finish) then frees the buffer if there is one.
struct SRecord20 { unsigned char bytes[20]; };
struct CRetailAllocator20 {};
static int g_destroyed = 0; static int g_freed = 0;
static void DestroyRange20(SRecord20* first, SRecord20* last, CRetailAllocator20*) { g_destroyed += (int)(last - first); }
static void FreeBlock(void* p) { if (p) { ++g_freed; std::free(p); } }
struct CAllocProxy20 { SRecord20* _M_data; void deallocate(SRecord20* block, unsigned long) { if (block != 0) FreeBlock(block); } };
struct CRetailVector20 {
    SRecord20* _M_start; SRecord20* _M_finish; CAllocProxy20 _M_end_of_storage;
    void _M_clear() { CRetailAllocator20 a; DestroyRange20(_M_start, _M_finish, &a); _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
int main()
{
    CRetailVector20 v; v._M_start = (SRecord20*)std::malloc(3 * sizeof(SRecord20)); v._M_finish = v._M_start + 2; v._M_end_of_storage._M_data = v._M_start + 3;
    v._M_clear();
    CRetailVector20 e = { 0, 0, { 0 } }; e._M_clear();
    int ok = g_destroyed == 2 && g_freed == 1;
    std::printf(ok ? "AUTO_TINY_005c1f1f_TEST PASS\n" : "AUTO_TINY_005c1f1f_TEST FAIL\n"); return ok ? 0 : 1;
}
