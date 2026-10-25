// _STL::_List_base::clear() -- free every graphic data-bank entry of the bank manager's list (counted bank released, bank handle string destroyed) and relink the sentinel to itself
// Instantiation: _STL::_List_base<CGraphicBankManager::CGraphicDataBankInfo,_STL::allocator<CGraphicBankManager::CGraphicDataBankInfo> >::clear
// Leaf: ?clear@?$_List_base@VCGraphicDataBankInfo@CGraphicBankManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F89B0, 97 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: called by ~CGraphicBankManager 0x009F86C0 (call 0x009F8710) and the STLport `~list` body 0x009F8A90 (catalogued
//   as the propagated CActiveFile::OnReadFinished). Element = Ego_r.pdb CGraphicBankManager::CGraphicDataBankInfo (Init
//   +0x00, a 0x2c-byte POD CGraphicDataBankInit; BankHandle +0x2c; GraphicDataBank +0x30). The catalog label
//   `remove_if<NEntityEvents::CConvFilterToCP<..> >` is propagated.
// Callees: the count block's DeleteFunc, operator delete @ 0x00BFE9BC (the count block), CCharString::~CCharString @
//   0x0099EAE0, free @ 0x00BFEA14 (the node).
// Shape: STLport 4.x `_List_base::clear()` (as 0x00A17390): the implicit ~CGraphicDataBankInfo releases GraphicDataBank
//   first, then destroys BankHandle.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

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

class CCharString {
public:
    void* PStringData;                                      // +0x0
    ~CCharString();                                         // 0x0099EAE0
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

template class _STL::_List_base<CGraphicBankManager::CGraphicDataBankInfo, _STL::allocator<CGraphicBankManager::CGraphicDataBankInfo> >;