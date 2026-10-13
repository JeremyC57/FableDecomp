// PushBackConstant_00bce030(CRetailVector_dword*) -- vector->push_back(0x11) with the retail push_back inlined (InsertGrow @ 0x00440990 when full)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCE030, oracle length 62 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov ecx,[esp+8] ; mov eax,[ecx+4] ; cmp eax,[ecx+8] ; mov dword ptr [esp],0x11 ; je G ; test eax,eax ; je S ; mov dword ptr [eax],0x11 ; S: add dword ptr [ecx+4],4 ; pop ecx ; ret 4 ; G: push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; lea edx,[esp+0Ch] ; push edx ; push eax ; call InsertGrow ; pop ecx ; ret 4
// Pushes the constant 0x11 onto the 4-byte-element retail vector passed on the stack (`ret 4`, no
// `this`): the value is a named stack local (it must be addressable for the reallocating
// insert's reference argument), stored in place when there is room, else handed to the insert
// @ 0x00440990 with the empty-tag temporary homed in the dead argument slot. The manifest label
// is propagated from an unrelated body; the function is named by its effect. One of 15 bodies.
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

void __stdcall PushBackConstant_00bce030(CRetailVector_dword* vector)
{
    long value = 0x11;
    vector->push_back(value);
}
