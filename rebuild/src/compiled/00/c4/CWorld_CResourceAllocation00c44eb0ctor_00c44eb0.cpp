// CWorld_00c44eb0::CResourceAllocation(world, owner) -- EH-framed constructor (the manifest's "OnResourceAllocate" is the propagated caller label): stores the world (+0) and owner (+4), builds the +8 part from the owner's +8 part's value at +0x1c (@ 0x00C1E750, ctor @ 0x00C23C10, state 0), the +0x20 part from the +8 part (ctor @ 0x00C45660, state 1), the 0x12B7CE8 container at +0x50 (root 0x12B1AB0 inline, node @ 0x00C194F0; states 1-3, the container pointer homed in the dead world argument slot) and the +0x80 array sized by the world's resource lookup (@ 0x00C36CF0 through the resource table at +0 with the count at +0x164; ctor @ 0x00C61B90, state 4), then registers itself with the world's +0x14 registry (slot 0x24) and runs the two initialisers (@ 0x00C44C50, @ 0x00C44CE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44EB0, oracle length 195 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; push esi ; mov esi,ecx ; mov [esi],eax ; mov eax,[esp+1Ch] ; mov [esi+4],eax ; mov ecx,[eax+8] ; push edi ; mov [esp+8],esi ; lea edi,[esi+8] ; call Get1C ; push eax ; mov ecx,edi ; call CPart8::CPart8 ; push edi ; lea ecx,[esi+20h] ; mov dword ptr [esp+18h],0 ; call CPart20::CPart20 ; lea eax,[esi+50h] ; lea ecx,[eax+8] ; mov [esp+1Ch],eax ; mov [eax+4],ecx ; push 0 ; mov byte ptr [esp+18h],2 ; mov dword ptr [eax],12B7CE8h ; call CNode::CNode ; mov eax,[esi] ; mov ecx,[eax+164h] ; push ecx ; mov ecx,[eax] ; mov byte ptr [esp+18h],3 ; call Lookup ; push eax ; lea ecx,[esi+80h] ; call CArray::CArray ; mov eax,[esi] ; mov edx,[eax+14h] ; lea ecx,[eax+14h] ; push esi ; mov byte ptr [esp+18h],4 ; call [edx+24h] ; mov ecx,esi ; call Init1 ; mov ecx,esi ; call Init2 ; ... mov eax,esi ; ... ret 8
// The container root and the container destructors are out of line (0x00C452C0 / 0x00C45750).
// The classes are named by address.
struct CNode_00c44eb0 {                    // 0x28 bytes
    unsigned char _body[0x28];
    CNode_00c44eb0(long mode);             // 0x00C194F0
    ~CNode_00c44eb0();                     // 0x00C19250
};

struct CRoot_00c44eb0 {                    // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c44eb0* m_pNode;               // +4
    CRoot_00c44eb0(CNode_00c44eb0* node) : m_pNode(node) { }
    ~CRoot_00c44eb0();                     // 0x00C452C0
};

struct CContainer_00c44eb0 : CRoot_00c44eb0 {   // vtable 0x012B7CE8
    CNode_00c44eb0 m_node;                 // +8
    CContainer_00c44eb0() : CRoot_00c44eb0(&m_node), m_node(0) { }
    ~CContainer_00c44eb0();                // 0x00C45750
};

struct CPart8_00c44eb0 {                   // +8, 0x18 bytes
    unsigned char _body[0x18];
    CPart8_00c44eb0(long value);           // 0x00C23C10
    ~CPart8_00c44eb0();                    // 0x00C22A00
};

struct CPart20_00c44eb0 {                  // +0x20, 0x30 bytes
    unsigned char _body[0x30];
    CPart20_00c44eb0(CPart8_00c44eb0* part);   // 0x00C45660
    ~CPart20_00c44eb0();                   // 0x00C456B0
};

struct CArray_00c44eb0 {                   // +0x80
    unsigned char _body[0xc];
    CArray_00c44eb0(long count);           // 0x00C61B90
    ~CArray_00c44eb0();                    // 0x00C61BD0
};

struct COwnerPart_00c44eb0 { long Get1C(); };   // 0x00C1E750
struct COwner_00c44eb0 {
    unsigned char _pad[8];
    COwnerPart_00c44eb0* m_pPart;          // +8
};

struct CResourceTable_00c44eb0 { long Lookup(long key); };   // 0x00C36CF0
struct CRegistry_00c44eb0 {
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Register(void* entry);    // slot 0x24
};
struct CWorld_00c44eb0 {
    CResourceTable_00c44eb0* m_pResources; // +0
    unsigned char _pad[0x10];
    CRegistry_00c44eb0 m_registry;         // +0x14
    unsigned char _pad2[0x164 - 0x18];
    long m_resourceKey;                    // +0x164
};

struct CResourceAllocation_00c44eb0 {
    CWorld_00c44eb0* m_pWorld;             // +0
    COwner_00c44eb0* m_pOwner;             // +4
    CPart8_00c44eb0 m_8;                   // +8
    CPart20_00c44eb0 m_20;                 // +0x20
    CContainer_00c44eb0 m_50;              // +0x50
    CArray_00c44eb0 m_80;                  // +0x80
    void Init1();                          // 0x00C44C50
    void Init2();                          // 0x00C44CE0
    CResourceAllocation_00c44eb0(CWorld_00c44eb0* world, COwner_00c44eb0* owner);
};

CResourceAllocation_00c44eb0::CResourceAllocation_00c44eb0(CWorld_00c44eb0* world, COwner_00c44eb0* owner)
    : m_pWorld(world), m_pOwner(owner), m_8(owner->m_pPart->Get1C()), m_20(&m_8), m_50(),
      m_80(m_pWorld->m_pResources->Lookup(m_pWorld->m_resourceKey))
{
    m_pWorld->m_registry.Register(this);
    Init1();
    Init2();
}