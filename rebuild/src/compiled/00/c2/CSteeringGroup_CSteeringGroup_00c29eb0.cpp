// CSteeringGroup::~CSteeringGroup() -- the group's empty destructor: VC7.1 still re-installs the group vtable (0x012B40A4)
// Leaf: ??1CSteeringGroup@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29EB0, 7 bytes: mov dword ptr [ecx],12B40A4h ; ret
// Naming (sweep1, 2026-10-25): was landed as `Ctor_SetVtable_012b40a4` (a descriptive placeholder on a struct T);
//   descriptive (no PDB covers this library). FINDINGS chunk 436: it is the CSteeringGroup destructor -- the element
//   destructor that `new CSteeringGroup[count]` hands to `eh vector constructor iterator` at 0x00C240AE together with the
//   landed constructor 0x00C29F50 (`push 0x00C29EB0 ; push 0x00C29F50 ; push count ; push 0Ch`); its other references are
//   0x00C22D22 / 0x00C22D4A / 0x00C24261 (the array's delete[] paths). Model: agent436's CSteeringGroup (vtable
//   0x012B40A4: slot 0 GetIndexSlot 0x00C29FA0, slot 1 ForEachMember 0x00C29D00).
class CSteeringAgent;

class CSteeringGroup {
public:
    ~CSteeringGroup();
    virtual long* GetIndexSlot();               // slot 0 @ 0x00C29FA0
    virtual void  ForEachMember(void* argument);   // slot 1 @ 0x00C29D00
    long            m_index;                    // +0x04 (-1 = not placed)
    CSteeringAgent* m_pFirst;                   // +0x08 first member
};

CSteeringGroup::~CSteeringGroup()
{
}
