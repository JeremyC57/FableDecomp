// Behaviour test for 007f7810: src/007f7810.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CPointerListOwner_007f7810::AddRecordItem(const SItemRecord* record)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPointerListOwner_007f7810::AddRecordItem(const SItemRecord* record) -- appends the record's item pointer (+4) to the 4-byte-element vector at +0x10 through the inlined retail push_back
// Leaf: AddRecordItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7810, length 58 bytes.
// Callees: the vector's reallocating insert @ 0x00436AC0.
// Label: the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected.
// Shape: the value is read through the argument (`mov eax,[esp+4] ; mov edx,[eax+4]`) and re-stored into the
// argument slot, so it is an rvalue (an inline accessor returning the pointer by value); vector at +0x10.
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

struct SItemRecord {
    long       m_id;                                // +0x00
    CListItem* m_pItem;                             // +0x04
    CListItem* GetItem() const { return m_pItem; }
};

struct CPointerListOwner_007f7810 {
    unsigned char             _pad[0x10];
    CRetailVector<CListItem*> m_items;              // +0x10 (InsertGrow @ 0x00436AC0)
    void AddRecordItem(const SItemRecord* record);
};

void CPointerListOwner_007f7810::AddRecordItem(const SItemRecord* record)
{
    m_items.push_back(record->GetItem());
}

// ---- stubs and driver ----
static void* g_store[8]; static int g_grow = 0; static unsigned long g_count = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; g_count = count; void*** v = (void***)vector; void** p = v[1]; v[1] += count; if (v[2] < v[1]) v[2] = v[1]; return p;
}
int main()
{
    CPointerListOwner_007f7810 o; o.m_items.first = (CListItem**)g_store; o.m_items.last = o.m_items.first; o.m_items.end = o.m_items.first + 1;
    SItemRecord r1 = { 7, (CListItem*)&g_grow }, r2 = { 8, (CListItem*)&g_count };
    o.AddRecordItem(&r1); CHECK(g_store[0] == (void*)&g_grow); CHECK(g_grow == 0); CHECK(o.m_items.last == o.m_items.first + 1);
    o.AddRecordItem(&r2); CHECK(g_grow == 1); CHECK(g_count == 1); CHECK(g_store[1] == (void*)&g_count); CHECK(o.m_items.last == o.m_items.first + 2);
    o.m_items.end = o.m_items.first + 8; o.AddRecordItem(&r1); CHECK(g_grow == 1); CHECK(g_store[2] == (void*)&g_grow); CHECK(o.m_items.last == o.m_items.first + 3);
    if (fails == 0) printf("007f7810_TEST PASS\n"); else printf("007f7810_TEST FAIL\n");
    return fails;
}