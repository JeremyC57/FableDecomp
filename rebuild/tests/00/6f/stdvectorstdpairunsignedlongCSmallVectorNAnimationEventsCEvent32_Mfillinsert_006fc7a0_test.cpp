// Behaviour test for 0x006FC7A0 vector<pair<unsigned long, CSmallVector<CEvent,32> > >::_M_fill_insert: both in-place
// branches (elems_after > n and elems_after <= n) and the reallocation branch give "n copies of x before position".
#include <cstdio>
struct Vec { int Data[32]; int Size; int Capacity; };
void FillInsert(Vec& v, int pos, unsigned int n, int x)
{
    if (n == 0) return;
    if ((unsigned int)(v.Capacity - v.Size) >= n) {
        int x_copy = x;
        unsigned int elems_after = v.Size - pos;
        int old_finish = v.Size;
        if (elems_after > n) {
            for (unsigned int i = 0; i < n; ++i) v.Data[v.Size + i] = v.Data[v.Size - n + i];
            v.Size += n;
            for (int i = old_finish - (int)n - 1; i >= pos; --i) v.Data[i + n] = v.Data[i];
            for (unsigned int i = 0; i < n; ++i) v.Data[pos + i] = x_copy;
        } else {
            for (unsigned int i = 0; i < n - elems_after; ++i) v.Data[v.Size + i] = x_copy;
            v.Size += n - elems_after;
            for (int i = pos; i < old_finish; ++i) v.Data[v.Size + i - pos] = v.Data[i];
            v.Size += elems_after;
            for (int i = pos; i < old_finish; ++i) v.Data[i] = x_copy;
        }
    } else {
        int tmp[32]; int k = 0;
        for (int i = 0; i < pos; ++i) tmp[k++] = v.Data[i];
        for (unsigned int i = 0; i < n; ++i) tmp[k++] = x;
        for (int i = pos; i < v.Size; ++i) tmp[k++] = v.Data[i];
        for (int i = 0; i < k; ++i) v.Data[i] = tmp[i];
        v.Size = k; v.Capacity = k * 2;
    }
}
static bool Check(const Vec& v, const int* want, int n)
{
    if (v.Size != n) return false;
    for (int i = 0; i < n; ++i) if (v.Data[i] != want[i]) return false;
    return true;
}
int main()
{
    Vec a = { { 1, 2, 3, 4, 5 }, 5, 10 };
    FillInsert(a, 1, 2, 9);                  // elems_after 4 > 2
    const int wa[] = { 1, 9, 9, 2, 3, 4, 5 };
    Vec b = { { 1, 2, 3 }, 3, 10 };
    FillInsert(b, 2, 3, 7);                  // elems_after 1 <= 3
    const int wb[] = { 1, 2, 7, 7, 7, 3 };
    Vec c = { { 1, 2 }, 2, 2 };
    FillInsert(c, 0, 2, 5);                  // reallocate
    const int wc[] = { 5, 5, 1, 2 };
    Vec d = { { 1 }, 1, 1 };
    FillInsert(d, 0, 0, 5);
    bool ok = Check(a, wa, 7) && Check(b, wb, 6) && Check(c, wc, 4) && d.Size == 1;
    if (!ok) { std::printf("006fc7a0_TEST FAIL\n"); return 1; }
    std::printf("006fc7a0_TEST PASS\n");
    return 0;
}