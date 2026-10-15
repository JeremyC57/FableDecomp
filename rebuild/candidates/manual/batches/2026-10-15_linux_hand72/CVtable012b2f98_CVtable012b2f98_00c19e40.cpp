// CVtable_012b2f98::CVtable_012b2f98(void* owner) -- one-argument constructor of a root polymorphic class: installs its vtable and keeps the argument at +4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19E40, length 18 bytes.
// Leaf: ??0CVtable_012b2f98
// Retail bytes: mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset vftable(0x012B2F98) ; mov [eax+4],ecx ; ret 4
// The manifest label "CSBAgents::CSBAgents" is propagated over byte-identical constructor bodies of many
// classes (CSBAgents keeps its agent vector at +0xc, sizeof 0x10); the vtable 0x012B2F98 (slots 0x00C19970,
// 0x00C19AD0, 0x00C19B70, ...) belongs to the no-RTTI 0x00C1xxxx module, so the class is named by its vtable.
// Shape: root polymorphic class (no base constructor), the vtable store first, then the one argument (`ret 4`).
struct CVtable_012b2f98 {
    virtual void v00();                    // slot 0 = 0x00C19970
    void* m_pOwner;                        // +0x04
    CVtable_012b2f98(void* owner);
};

CVtable_012b2f98::CVtable_012b2f98(void* owner)
    : m_pOwner(owner)
{
}
