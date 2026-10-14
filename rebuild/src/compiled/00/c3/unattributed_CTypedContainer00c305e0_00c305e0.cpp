// CTypedContainer_00c305e0::CTypedContainer_00c305e0() -- EH-framed constructor: the root base keeps a pointer to the node member at +8, the node is built out of line with 1 (@ 0x00C185B0, state 1 after it), then the virtual Init (slot 6) runs through an inline helper; vtable 0x012B4B90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C305E0, oracle length 89 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+8] ; mov [esp+4],esi ; mov [esi+4],ecx ; push 1 ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call CNode::CNode ; mov eax,[esi] ; mov ecx,esi ; mov dword ptr [esp+10h],1 ; call [eax+0x18] ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Three-level constructor. The polymorphic root takes the address of the node member (+4, stored
// before EH state 0 because it is the root's own member; the root's vtable store is dead and
// dropped), the container stores its vtable and builds the node member at +8 out of line with
// the constant 1 (state 1 once it exists), and the most-derived class -- `novtable`, so it
// stores nothing of its own -- calls the virtual Init through an inline non-virtual helper, which
// is what keeps the call virtual (a direct `this->Init()` in a constructor is devirtualised).
// Built under /EHsc (the funclets land through the funclet pass). The manifest label is
// propagated garbage; the classes are named by address. One of 3 bodies of this shape.
struct CNode {
    unsigned char _body[0x10];
    CNode(long kind);                      // 0x00C185B0
    ~CNode();
};

struct CRoot_00c305e0 {                          // the root owns the vptr and the node pointer
    virtual void v00();
    CNode* m_pNode;                        // +4
    CRoot_00c305e0(CNode* node) : m_pNode(node) { }
    ~CRoot_00c305e0() { }
};

struct CContainer_00c305e0 : CRoot_00c305e0 {          // vtable 0x012B4B90
    CNode m_node;                          // +8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void Init();                   // slot 6
    __forceinline void Initialise() { Init(); }
    CContainer_00c305e0() : CRoot_00c305e0(&m_node), m_node(1) { }
    ~CContainer_00c305e0() { }
};

struct __declspec(novtable) CTypedContainer_00c305e0 : CContainer_00c305e0 {
    CTypedContainer_00c305e0();
};

CTypedContainer_00c305e0::CTypedContainer_00c305e0()
{
    Initialise();
}