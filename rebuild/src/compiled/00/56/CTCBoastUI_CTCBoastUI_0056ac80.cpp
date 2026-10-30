// CTCBoastUI::~CTCBoastUI() -- boast UI thing-component dtor: leave the global TC update list, delete every UI component it created and empty the vector, then the member dtors (Components, BoastGroups) and ~CTCBase
// Leaf: ??1CTCBoastUI@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056AC80, 185 bytes, tail jump to the base destructor.
// Identity: the vtable store 0x0124DCFC carries RTTI `.?AVCTCBoastUI@@` (rtti_bases: CTCBoastUI : CTCBase : CBaseClassNonCopyable);
//   Ego_r.pdb has `??1CTCBoastUI@@UAE@XZ`. The catalog label is right.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (Ego_r.pdb `?RemoveFromGlobalTCs@CTCBase@@QAEXXZ`, bound so), memmove 0x00BFEAE6
//   (MSVCR71 thunk: STLport vector::clear through __copy_trivial), free 0x00BFEA14, std::_Rb_tree<NUISystem::CMenuEntry*,
//   pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>, ...>::_M_erase 0x0056D440 (private; its body destroys the CBoast
//   ScriptName / Text / Title strings at node +0x20/+0x24/+0x28 = CBoast +0xC/+0x10/+0x14, as the Ego_r CBoast layout puts them),
//   CTCBase::~CTCBase 0x00686830 (tail jump); each component's virtual scalar deleting destructor (slot 0, flag 1).
// Layout: Ego_r.pdb CTCBoastUI (0x34): BoastGroups +0x0C, Active +0x1C, Components +0x20, Quitting +0x30 ...; retail STLport sizes
//   (map 0xC, vector 0xC): BoastGroups +0x0C, Active +0x18, Components +0x1C, Quitting +0x28, AllowedToOpen +0x29,
//   ActivationRequested +0x2A, ActivateLater +0x2B. engine/CTCBoastUI.h has these offsets but is a flattened global struct (vptr
//   as data, no CTCBase base), so the polymorphic class is modelled here (header debt).
// Shape: user body `RemoveFromGlobalTCs(); for (unsigned long i = 0; i < Components.size(); ++i) delete Components[i];
//   Components.clear();` (size re-read each pass; `jbe` = unsigned), then the implicit member dtors: the vector frees its block,
//   the map's ~_Rb_tree clears (out-of-line _M_erase when node_count != 0) and ~_Rb_tree_base frees the header.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x0
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                         // +0x0 (0xC bytes)
    void clear() { _M_t.clear(); }
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
};
namespace NUISystem {
class CComponent { public: virtual ~CComponent(); };
class CMenuEntry;
}
class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class CQuestManager { public:
    class CBoast { public:                                  // Ego_r.pdb layout (0x20)
        long        ID;                                     // +0x00
        long        Cost;                                   // +0x04
        long        Reward;                                 // +0x08
        CCharString ScriptName;                             // +0x0C
        CWideString Text;                                   // +0x10
        CWideString Title;                                  // +0x14
        bool        Evil;                                   // +0x18
        bool        Failed;                                 // +0x19
        bool        Completed;                              // +0x1A
        long        GroupIndex;                             // +0x1C
    };
};

class CTCBoastUI : public CTCBase { public:
    std::map<NUISystem::CMenuEntry*, CQuestManager::CBoast> BoastGroups;   // +0x0C
    bool                                  Active;                         // +0x18
    std::vector<NUISystem::CComponent*>   Components;                     // +0x1C
    bool                                  Quitting;                       // +0x28
    bool                                  AllowedToOpen;                  // +0x29
    bool                                  ActivationRequested;            // +0x2A
    bool                                  ActivateLater;                  // +0x2B
    virtual ~CTCBoastUI();
};

CTCBoastUI::~CTCBoastUI()
{
    RemoveFromGlobalTCs();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
}