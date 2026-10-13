// CBase_00cbd510_Derived_00d1cbd0::`scalar deleting destructor'(unsigned int) -- non-virtual: 2 members destroyed out of line, CBase_00cbd510::~CBase_00cbd510, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1CBD0, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x88] ; call CMember_00cdbc90::~CMember_00cdbc90 ; lea ecx,[esi+0x80] ; call CMember_00cd4470::~CMember_00cd4470 ; mov ecx,esi ; call CBase_00cbd510::~CBase_00cbd510 ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: members at +0x88, +0x80 are destroyed in reverse
// declaration order, then the base class destructor @ 0x00CBD510 runs, and bit 0 of the flag
// argument selects the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`vector
// deleting destructor`) is propagated; names come from the destructor labels when unique.
// One of 15 bodies of this shape.
struct CMember_00cd4470 {
    unsigned char _body[0x8];
    ~CMember_00cd4470();                            // 0x00CD4470
};

struct CMember_00cdbc90 {
    unsigned char _body[0x8];
    ~CMember_00cdbc90();                            // 0x00CDBC90
};

struct CBase_00cbd510 {
    unsigned char _body[0x80];
    ~CBase_00cbd510();                             // 0x00CBD510
};

struct CBase_00cbd510_Derived_00d1cbd0 : CBase_00cbd510 {
    CMember_00cd4470 m_80;            // +0x80
    CMember_00cdbc90 m_88;            // +0x88
};

// The delete-expression that makes the compiler emit CBase_00cbd510_Derived_00d1cbd0::`scalar deleting destructor'.
void Destroy_CBase_00cbd510_Derived_00d1cbd0(CBase_00cbd510_Derived_00d1cbd0* object)
{
    delete object;
}