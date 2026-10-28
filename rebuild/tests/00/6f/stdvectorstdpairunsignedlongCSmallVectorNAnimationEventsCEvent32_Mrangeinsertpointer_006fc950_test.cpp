// Behaviour test for 0x006FC950 vector<pair<unsigned long, CSmallVector<CEvent,32> > >::_M_range_insert: both in-place
// branches and the reallocation branch insert [first, last) before position.
#include <cstdio>
struct Vec { int Data[32]; int Size; int Capacity; };
void RangeInsert(Vec& v, int pos, const int* first, const int* last)
{
    if (first == last) return;
    unsigned int n = (unsigned int)(last - first);
    if ((unsigned int)(v.Capacity - v.Size) >= n) {
        unsigned int elems_after = v.Size - pos;
        int old_finish = v.Size;
        if (elems_after > n) {
            for (unsigned int i = 0; i < n; ++i) v.Data[v.Size + i] = v.Data[v.Size - n + i];
            v.Size += n;
            for (int i = old_finish - (int)n - 1; i >= pos; --i) v.Data[i + n] = v.Data[i];
            for (unsigned int i = 0; i < n; ++i) v.Data[pos + i] = first[i];
        } else {
            const int* mid = first + elems_after;
            for (const int* p = mid; p != last; ++p) v.Data[v.Size++] = *p;
            for (int i = pos; i < old_finish; ++i) v.Data[v.Size++] = v.Data[i];
            for (const int* p = first; p != mid; ++p) v.Data[pos + (p - first)] = *p;
        }
    } else {
        int tmp[32]; int k = 0;
        for (int i = 0; i < pos; ++i) tmp[k++] = v.Data[i];
        for (const int* p = first; p != last; ++p) tmp[k++] = *p;
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
    const int src[] = { 8, 9 };
    Vec a = { { 1, 2, 3, 4, 5 }, 5, 10 };
    RangeInsert(a, 1, src, src + 2);
    const int wa[] = { 1, 8, 9, 2, 3, 4, 5 };
    Vec b = { { 1, 2, 3 }, 3, 10 };
    RangeInsert(b, 2, src, src + 2);
    const int wb[] = { 1, 2, 8, 9, 3 };
    Vec c = { { 1, 2 }, 2, 2 };
    RangeInsert(c, 1, src, src + 2);
    const int wc[] = { 1, 8, 9, 2 };
    bool ok = Check(a, wa, 7) && Check(b, wb, 5) && Check(c, wc, 4);
    if (!ok) { std::printf("006fc950_TEST FAIL\n"); return 1; }
    std::printf("006fc950_TEST PASS\n");
    return 0;
}