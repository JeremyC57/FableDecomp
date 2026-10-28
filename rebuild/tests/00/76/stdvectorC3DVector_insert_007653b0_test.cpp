// Behaviour test for 0x007653B0 vector<C3DVector>::insert(iterator, const C3DVector&): append at the end, shift-and-assign
// in the middle (including an alias of an element being shifted), and the overflow (grow) branch; returns begin()+index.
#include <cstdio>
struct V3 { float X, Y, Z; };
struct Vec { V3 Data[16]; int Size; int Capacity; };
static int Insert(Vec& v, int pos, const V3& x)
{
    int n = pos;
    if (v.Size != v.Capacity) {
        if (pos == v.Size) { v.Data[v.Size] = x; ++v.Size; }
        else {
            v.Data[v.Size] = v.Data[v.Size - 1]; ++v.Size;
            V3 x_copy = x;
            for (int i = v.Size - 2; i > pos; --i) v.Data[i] = v.Data[i - 1];
            v.Data[pos] = x_copy;
        }
    } else {
        V3 tmp[16]; int k = 0;
        for (int i = 0; i < pos; ++i) tmp[k++] = v.Data[i];
        tmp[k++] = x;
        for (int i = pos; i < v.Size; ++i) tmp[k++] = v.Data[i];
        for (int i = 0; i < k; ++i) v.Data[i] = tmp[i];
        v.Size = k; v.Capacity = k * 2;
    }
    return n;
}
static bool Same(const Vec& v, const float* xs, int n)
{
    if (v.Size != n) return false;
    for (int i = 0; i < n; ++i) if (v.Data[i].X != xs[i] || v.Data[i].Y != xs[i] * 2 || v.Data[i].Z != -xs[i]) return false;
    return true;
}
static V3 Mk(float f) { V3 r = { f, f * 2, -f }; return r; }
int main()
{
    Vec a; a.Size = 0; a.Capacity = 4;
    a.Data[0] = Mk(1); a.Data[1] = Mk(2); a.Size = 2;
    bool ok = Insert(a, 2, Mk(3)) == 2;                       // end
    ok = ok && Insert(a, 1, Mk(9)) == 1;                      // middle
    const float wa[] = { 1, 9, 2, 3 };
    ok = ok && Same(a, wa, 4);
    ok = ok && Insert(a, 0, a.Data[3]) == 0;                  // full: grow, alias of the last element
    const float wb[] = { 3, 1, 9, 2, 3 };
    ok = ok && Same(a, wb, 5) && a.Capacity == 10;
    ok = ok && Insert(a, 1, a.Data[2]) == 1;                  // middle, alias of an element the shift moves
    const float wc[] = { 3, 9, 1, 9, 2, 3 };
    ok = ok && Same(a, wc, 6);
    if (!ok) { std::printf("007653b0_TEST FAIL\n"); return 1; }
    std::printf("007653b0_TEST PASS\n");
    return 0;
}