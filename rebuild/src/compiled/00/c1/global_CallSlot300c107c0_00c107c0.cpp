// CallSlot3_00c107c0(IObject* object) -- `__stdcall`: calls virtual slot 3 of the argument object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C107C0, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [esp + 4] ; mov eax, dword ptr [ecx] ; call dword ptr [eax + 0xc] ; ret 4
// Tiny-body shape `CallSlot`: a `__stdcall` free function (callee-cleaned 1 slot(s)) that calls
// virtual slot 3 of the object passed in stack slot 0; the other slots are not read.
// The manifest label is propagated; the names are descriptive.
struct IObject {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Method();                 // slot 3
};

void __stdcall CallSlot3_00c107c0(IObject* object)
{
    object->Method();
}