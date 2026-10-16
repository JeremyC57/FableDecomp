// behaviour test for C3DSkeletalMorph::PeekCombinedMorph (0x00A45260)
#include <cstdio>
struct Target { float acc; int combines; void Combine(const Target& t, float w) { acc += t.acc * w; ++combines; } };
struct Shared { Target Positive, Negative; };
struct Param { float CurrentValue; Shared* SharedData; };
struct Morph {
    Param** params; unsigned long count; Target* Combined; int created; Target storage;
    Target* Peek()
    {
        if (!Combined) {
            storage.acc = 0; storage.combines = 0; Combined = &storage; ++created;
            for (unsigned long i = 0; i < count; ++i) {
                Param* p = params[i]; float v = p->CurrentValue;
                if (v >= 0.0f) Combined->Combine(p->SharedData->Positive, v);
                else Combined->Combine(p->SharedData->Negative, -v);
            }
        }
        return Combined;
    }
};
int main()
{
    Shared s = { { 2.0f, 0 }, { 10.0f, 0 } };
    Param a = { 0.5f, &s }, b = { -0.25f, &s };
    Param* list[2] = { &a, &b };
    Morph m = { list, 2, 0, 0 };
    Target* t = m.Peek(); Target* t2 = m.Peek();
    bool ok = t == t2 && m.created == 1 && t->combines == 2 && t->acc == 1.0f + 2.5f;
    if (!ok) { std::printf("00a45260_TEST FAIL\n"); return 1; }
    std::printf("00a45260_TEST PASS\n");
    return 0;
}
