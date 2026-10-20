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