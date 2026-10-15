// CGraphicBankManager::~CGraphicBankManager() -- releases every bank entry's counted bank pointer, then destroys the entry list and the CBase base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F86C0, length 111 bytes.
// Callees: the count block's per-type destroy function (__fastcall, object in ecx), operator delete @ 0x00BFE9BC (the
//          count block), the entry list's clear @ 0x009F89B0 (manifest "remove_if<...>", propagated: it walks the nodes
//          destroying the entry's string +0x34 and counted pointer +0x38), free @ 0x00BFEA14 (the sentinel node),
//          CBase::~CBase @ 0x0099A300 (tail jump).
// Vtable: CGraphicBankManager 0x0129C7E4.
// Shape: the destructor body walks the STLport list (sentinel at +4) and resets each entry's counted pointer through an
// inline Reset (release, then count block and object zeroed from the shared `xor ebx,ebx`); the list member's
// destructor is the out-of-line clear plus a null-tested free of the sentinel; the base destructor is tail-jumped.
// Model: local (entry layout from the clear routine; the leading entry words are opaque).
// Leaf: ??1CGraphicBankManager
void operator delete(void* p);                     // 0x00BFE9BC
extern "C" void free(void* p);                     // 0x00BFEA14
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CGraphicBank;
template <class T> struct CCountedPointer {
    T*           Data;                             // +0
    SCountBlock* Info;                             // +4
    void Reset()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct CCharString {
    void* m_pData;
};
struct SBankEntry {                                // the list element (node +0x08)
    unsigned char                 _pad00[0x2c];
    CCharString                   m_name;          // node +0x34
    CCountedPointer<CGraphicBank> m_pBank;         // node +0x38
};
struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    SBankEntry  _M_data;
};
struct list {
    _List_node* _M_node;
    void clear();                                  // 0x009F89B0
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct CBase {
    virtual ~CBase();                              // 0x0099A300
};
struct CGraphicBankManager : CBase {
    list m_banks;                                  // +0x04
    virtual ~CGraphicBankManager();
};

CGraphicBankManager::~CGraphicBankManager()
{
    for (_List_node* node = m_banks._M_node->_M_next; node != m_banks._M_node; node = node->_M_next)
        node->_M_data.m_pBank.Reset();
}
