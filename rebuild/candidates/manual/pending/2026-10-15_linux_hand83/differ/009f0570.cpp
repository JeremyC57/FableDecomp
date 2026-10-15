// CNameRegistry_009f0570::Contains(CCharString name) -- true when the name is a key of the registry's string-keyed tree at +0x10
// Leaf: Contains
// Grade: DIFFER(39v39) -- the lane computes `cmp eax,esi ; setne bl` before the by-value parameter's destructor and returns `mov al,bl` (saving ebx); retail keeps the node in edi (`mov edi,eax`), runs the destructor, then `cmp edi,esi ; setne al`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0570, length 39 bytes.
// Callees: the tree's node lookup @ 0x009D2DD0 (manifest "StringMap_FindNode": `__thiscall`, const CCharString&,
// returns the node or the head), CCharString::~CCharString @ 0x0099EAE0 (the by-value parameter, destroyed by
// the callee). Callers 0x004B3C7D / 0x004B3DDA build the CCharString copy in the argument slot and call it on
// the global object [0x013CAA68].
// Label: the manifest's "CTCCameraPointScriptedSpline::AddAnimIndexToAvailableAnimList" is propagated and
// rejected; the registry class is named by address.
// Shape: the head (end) is read into esi BEFORE the call, so it is a named local.
// Tried: `return Find(name) != end;`, a named node local, iterator objects with an inline operator!=,
// `? true : false`. Next: lane gap (register allocation of the compare across the parameter destructor).
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
    SNameNode* node = m_entries.FindNode(name);
    return node != end;
}
