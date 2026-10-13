#include <cstdio>

static int g_settled = 0;
struct V { unsigned char pad[0xa0]; long Paid; long fine;
    long GetFine() { return fine - Paid; } void Settle() { ++g_settled; }
    void PayFines(long a) { Paid += a; if (GetFine() <= 0) Settle(); } };
int main()
{
    V v; v.Paid = 0; v.fine = 100; v.PayFines(40); if (v.Paid != 40 || g_settled) { std::printf("0065c760_TEST FAIL\n"); return 1; }
    v.PayFines(60); if (g_settled != 1) { std::printf("0065c760_TEST FAIL\n"); return 1; }
    std::printf("0065c760_TEST PASS\n");
    return 0;
}