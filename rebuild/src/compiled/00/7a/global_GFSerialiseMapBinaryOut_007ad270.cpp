// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<CCharString,bool>& value) -- write the element count, then every string key (null terminated) and its bool in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AD270, 136 bytes (+8 int3), `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: the only caller is the landed CPersistContext::TransferMap<CCharString,bool> 0x007AE3D0 (mode 3, "the binary
//   map writer 0x007AD270"), the CTCAIScratchpad map<CCharString,bool>. The catalog label "LTextBinTree_SerializeNode" is
//   descriptive.
// Callees: CCharString::CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0 (the key traits' default value),
//   CDataOutputStream::WriteSLONG 0x00993EB0 (the count), WriteNullTerminatedString 0x00995AE0 (each key),
//   WriteBool 0x00993E30 (each value).
// Shape: the landed GFSerialiseMapBinaryOut template (0x00632B30 / 0x006327E0) with the KEY traits carrying the
//   CCharString DefaultVal (the `push ecx` frame word, built first, destroyed last); unlike those TUs the STLport
//   `_M_increment` successor step is inline here (the same TU's range erase 0x007AD180 inlines it too), so the walk
//   is open-coded; end() re-reads the header every pass.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter (as 0x00469309 / 0x00632B30).
#include "engine/CDataOutputStream.h"

class CCharString;
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteBool(bool value);                                      // 0x00993E30
    void WriteSLONG(long value);                                     // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& text);        // 0x00995AE0
};

namespace std {
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class T> class less { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class B> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
    const V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    const V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};
}
template <class T> struct CPersistTraits;
class CCharString {
public:
    void* PStringData;
    CCharString();                                    // 0x0099E4B0
    ~CCharString();                                   // 0x0099EAE0
};
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CCharString& value) const { stream.WriteNullTerminatedString(value); }
};
template <> struct CPersistTraits<bool> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const bool& value) const { stream.WriteBool(value); }
};
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(CDataOutputStreamWriter& stream, const std::map<K, V, C>& value)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    stream.WriteSLONG(value.size());
    for (typename std::map<K, V, C>::const_iterator it = value.begin(); it != value.end(); ++it) {
        keyTraits.TransferBinaryOut(stream, it->first);
        valueTraits.TransferBinaryOut(stream, it->second);
    }
}
template void __fastcall GFSerialiseMapBinaryOut<bool, CCharString, std::less<CCharString> >(
    CDataOutputStreamWriter& stream, const std::map<CCharString, bool, std::less<CCharString> >& value);