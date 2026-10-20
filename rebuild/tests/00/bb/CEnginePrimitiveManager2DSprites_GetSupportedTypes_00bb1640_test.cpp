// Behaviour test for 00bb1640: src/00bb1640.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEnginePrimitiveManager2DSprites::GetSupportedTypes(CArray<long>& types)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveManager2DSprites::GetSupportedTypes(CArray<long>& types) -- appends the two primitive types this manager creates: 0x22 (2D sprite) and 0x23 (2D clock sprite)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB1640, length 122 bytes.
// Callees: the retail vector's reallocating insert @ 0x00440990 (when full), as in the landed PushBackConstant family
// (e.g. 0x00BB1600).
// Shape: two inlined retail push_backs of literal constants: with room the value is placement-stored at `last` (null test)
// and `last` advances, else InsertGrow(last, value, SScalarTag(), 1, true). Each constant is a const-reference temporary
// the compiler must address: the first lives in the `push ecx` frame word, the second (the argument already in esi) is
// homed in the dead argument slot together with the empty tag. `ret 4`, `this` unused.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

template <class T>
struct CArray {                                    // retail CArray: first / last / end of storage (12 bytes)
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

void* GrowArrayStorage(void* array, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the CArray<long>::InsertGrow template 0x00440990 (retail's TU instantiated it): reads the value through
// its const reference, never touches the empty tag.
template <class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag&, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowArrayStorage(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)
struct CEnginePrimitiveManager2DSprites {
    void GetSupportedTypes(CArray<long>& types);
};

void CEnginePrimitiveManager2DSprites::GetSupportedTypes(CArray<long>& types)
{
    types.push_back(0x22);
    types.push_back(0x23);
}

// ---- stubs and driver ----
static long g_store[8]; static int g_grow = 0;
void* GrowArrayStorage(void* array, unsigned long count)
{
    ++g_grow; CArray<long>* a = (CArray<long>*)array; long* p = a->last; a->last += count; if (a->end < a->last) a->end = a->last; return p;
}
int main()
{
    CArray<long> types; types.first = g_store; types.last = g_store; types.end = g_store + 1;
    CEnginePrimitiveManager2DSprites m;
    m.GetSupportedTypes(types);
    CHECK(types.last == g_store + 2); CHECK(g_store[0] == 0x22); CHECK(g_store[1] == 0x23); CHECK(g_grow == 1);
    types.end = g_store + 8; m.GetSupportedTypes(types); CHECK(types.last == g_store + 4); CHECK(g_store[2] == 0x22 && g_store[3] == 0x23); CHECK(g_grow == 1);
    if (fails == 0) printf("00bb1640_TEST PASS\n"); else printf("00bb1640_TEST FAIL\n");
    return fails;
}