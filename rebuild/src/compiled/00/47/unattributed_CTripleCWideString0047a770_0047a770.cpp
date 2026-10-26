// CTripleCWideString_0047a770::~CTripleCWideString_0047a770() -- destroys three CWideString members at +0x8, +0x4, +0x0 (last one by tail call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047A770, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; lea ecx,[esi+0x8] ; call CWideString::~CWideString ; lea ecx,[esi+0x4] ; call CWideString::~CWideString ; mov ecx,esi ; pop esi ; jmp CWideString::~CWideString
// Implicit/empty destructor of a non-polymorphic aggregate of three 4-byte CWideString members
// (destructor @ 0x0099B510): members are destroyed in reverse declaration order and the first
// one's destructor is reached by tail call. The manifest label (`_Dest_val<...pair<CCharString,
// CSymbolInfo>>`) says this is the pair value of a map; the owning type is named descriptively.
// One of 22 bodies of this shape.
class CWideString { public:
    void* m_data;
    ~CWideString();                             // 0x0099B510
};

struct CTripleCWideString_0047a770 {
    CWideString m_first;                        // +0
    CWideString m_second;                       // +4
    CWideString m_third;                        // +8
    ~CTripleCWideString_0047a770();
};

CTripleCWideString_0047a770::~CTripleCWideString_0047a770()
{
}