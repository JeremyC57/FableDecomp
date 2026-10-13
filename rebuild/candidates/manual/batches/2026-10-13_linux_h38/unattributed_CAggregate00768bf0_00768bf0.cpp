// CAggregate_00768bf0::~CAggregate_00768bf0() -- implicit destructor: members at +0x10, +0x4 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00768BF0, oracle length 27 bytes.
// Grade: candidate -- still DIFFER(27v27) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x10] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x4] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; pop esi ; jmp CMember_0099eae0::~CMember_0099eae0
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x0099EAE0) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_0099eae0 {
    unsigned char _body[0xc];
    ~CMember_0099eae0();                            // 0x0099EAE0
};

struct CMember_00a01c10 {
    unsigned char _body[0xc];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CAggregate_00768bf0 {
    CMember_0099eae0 m_first;            // +0 (its destructor is the tail call)
    CMember_0099eae0 m_04;            // +0x4
    CMember_00a01c10 m_10;            // +0x10
    ~CAggregate_00768bf0();
};

CAggregate_00768bf0::~CAggregate_00768bf0()
{
}
