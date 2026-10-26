// Behaviour test for CGraphicBankManager::CloseGraphicBank (0x009F8290): the record whose handle text equals the bank's is erased (only
// that one, first match), a non-matching bank erases nothing, and the by-value bank pointer is released.
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
static int g_compares = 0, g_dtors = 0, g_erases = 0, g_destroyed = 0;
class CCharStringData { public:
    const char*   m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b) { ++g_compares; return strcmp(a, b); }

class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString() { ++g_dtors; }
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

class CBankFile { public:
    CCharStringData* m_handle;
    CCharString GetBankHandle() { CCharString s; s.PStringData = m_handle; return s; }
};
class CGraphicDataBank : public CBankFile { public: };

template <class T> class CCountedPointer { public:        // {Data, Info}; Info = {count, destroy(object in ecx), object}
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
    CListIterator<CGraphicDataBankInfo> erase(CListIterator<CGraphicDataBankInfo> where)
    { ++g_erases; where._M_node->_M_prev->_M_next = where._M_node->_M_next; where._M_node->_M_next->_M_prev = where._M_node->_M_prev; return CListIterator<CGraphicDataBankInfo>(where._M_node->_M_next); }   // 0x009F8920
};

class CGraphicBankManager { public:
    virtual void v00() {}
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

static void __fastcall DestroyBank(CGraphicDataBank* b) { ++g_destroyed; }
int main()
{
    static CCharStringData names[3] = { { "alpha", 5 }, { "beta", 4 }, { "gamma", 5 } };
    static CCharStringData query = { "beta", 4 }, miss = { "delta", 5 };
    static SListNode<CGraphicDataBankInfo> head, n[3];
    head._M_next = &n[0]; head._M_prev = &n[2];
    for (int i = 0; i < 3; ++i) {
        n[i]._M_next = i < 2 ? &n[i + 1] : &head; n[i]._M_prev = i ? &n[i - 1] : &head;
        n[i]._M_data.BankHandle.PStringData = &names[i];
    }
    CGraphicBankManager m; m.GraphicDataBanks._M_node = &head;
    CGraphicDataBank bank; bank.m_handle = &query;
    int bad = 0;
    {
        CCountedPointer<CGraphicDataBank>::SInfo* info = new CCountedPointer<CGraphicDataBank>::SInfo;
        info->m_references = 1; info->m_pfnDestroy = DestroyBank; info->m_pObject = &bank;
        CCountedPointer<CGraphicDataBank> p; p.Data = &bank; p.Info = info;
        CCountedPointer<CGraphicDataBank> arg; arg.Data = p.Data; arg.Info = info; ++info->m_references;
        m.CloseGraphicBank(arg);
        arg.Info = 0; arg.Data = 0;
        if (g_erases != 1 || n[0]._M_next != &n[2] || n[2]._M_prev != &n[0]) bad = 1;
        if (info->m_references != 1 || g_destroyed) bad = 2;
    }
    if (g_destroyed != 1) bad = 3;
    bank.m_handle = &miss;
    {
        CCountedPointer<CGraphicDataBank> arg; arg.Data = &bank; arg.Info = 0;
        m.CloseGraphicBank(arg);
        arg.Data = 0;
    }
    if (g_erases != 1 || g_dtors < 3) bad = 4;
    if (bad) { std::printf("009f8290_TEST FAIL\n"); return 1; }
    std::printf("009f8290_TEST PASS\n");
    return 0;
}