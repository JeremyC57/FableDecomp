// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<long, CBlendedParticleEffectSet>& value) -- write the element count, then every long key and its blended particle-effect set in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00469309, 60 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: catalog label "Serialization_WriteParticleEffect" is descriptive; this is the writer twin of the landed
//   GFSerialiseMapBinaryIn<long, CBlendedParticleEffectSet> @ 0x00469381 (caller: CPersistContext::TransferBinary @ 0x004692F2).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count and each key), CBlendedParticleEffectSet::TransferBinaryOut
//   @ 0x00469345 (landed; __fastcall on the mapped value at node+0x14), `_Rb_global<bool>::_M_increment(node)` @ 0x004292C0
//   (STLport's out-of-line static successor step, __cdecl, `pop ecx`).
// Shape: `stream.WriteSLONG(size()); for (it = begin(); it != end(); ++it) { keyTraits.TransferBinaryOut(stream, it->first);
//   valueTraits.TransferBinaryOut(stream, it->second); }` with both traits inlined (the long traits is WriteSLONG, the set's
//   forwards to its own member); begin() is header->_M_left, end() re-reads the header from the map every pass.
//   Size TU without a frame pointer: `#pragma optimize("s", on)` (`pop ecx` after the increment).
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; STLport map = {header node pointer, count, comparator};
//   node = {colour, parent, left, right, pair<const long, CBlendedParticleEffectSet>} (value at +0x10).
#pragma optimize("s", on)
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CDataOutputStreamWriter : CDataOutputStream {
};
class CBlendedParticleEffectSet {
public:
    long  Particle1Index;                                   // +0x00
    float Particle1Value;                                   // +0x04
    long  Particle2Index;                                   // +0x08
    float Particle2Value;                                   // +0x0c
    void TransferBinaryOut(CDataOutputStreamWriter& stream) const;   // 0x00469345
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
template <> struct CPersistTraits<CBlendedParticleEffectSet> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CBlendedParticleEffectSet& value) const { value.TransferBinaryOut(stream); }
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
template void __fastcall GFSerialiseMapBinaryOut<CBlendedParticleEffectSet, long, std::less<long> >(
    CDataOutputStreamWriter& stream, const std::map<long, CBlendedParticleEffectSet, std::less<long> >& value);