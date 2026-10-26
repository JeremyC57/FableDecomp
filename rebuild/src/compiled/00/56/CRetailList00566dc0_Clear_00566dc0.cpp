// CRetailList_00566dc0::Clear() -- clear a list whose 16-byte values hold a CWideString, a dword, a CWideString and a CCharString: the three strings destroyed in reverse order (0x0099EAE0, 0x0099B510 twice), every node freed, the sentinel relinked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00566DC0, length 80 bytes.
// Leaf: Clear
// The chunk label (CNavigatorQueueManager::FrameUpdate) is propagated; named by effect. Callees: free @ 0x00BFEA14, CCharString::~CCharString @ 0x0099EAE0, CWideString::~CWideString @ 0x0099B510. Shape: the VC7.1 `list::clear`: the successor read before the node is destroyed, the sentinel relinked to itself at the end.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
class CWideString { public:
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct SValue_00566dc0 {                           // 16 bytes: two wide strings and a narrow one
    CWideString m_0;
    long        m_4;
    CWideString m_8;
    CCharString m_c;
};
struct SNode_00566dc0 {
    SNode_00566dc0*   m_pNext;                        // +0x00
    SNode_00566dc0*   m_pPrev;                        // +0x04
    SValue_00566dc0   m_value;                        // +0x08
};

struct CRetailList_00566dc0 {
    SNode_00566dc0* m_pHead;                          // +0x00: the sentinel
    void Clear();
};

void CRetailList_00566dc0::Clear()
{
    SNode_00566dc0* next = m_pHead->m_pNext;
    while (next != m_pHead) {
        SNode_00566dc0* node = next;
        next = next->m_pNext;
        node->m_value.~SValue_00566dc0();
        free(node);
    }
    m_pHead->m_pNext = m_pHead;
    m_pHead->m_pPrev = m_pHead;
}