// CAggregate_0057a545::~CAggregate_0057a545() -- implicit destructor: members at +0xc0, +0xbc destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A545, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xc0] ; call CMember_0099eae0::~CMember_0099eae0 ; lea ecx,[esi+0xbc] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; pop esi ; jmp CPhysicsMeshInfo::~CPhysicsMeshInfo
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00693EF0) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CPhysicsMeshInfo {
    unsigned char _body[0x4];
    ~CPhysicsMeshInfo();                            // 0x00693EF0
};

struct CMember_0099eae0 {
    unsigned char _body[0x4];
    ~CMember_0099eae0();                            // 0x0099EAE0
};

struct CAggregate_0057a545 {
    CPhysicsMeshInfo m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0xb8];
    CMember_0099eae0 m_bc;            // +0xbc
    CMember_0099eae0 m_c0;            // +0xc0
    ~CAggregate_0057a545();
};

CAggregate_0057a545::~CAggregate_0057a545()
{
}