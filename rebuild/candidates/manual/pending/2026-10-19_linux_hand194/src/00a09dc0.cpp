// CMeshBankManager::CloseMeshBank(CCountedPointer<CMeshDataBank> bank) -- find the open mesh bank whose bank handle equals this bank's and erase its info record from the list; the by-value bank pointer is released on exit
// Leaf: CloseMeshBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A09DC0, 162 bytes, `ret 8`. The catalog label "CGraphicBankManager::CloseGraphicBank"
//   is propagated from the byte-twin 0x009F8290: this body walks the list at +8 whose records hold the handle at +0
//   (node+8), i.e. Ego_r.pdb CMeshBankManager::MeshDataBanks of CMeshDataBankInfo; its caller 0x0042DD94 runs it on the
//   system record's mesh manager (+0x74).
// Callees: CBankFile::GetBankHandle() @ 0x009CBA40 (CCharString by value), Compare @ 0x00411570 (`__fastcall`, inside the
//   inlined CCharString ==), CCharString::~CCharString @ 0x0099EAE0, list<CMeshDataBankInfo>::erase(iterator) @
//   0x00A0A650, operator delete @ 0x00BFE9BC (the argument's count block).
// Layout: Ego_r.pdb CMeshBankManager {vptr, +4 (unnamed base word), MeshDataBanks +8} (retail list = sentinel
//   pointer); CMeshDataBankInfo {BankHandle +0, MeshDataBank +4}.
// Shape: identical to CloseGraphicBank 0x009F8290 (same source shape, different record offsets).
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
struct CMeshDataBank : CBankFile { };

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

struct CMeshDataBankInfo {                         // CMeshBankManager::CMeshDataBankInfo (12 bytes)
    CCharString   BankHandle;                      // +0x00
    CCountedPointer<CMeshDataBank> MeshDataBank;   // +0x04
};

struct CMeshDataBankInfoList {                  // retail list<CMeshDataBankInfo>: the sentinel pointer only
    SListNode<CMeshDataBankInfo>* _M_node;
    CListIterator<CMeshDataBankInfo> begin() const { return CListIterator<CMeshDataBankInfo>(_M_node->_M_next); }
    CListIterator<CMeshDataBankInfo> end() const { return CListIterator<CMeshDataBankInfo>(_M_node); }
    CListIterator<CMeshDataBankInfo> erase(CListIterator<CMeshDataBankInfo> where);   // 0x00A0A650
};

struct CMeshBankManager {
    virtual void v00();
    unsigned char         _base04[4];              // +0x04 (unnamed in the PDB layout)
    CMeshDataBankInfoList MeshDataBanks;           // +0x08
    void CloseMeshBank(CCountedPointer<CMeshDataBank> bank);
};

void CMeshBankManager::CloseMeshBank(CCountedPointer<CMeshDataBank> bank)
{
    for (CListIterator<CMeshDataBankInfo> it = MeshDataBanks.begin(); it != MeshDataBanks.end(); ++it) {
        if (it->BankHandle == bank->GetBankHandle()) {
            MeshDataBanks.erase(it);
            break;
        }
    }
}
