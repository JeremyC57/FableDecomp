// Behaviour test for 00ba6ef0: src/00ba6ef0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes(CArray<EEnginePrimitiveType>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes(CArray<EEnginePrimitiveType>&) -- report the three primitive types this manager handles (8, 0xa, 0xc) by appending them to the caller's type list
// Leaf: GetSupportedTypes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA6EF0, 176 bytes (+ int3 padding), `ret 4` (`this` unused). Header: engine/CEnginePrimitiveManagerStaticMeshesBase.h (`this` is never read).
// Callees: the retail vector<long>::_M_insert_overflow / InsertGrow 0x00440990 (as the landed single-type siblings 0x00B9BFA0
//   and 0x00BE1A80).
// Shape: three inline push_backs of named constant locals (the first homed in the `push ecx` word, the later two in the dead
//   `types` argument slot); the empty __true_type tag passed by const reference to each overflow call.
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
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
__declspec(noinline) void CRetailVector_dword::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    long* p = (long*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CEnginePrimitiveManagerStaticMeshesBase {
    void GetSupportedTypes(CRetailVector_dword& types);
};

void CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes(CRetailVector_dword& types)
{
    {
        long type = 8;
        types.push_back(type);
    }
    {
        long type = 0xa;
        types.push_back(type);
    }
    {
        long type = 0xc;
        types.push_back(type);
    }
}

// ---- stubs and driver ----
static long g_store[8]; static int g_grow = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CRetailVector_dword* v = (CRetailVector_dword*)vector; long* p = v->last; v->last += count; if (v->end < v->last) v->end = v->last; return p;
}
int main()
{
    CRetailVector_dword types; types.first = g_store; types.last = g_store; types.end = g_store + 1;
    CEnginePrimitiveManagerStaticMeshesBase m;
    m.GetSupportedTypes(types);
    CHECK(types.last == g_store + 3); CHECK(g_store[0] == 8); CHECK(g_store[1] == 10); CHECK(g_store[2] == 12); CHECK(g_grow == 2);
    types.end = g_store + 8; m.GetSupportedTypes(types); CHECK(types.last == g_store + 6); CHECK(g_store[3] == 8 && g_store[4] == 10 && g_store[5] == 12); CHECK(g_grow == 2);
    if (fails == 0) printf("00ba6ef0_TEST PASS\n"); else printf("00ba6ef0_TEST FAIL\n");
    return fails;
}