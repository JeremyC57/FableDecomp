// CAggregate_006df860::~CAggregate_006df860() -- implicit destructor: members at +0xbc, +0xb8 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF860, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xbc] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0xb8] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; pop esi ; jmp CMember_005cc091::~CMember_005cc091
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x005CC091) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_005cc091 {
    unsigned char _body[0x4];
    ~CMember_005cc091();                            // 0x005CC091
};

struct CMember_0099eae0 {
    unsigned char _body[0x4];
    ~CMember_0099eae0();                            // 0x0099EAE0
};

struct CMember_00a01c10 {
    unsigned char _body[0x4];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CAggregate_006df860 {
    CMember_005cc091 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0xb4];
    CMember_0099eae0 m_b8;            // +0xb8
    CMember_00a01c10 m_bc;            // +0xbc
    ~CAggregate_006df860();
};

CAggregate_006df860::~CAggregate_006df860()
{
}