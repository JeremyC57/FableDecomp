// CPointerListOwner_00778b30::Add(const CListItem* item) -- appends the item pointer to the 4-byte-element vector at +0xc through the inlined retail push_back
// Leaf: Add
// Grade: DIFFER(57v55) -- the lane builds the converted value in a `push ecx` frame slot; retail re-stores it into the dead argument slot (`mov [esp+4],edx`) and passes that address as both the value and the empty tag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778B30, length 55 bytes.
// Callees: the vector's reallocating insert @ 0x0049C370 (manifest "Std_Vector_Insert_Range").
// Label: the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected.
// Shape: as 0x005202B0 with the vector at +0xc (`add ecx,0Ch`).
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

struct CListItem;

struct CPointerListOwner_00778b30 {
    unsigned char             _pad[0x0c];
    CRetailVector<CListItem*> m_items;              // +0x0c (InsertGrow @ 0x0049C370)
    void Add(const CListItem* item);
};

void CPointerListOwner_00778b30::Add(const CListItem* item)
{
    m_items.push_back(const_cast<CListItem*>(item));
}
