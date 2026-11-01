// NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript() -- implicit cut-scene script destructor: v7..v1 scripted-thing handles, the OakValeFlag map, the Fires / FirePoint CScriptThing vectors, then ~CScriptBase
// Leaf: ??1CCS_OakValeRevisitedScript@NScript@@UAE@XZ
// Retail 0x00EE8C30, 585 bytes. Identity: sole caller ??_G 0x00EE8C10 = slot 0 of the RTTI-less vtable 0x012F53C0,
//   stored by the OakValeRevisited ctor 0x00EE8100; tail-jumps to ~CScriptBase 0x00CBD510 (the local CLevelScriptBase<T>
//   declares no dtor, as the landed ChickenKicking 0x00E68EB0 model). Callees: ~CBaseObject 0x0099A430, operator delete
//   0x00BFE9BC, STLport _Rb_tree<CCharString,bool>::_M_erase 0x007ACC50, free 0x00BFEA14.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    bool operator<(const CCharString&) const;
    CCharStringData* PStringData;
};
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                       // 0x007ACC50 (map<CCharString,bool>)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
};
}
namespace NScript {
class CQ_SunnyvaleMasterData;
class CCS_OakValeRevisitedScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCS_OakValeRevisitedScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    std::vector<CScriptThing>                  FirePoint;                // +0x48
    std::vector<CScriptThing>                  Fires;                    // +0x54
    std::map<CCharString, bool>                OakValeFlag;              // +0x60
    CScriptGameResourceObjectScriptedThingBase v1;                       // +0x6c
    CScriptGameResourceObjectScriptedThingBase v2;                       // +0x7c
    CScriptGameResourceObjectScriptedThingBase v3;                       // +0x8c
    CScriptGameResourceObjectScriptedThingBase v4;                       // +0x9c
    CScriptGameResourceObjectScriptedThingBase v5;                       // +0xac
    CScriptGameResourceObjectScriptedThingBase v6;                       // +0xbc
    CScriptGameResourceObjectScriptedThingBase v7;                       // +0xcc
};
CCS_OakValeRevisitedScript::CCS_OakValeRevisitedScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00EE8100
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}