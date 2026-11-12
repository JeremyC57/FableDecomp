// CCodeSectionManager::IsSectionRegisteredLongName(CCharString longName) -- true when the long name is a key of LongNameToHandle (+0x10)
// Leaf: ?IsSectionRegisteredLongName@CCodeSectionManager@@QAE_NVCCharString@@@Z
// NF28 (2026-11-12, queue land1011eh): was landed as CNameRegistry_009f0570::Contains. Identity: the receiver is the
//   CTBaseSingleton<CCodeSectionManager> instance 0x013CAA68 (callers 0x004B3C7D / 0x004B3DDA); Ego_r.pdb 0x00B425B0
//   ?IsSectionRegisteredLongName@CCodeSectionManager@@QAE_NVCCharString@@@Z (by-value CCharString, bool) beside
//   IsSectionRegisteredShortName; the tree read is at +0x10, which is Ego_r CCodeSectionManager::LongNameToHandle (Ego_r +0x14 with
//   16-byte maps; retail 12-byte STLport maps: ShortNameToHandle +0x04, LongNameToHandle +0x10), the map the landed
//   GetHandleFromLongName 0x009F0500 also reads; the caller's true arm calls GetHandleFromLongName with the same name.
// Retry 10b (B-12 sweep): the found node is a const REFERENCE to the call result (`SNameNode* const& node = m_entries.FindNode(name);`): retail keeps it in edi across the by-value parameter's destructor and compares after it (`cmp edi,esi ; setne al`); a value local is compared before the destructor (`setne bl`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F0570, length 39 bytes.
// Callees: the tree's node lookup @ 0x009D2DD0 (manifest "StringMap_FindNode": `__fastcall`, const CCharString&,
// returns the node or the head), CCharString::~CCharString @ 0x0099EAE0 (the by-value parameter, destroyed by
// the callee). Callers 0x004B3C7D / 0x004B3DDA build the CCharString copy in the argument slot and call it on
// the global object [0x013CAA68].
// Label: the manifest's "CTCCameraPointScriptedSpline::AddAnimIndexToAvailableAnimList" is propagated and rejected.
// Shape: the head (end) is read into esi BEFORE the call, so it is a named local.
#include "fable_string.h"

struct SNameNode;

struct CNameTree {                                  // VC6 Dinkumware map<CCharString, ...>: {head, size}
    SNameNode*    m_head;
    unsigned long m_size;
    SNameNode* FindNode(const CCharString& key);    // 0x009D2DD0
};

class CCodeSectionManager {                         // no shared header (the landed 0x009F0500 models it locally too)
public:
    unsigned char _pad[0x10];                       // vptr +0x00, ShortNameToHandle +0x04
    CNameTree     LongNameToHandle;                 // +0x10
    bool IsSectionRegisteredLongName(CCharString longName);
};

bool CCodeSectionManager::IsSectionRegisteredLongName(CCharString longName)
{
    SNameNode* end = LongNameToHandle.m_head;
    SNameNode* const& node = LongNameToHandle.FindNode(longName);
    return node != end;
}