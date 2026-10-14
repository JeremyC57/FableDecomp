// CNamedEntryList_00a3bde0::~CNamedEntryList_00a3bde0() -- destructor: the CWideString name at +0x14 is destroyed, then the retail std::list at +0 is cleared (every node freed) and its sentinel head freed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BDE0, length 35 bytes.
// Callees: CWideString::~CWideString @ 0x0099B510, CRetailList::Clear @ 0x009BE060 (destroys each
// node's CCharString +0x10, CWideString +0xc and counted pointer +8, frees the nodes and relinks the
// head; the manifest's "list_node_destructor" label), free @ 0x00BFEA14.
// Shape: an empty destructor body, so the members go in reverse declaration order: the wide string
// first (out of line), then the list member's inline destructor (Clear out of line, then the head
// null-tested and freed; the head overwrites `this` in esi because nothing else needs it). The
// manifest label (CActiveFile::OnReadFinished) is propagated over the body shape.
#include "fable_string.h"
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

struct CRetailList_009be060 {                                // std::list<entry> (VC6-era Dinkumware)
    struct SNode;
    SNode*        m_pHead;                                   // +0x00 sentinel
    unsigned long m_size;                                    // +0x04
    void Clear();                                            // 0x009BE060
    ~CRetailList_009be060() { Clear(); if (m_pHead) free(m_pHead); }
};

struct CNamedEntryList_00a3bde0 {
    CRetailList_009be060 m_entries;                          // +0x00
    unsigned char        _pad[0x14 - 0x08];                  // +0x08
    CWideString          m_name;                             // +0x14
    ~CNamedEntryList_00a3bde0();
};

CNamedEntryList_00a3bde0::~CNamedEntryList_00a3bde0()
{
}