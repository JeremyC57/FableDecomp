// CVtable_012b4f4c::CVtable_012b4f4c(void* first, void* second) -- two-argument constructor of a root polymorphic class: vtable, then the arguments at +4 and +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C32FC0, length 25 bytes.
// Leaf: ??0CVtable_012b4f4c
// Retail bytes: mov edx,[esp+8] ; mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset vftable(0x012B4F4C) ; mov [eax+4],ecx ; mov [eax+8],edx ; ret 8
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies; the vtable
// 0x012B4F4C (slots 0x00C32F40, 0x00C32F60, 0x00C32F30, ...) belongs to the no-RTTI 0x00C3xxxx module, so the
// class is named by its vtable. Shape: root class, vtable store first, the two arguments in declaration order.
struct CVtable_012b4f4c {
    virtual void v00();                    // slot 0 = 0x00C32F40
    void* m_pFirst;                        // +0x04
    void* m_pSecond;                       // +0x08
    CVtable_012b4f4c(void* first, void* second);
};

CVtable_012b4f4c::CVtable_012b4f4c(void* first, void* second)
    : m_pFirst(first), m_pSecond(second)
{
}
