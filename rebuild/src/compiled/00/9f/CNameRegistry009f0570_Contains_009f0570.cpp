// CNameRegistry_009f0570::Contains(CCharString name) -- true when the name is a key of the registry's string-keyed tree at +0x10
// Leaf: Contains
// Retry 10b (B-12 sweep): the found node is a const REFERENCE to the call result (`SNameNode* const& node = m_entries.FindNode(name);`): retail keeps it in edi across the by-value parameter's destructor and compares after it (`cmp edi,esi ; setne al`); a value local is compared before the destructor (`setne bl`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0570, length 39 bytes.
// Callees: the tree's node lookup @ 0x009D2DD0 (manifest "StringMap_FindNode": `__fastcall`, const CCharString&,
// returns the node or the head), CCharString::~CCharString @ 0x0099EAE0 (the by-value parameter, destroyed by
// the callee). Callers 0x004B3C7D / 0x004B3DDA build the CCharString copy in the argument slot and call it on
// the global object [0x013CAA68].
// Label: the manifest's "CTCCameraPointScriptedSpline::AddAnimIndexToAvailableAnimList" is propagated and
// rejected; the registry class is named by address.
// Shape: the head (end) is read into esi BEFORE the call, so it is a named local.
#include "fable_string.h"

struct SNameNode;

struct CNameTree {                                  // VC6 Dinkumware map<CCharString, ...>: {head, size}
    SNameNode*    m_head;
    unsigned long m_size;
    SNameNode* FindNode(const CCharString& key);    // 0x009D2DD0
};

struct CNameRegistry_009f0570 {
    unsigned char _pad[0x10];
    CNameTree     m_entries;                        // +0x10
    bool Contains(CCharString name);
};

bool CNameRegistry_009f0570::Contains(CCharString name)
{
    SNameNode* end = m_entries.m_head;
    SNameNode* const& node = m_entries.FindNode(name);
    return node != end;
}