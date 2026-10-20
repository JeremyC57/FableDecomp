// Behaviour test for 00bc7920: src/00bc7920.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEnginePrimitiveManagerText::GetSupportedTypes(std::vector<EEnginePrimitiveType>& types)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveManagerText::GetSupportedTypes(std::vector<EEnginePrimitiveType>& types) -- appends the two primitive types this manager creates: 5 (3D text) and 0x27 (2D text)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7920, length 122 bytes.
// Callees: 0x00440990 the STLport vector's reallocating insert (`_M_insert_overflow(pos, const T&, __true_type,
// count, atend)`, landed PushBackConstant family) when the vector is full. `this` is unused (`ret 4`).
// Shape: two inlined STLport `push_back(const T&)` calls (null-tested placement store + `add [last],4`, else the
// overflow insert with the empty `__true_type` tag homed in the dead argument slot); each value is a literal
// bound to the const reference, so VC materialises it in a stack word: the first in the `push ecx` frame word,
// the second in the vector argument's slot (dead once the pointer sits in esi).
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_long {                        // STLport vector<long>: first, last, end_of_storage
    long* first;
    long* last;
    long* end;
    void InsertOverflow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertOverflow(last, value, SScalarTag(), 1, true);
    }
};

void* GrowArrayStorage(void* array, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector<long> insert-overflow template 0x00440990 (retail's TU instantiated it): reads the value through
// its const reference, never touches the empty tag.
__declspec(noinline) void CRetailVector_long::InsertOverflow(long* where, const long& value, const SScalarTag&, unsigned long count, bool atEnd)
{
    long* p = (long*)GrowArrayStorage(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)

struct CEnginePrimitiveManagerText {
    void GetSupportedTypes(CRetailVector_long& types);
};

void CEnginePrimitiveManagerText::GetSupportedTypes(CRetailVector_long& types)
{
    types.push_back(5);                            // ENGINE_PRIMITIVE_TEXT
    types.push_back(0x27);                         // ENGINE_PRIMITIVE_2D_TEXT
}

// ---- stubs and driver ----
static long g_store[8]; static int g_grow = 0;
void* GrowArrayStorage(void* array, unsigned long count)
{
    ++g_grow; CRetailVector_long* a = (CRetailVector_long*)array; long* p = a->last; a->last += count; if (a->end < a->last) a->end = a->last; return p;
}
int main()
{
    CRetailVector_long types; types.first = g_store; types.last = g_store; types.end = g_store + 1;
    CEnginePrimitiveManagerText m;
    m.GetSupportedTypes(types);
    CHECK(types.last == g_store + 2); CHECK(g_store[0] == 5); CHECK(g_store[1] == 0x27); CHECK(g_grow == 1);
    types.end = g_store + 8; m.GetSupportedTypes(types); CHECK(types.last == g_store + 4); CHECK(g_store[2] == 5 && g_store[3] == 0x27); CHECK(g_grow == 1);
    if (fails == 0) printf("00bc7920_TEST PASS\n"); else printf("00bc7920_TEST FAIL\n");
    return fails;
}