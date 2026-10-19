// CGraphicBankManager::CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank) -- find the open graphic bank whose bank handle equals this bank's and erase its info record from the list; the by-value bank pointer is released on exit
// Leaf: CloseGraphicBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F8290, 162 bytes, `ret 8`; called from 0x0042DD6B on the global manager.
// Callees: CBankFile::GetBankHandle() @ 0x009CBA40 (CCharString by value), Compare @ 0x00411570 (`__fastcall`, inside the
//   inlined CCharString ==), CCharString::~CCharString @ 0x0099EAE0, list<CGraphicDataBankInfo>::erase(iterator) @
//   0x009F8920, operator delete @ 0x00BFE9BC (the argument's count block).
// Layout: Ego_r.pdb CGraphicBankManager {vptr, GraphicDataBanks +4} (retail list = sentinel pointer);
//   CGraphicDataBankInfo {Init (CGraphicDataBankInit, 0x2c bytes), BankHandle +0x2c, GraphicDataBank +0x30} (the node
//   value starts at +8, so the handle is read at node+0x34).
// Shape: the TerminateActionByName (0x00712CE0) two-level CCharString == with the record's handle on the left and the
//   GetBankHandle temporary on the right; the bool survives the temporary's destructor in bl; on a hit the iterator is
//   copied into the argument area (user copy ctor) and erase's result lands in the dead temporary slot, then `break`;
//   `end()` is re-read every pass.
struct CCharStringData {
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) == 0;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};

struct CBankFile {
    CCharString GetBankHandle();                   // 0x009CBA40
};
struct CGraphicDataBank : CBankFile { };

template <class T> struct CCountedPointer {        // {Data, Info}; Info = {count, destroy(object in ecx), object}
    struct SInfo {
        long m_references;
        void (__fastcall* m_pfnDestroy)(T*);
        T*   m_pObject;
    };
    T*     Data;
    SInfo* Info;
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {          // list<T>::iterator: one node pointer, user copy ctor
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* node) : _M_node(node) { }
    CListIterator(const CListIterator& other) : _M_node(other._M_node) { }
    T* operator->() const { return &_M_node->_M_data; }
    CListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CListIterator& other) const { return _M_node != other._M_node; }
};

struct CGraphicDataBankInfo {                      // CGraphicBankManager::CGraphicDataBankInfo (0x38 bytes)
    unsigned char Init[0x2c];                      // +0x00 CGraphicDataBankInit
    CCharString   BankHandle;                      // +0x2c
    CCountedPointer<CGraphicDataBank> GraphicDataBank;   // +0x30
};

struct CGraphicDataBankInfoList {                  // retail list<CGraphicDataBankInfo>: the sentinel pointer only
    SListNode<CGraphicDataBankInfo>* _M_node;
    CListIterator<CGraphicDataBankInfo> begin() const { return CListIterator<CGraphicDataBankInfo>(_M_node->_M_next); }
    CListIterator<CGraphicDataBankInfo> end() const { return CListIterator<CGraphicDataBankInfo>(_M_node); }
    CListIterator<CGraphicDataBankInfo> erase(CListIterator<CGraphicDataBankInfo> where);   // 0x009F8920
};

struct CGraphicBankManager {
    virtual void v00();
    CGraphicDataBankInfoList GraphicDataBanks;     // +0x04
    void CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank);
};

void CGraphicBankManager::CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank)
{
    for (CListIterator<CGraphicDataBankInfo> it = GraphicDataBanks.begin(); it != GraphicDataBanks.end(); ++it) {
        if (it->BankHandle == bank->GetBankHandle()) {
            GraphicDataBanks.erase(it);
            break;
        }
    }
}