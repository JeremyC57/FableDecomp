// CSB_Derived_00a22330::`scalar deleting destructor'(unsigned int) -- the list member at +0x18 cleared out of line (@ 0x00A21250), the CSB vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A22330, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x18] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CSB::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CSB-derived class that adds one list member: the member's
// out-of-line destructor @ 0x00A21250 runs, the inlined base destructor stores the base vtable
// (0x0129CC1C, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_00a21250 {
    void* m_head;
    ~CListMember_00a21250();            // 0x00A21250
};

struct CSB {
    virtual ~CSB() { }
    unsigned char _body[0x14];
};

struct CSB_Derived_00a22330 : CSB {
    CListMember_00a21250 m_18;        // +0x18
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CSB_Derived_00a22330* Create_CSB_Derived_00a22330()
{
    return new CSB_Derived_00a22330;
}

void Destroy_CSB_Derived_00a22330(CSB_Derived_00a22330* object)
{
    delete object;
}
