// Behaviour test for the packed-type factory (0x00C1EFC0): each of the twelve codes builds its own product kind from
// the source's slot 0 (kind 1), slot 1 (kind 2) or slots 2 and 1 (kind 3) and hands the product's target the source's
// slot-5 value; codes outside the table build nothing.
#include <cstdio>

struct Source { long v0, v1, v2, v5; };
struct Product { unsigned long type; long a, b; long target; };

static bool Create(unsigned long type, const Source* s, Product* out)
{
    static const unsigned long codes[12] = { 0x10101, 0x20101, 0x10201, 0x20201, 0x10102, 0x20102,
                                             0x10202, 0x20202, 0x10103, 0x20103, 0x10203, 0x20203 };
    bool known = false;
    for (int i = 0; i < 12; ++i)
        if (codes[i] == type)
            known = true;
    if (!known)
        return false;
    out->type = type;
    out->b = 0;
    switch (type & 0xff) {
    case 1: out->a = s->v0; break;
    case 2: out->a = s->v1; break;
    default: out->a = s->v2; out->b = s->v1; break;
    }
    out->target = s->v5;
    return true;
}

int main()
{
    Source s = { 10, 11, 12, 15 };
    Product p;
    if (!Create(0x10101, &s, &p) || p.a != 10 || p.target != 15) { std::printf("00c1efc0_TEST FAIL\n"); return 1; }
    if (!Create(0x20202, &s, &p) || p.a != 11 || p.b != 0) { std::printf("00c1efc0_TEST FAIL\n"); return 1; }
    if (!Create(0x20203, &s, &p) || p.a != 12 || p.b != 11 || p.type != 0x20203) { std::printf("00c1efc0_TEST FAIL\n"); return 1; }
    if (Create(0x10104, &s, &p) || Create(0x30101, &s, &p)) { std::printf("00c1efc0_TEST FAIL\n"); return 1; }
    std::printf("00c1efc0_TEST PASS\n");
    return 0;
}