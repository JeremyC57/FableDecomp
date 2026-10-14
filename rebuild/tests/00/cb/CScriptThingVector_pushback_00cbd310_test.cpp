#include <cstdio>
#include <cstring>
#include <stddef.h>
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCharString { const char* m_pData; CCharString(const char* s) : m_pData(s) {} CCharString(const CCharString& o) : m_pData(o.m_pData) {} };
struct CCountedPointer {
    void* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(void* o, SCountBlock* b) : m_pObject(o), m_pCount(b) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};
struct CScriptThing { CCharString Name; CCountedPointer PImp; CScriptThing(const char* n, void* o, SCountBlock* b) : Name(n), PImp(o, b) {} };
struct __false_type { __false_type() {} };
inline void* operator new(size_t, void* where) { return where; }
static int g_overflow = 0;
struct CScriptThingVector {
    CScriptThing* m_start; CScriptThing* m_finish; CScriptThing* m_end_of_storage;
    void _M_insert_overflow(CScriptThing*, const CScriptThing&, const __false_type&, unsigned long, bool) { ++g_overflow; }
    void push_back(const CScriptThing& x) { if (m_finish != m_end_of_storage) { new (m_finish) CScriptThing(x); ++m_finish; } else _M_insert_overflow(m_finish, x, __false_type(), 1, true); }
};
int main()
{
    unsigned char storage[2 * sizeof(CScriptThing)];
    CScriptThingVector v; v.m_start = reinterpret_cast<CScriptThing*>(storage); v.m_finish = v.m_start; v.m_end_of_storage = v.m_start + 2;
    SCountBlock block = { 1, 0, 0 }; int obj = 0;
    CScriptThing thing("hero", &obj, &block);
    v.push_back(thing);
    if (v.m_finish != v.m_start + 1 || block.m_references != 2 || std::strcmp(v.m_start[0].Name.m_pData, "hero") != 0 || v.m_start[0].PImp.m_pObject != &obj || g_overflow != 0) { std::printf("00cbd310_TEST FAIL\n"); return 1; }
    v.push_back(thing);
    if (v.m_finish != v.m_start + 2 || block.m_references != 3 || g_overflow != 0) { std::printf("00cbd310_TEST FAIL\n"); return 1; }
    v.push_back(thing);   // full: the overflow path
    if (v.m_finish != v.m_start + 2 || block.m_references != 3 || g_overflow != 1) { std::printf("00cbd310_TEST FAIL\n"); return 1; }
    std::printf("00cbd310_TEST PASS\n");
    return 0;
}