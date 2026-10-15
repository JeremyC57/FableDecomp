// CVtable_012b4f60::CVtable_012b4f60(void* first, void* second) -- two-argument constructor of a root polymorphic class: vtable, the SECOND argument at +4 and the first at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C330A0, length 25 bytes.
// Leaf: ??0CVtable_012b4f60
// Retail bytes: mov edx,[esp+4] ; mov eax,ecx ; mov ecx,[esp+8] ; mov dword ptr [eax],offset vftable(0x012B4F60) ; mov [eax+4],ecx ; mov [eax+8],edx ; ret 8
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies; the vtable
// 0x012B4F60 (slots 0x00C33070, 0x00C33080, 0x00C33090, ...) belongs to the no-RTTI 0x00C3xxxx module, so the
// class is named by its vtable. Shape: as 0x00C32FC0 with the arguments crossed -- the member at +4 takes the
// second argument ([esp+8]) and the member at +8 the first ([esp+4]).
struct CVtable_012b4f60 {
    virtual void v00();                    // slot 0 = 0x00C33070
    void* m_pSecond;                       // +0x04 (second argument)
    void* m_pFirst;                        // +0x08 (first argument)
    CVtable_012b4f60(void* first, void* second);
};

CVtable_012b4f60::CVtable_012b4f60(void* first, void* second)
    : m_pSecond(second), m_pFirst(first)
{
}