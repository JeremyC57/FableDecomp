#include <cstdio>
#include <new>
struct CRefCounted { void* __vftable; long m_refCount; };
struct CCountedPointer { CRefCounted* m_pObject; CCountedPointer(CRefCounted* p) : m_pObject(p) {} CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; } };
struct SInsertTag { SInsertTag() {} };
static int g_grow = 0;
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    void GrowInsert(T*, const T&, const SInsertTag&, unsigned long, unsigned long) { ++g_grow; }
    void push_back(const T& value) { if (last != end_of_storage) { new (last) T(value); ++last; } else { GrowInsert(last, value, SInsertTag(), 1, 1); } }
};
int main()
{
    CRefCounted obj = { 0, 1 };
    unsigned char raw[2 * sizeof(CCountedPointer)];
    CRetailVector<CCountedPointer> v; v.first = (CCountedPointer*)raw; v.last = v.first; v.end_of_storage = v.first + 2;
    CCountedPointer cp(&obj);
    v.push_back(cp);
    if (v.last != v.first + 1 || obj.m_refCount != 2 || v.first[0].m_pObject != &obj || g_grow != 0) { std::printf("00711c50_TEST FAIL\n"); return 1; }
    v.push_back(cp); v.push_back(cp);
    if (v.last != v.first + 2 || obj.m_refCount != 3 || g_grow != 1) { std::printf("00711c50_TEST FAIL\n"); return 1; }
    std::printf("00711c50_TEST PASS\n");
    return 0;
}