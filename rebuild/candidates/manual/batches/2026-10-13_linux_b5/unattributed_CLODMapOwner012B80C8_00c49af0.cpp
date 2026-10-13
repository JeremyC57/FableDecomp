// <unattributed> destructor: vtable 0x012B80C8, frees the buffer at +0x08 with operator delete[]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49AF0, oracle length 17 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[ecx+8] ; push eax ; mov dword ptr [ecx],offset vftable ; call operator delete[] ; pop ecx ; ret
// A polymorphic class whose destructor re-points the vtable (the compiler's own store) and frees
// the trivially-destructible buffer at +0x08 (operator delete[], landed-trusted identity). Three sibling bodies
// differ only in the vtable address (0x012B2FC8 / 0x012B80C8 / 0x012B8364) and delete vs delete[].
struct CLODMapOwner_012B80C8 {
    virtual void v00();                     // polymorphic (the destructor stores the vtable)
    void* Reserved;                         // +0x04
    unsigned char* Buffer;                  // +0x08 freed here
    ~CLODMapOwner_012B80C8();              // non-virtual here so only the plain destructor body is emitted
};

CLODMapOwner_012B80C8::~CLODMapOwner_012B80C8()
{
    delete[] this->Buffer;
}
