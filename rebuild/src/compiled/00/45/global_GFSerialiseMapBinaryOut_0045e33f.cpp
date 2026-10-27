// GFSerialiseMapBinaryOut(CDataOutputStream& out, const std::map<EOpinionDeedType, bool>& m) -- write the count, then every (key, flag)
// Leaf: ??$GFSerialiseMapBinaryOut@_NW4EOpinionDeedType@@U?$less@W4EOpinionDeedType@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@W4EOpinionDeedType@@_NU?$less@W4EOpinionDeedType@@@std@@V?$allocator@U?$pair@$$CBW4EOpinionDeedType@@_N@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E33F, 64 bytes, `__fastcall` (stream in ecx, map in edx), plain `ret` (size-optimised module).
// Identity (proven): reached through 0x0045E30A from COpinionDeedMaskDef::Transfer 0x0045E2E9: Ego_r.pdb COpinionDeedMaskDef::DeedEnabled is map<EOpinionDeedType, bool>; Ego_r.pdb ??$GFSerialiseMapBinaryOut@_NW4EOpinionDeedType@@....
// Callees: CDataOutputStream::WriteSLONG 0x00993EB0, CDataOutputStream::WriteEBOOL 0x00993E30, std::_Rb_global<bool>::_M_increment
//   0x004292C0 (cdecl).
// Shape (family M044, representative 0x0045DF1E): as M024 (traits objects, count, per-node key/value) -- the enum and bool
//   traits have no state to construct, so no frame; the bool is widened with `xor eax,eax ; mov al,[esi+14h]` (`optimize("s")`).
#pragma optimize("s", on)
class CDataOutputStream {
public:
    void WriteSLONG(long value);                          // 0x00993EB0
    void WriteEBOOL(bool value);                          // 0x00993E30
};
enum EOpinionDeedType { EOPINIONDEEDTYPE_NONE = 0 };
template <class T> class CPersistTraits;
template <> class CPersistTraits<EOpinionDeedType> {
public:
    void TransferBinaryOut(CDataOutputStream& out, const EOpinionDeedType& value) const { out.WriteSLONG((long)value); }
};
template <> class CPersistTraits<bool> {
public:
    void TransferBinaryOut(CDataOutputStream& out, const bool& value) const { out.WriteEBOOL(value); }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &operator*(); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_node_base* _M_node;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef _Rb_tree_iterator<pair<const _Key, _Tp> > const_iterator;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
    _Rb_tree_node_base* _M_header;                        // +0x00
    unsigned int _M_node_count;                           // +0x04
    _Compare _M_key_compare;                              // +0x08
};
}
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& out, const std::map<K, V, C>& m)
{
    CPersistTraits<K> key_traits;
    CPersistTraits<V> value_traits;
    out.WriteSLONG(m.size());
    for (typename std::map<K, V, C>::const_iterator it = m.begin(); it != m.end(); ++it) {
        key_traits.TransferBinaryOut(out, it->first);
        value_traits.TransferBinaryOut(out, it->second);
    }
}
template void __fastcall GFSerialiseMapBinaryOut<bool, EOpinionDeedType, std::less<EOpinionDeedType> >(CDataOutputStream& out, const std::map<EOpinionDeedType, bool, std::less<EOpinionDeedType> >& m);