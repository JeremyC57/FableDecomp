// CAggregate_004e0080::~CAggregate_004e0080() -- implicit destructor: members at +0x48, +0x40, +0x38 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0080, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x48] ; call CMember_004b8940::~CMember_004b8940 ; lea ecx,[esi+0x40] ; call CMember_00a01c10::~CMember_00a01c10 ; lea ecx,[esi+0x38] ; call CMember_00a01c10::~CMember_00a01c10 ; mov ecx,esi ; pop esi ; jmp CMember_004da8b7::~CMember_004da8b7
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x004DA8B7) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
struct CMember_004b8940 {
    unsigned char _body[0x8];
    ~CMember_004b8940();                            // 0x004B8940
};

struct CMember_004da8b7 {
    unsigned char _body[0x4];
    ~CMember_004da8b7();                            // 0x004DA8B7
};

struct CMember_00a01c10 {
    unsigned char _body[0x8];
    ~CMember_00a01c10();                            // 0x00A01C10
};

struct CAggregate_004e0080 {
    CMember_004da8b7 m_first;            // +0 (its destructor is the tail call)
    unsigned char _gap0[0x34];
    CMember_00a01c10 m_38;            // +0x38
    CMember_00a01c10 m_40;            // +0x40
    CMember_004b8940 m_48;            // +0x48
    ~CAggregate_004e0080();
};

CAggregate_004e0080::~CAggregate_004e0080()
{
}
