// std::_List_base::clear() -- free every mesh data-bank entry of the bank manager's list (counted bank released, bank handle string destroyed) and relink the sentinel to itself
// Instantiation: std::_List_base<CMeshBankManager::CMeshDataBankInfo,std::allocator<CMeshBankManager::CMeshDataBankInfo> >::clear
// Leaf: ?clear@?$_List_base@VCMeshDataBankInfo@CMeshBankManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A720, 105 bytes, thiscall, plain `ret`.
// Identity: called by ~CMeshBankManager 0x00A0A150 (call 0x00A0A1A1, MeshDataBanks +0x08) and the STLport `~list` bodies
//   0x00A0A840 / 0x00A0A880 (catalogued as the propagated CActiveFile::OnReadFinished). Element = Ego_r.pdb
//   CMeshBankManager::CMeshDataBankInfo (BankHandle +0x0, MeshDataBank +0x4). The catalog label
//   `remove_if<NEntityEvents::CConvFilterToCP<..> >` is propagated.
// Callees: the count block's DeleteFunc, operator delete @ 0x00BFE9BC (the count block), CCharString::~CCharString @
//   0x0099EAE0, free @ 0x00BFEA14 (the node).
// Shape: STLport 4.x `_List_base::clear()` (as 0x00A17390). With `this`, the node, the cursor and `&node->_M_data` all in
//   registers no zero register is left: the counted pointer's null tests are `test`/`cmp [eax],0` and its two zero stores
//   go through `xor eax,eax`; the string destructor takes `&_M_data` straight from esi.
// Note: engine/CMeshBankManager.h carries the Ego_r 12-byte Dinkumware list as opaque bytes; only the element is modelled.
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

class CMeshDataBank;
class CMeshBankManager {
public:
    class CMeshDataBankInfo {                               // Ego_r.pdb, 0xc bytes
    public:
        CCharString                    BankHandle;          // +0x0
        CCountedPointer<CMeshDataBank> MeshDataBank;        // +0x4
    };
};

namespace std {
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

template class std::_List_base<CMeshBankManager::CMeshDataBankInfo, std::allocator<CMeshBankManager::CMeshDataBankInfo> >;