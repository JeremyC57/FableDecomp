// CSteeringGroup::CSteeringGroup() -- steering-group constructor: install the group vtable (0x012B40A4), index word -1 (not placed), no members
// Leaf: ??0CSteeringGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29F50, length 32 bytes (23 + int3 padding), __fastcall, returns `this`.
// Reference: the only use is the `eh vector constructor iterator` call at 0x00C240AE (`push 0x00C29EB0 ; push 0x00C29F50 ;
//   push count ; push 0Ch`): `new CSteeringGroup[count]` (12-byte elements, the count word stored before the array), with the
//   destructor @ 0x00C29EB0 (a bare vtable store: the class declares an empty destructor) as the element destructor.
// Identity: the vtable 0x012B40A4 is the group's: slot 0 @ 0x00C29FA0 returns &m_index, slot 1 @ 0x00C29D00 forwards a call
//   to every member (the +0x08 list linked through the agents' +0x34), and AddAgent @ 0x00C29EC0 / RemoveAgent @ 0x00C29EE0
//   (this chunk) maintain the same list. No PDB covers the AI steering library: the names are descriptive.
// Shape: the vptr store first, then the members in declaration order through eax (= this, the return value).
// Label correction: the catalog's `Ctor_Init_012b40a4` is descriptive.
class CSteeringAgent;

class CSteeringGroup {
public:
    CSteeringGroup();
    ~CSteeringGroup() { }                           // 0x00C29EB0 (the vtable store only)
    virtual long* GetIndexSlot();                   // slot 0 @ 0x00C29FA0
    virtual void  ForEachMember(void* argument);    // slot 1 @ 0x00C29D00
    long            m_index;                        // +0x04 (-1 = not placed)
    CSteeringAgent* m_pFirst;                       // +0x08 first member (linked through the agent's +0x34)
};

CSteeringGroup::CSteeringGroup()
    : m_index(-1), m_pFirst(0)
{
}