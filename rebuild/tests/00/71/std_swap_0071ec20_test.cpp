// Behaviour test for 0x0071EC20 std::swap of a 12-byte weighted morph entry.
#include <cstdio>

struct SEntry { int MorphName; short Priority; bool Supresses; };
struct SWeighted { SEntry first; float second; };
template <class T> static void Swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }
int main()
{
    SWeighted a = { { 1, 2, true }, 0.25f }, b = { { 7, 8, false }, 4.0f };
    Swap(a, b);
    bool ok = a.first.MorphName == 7 && a.first.Priority == 8 && !a.first.Supresses && a.second == 4.0f
        && b.first.MorphName == 1 && b.first.Priority == 2 && b.first.Supresses && b.second == 0.25f;
    Swap(a, a);
    ok = ok && a.first.MorphName == 7;
    if (!ok) { std::printf("0071ec20_TEST FAIL\n"); return 1; }
    std::printf("0071ec20_TEST PASS\n");
    return 0;
}