// CBase_0062e900_Derived_008cedd0::`scalar deleting destructor'(unsigned int) -- non-virtual: 2 members destroyed out of line, CBase_0062e900::~CBase_0062e900, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CEDD0, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x11c] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x114] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; call CBase_0062e900::~CBase_0062e900 ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: members at +0x11c, +0x114 are destroyed in reverse
// declaration order, then the base class destructor @ 0x0062E900 runs, and bit 0 of the flag
// argument selects the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`vector
// deleting destructor`) is propagated; names come from the destructor labels when unique.
// One of 15 bodies of this shape.
struct CMember_00a01c10 {
    unsigned char _body[0x8];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CBase_0062e900 {
    unsigned char _body[0x114];
    ~CBase_0062e900();                             // 0x0062E900
};

struct CBase_0062e900_Derived_008cedd0 : CBase_0062e900 {
    CMember_00a01c10 m_114;            // +0x114
    CMember_00a01c10 m_11c;            // +0x11c
};

// The delete-expression that makes the compiler emit CBase_0062e900_Derived_008cedd0::`scalar deleting destructor'.
void Destroy_CBase_0062e900_Derived_008cedd0(CBase_0062e900_Derived_008cedd0* object)
{
    delete object;
}