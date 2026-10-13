// CAggregate_004e3275::~CAggregate_004e3275() -- implicit destructor: members at +0x18, +0xc destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3275, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x18] ; call CMember_004dd52b::~CMember_004dd52b ; lea ecx,[esi+0xc] ; call CMember_004dd56c::~CMember_004dd56c ; mov ecx,esi ; pop esi ; jmp CMember_00686830::~CMember_00686830
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00686830) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~CSymbolInfo>_>`) is propagated. One of 19 bodies of this shape.
struct CMember_004dd52b {
    unsigned char _body[0xc];
    ~CMember_004dd52b();                            // 0x004DD52B
};

struct CMember_004dd56c {
    unsigned char _body[0xc];
    ~CMember_004dd56c();                            // 0x004DD56C
};

struct CMember_00686830 {
    unsigned char _body[0x4];
    ~CMember_00686830();                            // 0x00686830
};

struct CAggregate_004e3275 {
    CMember_00686830 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x8];
    CMember_004dd56c m_0c;            // +0xc
    CMember_004dd52b m_18;            // +0x18
    ~CAggregate_004e3275();
};

CAggregate_004e3275::~CAggregate_004e3275()
{
}
