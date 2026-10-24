// _global::GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<EObjectAugmentationType, CWeaponTrailGraphicSet>& value) -- write the element count, then every augmentation key and its weapon-trail graphic set (Attack, Knockdown) in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E910F, 71 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: the writer twin of the landed GFSerialiseMapBinaryIn<CWeaponTrailGraphicSet, EObjectAugmentationType> @ 0x004E9156
//   (same TU); the caller 0x004E90DA is the CPersistContext map transfer switch (its save arm passes the context's output
//   stream [esi+28h] in ecx and the map in edx). The catalog's "WriteAllocatedAreasDataWithThreeFields" is descriptive.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count, each key, Attack and Knockdown),
//   `_Rb_global<bool>::_M_increment(node)` @ 0x004292C0 (STLport's out-of-line successor step, __cdecl, `pop ecx`).
// Shape: the generic map writer of the landed 0x00469309 with both traits inlined: the enum key's traits write it as a long
//   (`push [esi+10h]`), CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryOut writes its two longs (Ego_r.pdb
//   CWeaponTrailGraphicSet {long Attack, long Knockdown}: `push [esi+14h]`, `push [esi+18h]`); end() re-reads the header.
//   Size TU without a frame pointer: `#pragma optimize("s", on)`; the set's two-call TransferBinaryOut must be
//   `__forceinline` (the size TU otherwise calls it out of line through a frame word, `push ecx` prologue).
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; STLport map = {header node pointer, count, comparator};
//   node = {colour, parent, left, right, pair<const EObjectAugmentationType, CWeaponTrailGraphicSet>} (value at +0x10).
#pragma optimize("s", on)
#include "engine/CDataOutputStream.h"

struct CDataOutputStreamWriter : CDataOutputStream {
    void WriteSLONG(long value);                             // 0x00993EB0
};
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CWeaponTrailGraphicSet {
public:
    long Attack;                                            // +0x00
    long Knockdown;                                         // +0x04
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
template <> struct CPersistTraits<EObjectAugmentationType> {
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const EObjectAugmentationType& value) const { stream.WriteSLONG((long)value); }
};
template <> struct CPersistTraits<CWeaponTrailGraphicSet> {
    __forceinline void TransferBinaryOut(CDataOutputStreamWriter& stream, const CWeaponTrailGraphicSet& value) const
    {
        stream.WriteSLONG(value.Attack);
        stream.WriteSLONG(value.Knockdown);
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
template void __fastcall GFSerialiseMapBinaryOut<CWeaponTrailGraphicSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(
    CDataOutputStreamWriter& stream, const std::map<EObjectAugmentationType, CWeaponTrailGraphicSet, std::less<EObjectAugmentationType> >& value);