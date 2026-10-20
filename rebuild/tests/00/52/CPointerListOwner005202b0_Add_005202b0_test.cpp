// Behaviour test for 005202b0: src/005202b0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CPointerListOwner_005202b0::Add(CListItem* item)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPointerListOwner_005202b0::Add(CListItem* item) -- appends the item pointer to the 4-byte-element vector at +0x48 through the inlined retail push_back
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005202B0, length 55 bytes.
// Callees: the vector's reallocating insert @ 0x00436AC0 (landed callers: (where, const T&, const tag&, count, noTail)).
// Label: the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected
// (that class's key cameras are 0x38-byte records); the owner is named by address.
// Shape: `this` advanced to the member vector (`add ecx,48h`); the argument is re-stored into its own slot,
// so the source passes an rvalue (a pointer conversion) to push_back(const T&).
// Tried: an rvalue argument (the cast / inline accessor that retail copies into the dead argument slot),
// the landed SScalarTag-with-user-constructor push_back. The lane gives the value temporary its own `push ecx`
// slot and the tag the dead argument slot (or, for an lvalue parameter, the tag its own frame byte), never
// both on the same live slot. Next: lane gap (GOTCHAS 24l: empty tag temporary overlaid on a live argument
// slot; same as the parked CTCRegionFollower/CTCFollowed GetFollowers 0x006AEE40 / 0x006D5850).
#include <new>

struct SScalarTag { SScalarTag() { } };             // empty pod-type tag (STLport __true_type), bound to a const reference

template <class T> struct CRetailVector {           // retail std::vector: {first, last, end}
    T* first;
    T* last;
    T* end;
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);
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
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CListItem;
struct CListItemBase;

struct CPointerListOwner_005202b0 {
    unsigned char                  _pad[0x48];
    CRetailVector<CListItemBase*>  m_items;         // +0x48 (InsertGrow @ 0x00436AC0)
    void Add(CListItem* item);
};

void CPointerListOwner_005202b0::Add(CListItem* item)
{
    m_items.push_back((CListItemBase*)item);
}

// ---- stubs and driver ----
static void* g_store[8]; static int g_grow = 0; static unsigned long g_count = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; g_count = count; void*** v = (void***)vector; void** p = v[1]; v[1] += count; if (v[2] < v[1]) v[2] = v[1]; return p;
}
int main()
{
    CPointerListOwner_005202b0 o; o.m_items.first = (CListItemBase**)g_store; o.m_items.last = o.m_items.first; o.m_items.end = o.m_items.first + 1;
    o.Add((CListItem*)&g_grow); CHECK(g_store[0] == (void*)&g_grow); CHECK(g_grow == 0); CHECK(o.m_items.last == o.m_items.first + 1);
    o.Add((CListItem*)&g_count); CHECK(g_grow == 1); CHECK(g_count == 1); CHECK(g_store[1] == (void*)&g_count); CHECK(o.m_items.last == o.m_items.first + 2);
    o.m_items.end = o.m_items.first + 8; o.Add((CListItem*)0); CHECK(g_grow == 1); CHECK(g_store[2] == 0); CHECK(o.m_items.last == o.m_items.first + 3);
    if (fails == 0) printf("005202b0_TEST PASS\n"); else printf("005202b0_TEST FAIL\n");
    return fails;
}