// Behaviour test for 0x00818180 __uninitialized_copy<28-byte particle-morph record> (model copy): the copies add-ref
// both definition pointers, null pointers are skipped.
#include <cstdio>
#include <new>
struct CRefCountedDef { int vptr_slot; int RefCount; };
struct CRefDefPointer {
    CRefCountedDef* PDef;
    CRefDefPointer() : PDef(0) { }
    CRefDefPointer(const CRefDefPointer& o) : PDef(o.PDef) { if (PDef) ++PDef->RefCount; }
};
struct R {
    int id; CRefDefPointer def0; CRefDefPointer def1; int m_0c; int m_10; bool m_14; int m_18;
};
static R* uninitialized_copy_model(R* first, R* last, R* result)
{
    R* cur = result;
    for (; first != last; ++first, ++cur) new (cur) R(*first);
    return cur;
}
int main()
{
    CRefCountedDef a = { 0, 1 }, b = { 0, 5 };
    R src[3];
    int i;
    for (i = 0; i < 3; ++i) { src[i].id = 10 + i; src[i].m_0c = i; src[i].m_10 = -i; src[i].m_14 = (i & 1) != 0; src[i].m_18 = 100 * i; }
    src[0].def0.PDef = &a; src[0].def1.PDef = &b;
    src[1].def0.PDef = &a;                         // def1 null
    src[2].def1.PDef = &b;                         // def0 null
    double raw[(sizeof(R) * 3) / sizeof(double) + 1];
    R* dst = (R*)raw;
    R* end = uninitialized_copy_model(src, src + 3, dst);
    int ok = (end == dst + 3);
    if (a.RefCount != 3 || b.RefCount != 7) ok = 0;
    for (i = 0; i < 3; ++i)
        if (dst[i].id != 10 + i || dst[i].m_0c != i || dst[i].m_10 != -i || dst[i].m_14 != ((i & 1) != 0) || dst[i].m_18 != 100 * i ||
            dst[i].def0.PDef != src[i].def0.PDef || dst[i].def1.PDef != src[i].def1.PDef) ok = 0;
    if (uninitialized_copy_model(src, src, dst) != dst) ok = 0;
    if (!ok) { std::printf("00818180_TEST FAIL\n"); return 1; }
    std::printf("00818180_TEST PASS\n");
    return 0;
}