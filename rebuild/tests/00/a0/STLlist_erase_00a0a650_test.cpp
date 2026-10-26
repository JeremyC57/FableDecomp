// Behaviour test for 0x00A0A650 std::list<CMeshBankManager::CMeshDataBankInfo>::erase: unlink the node,
// destroy the bank info (counted mesh-bank pointer released, then the handle string), free the node, return the next
#include <stdio.h>
#include <stdlib.h>
#if !defined(_MSC_VER)
#define __fastcall
#endif
static int gFrees = 0;
static int gStringDtors = 0;
static int gBankDeletes = 0;
static void* gDeletedBank = 0;
static void test_free(void* p) { ++gFrees; free(p); }
class CCharString {
public:
    const char* PStringData;
    ~CCharString() { ++gStringDtors; }                  // stand-in
};
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
static void __fastcall DeleteBank(void* data) { ++gBankDeletes; gDeletedBank = data; }
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CMeshDataBank { public: int Id; };
class CMeshBankManager {
public:
    class CMeshDataBankInfo {
    public:
        CCharString                    BankHandle;
        CCountedPointer<CMeshDataBank> MeshDataBank;
    };
};
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { test_free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};
}
typedef CMeshBankManager::CMeshDataBankInfo CInfo;
typedef std::list<CInfo> CInfoList;
typedef CInfoList::_Node CNode;
static CMeshDataBank gBanks[3];
static CNode* MakeNode(int i, CCPPointerInfo* info)
{
    CNode* n = (CNode*)malloc(sizeof(CNode));
    n->_M_data.BankHandle.PStringData = "bank";
    n->_M_data.MeshDataBank.Data = &gBanks[i];
    n->_M_data.MeshDataBank.Info = info;
    return n;
}
int main()
{
    CCPPointerInfo* shared = new CCPPointerInfo;          // held twice: erase only drops a reference
    shared->RefCount = 2; shared->DeleteFunc = DeleteBank; shared->Data = &gBanks[1];
    CCPPointerInfo* sole = new CCPPointerInfo;            // held once: erase deletes the bank
    sole->RefCount = 1; sole->DeleteFunc = DeleteBank; sole->Data = &gBanks[0];
    CNode* h = (CNode*)malloc(sizeof(CNode));
    CNode* n0 = MakeNode(0, sole);
    CNode* n1 = MakeNode(1, shared);
    CNode* n2 = MakeNode(2, 0);                           // empty counted pointer
    h->_M_next = n0; n0->_M_next = n1; n1->_M_next = n2; n2->_M_next = h;
    h->_M_prev = n2; n2->_M_prev = n1; n1->_M_prev = n0; n0->_M_prev = h;
    CInfoList l;
    l._M_node._M_data = h;
    bool ok = true;
    CInfoList::iterator r = l.erase(CInfoList::iterator(n1));
    ok = ok && r._M_node == n2 && n0->_M_next == n2 && n2->_M_prev == n0;
    ok = ok && gStringDtors == 1 && gFrees == 1 && gBankDeletes == 0 && shared->RefCount == 1;
    r = l.erase(CInfoList::iterator(n0));
    ok = ok && r._M_node == n2 && h->_M_next == n2 && n2->_M_prev == h;
    ok = ok && gStringDtors == 2 && gFrees == 2 && gBankDeletes == 1 && gDeletedBank == (void*)&gBanks[0];
    r = l.erase(CInfoList::iterator(n2));
    ok = ok && r._M_node == h && h->_M_next == h && h->_M_prev == h;
    ok = ok && gStringDtors == 3 && gFrees == 3 && gBankDeletes == 1;
    delete shared;
    free(h);
    if (ok) { printf("00a0a650_TEST PASS\n"); return 0; }
    printf("00a0a650_TEST FAIL\n");
    return 1;
}