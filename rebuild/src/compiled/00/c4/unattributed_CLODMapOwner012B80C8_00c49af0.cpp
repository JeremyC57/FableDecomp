// <unattributed> destructor: vtable 0x012B80C8, frees the buffer at +0x08 with operator delete[]
// Leaf: ??1CEngineLandscapeLODMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49AF0, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[ecx+8] ; push eax ; mov dword ptr [ecx],offset vftable ; call operator delete[] ; pop ecx ; ret
// A polymorphic class whose destructor re-points the vtable (the compiler's own store) and frees
// the trivially-destructible buffer at +0x08 (operator delete[], landed-trusted identity). Three sibling bodies
// differ only in the vtable address (0x012B2FC8 / 0x012B80C8 / 0x012B8364) and delete vs delete[].
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
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