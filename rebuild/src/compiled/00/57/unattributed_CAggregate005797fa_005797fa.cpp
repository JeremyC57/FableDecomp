// CAggregate_005797fa::~CAggregate_005797fa() -- implicit destructor: members at +0xc, +0x8, +0x4 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005797FA, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xc] ; call CMember_0042ab73::~CMember_0042ab73 ; lea ecx,[esi+0x8] ; call CMember_0042ab73::~CMember_0042ab73 ; lea ecx,[esi+0x4] ; call CMember_0042ab73::~CMember_0042ab73 ; mov ecx,esi ; pop esi ; jmp CMember_0042ab73::~CMember_0042ab73
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x0042AB73) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
struct CMember_0042ab73 {
    unsigned char _body[0x4];
    ~CMember_0042ab73();                            // 0x0042AB73
};

struct CAggregate_005797fa {
    CMember_0042ab73 m_first;            // +0 (its destructor is the tail call)
    CMember_0042ab73 m_04;            // +0x4
    CMember_0042ab73 m_08;            // +0x8
    CMember_0042ab73 m_0c;            // +0xc
    ~CAggregate_005797fa();
};

CAggregate_005797fa::~CAggregate_005797fa()
{
}