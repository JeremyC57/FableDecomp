// std::_Rb_tree<CCharString, std::pair<const CCharString, NBankFileManager::CContainedBank>, std::_Select1st<..>, std::less<CCharString>, std::allocator<..> >::_M_create_node(const value_type& __x) -- allocate one 40-byte node of CRetailBank::ContainedBanks and copy-construct the value at +0x10 (the links are left for the caller)
// Leaf: ?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@2@ABU?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA9E0, length 76 bytes, thiscall `ret 4` (`this` unused).
// Identity (sweep3, family pass 1 "invented SRecord20"): the only callers are the ContainedBanks tree's _M_insert
//   0x009AAF70 (0x009AAFAF / 0x009AAFC8), so this is that tree's STLport `_M_create_node`. The value copy is
//   CCharString's copy constructor + five dwords: Ego_r.pdb NBankFileManager::CContainedBank {DataVersion,
//   EntryCount, InfoOffset, InfoSize, Alignment} (0x14 bytes). The old CRetailTree_009aa9e0::BuyNode (and the chunk
//   label "CCreatureAction_NavToPos::Clone") were stand-ins.
// Callees: malloc @ 0x00BFEA0E (allocator<_Rb_tree_node<..> >::allocate(1)), CCharString::CCharString(const
//   CCharString&) @ 0x0099EC30.
// Shape: STLport 4.6 `_tree.h` `_M_create_node` (protected, sweep2 decision 1d): `_M_header.allocate(1)` then
//   `_Construct(&__tmp->_M_value_field, __x)` -- placement new with VC7.1's null test, the implicit pair copy (key
//   copy constructor, then the 20-byte CContainedBank copied word by word through both pointers re-based to +4).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                                      // +0x00 (CCharStringData*)
    CCharString(const CCharString& other);                  // 0x0099EC30
    bool operator<(const CCharString& other) const;
};

namespace NBankFileManager {
class CContainedBank {                                      // 0x14 bytes (Ego_r.pdb NBankFileManager::CContainedBank)
public:
    unsigned long DataVersion;                              // +0x00
    unsigned long EntryCount;                               // +0x04
    unsigned long InfoOffset;                               // +0x08
    unsigned long InfoSize;                                 // +0x0C
    unsigned long Alignment;                                // +0x10
};
}

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
protected:
    _STLP_alloc_proxy<_Link_type, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
};
}

typedef std::pair<const CCharString, NBankFileManager::CContainedBank> _TreeValue;
typedef std::_Rb_tree<CCharString, _TreeValue, std::_Select1st<_TreeValue>, std::less<CCharString>, std::allocator<_TreeValue> > _Tree;
template _Tree::_Link_type _Tree::_M_create_node(const _TreeValue&);
