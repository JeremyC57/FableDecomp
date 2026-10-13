// Mass_Derived_008effd0::`scalar deleting destructor'(unsigned int) -- the list member at +0x25 cleared out of line (@ 0x00A01C10), the Mass vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EFFD0, oracle length 39 bytes.
// Grade: candidate -- still UNGRADED in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x25] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset Mass::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a Mass-derived class that adds one list member: the member's
// out-of-line destructor @ 0x00A01C10 runs, the inlined base destructor stores the base vtable
// (0x01288230, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_00a01c10 {
    void* m_head;
    ~CListMember_00a01c10();            // 0x00A01C10
};

struct Mass {
    virtual ~Mass() { }
    unsigned char _body[0x21];
};

struct Mass_Derived_008effd0 : Mass {
    CListMember_00a01c10 m_25;        // +0x25
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
Mass_Derived_008effd0* Create_Mass_Derived_008effd0()
{
    return new Mass_Derived_008effd0;
}

void Destroy_Mass_Derived_008effd0(Mass_Derived_008effd0* object)
{
    delete object;
}
