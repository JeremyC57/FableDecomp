// CAggregate_009a8e60::~CAggregate_009a8e60() -- implicit destructor: members at +0xc, +0x8, +0x4 destroyed out of line, the +0 member by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A8E60, oracle length 35 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0xc] ; call CWideString::~CWideString ; lea ecx,[esi+0x8] ; call CWideString::~CWideString ; lea ecx,[esi+0x4] ; call CWideString::~CWideString ; mov ecx,esi ; pop esi ; jmp CWideString::~CWideString
// Implicit (compiler-generated) destructor of a non-polymorphic aggregate: members are destroyed
// in reverse declaration order through their out-of-line destructors and the first member's
// destructor (@ 0x0099B510) is reached by tail call. Member types are named from the destructor
// labels when unique, else by the destructor address. The manifest label (`_Dest_val<...>` /
// `~allocator<CMacro>,CMacro>`) is propagated. One of 13 bodies of this shape.
struct CWideString {
    unsigned char _body[0x4];
    ~CWideString();                            // 0x0099B510
};

struct CAggregate_009a8e60 {
    CWideString m_first;                 // +0 (its destructor is the tail call)
    CWideString m_04;                 // +0x4
    CWideString m_08;                 // +0x8
    CWideString m_0c;                 // +0xc
    ~CAggregate_009a8e60();
};

CAggregate_009a8e60::~CAggregate_009a8e60()
{
}