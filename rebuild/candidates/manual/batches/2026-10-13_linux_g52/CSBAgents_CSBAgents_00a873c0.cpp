// CSBAgents::CSBAgents(void*, void*) -- root polymorphic class: vtable, first argument kept at +0xc, second unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A873C0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset CSBAgents::`vftable' ; mov [eax+0xc],ecx ; ret 8
// Two-argument constructor of a root polymorphic class (no base constructor call): installs the
// vtable (0x0X129DEBC, rtti-vtables.tsv), stores the first argument at +0xc and ignores the second (`ret 8`).
// Argument types are not recoverable; `void*` models them. One of 19 bodies.
struct CSBAgents {
    virtual void v00();
    unsigned char _pad[0x8];
    void* m_first;                         // +0xc
    CSBAgents(void* first, void* /*second*/);
};

CSBAgents::CSBAgents(void* first, void* /*second*/)
    : m_first(first)
{
}
