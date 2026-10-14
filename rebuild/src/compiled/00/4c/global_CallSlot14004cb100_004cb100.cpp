// CallSlot14_004cb100(void* unused0, IObject* object) -- `__stdcall`: calls virtual slot 14 of the argument object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CB100, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ecx, dword ptr [esp + 8] ; mov eax, dword ptr [ecx] ; call dword ptr [eax + 0x38] ; ret 8
// Tiny-body shape `CallSlot`: a `__stdcall` free function (callee-cleaned 2 slot(s)) that calls
// virtual slot 14 of the object passed in stack slot 1; the other slots are not read.
// The manifest label is propagated; the names are descriptive.
struct IObject {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10();
    virtual void Slot11();
    virtual void Slot12();
    virtual void Slot13();
    virtual void Method();                 // slot 14
};

void __stdcall CallSlot14_004cb100(void* unused0, IObject* object)
{
    object->Method();
}