// CIVCountedPointeeBase_Derived_00bc7ab0::`scalar deleting destructor'(unsigned int) -- the list member at +0x60 cleared out of line (@ 0x0099B510), the CIVCountedPointeeBase vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7AB0, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x60] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CIVCountedPointeeBase::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CIVCountedPointeeBase-derived class that adds one list member: the member's
// out-of-line destructor @ 0x0099B510 runs, the inlined base destructor stores the base vtable
// (0x012354D0, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_0099b510 {
    void* m_head;
    ~CListMember_0099b510();            // 0x0099B510
};

struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() { }
    unsigned char _body[0x5c];
};

struct CIVCountedPointeeBase_Derived_00bc7ab0 : CIVCountedPointeeBase {
    CListMember_0099b510 m_60;        // +0x60
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CIVCountedPointeeBase_Derived_00bc7ab0* Create_CIVCountedPointeeBase_Derived_00bc7ab0()
{
    return new CIVCountedPointeeBase_Derived_00bc7ab0;
}

void Destroy_CIVCountedPointeeBase_Derived_00bc7ab0(CIVCountedPointeeBase_Derived_00bc7ab0* object)
{
    delete object;
}
