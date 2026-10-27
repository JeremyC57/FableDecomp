// Behaviour test for 0x00806800: __copy_ptrs (true_type): memmove copy returning the destination end
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

#include <string.h>
static long* CopyPtrs(long* first, long* last, long* result)
{
    if (last == first) return result;
    memmove(result, first, (char*)last - (char*)first);
    return (long*)((char*)result + ((char*)last - (char*)first));
}
int main()
{
    long a[6] = { 1, 2, 3, 4, 5, 6 };
    long* e = CopyPtrs(a, a + 3, a + 2); CHECK(e == a + 5 && a[2] == 1 && a[3] == 2 && a[4] == 3);
    CHECK(CopyPtrs(a, a, a + 1) == a + 1);
    if (!g_fail) printf("00806800_TEST PASS\n");
    return g_fail;
}