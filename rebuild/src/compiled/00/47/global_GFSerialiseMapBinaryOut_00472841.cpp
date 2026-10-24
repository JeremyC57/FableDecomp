// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<long, CCharString>& value) -- write the element count, then every long key and its string in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00472841, 85 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: the catalog's "ParticleMorphs_WriteToStream" is invented. The single caller is CPersistContext's binary
//   TransferMap dispatcher @ 0x0047280C (landed as "CEngineLightingManager::UpdateShadowScene": mode 3 passes the output
//   stream at +0x28 here, mode 2 the input stream at +0x24 to the reader 0x00472896), called from CGlobalDef::Transfer
//   @ 0x00472506: Ego_r.pdb CGlobalDef::ConversationCameras is map<long, CCharString, less<long> >.
// Callees: CCharString::CCharString() @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (the value traits' DefaultVal),
//   CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count and each key), CDataOutputStream::WriteNullTerminatedString
//   @ 0x00995AE0 (each value), `_Rb_global<bool>::_M_increment(node)` @ 0x004292C0 (__cdecl, `pop ecx`).
// Shape: the map writer template (twin of 0x00469309): `stream.WriteSLONG(size()); for (it = begin(); it != end(); ++it)
//   { keyTraits.TransferBinaryOut(stream, it->first); valueTraits.TransferBinaryOut(stream, it->second); }`. The
//   CCharString traits carries a DefaultVal string, so the traits is a frame local built first and destroyed last (its
//   implicit constructor/destructor inline to the two CCharString calls). Size TU with a frame
//   pointer: `optimize("y", off)` + `("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; STLport map = {header node pointer, count, comparator};
//   node = {colour, parent, left, right, pair<const long, CCharString>} (value at +0x10, the string at +0x14).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct PStringData;
class CCharString {
public:
    PStringData* PData;                                     // +0x00
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
};
struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                            // 0x00993EB0
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less { };
struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class B> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0
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
template <> struct CPersistTraits<long> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const long& value) const { stream.WriteSLONG(value); }
};
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CCharString& value) const { stream.WriteNullTerminatedString(value); }
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
template void __fastcall GFSerialiseMapBinaryOut<CCharString, long, std::less<long> >(
    CDataOutputStreamWriter& stream, const std::map<long, CCharString, std::less<long> >& value);