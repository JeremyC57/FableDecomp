// CAggregate_007a9820::~CAggregate_007a9820() -- implicit destructor: members at +0x11c, +0x114 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A9820, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x11c] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x114] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CMember_0062e900::~CMember_0062e900
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x0062E900) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_0062e900 {
    unsigned char _body[0x4];
    ~CMember_0062e900();                            // 0x0062E900
};

struct CMember_00a01c10 {
    unsigned char _body[0x8];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CAggregate_007a9820 {
    CMember_0062e900 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x110];
    CMember_00a01c10 m_114;            // +0x114
    CMember_00a01c10 m_11c;            // +0x11c
    ~CAggregate_007a9820();
};

CAggregate_007a9820::~CAggregate_007a9820()
{
}