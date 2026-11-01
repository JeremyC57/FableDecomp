// CTripleCMember_0099eae0_00566940::~CTripleCMember_0099eae0_00566940() -- destroys three CCharString members at +0x8, +0x4, +0x0 (last one by tail call)
// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was CMember_0099eae0); name-only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00566940, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call CCharString::~CCharString ; lea ecx,[esi+0x4] ; call CCharString::~CCharString ; mov ecx,esi ; pop esi ; jmp CCharString::~CCharString
// Implicit/empty destructor of a non-polymorphic aggregate of three 4-byte CCharString members
// (destructor @ 0x0099EAE0): members are destroyed in reverse declaration order and the first
// one's destructor is reached by tail call. The manifest label (`_Dest_val<...pair<CCharString,
// CSymbolInfo>>`) says this is the pair value of a map; the owning type is named descriptively.
// One of 22 bodies of this shape.
class CCharString { public:
    void* m_data;
    ~CCharString();                             // 0x0099EAE0
};

struct CTripleCMember_0099eae0_00566940 {
    CCharString m_first;                        // +0
    CCharString m_second;                       // +4
    CCharString m_third;                        // +8
    ~CTripleCMember_0099eae0_00566940();
};

CTripleCMember_0099eae0_00566940::~CTripleCMember_0099eae0_00566940()
{
}