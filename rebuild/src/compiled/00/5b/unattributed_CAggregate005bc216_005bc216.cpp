// CAggregate_005bc216::~CAggregate_005bc216() -- implicit destructor: members at +0x8c, +0x80 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC216, oracle length 33 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8c] ; call CMember_004ac8f0::~CMember_004ac8f0 ; lea ecx,[esi+0x80] ; call CMember_005bc0d3::~CMember_005bc0d3 ; mov ecx,esi ; pop esi ; jmp CMember_00550a00::~CMember_00550a00
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00550A00) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_004ac8f0 {
    unsigned char _body[0xc];
    ~CMember_004ac8f0();                            // 0x004AC8F0
};

struct CMember_00550a00 {
    unsigned char _body[0x4];
    ~CMember_00550a00();                            // 0x00550A00
};

struct CMember_005bc0d3 {
    unsigned char _body[0xc];
    ~CMember_005bc0d3();                            // 0x005BC0D3
};

struct CAggregate_005bc216 {
    CMember_00550a00 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x7c];
    CMember_005bc0d3 m_80;            // +0x80
    CMember_004ac8f0 m_8c;            // +0x8c
    ~CAggregate_005bc216();
};

CAggregate_005bc216::~CAggregate_005bc216()
{
}