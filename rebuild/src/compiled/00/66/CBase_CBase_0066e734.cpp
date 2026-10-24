// CBase::CBase() -- root polymorphic class: only the vtable store
// Leaf: ??0CBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066E734, oracle length 9 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0x123a3e0 ; ret 
// Default constructor of a root polymorphic class with nothing to initialise: installs the vtable
// (0x0123A3E0, rtti-vtables.tsv) and returns `this`. One of the tiny-body shapes.
struct CBase {
    virtual void v00();
    CBase();
};

CBase::CBase()
{
}