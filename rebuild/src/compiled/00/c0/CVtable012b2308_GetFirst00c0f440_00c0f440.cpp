// CVtable_012b2308::GetFirst_00c0f440() -- libpk container shell: forward "first item" (slot 11) to the implementation at +4
// Leaf: ?GetFirst_00c0f440@CVtable_012b2308@@UAEPAXXZ
// Retail: Fable.exe @ 0x00C0F440, 8 bytes (`mov ecx,[ecx+4] ; mov eax,[ecx] ; jmp [eax+2Ch]`; int3 padding to 0x00C0F450).
// Identity (descriptive -- libpk, d:\dev\audio\ver_fable_1.5_b\libpk, has no PDB and no RTTI): slot 11 of vtable 0x012B2308 (entry
// 0x012B2334), inherited unchanged by the derived vtables 0x012B23F0 / 0x012B2518. Vtable 0x012B2308 is abstract (slots 0-3 _purecall),
// its constructor 0x00C0F3A0 stores the vtable and the one argument at +4 (8 bytes); the derived constructor 0x00C0FF40 points +4 at an
// embedded red-black tree (vtable 0x012B2F0C) whose slot 11 (0x00C19150) walks to the leftmost node and returns its value (+0x10), or
// 0 when empty: hence "GetFirst" and the void* result. Class and implementation are named by vtable (the libpk CVtable_<va> convention);
// the method name is descriptive and carries the row address.
class CUnprovenContainerImplementation_00c0f440 {   // static type of the +4 pointer: only its slot order is proven (slot 11 = first item)
public:
    virtual void Slot00() = 0;
    virtual void Slot01() = 0;
    virtual void Slot02() = 0;
    virtual void Slot03() = 0;
    virtual void Slot04() = 0;
    virtual void Slot05() = 0;
    virtual void Slot06() = 0;
    virtual void Slot07() = 0;
    virtual void Slot08() = 0;
    virtual void Slot09() = 0;
    virtual void Slot10() = 0;
    virtual void* GetFirst() = 0;                  // slot 11 (+0x2C)
};

class CVtable_012b2308 {                          // 8 bytes: vptr 0x012B2308, implementation +4
public:
    CVtable_012b2308(CUnprovenContainerImplementation_00c0f440* implementation) : Implementation(implementation) {}
    virtual void Slot00() = 0;
    virtual void Slot01() = 0;
    virtual void Slot02() = 0;
    virtual void Slot03() = 0;
    virtual ~CVtable_012b2308() {}                  // slot 4: scalar deleting destructor 0x00C0F490
    virtual void Slot05();
    virtual void Slot06();
    virtual void Slot07();
    virtual void Slot08();
    virtual void Slot09();
    virtual void Slot10();
    virtual void* GetFirst_00c0f440();              // slot 11
    CUnprovenContainerImplementation_00c0f440* Implementation;
};

void* CVtable_012b2308::GetFirst_00c0f440()
{
    return Implementation->GetFirst();
}