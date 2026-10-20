// Behaviour test for 0x00BA7820 CEnginePrimitiveManagerStaticMeshesBase::CreateInternalPrimitive (model copy).
#include <cstdio>
#include <cstdlib>

struct CPrim {
    long type; unsigned long flags; long kind;
    long m_mesh, m_material, m_animation, sim, effect; bool a, b, c;
    CPrim(long t, unsigned long f, long k) : type(t), flags(f), kind(k), m_mesh(0), m_material(0), m_animation(0),
        sim(0), effect(0), a(false), b(false), c(false) { }
};

static CPrim* Create(long type)
{
    switch (type) {
    case 8:  return new CPrim(8, 0xa, 1);
    case 10: return new CPrim(0xa, 8, 2);
    case 12: return new CPrim(0xc, 8, 3);
    }
    return 0;
}

int main()
{
    CPrim* s = Create(8); CPrim* m = Create(10); CPrim* f = Create(12);
    if (!s || s->kind != 1 || s->flags != 0xa || !m || m->kind != 2 || m->type != 0xa || !f || f->kind != 3 ||
        f->type != 0xc || f->m_mesh != 0 || f->c || Create(9) != 0 || Create(0) != 0 || Create(14) != 0)
    { std::printf("00ba7820_TEST FAIL\n"); return 1; }
    delete s; delete m; delete f;
    std::printf("00ba7820_TEST PASS\n");
    return 0;
}