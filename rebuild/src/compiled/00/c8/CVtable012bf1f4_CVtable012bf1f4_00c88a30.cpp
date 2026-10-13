// CVtable_012bf1f4::CVtable_012bf1f4() -- root polymorphic class: only the vtable store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88A30, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0x12bf1f4 ; ret 
// Default constructor of a root polymorphic class with nothing to initialise: installs the vtable
// (0x012BF1F4, rtti-vtables.tsv) and returns `this`. One of the tiny-body shapes.
struct CVtable_012bf1f4 {
    virtual void v00();
    CVtable_012bf1f4();
};

CVtable_012bf1f4::CVtable_012bf1f4()
{
}