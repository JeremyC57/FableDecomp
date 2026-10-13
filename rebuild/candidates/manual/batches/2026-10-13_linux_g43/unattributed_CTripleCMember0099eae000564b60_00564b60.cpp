// CTripleCMember_0099eae0_00564b60::~CTripleCMember_0099eae0_00564b60() -- destroys three CMember_0099eae0 members at +0x8, +0x4, +0x0 (last one by tail call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00564B60, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call CMember_0099eae0::~CMember_0099eae0 ; lea ecx,[esi+0x4] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; pop esi ; jmp CMember_0099eae0::~CMember_0099eae0
// Implicit/empty destructor of a non-polymorphic aggregate of three 4-byte CMember_0099eae0 members
// (destructor @ 0x0099EAE0): members are destroyed in reverse declaration order and the first
// one's destructor is reached by tail call. The manifest label (`_Dest_val<...pair<CCharString,
// CSymbolInfo>>`) says this is the pair value of a map; the owning type is named descriptively.
// One of 22 bodies of this shape.
struct CMember_0099eae0 {
    void* m_data;
    ~CMember_0099eae0();                             // 0x0099EAE0
};

struct CTripleCMember_0099eae0_00564b60 {
    CMember_0099eae0 m_first;                        // +0
    CMember_0099eae0 m_second;                       // +4
    CMember_0099eae0 m_third;                        // +8
    ~CTripleCMember_0099eae0_00564b60();
};

CTripleCMember_0099eae0_00564b60::~CTripleCMember_0099eae0_00564b60()
{
}
