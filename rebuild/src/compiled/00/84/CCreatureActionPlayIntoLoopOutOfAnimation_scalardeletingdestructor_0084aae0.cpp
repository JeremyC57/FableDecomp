// CPhysicsMeshInfo_Derived_0084aae0::`scalar deleting destructor'(unsigned int) -- non-virtual: 2 members destroyed out of line, CPhysicsMeshInfo::~CPhysicsMeshInfo, flag-tested delete
// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was CMember_0099eae0); name-only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084AAE0, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xc0] ; call CCharString::~CCharString ; lea ecx,[esi+0xbc] ; call CCharString::~CCharString ; mov ecx,esi ; call CPhysicsMeshInfo::~CPhysicsMeshInfo ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic class (no vtable store anywhere): VC7.1 emits a scalar deleting
// destructor for any class with a non-trivial destructor that a translation unit deletes. The
// implicit destructor is inlined: members at +0xc0, +0xbc are destroyed in reverse
// declaration order, then the base class destructor @ 0x00693EF0 runs, and bit 0 of the flag
// argument selects the flag-tested operator delete @ 0x00BFE9BC. The manifest label (`vector
// deleting destructor`) is propagated; names come from the destructor labels when unique.
// One of 15 bodies of this shape.
class CCharString { public:
    unsigned char _body[0x4];
    ~CCharString();                            // 0x0099EAE0
};

class CPhysicsMeshInfo { public:
    unsigned char _body[0xbc];
    ~CPhysicsMeshInfo();                             // 0x00693EF0
};

struct CPhysicsMeshInfo_Derived_0084aae0 : CPhysicsMeshInfo {
    CCharString m_bc;            // +0xbc
    CCharString m_c0;            // +0xc0
};

// The delete-expression that makes the compiler emit CPhysicsMeshInfo_Derived_0084aae0::`scalar deleting destructor'.
void Destroy_CPhysicsMeshInfo_Derived_0084aae0(CPhysicsMeshInfo_Derived_0084aae0* object)
{
    delete object;
}