// std::_List_base::clear() -- (list<<CActiveDialogueInfo>>) STLport 4.6 list clear: walk the nodes from the sentinel's next, destroy each element and free its node, then relink the sentinel to itself
// Leaf: ?clear@?$_List_base@VCActiveDialogueInfo@@V?$allocator@VCActiveDialogueInfo@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E4B90, 56 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): list<<CActiveDialogueInfo>>; evidence: it is the clear of the static list at 0x013BABA0 -- the landed at-exit destructor 0x01229510 / initialiser 0x0121E3F0 and the landed NSpeechGainManager::Update 0x006E37D0, which walks it as list<CActiveDialogueInfo> (0x20-byte nodes, ThingSpeaking +0); CWorld_InitGuiTreePane 0x006E3EC0 clears it too
// Label: the manifest's "SortTreeRecursively" is propagated (family H066: six clear() bodies that differ only in the element
//   destructor; retail has no identical-code folding, so each address is one instantiation); this is STLport 4.6
//   `_List_base::clear` (`_list.c`), the out-of-line half of ~list and list::clear.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 on CActiveDialogueInfo::ThingSpeaking (node +8), free 0x00BFEA14 (allocator<_Node>::deallocate; no null test: the node was dereferenced).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CThing;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* PData;                                        // +4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
enum EDialogueLayer { };
class CActiveDialogueInfo {                          // 0x18 bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> ThingSpeaking;          // +0x00
    unsigned long SoundID;                              // +0x08
    EDialogueLayer Layer;                               // +0x0C
    bool IsLayerHead;                                   // +0x10
    float OutputGain;                                   // +0x14
};

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the sentinel node
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
}   // namespace std

template class std::_List_base<CActiveDialogueInfo, std::allocator<CActiveDialogueInfo > >;