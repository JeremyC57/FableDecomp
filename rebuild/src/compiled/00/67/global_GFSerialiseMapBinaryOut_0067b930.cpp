// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<EOpinionReactionType, EOpinionTargetingConditionType>& value) -- write the element count, then every key and value in key order
// Leaf: ??$GFSerialiseMapBinaryOut@W4EOpinionTargetingConditionType@@W4EOpinionReactionType@@U?$less@W4EOpinionReactionType@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@W4EOpinionReactionType@@W4EOpinionTargetingConditionType@@U?$less@W4EOpinionReactionType@@@std@@V?$allocator@U?$pair@$$CBW4EOpinionReactionType@@W4EOpinionTargetingConditionType@@@std@@@4@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067b930, 123 bytes, __fastcall (stream in ecx, map in edx), plain `ret`.
// Identity (proven): the landed COpinionReactionManagerDef::Transfer 0x00679740 passes COpinionReactionManagerDef::TargetingCondition (Ego_r.pdb map<EOpinionReactionType, EOpinionTargetingConditionType>) through its binary-save arm; Ego_r.pdb names the template ??$GFSerialiseMapBinaryOut@V@K@C@@YIXAAVCDataOutputStream@@ABV?$map@...@std@@@Z; bound only as the stand-in ?GFSerialiseMapBinaryOut@@YIXPAVCDataOutputStream@@PAUCReactionTargetingMap@@@Z.
// Callees: CDataOutputStream::WriteSLONG 0x00993EB0.
// Shape (family L033): count from _M_node_count (+4), begin = header->_M_left; per node key (+0x10) then value (+0x14);
//   the in-order step is STLport's static _Rb_global<bool>::_M_increment inlined onto a copy of the node.
class CDataOutputStream {                                  // (engine/CDataOutputStream.h has the layout but no methods)
public:
    void WriteSLONG(long value);                          // 0x00993EB0
    void WriteFloat(float value);                         // 0x00993EE0
};
enum EOpinionReactionType { };
enum EOpinionTargetingConditionType { };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
struct _Rb_tree_node_base {
    bool _M_color;                                        // +0x00
    _Rb_tree_node_base* _M_parent;                        // +0x04
    _Rb_tree_node_base* _M_left;                          // +0x08
    _Rb_tree_node_base* _M_right;                         // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                // +0x10
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                // STLport 4.6 (_Rb_tree inlined)
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                        // +0x00
    unsigned int _M_node_count;                           // +0x04
    _Compare _M_key_compare;                              // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
};
}
template <class T> class CPersistTraits {                  // scalars and enums: one SLONG
public:
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) const { stream.WriteSLONG((long)value); }
};
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStream& stream, const std::map<K, V, C>& value)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    stream.WriteSLONG(value.size());
    for (typename std::map<K, V, C>::const_iterator it = value.begin(); it != value.end(); ++it) {
        keyTraits.TransferBinaryOut(stream, it->first);
        valueTraits.TransferBinaryOut(stream, it->second);
    }
}
template void __fastcall GFSerialiseMapBinaryOut<EOpinionTargetingConditionType, EOpinionReactionType, std::less<EOpinionReactionType > >(CDataOutputStream&, const std::map<EOpinionReactionType, EOpinionTargetingConditionType, std::less<EOpinionReactionType > >&);