// CStringParser::CStringParser() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485C90, length 66 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 twice (+0xc,
// +0x10), malloc @ 0x00BFEA0E (the list head node, 16 bytes, self-linked next/prev).
// Vtable 0x0122D874 is CStringParser (rtti-vtables.tsv). The token list at +0x14 is a
// VC6-era Dinkumware std::list whose constructor zeroes the head pointer, allocates the head
// node, links it to itself and stores it (the `add esp,4` lands before the final store); the
// state long at +0x4 is assigned in the body afterwards.
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};

struct CCharString {
    char* m_pData;
    CCharString();                          // 0x0099E4B0
};

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CTokenList {                         // std::list<CCharString> head only (VC6-era layout)
    struct SNode { SNode* next; SNode* prev; char* text; long length; };
    SNode* m_head;
    CTokenList() : m_head(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        m_head = node;
    }
};

struct CStringParser : CBase {              // vtable 0x0122D874
    long        m_position;                 // +0x04
    long        m_length;                   // +0x08
    CCharString m_text;                     // +0x0c
    CCharString m_separators;               // +0x10
    CTokenList  m_tokens;                   // +0x14
    CStringParser();
};

CStringParser::CStringParser()
    : m_text()
    , m_separators()
    , m_tokens()
{
    m_position = 0;
}