// behaviour test for 0x004ea3c3 std::vector<CEngineGraphic >::resize(n, x): shrinking erases [begin() + n, end()); growing (or the same
// size) inserts n - size() copies of x at end(); the element is 0xE bytes
#include <cstdio>
static int gErase = 0, gInsert = 0, gLastCount = -1, gLastPos = -1, gLastLast = -1;
struct VecE {
    int Size;                                              // stands for (finish - start) / 0xE
    void Erase(int first, int last) { ++gErase; gLastPos = first; gLastLast = last; Size -= last - first; }
    void FillInsert(int pos, int n) { ++gInsert; gLastPos = pos; gLastCount = n; Size += n; }
    void Resize(int n)
    {
        if (n < Size) Erase(n, Size);
        else FillInsert(Size, n - Size);
    }
};
int main()
{
    int fails = 0;
    VecE v = { 7 };
    v.Resize(3);
    if (gErase != 1 || gLastPos != 3 || gLastLast != 7 || v.Size != 3) fails |= 1;
    v.Resize(10);
    if (gInsert != 1 || gLastPos != 3 || gLastCount != 7 || v.Size != 10) fails |= 2;
    v.Resize(10);
    if (gInsert != 2 || gLastCount != 0 || gErase != 1 || v.Size != 10) fails |= 4;
    v.Resize(0);
    if (gErase != 2 || gLastPos != 0 || v.Size != 0) fails |= 8;
    if (fails) { std::printf("004ea3c3_TEST FAIL %d\n", fails); return 1; }
    std::printf("004ea3c3_TEST PASS\n");
    return 0;
}