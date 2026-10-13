// CRetailVector_dword::push_back(const long&) -- store at `last` when there is room, else InsertGrow @ 0x00916D50 for one element
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00916EF0, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[ecx+4] ; cmp eax,[ecx+8] ; je G ; test eax,eax ; je S ; mov edx,[esp+4] ; mov edx,[edx] ; mov [eax],edx ; S: add dword ptr [ecx+4],4 ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esp+10h] ; push edx ; push eax ; call InsertGrow ; ret 4
// Retail vector push_back for 4-byte scalar elements {first, last, end}: with spare capacity the
// value is placement-copied at `last` (hence the null test) and `last` advances; when full, the
// reallocating insert @ 0x00916D50 is called for one element with `noTail` set, its third
// argument the address of a 1-byte empty-tag temporary homed in the dead argument slot. `this`
// stays in ecx for the call. One of 13 bodies of this shape.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct CRetailVector_dword {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00916D50
    void PushBack_dword_00916ef0(const long& value);
};

void CRetailVector_dword::PushBack_dword_00916ef0(const long& value)
{
    if (last != end) {
        new (last) long(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}