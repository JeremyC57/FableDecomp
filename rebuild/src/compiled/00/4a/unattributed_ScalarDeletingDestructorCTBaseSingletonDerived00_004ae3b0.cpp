// CTBaseSingleton_Derived_004ae3b0::`scalar deleting destructor'(unsigned int) -- the list member at +0x4 cleared out of line (@ 0x004AE200), the CTBaseSingleton vtable stored by the inlined base destructor, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE3B0, oracle length 39 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x4] ; call Clear ; test byte ptr [esp+8],1 ; mov dword ptr [esi],offset CTBaseSingleton::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Scalar deleting destructor of a CTBaseSingleton-derived class that adds one list member: the member's
// out-of-line destructor @ 0x004AE200 runs, the inlined base destructor stores the base vtable
// (0x01238C50, rtti-vtables.tsv), and bit 0 of the flag argument selects the flag-tested operator
// delete @ 0x00BFE9BC. The manifest label is propagated across the family; the derived class
// is named by its base. One of 14 bodies of this shape.
struct CListMember_004ae200 {
    void* m_head;
    ~CListMember_004ae200();            // 0x004AE200
};

struct CTBaseSingleton {
    virtual ~CTBaseSingleton() { }
};

struct CTBaseSingleton_Derived_004ae3b0 : CTBaseSingleton {
    CListMember_004ae200 m_04;        // +0x4
};

// Constructing the class emits its vtable (and with it the scalar deleting destructor); the
// delete-expression then calls it through the vtable.
CTBaseSingleton_Derived_004ae3b0* Create_CTBaseSingleton_Derived_004ae3b0()
{
    return new CTBaseSingleton_Derived_004ae3b0;
}

void Destroy_CTBaseSingleton_Derived_004ae3b0(CTBaseSingleton_Derived_004ae3b0* object)
{
    delete object;
}