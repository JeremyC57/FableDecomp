// CTypedContainer_00c30640::~CTypedContainer_00c30640() -- EH-framed destructor: drains the container (virtual RemoveFirst slot 11 / OnRemove slot 8, each item destroyed @ 0x00C3B960 and freed), then the node member at +8 (@ 0x00C17B10) and the base vtable (0x012B4B90 then 0x012B1AB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C30640, oracle length 135 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov eax,[esi] ; mov dword ptr [esp+14h],0 ; call [eax+0x2c] ; mov edi,eax ; test edi,edi ; je done ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+0x20] ; test edi,edi ; je next ; mov ecx,edi ; call CItem::~CItem ; push edi ; call operator delete ; add esp,4 ; next: mov eax,[esi] ; mov ecx,esi ; call [eax+0x2c] ; mov edi,eax ; test edi,edi ; jne loop ; done: mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov dword ptr [esp+14h],1 ; call CNode::~CNode ; mov ecx,[esp+0Ch] ; mov dword ptr [esi],offset root vtable ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The most-derived class is `novtable` (it stores no vtable of its own, which is why the loop
// runs before any vtable store) and its destructor drains the container through an inline
// non-virtual helper -- the virtual calls stay virtual only because they are not written
// directly in the destructor body. Each item is `delete`d (null-checked, destroyed @
// 0x00C3B960, freed through operator delete @ 0x00BFE9BC). Then the container's inline
// destructor runs: its vtable, the node member at +8 (@ 0x00C17B10, EH state 1 while the root
// still lives), and the root's inline empty destructor restores the root vtable. Built under
// /EHsc (the funclets land through the funclet pass). The manifest label is propagated garbage;
// the classes are named by address. One of 3 bodies of this shape.
struct CItem {
    ~CItem();                              // 0x00C3B960
};

struct CNode {
    unsigned char _body[0x10];
    CNode(long kind);
    ~CNode();                              // 0x00C17B10
};

struct CRoot_00c30640 {                          // vtable 0x012B1AB0
    virtual void v00();
    CNode* m_pNode;                        // +4
    CRoot_00c30640(CNode* node) : m_pNode(node) { }
    ~CRoot_00c30640() { }
};

struct CContainer_00c30640 : CRoot_00c30640 {          // vtable 0x012B4B90
    CNode m_node;                          // +8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void OnRemove(CItem* item);   // slot 8
    virtual void v09();
    virtual void v10();
    virtual CItem* RemoveFirst();          // slot 11
    __forceinline void Clear()
    {
        while (CItem* item = RemoveFirst()) {
            OnRemove(item);
            delete item;
        }
    }
    CContainer_00c30640() : CRoot_00c30640(&m_node), m_node(1) { }
    ~CContainer_00c30640() { }
};

struct __declspec(novtable) CTypedContainer_00c30640 : CContainer_00c30640 {
    ~CTypedContainer_00c30640();
};

CTypedContainer_00c30640::~CTypedContainer_00c30640()
{
    Clear();
}
