// Behaviour test for libvorbis floor1.c post_Y (0x00CA26A0): merge two fitted posts (the other when one is negative).
#include <cstdio>
static int post_Y(int* A, int* B, int pos)
{
    if (A[pos] < 0) return B[pos];
    if (B[pos] < 0) return A[pos];
    return (A[pos] + B[pos]) >> 1;
}
int main()
{
    int A[3] = { -1, 10, 7 }, B[3] = { 5, -1, 4 };
    if (post_Y(A, B, 0) != 5 || post_Y(A, B, 1) != 10 || post_Y(A, B, 2) != 5) { std::printf("00ca26a0_TEST FAIL\n"); return 1; }
    std::printf("00ca26a0_TEST PASS\n"); return 0;
}