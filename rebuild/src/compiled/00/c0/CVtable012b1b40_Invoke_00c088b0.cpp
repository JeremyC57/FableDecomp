// CVtable_012b1b40::Invoke() -- vtable slot 0 of the libpk forwarding wrapper: run slot 0 of the object it was constructed around
// Leaf: ?Invoke@CVtable_012b1b40@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C088B0, 7 bytes (`mov ecx,[ecx+4]; mov eax,[ecx]; jmp [eax]`), thiscall; slot 0 of
//   the vtable 0x012B1B40 (slot 1 = the scalar deleting destructor 0x00C09AE0).
// Identity (descriptive; libpk has no PDB or RTTI): the class is named by its vtable as its landed constructor 0x00C08D20
//   (vtable, the wrapped object kept at +4) does. `Invoke` / `m_pTarget` are descriptive names (NOTES).
// Extent: the catalog row ran through int3 padding into 0x00C088C0 (12 bytes, `mov ecx,[esp+4]; call 0x00C0EC90; ret 4`,
//   slot 0 of the table 0x012B1B70); see SPLITS.tsv.
class CVtable_012b1b40 {
public:
    virtual void Invoke();                          // slot 0: 0x00C088B0
    CVtable_012b1b40* m_pTarget;                    // +0x04: the wrapped object (constructor argument, 0x00C08D20)
};

void CVtable_012b1b40::Invoke()
{
    m_pTarget->Invoke();
}