// CAggregate_004585bd::~CAggregate_004585bd() -- implicit destructor: members at +0x54, +0x48, +0x3c destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004585BD, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x54] ; call CMember_00451ac6::~CMember_00451ac6 ; lea ecx,[esi+0x48] ; call CMember_00451ac6::~CMember_00451ac6 ; lea ecx,[esi+0x3c] ; call CMember_00451a85::~CMember_00451a85 ; mov ecx,esi ; pop esi ; jmp CMember_00430300::~CMember_00430300
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x00430300) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
struct CMember_00430300 {
    unsigned char _body[0x4];
    ~CMember_00430300();                            // 0x00430300
};

struct CMember_00451a85 {
    unsigned char _body[0xc];
    ~CMember_00451a85();                            // 0x00451A85
};

struct CMember_00451ac6 {
    unsigned char _body[0xc];
    ~CMember_00451ac6();                            // 0x00451AC6
};

struct CAggregate_004585bd {
    CMember_00430300 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x38];
    CMember_00451a85 m_3c;            // +0x3c
    CMember_00451ac6 m_48;            // +0x48
    CMember_00451ac6 m_54;            // +0x54
    ~CAggregate_004585bd();
};

CAggregate_004585bd::~CAggregate_004585bd()
{
}
