// CPointerListOwner_005202b0::Add(CListItem* item) -- appends the item pointer to the 4-byte-element vector at +0x48 through the inlined retail push_back
// Leaf: Add
// Grade: DIFFER(57v55) -- the lane builds the converted value in a `push ecx` frame slot (`mov [esp],edx`, `pop ecx` x2); retail stores it back into the dead argument slot (`mov [esp+4],edx`) and passes that one address as BOTH the value and the empty tag (`lea edx,[esp+0Ch]` / `lea edx,[esp+10h]` after the pushes)
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
