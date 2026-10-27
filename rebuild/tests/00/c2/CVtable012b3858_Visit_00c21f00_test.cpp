#include <cstdio>

// Behaviour model of CVtable_012b3858::Visit (0x00C21F00): release a reference whose object's key matches.
struct Obj { int _00; int m_key; unsigned short m_lifetime; };
struct Ref {
    Obj* m_pObject;
    void Release() { --m_pObject->m_lifetime; m_pObject = 0; }   // CLifetimeRef_00c26060::~CLifetimeRef_00c26060
};
struct Visitor {
    int m_key;
    void Visit(Ref* r) { if (r->m_pObject && r->m_pObject->m_key == m_key) r->Release(); }
};

int main()
{

    Obj o; o.m_key = 3; o.m_lifetime = 2;
    Ref r; r.m_pObject = &o; Ref empty; empty.m_pObject = 0;
    Visitor v; v.m_key = 4;
    v.Visit(&r); if (r.m_pObject != &o || o.m_lifetime != 2) { std::printf("00c21f00_TEST FAIL\n"); return 1; }
    v.Visit(&empty); if (empty.m_pObject) { std::printf("00c21f00_TEST FAIL\n"); return 1; }
    v.m_key = 3; v.Visit(&r); if (r.m_pObject != 0 || o.m_lifetime != 1) { std::printf("00c21f00_TEST FAIL\n"); return 1; }
    std::printf("00c21f00_TEST PASS\n");
    return 0;
}