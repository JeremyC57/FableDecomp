#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SPair { long m_type; long m_count; }; static long g_pushes = 0; static SPair g_pair; struct CVec { void PushBack(const SPair& p) { ++g_pushes; g_pair = p; } };
struct CItem { long ItemType; long NumberOfItems; void* a; void* b; long AverageBoughtPrice; void GetItemThings(CVec* out) { if (NumberOfItems > 0) { SPair pair; pair.m_type = ItemType; pair.m_count = NumberOfItems; out->PushBack(pair); } } };

int main() {
    CItem i; i.ItemType = 5; i.NumberOfItems = 2; CVec v; i.GetItemThings(&v); if (g_pushes != 1 || g_pair.m_type != 5 || g_pair.m_count != 2) { std::printf("005bf03c_TEST FAIL\n"); return 1; } i.NumberOfItems = 0; i.GetItemThings(&v); if (g_pushes != 1) { std::printf("005bf03c_TEST FAIL\n"); return 1; }
    std::printf("005bf03c_TEST PASS\n"); return 0;
}
