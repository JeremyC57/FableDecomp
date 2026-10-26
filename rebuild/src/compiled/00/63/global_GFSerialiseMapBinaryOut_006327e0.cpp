// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<ETableSprites,long>& value) -- write the element count, then every sprite key and its long value in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006327E0, 75 bytes (+ int3 padding), `ret` (__fastcall: the stream in ecx,
// the map in edx).
// Identity: the only caller is the binary map transfer 0x006327A0 (mode 3) for PDB CUIDef::Sprites, a
// map<NUISystem::ETableSprites, long>; the writer twin of the landed GFSerialiseMapBinaryOut<CBlendedParticleEffectSet, long>
// 0x00469309. The catalog label "CDataOutputStream_WriteMemoryAreas" is wrong.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count, each key and each value), `_Rb_global<bool>::_M_increment`
// @ 0x004292C0 (STLport's out-of-line successor step, __cdecl, `add esp,4`).
// Shape: `stream.WriteSLONG(size()); for (it = begin(); it != end(); ++it) { keyTraits.TransferBinaryOut(stream, it->first);
// valueTraits.TransferBinaryOut(stream, it->second); }` with both (empty) traits inlined to WriteSLONG; begin() is
// header->_M_left, end() re-reads the header from the map every pass. Speed TU (aligned loop head, `add esp,4`).
// Model: engine/CDataOutputStream.h + a methods-only writer adapter (as 0x00469309).
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

class CCharString;
struct CDataOutputStreamWriter : CDataOutputStream {
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
namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_0 };
}
template <> struct CPersistTraits<long> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const long& value) const { stream.WriteSLONG(value); }
};
template <> struct CPersistTraits<NUISystem::ETableSprites> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const NUISystem::ETableSprites& value) const { stream.WriteSLONG(value); }
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
template void __fastcall GFSerialiseMapBinaryOut<long, NUISystem::ETableSprites, std::less<NUISystem::ETableSprites> >(
    CDataOutputStreamWriter& stream, const std::map<NUISystem::ETableSprites, long, std::less<NUISystem::ETableSprites> >& value);