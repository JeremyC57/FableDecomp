// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<CCharString,pair<long,long>>& value) -- write the element count, then every region name (null terminated) and its crime duration and fine in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756540, 149 bytes (+11 int3), `ret` (__fastcall: the stream in ecx, the map in edx).
// Caller: 0x00756D64, the binary-save arm (mode 3, stream = PSaveStream +0x28) of CPersistContext::TransferMap
//   (0x00756D00) for CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine (landed OnSerialise 0x007556B0).
// Callees: CCharString::CCharString() @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0 (the key traits' DefaultVal),
//   CDataOutputStream::WriteSLONG @ 0x00993EB0 (count, duration, fine; manifest "WriteCBYTE" is a fold),
//   CDataOutputStream::WriteNullTerminatedString @ 0x00995AE0.
// Label: the manifest's "LTextBinTree_WriteBinary" is descriptive; Ego_r.pdb names the template
//   `??$GFSerialiseMapBinaryOut@...@@YIXAAVCDataOutputStream@@ABV?$map@...@std@@@Z` (this is its
//   <pair<long,long>, CCharString, less<CCharString>> instance; siblings landed at 0x00632B30 / 0x006327E0).
// Shape: the landed 0x00632B30 template with the roles swapped: CPersistTraits<CCharString> (the KEY traits here) holds
//   the CCharString DefaultVal, so the `push ecx` frame word is built first and destroyed last; the pair<long,long>
//   value traits are empty and write both longs. Unlike 0x00632B30 this TU inlines the static
//   `_Rb_global<bool>::_M_increment(node)` (right subtree's leftmost, else climb while a right child): it walks a COPY of
//   the node (eax, parent in ecx) that operator++ assigns back -- an in-place member walk on the iterator moves esi itself.
#include "engine/CDataOutputStream.h"

class CCharString;
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                                     // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& text);        // 0x00995AE0
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less { };
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
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
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
template <> struct CPersistTraits<std::pair<long, long> > {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const std::pair<long, long>& value) const
    {
        stream.WriteSLONG(value.first);
        stream.WriteSLONG(value.second);
    }
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
template void __fastcall GFSerialiseMapBinaryOut<std::pair<long, long>, CCharString, std::less<CCharString> >(
    CDataOutputStreamWriter& stream, const std::map<CCharString, std::pair<long, long>, std::less<CCharString> >& value);