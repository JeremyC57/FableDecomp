// CDataOwner_00c14a50::DeleteData(CData_00c14a50* data) -- unregisters the data object from the two registries at +8 and +0xc, then deletes it through its virtual destructor (slot 7)
// Leaf: DeleteData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14A50, 44 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: a libpk owner class (vtable 0x012B29F8: its destructor @ 0x00C14FA0 deletes every object its +0x10 set
//   yields through this function; the second caller 0x00C2C785 runs it under the module lock on the object's +4
//   owner). Callees: the registry removals @ 0x00C2A640 (+8) and @ 0x00C2BC10 (+0xc), each the only call site.
// Shape: the argument is loaded into esi before `this` moves to edi; `delete data` on a class whose virtual
//   destructor is its eighth virtual gives `test esi,esi ; je ; mov eax,[esi] ; push 1 ; mov ecx,esi ; call [eax+1Ch]`.
// Label: the catalog's `DeleteData` is descriptive; no PDB name set covers libpk, the classes are named by address.
class CData_00c14a50 {
public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual void Slot04(); virtual void Slot05(); virtual void Slot06();
    virtual ~CData_00c14a50();                                 // slot 7 (+0x1c): the deleting destructor
};

struct CRegistryA_00c14a50 {
    void Remove(CData_00c14a50* data);                         // 0x00C2A640
};

struct CRegistryB_00c14a50 {
    void Remove(CData_00c14a50* data);                         // 0x00C2BC10
};

struct CDataOwner_00c14a50 {
    void* m_vtable;                                            // +0x00 (0x012B29F8)
    long m_4;
    CRegistryA_00c14a50* m_pRegistryA;                         // +0x08
    CRegistryB_00c14a50* m_pRegistryB;                         // +0x0c
    void DeleteData(CData_00c14a50* data);
};

void CDataOwner_00c14a50::DeleteData(CData_00c14a50* data)
{
    m_pRegistryA->Remove(data);
    m_pRegistryB->Remove(data);
    delete data;
}