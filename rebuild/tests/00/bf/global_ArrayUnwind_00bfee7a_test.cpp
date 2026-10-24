// Behaviour test for 0x00BFEE7A __ArrayUnwind: destroys count elements backwards from ptr, element by element.
#include <cstdio>
struct Elem { int id; };
static int g_order[8]; static int g_n = 0;
static void DestroyElem(void* p) { g_order[g_n++] = ((Elem*)p)->id; }
static void ArrayUnwind(void* ptr, unsigned int size, int count, void (*pDtor)(void*))
{
    while (--count >= 0) { ptr = (char*)ptr - size; (*pDtor)(ptr); }
}
int main()
{
    Elem a[4] = { {0}, {1}, {2}, {3} };
    ArrayUnwind(a + 3, sizeof(Elem), 3, DestroyElem);     // elements 2, 1, 0 were built
    if (g_n != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 0) { std::printf("00bfee7a_TEST FAIL\n"); return 1; }
    ArrayUnwind(a, sizeof(Elem), 0, DestroyElem);
    if (g_n != 3) { std::printf("00bfee7a_TEST FAIL\n"); return 1; }
    std::printf("00bfee7a_TEST PASS\n");
    return 0;
}
