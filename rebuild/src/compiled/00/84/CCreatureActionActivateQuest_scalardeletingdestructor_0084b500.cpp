// CPhysicsMeshInfo_Derived_0084b500::`scalar deleting destructor'(unsigned int) -- non-virtual: 2 members destroyed out of line, CPhysicsMeshInfo::~CPhysicsMeshInfo, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B500, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xad] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0xa8] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; call CPhysicsMeshInfo::~CPhysicsMeshInfo ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: members at +0xad, +0xa8 are destroyed in reverse
// declaration order, then the base class destructor @ 0x00693EF0 runs, and bit 0 of the flag
// argument selects the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`vector
// deleting destructor`) is propagated; names come from the destructor labels when unique.
// One of 15 bodies of this shape.
struct CMember_0099eae0 {
    unsigned char _body[0x5];
    ~CMember_0099eae0();                            // 0x0099EAE0
};

struct CMember_00a01c10 {
    unsigned char _body[0x5];
    ~CMember_00a01c10();                            // 0x00A01C10
};

class CPhysicsMeshInfo { public:
    unsigned char _body[0xa8];
    ~CPhysicsMeshInfo();                             // 0x00693EF0
};

struct CPhysicsMeshInfo_Derived_0084b500 : CPhysicsMeshInfo {
    CMember_0099eae0 m_a8;            // +0xa8
    CMember_00a01c10 m_ad;            // +0xad
};

// The delete-expression that makes the compiler emit CPhysicsMeshInfo_Derived_0084b500::`scalar deleting destructor'.
void Destroy_CPhysicsMeshInfo_Derived_0084b500(CPhysicsMeshInfo_Derived_0084b500* object)
{
    delete object;
}