// CAggregate_0092c2e0::~CAggregate_0092c2e0() -- implicit destructor: members at +0xb4, +0xb0, +0xa8 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C2E0, oracle length 44 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xb4] ; call CMember_0099eae0::~CMember_0099eae0 ; lea ecx,[esi+0xb0] ; call CMember_0099eae0::~CMember_0099eae0 ; lea ecx,[esi+0xa8] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CPhysicsMeshInfo::~CPhysicsMeshInfo
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00693EF0) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
struct CPhysicsMeshInfo {
    unsigned char _body[0x4];
    ~CPhysicsMeshInfo();                            // 0x00693EF0
};

struct CMember_0099eae0 {
    unsigned char _body[0x4];
    ~CMember_0099eae0();                            // 0x0099EAE0
};

struct CMember_00a01c10 {
    unsigned char _body[0x4];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CAggregate_0092c2e0 {
    CPhysicsMeshInfo m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0xa4];
    CMember_00a01c10 m_a8;            // +0xa8
    unsigned char _gap1[0x4];
    CMember_0099eae0 m_b0;            // +0xb0
    CMember_0099eae0 m_b4;            // +0xb4
    ~CAggregate_0092c2e0();
};

CAggregate_0092c2e0::~CAggregate_0092c2e0()
{
}