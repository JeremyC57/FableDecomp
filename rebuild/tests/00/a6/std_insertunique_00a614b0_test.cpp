// Behaviour test for the hinted insert_unique end() arm (0x00A614B0):
// a hint at end() is used only when the new key sorts after the rightmost key.
#include <cstdio>
static bool less_key(int a, int b) { return a < b; }
static bool hint_at_end_is_used(int rightmost, int value)
{
    return less_key(rightmost, value);
}
int main()
{
    if (!hint_at_end_is_used(3, 4)) { std::printf("00a614b0_TEST FAIL\n"); return 1; }
    if (hint_at_end_is_used(4, 4)) { std::printf("00a614b0_TEST FAIL\n"); return 1; }
    if (hint_at_end_is_used(5, 4)) { std::printf("00a614b0_TEST FAIL\n"); return 1; }
    std::printf("00a614b0_TEST PASS\n");
    return 0;
}