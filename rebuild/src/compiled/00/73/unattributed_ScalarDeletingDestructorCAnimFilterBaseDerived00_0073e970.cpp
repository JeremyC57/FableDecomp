// CAnimFilterBase_Derived_0073e970::`scalar deleting destructor'(unsigned int) -- the list member at +0x4 cleared out of line (@ 0x0099EAE0), the CAnimFilterBase vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E970, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x4] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CAnimFilterBase::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CAnimFilterBase-derived class that adds one list member: the member's
// out-of-line destructor @ 0x0099EAE0 runs, the inlined base destructor stores the base vtable
// (0x0122FFCC, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_0099eae0 {
    void* m_head;
    ~CListMember_0099eae0();            // 0x0099EAE0
};

class CAnimFilterBase { public:
    virtual ~CAnimFilterBase() { }
};

struct CAnimFilterBase_Derived_0073e970 : CAnimFilterBase {
    CListMember_0099eae0 m_04;        // +0x4
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CAnimFilterBase_Derived_0073e970* Create_CAnimFilterBase_Derived_0073e970()
{
    return new CAnimFilterBase_Derived_0073e970;
}

void Destroy_CAnimFilterBase_Derived_0073e970(CAnimFilterBase_Derived_0073e970* object)
{
    delete object;
}