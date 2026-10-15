// CVtable_012b3850::CVtable_012b3850(void* first, void* second) -- two-argument constructor of a root polymorphic class: vtable, then the arguments at +4 and +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C22060, length 25 bytes.
// Leaf: ??0CVtable_012b3850
// Retail bytes: mov edx,[esp+8] ; mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset vftable(0x012B3850) ; mov [eax+4],ecx ; mov [eax+8],edx ; ret 8
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies; the vtable
// 0x012B3850 (slots 0x00C22E30 deleting destructor, 0x00C22080, 0x00C21F00, ...) belongs to the no-RTTI
// 0x00C2xxxx module, so the class is named by its vtable. Shape: root class, vtable store first, the two
// arguments in declaration order.
struct CVtable_012b3850 {
    virtual void v00();                    // slot 0 = 0x00C22E30
    void* m_pFirst;                        // +0x04
    void* m_pSecond;                       // +0x08
    CVtable_012b3850(void* first, void* second);
};

CVtable_012b3850::CVtable_012b3850(void* first, void* second)
    : m_pFirst(first), m_pSecond(second)
{
}