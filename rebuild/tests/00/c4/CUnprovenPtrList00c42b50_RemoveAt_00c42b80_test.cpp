// Behaviour test for CUnprovenPtrList_00c42b50::RemoveAt (0x00C42B80): removing index i removes the element stored there.
#include <stdio.h>
struct List { int* data[4]; unsigned long count;
    void Remove(int* p) { unsigned long j = 0; while (j < count && data[j] != p) ++j; if (j == count) return; for (; j + 1 < count; ++j) data[j] = data[j + 1]; --count; }
    void RemoveAt(unsigned long index) { Remove(data[index]); } };
int main()
{
    int a, b, c; List l; l.data[0] = &a; l.data[1] = &b; l.data[2] = &c; l.count = 3;
    l.RemoveAt(1);
    bool ok = l.count == 2 && l.data[0] == &a && l.data[1] == &c;
    printf(ok ? "00c42b80_TEST PASS\n" : "00c42b80_TEST FAIL\n");
    return ok ? 0 : 1;
}