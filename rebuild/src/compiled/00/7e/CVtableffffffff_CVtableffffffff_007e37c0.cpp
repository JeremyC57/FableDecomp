// CVtable_ffffffff::CVtable_ffffffff() -- root polymorphic class: only the vtable store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E37C0, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0xffffffff ; ret 
// Default constructor of a root polymorphic class with nothing to initialise: installs the vtable
// (0xFFFFFFFF, rtti-vtables.tsv) and returns `this`. One of the tiny-body shapes.
struct CVtable_ffffffff {
    virtual void v00();
    CVtable_ffffffff();
};

CVtable_ffffffff::CVtable_ffffffff()
{
}