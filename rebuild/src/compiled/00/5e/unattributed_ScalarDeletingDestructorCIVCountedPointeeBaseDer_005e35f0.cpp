// CIVCountedPointeeBase_Derived_005e35f0::`scalar deleting destructor'(unsigned int) -- the list member at +0x8 cleared out of line (@ 0x005E3040), the CIVCountedPointeeBase vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E35F0, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CIVCountedPointeeBase::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CIVCountedPointeeBase-derived class that adds one list member: the member's
// out-of-line destructor @ 0x005E3040 runs, the inlined base destructor stores the base vtable
// (0x012354D0, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_005e3040 {
    void* m_head;
    ~CListMember_005e3040();            // 0x005E3040
};

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    unsigned char _body[0x4];
};

struct CIVCountedPointeeBase_Derived_005e35f0 : CIVCountedPointeeBase {
    CListMember_005e3040 m_08;        // +0x8
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CIVCountedPointeeBase_Derived_005e35f0* Create_CIVCountedPointeeBase_Derived_005e35f0()
{
    return new CIVCountedPointeeBase_Derived_005e35f0;
}

void Destroy_CIVCountedPointeeBase_Derived_005e35f0(CIVCountedPointeeBase_Derived_005e35f0* object)
{
    delete object;
}