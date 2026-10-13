// CVtable_012b3858::CVtable_012b3858(void*) -- root polymorphic class: vtable, first argument kept at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C22240, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset CVtable_012b3858::`vftable' ; mov [eax+0x4],ecx ; ret 4
// One-argument constructor of a root polymorphic class (no base constructor call): installs the
// vtable (0x0X12B3858, rtti-vtables.tsv), stores the first argument at +0x4.
// Argument types are not recoverable; `void*` models them. One of 19 bodies.
struct CVtable_012b3858 {
    virtual void v00();
    void* m_first;                         // +0x4
    CVtable_012b3858(void* first);
};

CVtable_012b3858::CVtable_012b3858(void* first)
    : m_first(first)
{
}
