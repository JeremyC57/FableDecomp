// CRetailVector_006bc4a0::push_back(long value) -- the retail vector push_back for 4-byte elements taking the value by value: place it at `last` when there is room, else the reallocating insert
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC4A0, length 48 bytes.
// Callees: the reallocating insert @ 0x00440990 (manifest "std::vector::InsertMultipleAndGrow"; (where, const T&, const
// tag&, count, noTail), the tag unused).
// Label: the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected.
// Shape: `this` is the vector itself; the in-place path reads the value from its slot (`mov edx,[esp+4]`).
// Tried: an rvalue argument (the cast / inline accessor that retail copies into the dead argument slot),
// the landed SScalarTag-with-user-constructor push_back. The lane gives the value temporary its own `push ecx`
// slot and the tag the dead argument slot (or, for an lvalue parameter, the tag its own frame byte), never
// both on the same live slot. Next: lane gap (GOTCHAS 24l: empty tag temporary overlaid on a live argument
// slot; same as the parked CTCRegionFollower/CTCFollowed GetFollowers 0x006AEE40 / 0x006D5850).
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_006bc4a0 {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(long value);
};
void* FindRef_CRetailVector_006bc4a0_InsertGrow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CRetailVector_006bc4a0::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    void* p = FindRef_CRetailVector_006bc4a0_InsertGrow_0();
    if (!p)
        return;
    *(void**)&tag = p;
    
}
#pragma auto_inline(on)

void CRetailVector_006bc4a0::push_back(long value)
{
    if (last != end) {
        new (last) long(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}