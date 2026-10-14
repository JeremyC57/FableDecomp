// CNavButton_00c10320::~CNavButton() -- EH-framed destructor of a `__declspec(novtable)` most-derived class (no vtable store at entry; state 1): drains the member container at +0x10 (slot 0x2c yields the first entry; each entry is detached from the button through slot 0x1c and released with 1 through slot 0x20) and then the button's own container (same slots on `this`); the member container's inline destructor follows (vtable 0x012B1B78, node at +0x18 @ 0x00C17B10, root vtable 0x012B1AB0; state 2) and the base container's (vtable 0x012B1C40, node at +8, root vtable; state 3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10320, oracle length 217 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; mov [esp+0Ch],edi ; mov eax,[edi+10h] ; lea ebx,[edi+10h] ; mov ecx,ebx ; mov dword ptr [esp+1Ch],1 ; call [eax+2Ch] ; mov esi,eax ; test esi,esi ; je second ; loop1: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+1Ch] ; mov eax,[esi] ; push 1 ; mov ecx,esi ; call [eax+20h] ; mov edx,[ebx] ; mov ecx,ebx ; call [edx+2Ch] ; mov esi,eax ; test esi,esi ; jne loop1 ; second: mov eax,[edi] ; mov ecx,edi ; call [eax+2Ch] ; ... (same loop on this) ; mov [esp+10h],ebx ; mov dword ptr [ebx],12B1B78h ; lea ecx,[ebx+8] ; mov byte ptr [esp+1Ch],2 ; call CNode::~CNode ; mov dword ptr [ebx],12B1AB0h ; mov dword ptr [edi],12B1C40h ; lea ecx,[edi+8] ; mov dword ptr [esp+1Ch],3 ; call CNode::~CNode ; ... mov dword ptr [edi],12B1AB0h ; ... ret
// The unwind funclets call out-of-line copies of the inline root and container destructors (0x00C09B60 / 0x00C098C0 / 0x00C09F30 / 0x00C09B70). The classes are named by address.
struct IEntry_00c10320 {                   // a registered entry
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Detach(void* owner);      // slot 0x1c
    virtual void Release(long flags);      // slot 0x20
};

struct CNode_00c10320 {                    // 8-byte list node
    unsigned char _body[8];
    ~CNode_00c10320();                     // 0x00C17B10
};

struct CRoot_00c10320 {                    // vtable 0x012B1AB0 (folded)
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual IEntry_00c10320* First();      // slot 0x2c
    CNode_00c10320* m_pNode;               // +4
    ~CRoot_00c10320() { }
};

struct CContainerA_00c10320 : CRoot_00c10320 {   // vtable 0x012B1C40: the button's own list
    CNode_00c10320 m_node;                 // +8
    ~CContainerA_00c10320() { }
};

struct CContainerB_00c10320 : CRoot_00c10320 {   // vtable 0x012B1B78: the member list at +0x10
    CNode_00c10320 m_node;                 // +8
    ~CContainerB_00c10320() { }
};

struct __declspec(novtable) CNavButton_00c10320 : CContainerA_00c10320 {
    CContainerB_00c10320 m_10;             // +0x10
    IEntry_00c10320* FirstEntry() { return First(); }   // the virtual call written through a non-virtual helper stays a dynamic dispatch in the destructor
    virtual ~CNavButton_00c10320();
};

CNavButton_00c10320::~CNavButton_00c10320()
{
    IEntry_00c10320* entry;
    for (entry = m_10.First(); entry; entry = m_10.First()) {
        entry->Detach(this);
        entry->Release(1);
    }
    for (entry = FirstEntry(); entry; entry = FirstEntry()) {
        entry->Detach(this);
        entry->Release(1);
    }
}
