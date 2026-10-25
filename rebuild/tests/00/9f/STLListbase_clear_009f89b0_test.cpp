#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of _List_base<CGraphicBankManager::CGraphicDataBankInfo>::clear() @ 0x009F89B0: destroy and free every node
// (counted bank, then the bank handle string), then relink the sentinel to itself.

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    bool operator==(const CCountedPointer& other) const { return Data == other.Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

static int gStringDtors = 0;
class CCharString {
public:
    void* PStringData;                                      // +0x0
    ~CCharString() { ++gStringDtors; }                                         // 0x0099EAE0
};

class CGraphicDataBank;
class CPixelFormat {
public:
    unsigned long Format;
};
class CGraphicDataBankInit {                                // Ego_r.pdb, 0x2c bytes (POD)
public:
    CPixelFormat  NonAlphaFormat;                           // +0x00
    CPixelFormat  AlphaFormat;                              // +0x04
    CPixelFormat  InterpolatedAlphaFormat;                  // +0x08
    CPixelFormat  BooleanAlphaFormat;                       // +0x0c
    CPixelFormat  UncompressedNonAlphaFormat;               // +0x10
    CPixelFormat  UncompressedAlphaFormat;                  // +0x14
    CPixelFormat  SignedFormat;                             // +0x18
    bool          GenerateMipmaps;                          // +0x1c
    unsigned long MaxGraphicWidth;                          // +0x20
    unsigned long MaxGraphicHeight;                         // +0x24
    bool          AllowDither;                              // +0x28
    bool          AllowCompressedTextures;                  // +0x29
    bool          AllowDXT1ForBooleanAlpha;                 // +0x2a
};
class CGraphicBankManager {
public:
    class CGraphicDataBankInfo {                            // Ego_r.pdb, 0x38 bytes
    public:
        CGraphicDataBankInit              Init;             // +0x00
        CCharString                       BankHandle;       // +0x2c
        CCountedPointer<CGraphicDataBank> GraphicDataBank;  // +0x30
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // the sentinel
    void clear();
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
}


template <class L> void InitList(L& l)
{
    typedef typename L::_Node Node;
    Node* s = (Node*)malloc(sizeof(Node));
    s->_M_next = s;
    s->_M_prev = s;
    l._M_node._M_data = s;
}
template <class L> typename L::_Node* NewBack(L& l)
{
    typedef typename L::_Node Node;
    Node* n = (Node*)malloc(sizeof(Node));
    _STL::_List_node_base* s = l._M_node._M_data;
    n->_M_next = s;
    n->_M_prev = s->_M_prev;
    s->_M_prev->_M_next = n;
    s->_M_prev = n;
    return n;
}
template <class L> int CountList(L& l)
{
    int c = 0;
    for (_STL::_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next)
        ++c;
    return c;
}
template <class L> bool LinksOk(L& l)
{
    _STL::_List_node_base* s = l._M_node._M_data;
    for (_STL::_List_node_base* p = s->_M_next; p != s; p = p->_M_next)
        if (p->_M_next->_M_prev != p || p->_M_prev->_M_next != p)
            return false;
    return s->_M_next->_M_prev == s && s->_M_prev->_M_next == s;
}

static int gDeleted = 0;
static void __fastcall CountDelete(void*) { ++gDeleted; }
static CCPPointerInfo* NewInfo(void* data, unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs;
    info->DeleteFunc = CountDelete;
    info->Data = data;
    return info;
}
int main()
{
    bool ok = true;
    typedef _STL::_List_base<CGraphicBankManager::CGraphicDataBankInfo, _STL::allocator<CGraphicBankManager::CGraphicDataBankInfo> > L;
    L l;
    InitList(l);
    static int dummies[3];
    CCPPointerInfo* infos[3];
    for (int i = 0; i < 3; ++i) {
        L::_Node* n = NewBack(l);
        infos[i] = NewInfo(&dummies[i], i == 1 ? 2 : 1);
        memset(&n->_M_data.Init, 0, sizeof(n->_M_data.Init));
        n->_M_data.BankHandle.PStringData = 0;
        n->_M_data.GraphicDataBank.Data = (CGraphicDataBank*)(void*)&dummies[i];
        n->_M_data.GraphicDataBank.Info = infos[i];
    }
    l.clear();
    _STL::_List_node_base* s = l._M_node._M_data;
    if (s->_M_next != s || s->_M_prev != s || gStringDtors != 3 || gDeleted != 2 || infos[1]->RefCount != 1)
        ok = false;
    l.clear();
    if (s->_M_next != s || gStringDtors != 3 || gDeleted != 2)
        ok = false;
    if (!ok) { printf("009f89b0_TEST FAIL\n"); return 1; }
    printf("009f89b0_TEST PASS\n");
    return 0;
}