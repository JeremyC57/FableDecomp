// Behaviour test for 0x007CA350 CArray<CDefPointer<const CCreatureGenerationFamilyDef> >::push_back: links the
// reconstructed object; in-place copy adds a reference to a non-null def; a full array grows through
// _M_insert_overflow (the source object's same-TU stand-in, which only moves _M_finish).
#include <cstdio>
#include <cstdlib>
class CDefPointeeBase {
public:
    CDefPointeeBase() : RefCount(1) {}
    virtual void v00();
    virtual void Destroy();
    long RefCount;
};
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() {}
class CDefClassBase : public CDefPointeeBase { };
class CCreatureGenerationFamilyDef : public CDefClassBase { };
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class T, class _Alloc = allocator<T> > class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
}
template <class T> class CArray : public std::vector<T> {
public:
    void push_back(const T& __x);
};
typedef CDefPointer<const CCreatureGenerationFamilyDef> Ptr;
int main()
{
    CCreatureGenerationFamilyDef a, b;
    CArray<Ptr> v;
    Ptr* storage = (Ptr*)std::malloc(4 * sizeof(Ptr));
    v._M_start = v._M_finish = storage; v._M_end_of_storage = storage + 2;
    Ptr pa; pa.Object = &a;
    Ptr pn; pn.Object = 0;
    Ptr pb; pb.Object = &b;
    v.push_back(pa);
    v.push_back(pn);
    bool ok = v._M_finish - v._M_start == 2 && a.RefCount == 2 && storage[0].Object == &a && storage[1].Object == 0;
    v.push_back(pn);                      // full: the growth path (stand-in) runs, no copy made here
    ok = ok && v._M_finish - v._M_start == 3 && v._M_end_of_storage == storage + 2;
    v._M_finish = storage + 2; v._M_end_of_storage = storage + 4;
    v.push_back(pb);
    ok = ok && v._M_finish - v._M_start == 3 && storage[2].Object == &b && b.RefCount == 2;
    std::free(storage);
    if (!ok) { std::printf("007ca350_TEST FAIL\n"); return 1; }
    std::printf("007ca350_TEST PASS\n");
    return 0;
}
