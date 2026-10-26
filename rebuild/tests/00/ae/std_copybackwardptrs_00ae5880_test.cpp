// behaviour test for 0x00AE5880 std::__copy_backward_ptrs (C3DQuad2*): n = last - first elements copied back to front into the
// range ending at result (safe for an overlapping right shift), both words of every element moved, result - n returned;
// an empty range is a no-op
#include <cstdio>
struct TElem { int A; int B; };                   // stand-in for C3DQuad2: two words, compiler-generated block assignment
static TElem* Run(TElem* first, TElem* last, TElem* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
static void Fill(TElem* v, int n) { for (int i = 0; i < n; ++i) { v[i].A = 100 + i; v[i].B = -(100 + i); } }
static int Check()
{
    TElem v[10];
    Fill(v, 10);
    if (Run(v + 2, v + 5, v + 8) != v + 5) return 1;        // disjoint: [2,5) -> [5,8)
    for (int i = 0; i < 10; ++i) {
        int want = (i >= 5 && i < 8) ? 100 + i - 3 : 100 + i;
        if (v[i].A != want || v[i].B != -want) return 2;
    }
    Fill(v, 10);
    if (Run(v + 1, v + 6, v + 8) != v + 3) return 3;        // overlapping right shift by 2: [1,6) -> [3,8)
    int want2[10] = { 100, 101, 102, 101, 102, 103, 104, 105, 108, 109 };
    for (int i = 0; i < 10; ++i) if (v[i].A != want2[i] || v[i].B != -want2[i]) return 4;
    if (Run(v + 4, v + 4, v + 9) != v + 9) return 5;        // empty range
    return 0;
}
int main()
{
    int r = Check();
    if (r) { std::printf("00ae5880_TEST FAIL %d\n", r); return 1; }
    std::printf("00ae5880_TEST PASS\n");
    return 0;
}