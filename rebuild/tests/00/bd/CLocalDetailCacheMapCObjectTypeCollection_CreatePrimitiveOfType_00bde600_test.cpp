#include <cstdio>
#include <cstdlib>
struct Owner { int allocs; };
struct Base { int kind; const void* type; void* map; virtual ~Base() { } };
static void* PoolNew(unsigned int size, Owner* o) { ++o->allocs; return std::malloc(size); }
struct Mesh : Base { char d[0x40]; Mesh(const void* t, void* m) { kind = 0; type = t; map = m; } static void* operator new(size_t s, Owner* o) { return PoolNew((unsigned int)s, o); } static void operator delete(void* p, Owner*) { std::free(p); } static void operator delete(void* p) { std::free(p); } };
struct Repeated : Base { char d[0x30]; Repeated(const void* t, void* m) { kind = 1; type = t; map = m; } static void* operator new(size_t s, Owner* o) { return PoolNew((unsigned int)s, o); } static void operator delete(void* p, Owner*) { std::free(p); } static void operator delete(void* p) { std::free(p); } };
struct Batch : Base { char d[0x20]; Batch(const void* t, void* m) { kind = 2; type = t; map = m; } static void* operator new(size_t s, Owner* o) { return PoolNew((unsigned int)s, o); } static void operator delete(void* p, Owner*) { std::free(p); } static void operator delete(void* p) { std::free(p); } };
struct Coll : Owner {
    const void* m_pType; void* m_pMap;
    Base* Create(int type)
    {
        switch (type) {
        case 0: return new (this) Mesh(m_pType, m_pMap);
        case 1: return new (this) Repeated(m_pType, m_pMap);
        default: return new (this) Batch(m_pType, m_pMap);
        }
    }
};
int main()
{
    int t, m; Coll c; c.allocs = 0; c.m_pType = &t; c.m_pMap = &m;
    Base* a = c.Create(0); Base* b = c.Create(1); Base* d = c.Create(5);
    bool fail = a->kind != 0 || b->kind != 1 || d->kind != 2 || a->type != &t || d->map != &m || c.allocs != 3;
    delete a; delete b; delete d;
    if (fail) { std::printf("00bde600_TEST FAIL\n"); return 1; }
    std::printf("00bde600_TEST PASS\n");
    return 0;
}