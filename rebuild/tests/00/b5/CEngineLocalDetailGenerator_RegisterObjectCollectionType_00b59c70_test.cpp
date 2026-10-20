// Behaviour test for 00b59c70: src/00b59c70.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEngineLocalDetailGenerator::RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineLocalDetailGenerator::RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type) -- returns the registered collection type equivalent to `type`, or registers (appends) a new copy of it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59C70, length 157 bytes (160 with padding).
// Callees: CLocalDetailObjectCollectionType::IsEquivalent(const CLocalDetailObjectCollectionType&) @ 0x00BE2680 (bool),
// operator new @ 0x00BFEA1A (0x54 bytes), CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType(const
// CLocalDetailObjectCollectionType&) @ 0x00BE26C0, the vector's reallocating insert @ 0x00B59F60 (manifest
// "Vector_InsertMultipleFill_ObjectType").
// Shape: the registered types are a retail vector<T*> {first, last, end} at +0x0C reached through a pointer local (esi);
// the search runs `i < size()` with size = (last - first) >> 2 re-computed per iteration (`sar` + unsigned `jb`) and
// `operator[]` = `*(begin() + i)`; a hit returns the element re-read through the vector. A miss allocates the copy
// (null when the allocation fails, still appended) and push_backs it: room -> placement store at `last` and `last += 4`,
// full -> InsertGrow(last, value, SScalarTag(), 1, true) with the value and the empty tag both homed in the dead
// argument slot. The generator has no PDB header in the tree; it is modelled locally.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct CLocalDetailObjectCollectionType {          // 0x54 bytes
    unsigned char _body[0x54];
    bool IsEquivalent(const CLocalDetailObjectCollectionType& other);                // 0x00BE2680
    CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other);  // 0x00BE26C0
};

template <class T>
struct CRetailVector {                             // STLport vector: first / last / end of storage
    T* first;
    T* last;
    T* end;
    T* begin() { return first; }
    unsigned long size() const { return last - first; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00B59F60
    void push_back(T value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CEngineLocalDetailGenerator {
    unsigned char _pad00[0xc];
    CRetailVector<CLocalDetailObjectCollectionType*> m_collectionTypes;   // +0x0C
    CLocalDetailObjectCollectionType* RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type);
};

CLocalDetailObjectCollectionType* CEngineLocalDetailGenerator::RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type)
{
    CRetailVector<CLocalDetailObjectCollectionType*>* types = &m_collectionTypes;
    for (unsigned long i = 0; i < types->size(); ++i) {
        if ((*types)[i]->IsEquivalent(type))
            return (*types)[i];
    }
    CLocalDetailObjectCollectionType* added = new CLocalDetailObjectCollectionType(type);
    types->push_back(added);
    return added;
}

// ---- stubs and driver ----
#include <cstdlib>
static void* g_store[8]; static int g_grow = 0, g_eq = 0, g_copy = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; void*** v = (void***)vector; void** p = v[1]; v[1] += count; if (v[2] < v[1]) v[2] = v[1]; return p;
}
bool CLocalDetailObjectCollectionType::IsEquivalent(const CLocalDetailObjectCollectionType& other) { ++g_eq; return _body[0] == other._body[0]; }
CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other) { ++g_copy; memcpy(_body, other._body, sizeof(_body)); }
int main()
{
    CEngineLocalDetailGenerator g; g.m_collectionTypes.first = (CLocalDetailObjectCollectionType**)g_store; g.m_collectionTypes.last = g.m_collectionTypes.first; g.m_collectionTypes.end = g.m_collectionTypes.first + 1;
    CLocalDetailObjectCollectionType* a = (CLocalDetailObjectCollectionType*)malloc(sizeof(CLocalDetailObjectCollectionType));
    CLocalDetailObjectCollectionType* b = (CLocalDetailObjectCollectionType*)malloc(sizeof(CLocalDetailObjectCollectionType));
    memset(a, 1, sizeof(*a)); memset(b, 2, sizeof(*b));
    CLocalDetailObjectCollectionType* r1 = g.RegisterObjectCollectionType(*a);
    CHECK(r1 != a && r1->_body[0] == 1 && r1->_body[0x53] == 1); CHECK(g_copy == 1); CHECK(g_eq == 0); CHECK(g_store[0] == r1); CHECK(g_grow == 0);
    CLocalDetailObjectCollectionType* r2 = g.RegisterObjectCollectionType(*b);
    CHECK(r2 != r1 && r2->_body[0] == 2); CHECK(g_copy == 2); CHECK(g_eq == 1); CHECK(g_store[1] == r2); CHECK(g_grow == 1); CHECK(g.m_collectionTypes.size() == 2);
    CLocalDetailObjectCollectionType* r3 = g.RegisterObjectCollectionType(*b);
    CHECK(r3 == r2); CHECK(g_copy == 2); CHECK(g_eq == 3); CHECK(g.m_collectionTypes.size() == 2);
    CLocalDetailObjectCollectionType* r4 = g.RegisterObjectCollectionType(*a);
    CHECK(r4 == r1); CHECK(g_eq == 4);
    if (fails == 0) printf("00b59c70_TEST PASS\n"); else printf("00b59c70_TEST FAIL\n");
    return fails;
}