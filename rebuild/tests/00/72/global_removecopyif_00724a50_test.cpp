// Behaviour test for 0x00724A50 remove_copy_if(CThing**, CThing**, CThing**, CIsCreatureIsIntersecting).
#include <cstdio>
class CThing { public: int hit; };
class CIsCreatureIsIntersecting { public:
    CThing* Owner;
    bool operator()(CThing* thing) const { return thing->hit == Owner->hit; }
};
static CThing** remove_copy_if_(CThing** first, CThing** last, CThing** result, CIsCreatureIsIntersecting pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}
int main()
{
    CThing t[6];
    int hits[6] = { 1, 0, 1, 1, 0, 0 };
    CThing* p[6];
    for (int i = 0; i < 6; ++i) { t[i].hit = hits[i]; p[i] = &t[i]; }
    CThing owner; owner.hit = 1;
    CIsCreatureIsIntersecting pred; pred.Owner = &owner;
    CThing** e = remove_copy_if_(p, p + 6, p, pred);
    if (e != p + 3 || p[0] != &t[1] || p[1] != &t[4] || p[2] != &t[5]) { std::printf("00724a50_TEST FAIL\n"); return 1; }
    CThing* out[1];
    if (remove_copy_if_(p, p, out, pred) != out) { std::printf("00724a50_TEST FAIL\n"); return 1; }
    std::printf("00724a50_TEST PASS\n");
    return 0;
}