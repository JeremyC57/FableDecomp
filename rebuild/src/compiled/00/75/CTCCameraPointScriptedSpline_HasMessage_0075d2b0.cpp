// CTCCameraPointScriptedSpline::HasMessage(const CCharString& message) -- look the message up in MyMessages; when it is there, erase it and return true
//
// Retail 0x0075D2B0, 110 bytes (`ret 4`). Callees: Compare(const char*, const char*) @ 0x00411570
// (`__fastcall`, the inlined CCharString equality), CCharString::~CCharString @ 0x0099EAE0, free
// @ 0x00BFEA14 (the list node).
// Layout: MyMessages (STLport list<CCharString>, head node pointer) at +0x98 (PDB +0x9c less the
// 4 bytes of the narrower retail KeyCameras vector); node {next, prev, CCharString +8}.
// Shape: the inlined CCharString operator== (same block -> equal; both non-null with equal length
// -> Compare == 0) written as `Compare(...) ? false : true` gives the 8-bit `neg ; sbb al,al ;
// inc al`. The message's string block stays in edi across the Compare call: Compare's real body
// (landed 0x00411570) is defined above under `#pragma auto_inline(off)` so VC7.1 knows it writes no
// memory. The hit erases the node with STLport's inline erase (unlink, ~CCharString, free).
extern "C" void __cdecl free(void* p);             // 0x00BFEA14

struct CCharStringData {                           // the shared string block
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

struct CMessageNode {                              // STLport list<CCharString> node
    CMessageNode* next;
    CMessageNode* prev;
    CCharString   value;                           // +0x08
};

inline void Erase(CMessageNode* pos)               // STLport list::erase(iterator)
{
    CMessageNode* next = pos->next;
    CMessageNode* prev = pos->prev;
    prev->next = next;
    next->prev = prev;
    pos->value.~CCharString();
    free(pos);
}

struct CTCCameraPointScriptedSpline {
    unsigned char _pad00[0x98];
    CMessageNode* MyMessages;                      // +0x98 list<CCharString> (head node; PDB +0x9c)
    bool HasMessage(const CCharString& message);
};

bool CTCCameraPointScriptedSpline::HasMessage(const CCharString& message)
{
    for (CMessageNode* node = MyMessages->next; node != MyMessages; node = node->next)
    {
        if (node->value == message)
        {
            Erase(node);
            return true;
        }
    }
    return false;
}