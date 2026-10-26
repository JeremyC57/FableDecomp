// _global::GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<EObjectAugmentationType, CObjectAugmentationParticleSet>& value) -- write the element count, then every augmentation key and its particle set in key order
// Leaf: GFSerialiseMapBinaryOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC85A, 104 bytes, `ret` (__fastcall: the stream in ecx, the map in edx).
// Identity: the save arm of the CPersistContext map transfer at 0x004EC843; its load arm is GFSerialiseMapBinaryIn
//   <CObjectAugmentationParticleSet, EObjectAugmentationType> 0x004EC8C2 (whose default pair is the landed 0x004EC9B5).
// Label correction: the catalog's "Shadow_WriteAugmentationParticles" is descriptive; this is the generic map writer of the
//   landed 0x00469309 / 0x004E910F.
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (the count and each key), CPersistTraits<CObjectAugmentationParticleSet>::
//   TransferBinaryOut @ 0x004E959D (landed; out of line, thiscall on the traits), `_Rb_global<bool>::_M_increment` @ 0x004292C0,
//   ~CObjectAugmentationParticleSet @ 0x004E25F1 (the traits' DefaultVal; catalogued under a propagated renderer name).
// Shape: `CPersistTraits<K> keyTraits; CPersistTraits<V> valueTraits; WriteSLONG(size()); for (it = begin(); it != end();
//   ++it) { keyTraits.TransferBinaryOut(stream, it->first); valueTraits.TransferBinaryOut(stream, it->second); }` -- the value
//   traits hold a DefaultVal CObjectAugmentationParticleSet built inline (PeriodSecs 0.0 by `fldz ; fstp`, the two empty
//   CArray<CDefString> vectors as six zero stores from one register; the four effect indices left alone, as in 0x004EC9B5)
//   and destroyed out of line at the end; the enum key traits are inline (WriteSLONG). Frame-pointer size TU:
//   `optimize("y", off)` + `("s", on)`.
// Model: engine/CDataOutputStream.h + a methods-only writer adapter; CObjectAugmentationParticleSet per Ego_r.pdb with the
//   retail 12-byte STLport CArray (local model as in 0x004EC9B5); STLport map/tree iterator as in 0x00469309.
#pragma optimize("y", off)
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
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CDefString;
template <class T> class CArray {                           // retail 12 bytes: start, finish, end of storage
public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    CArray() : Start(0), Finish(0), EndOfStorage(0) { }
};
class CObjectAugmentationParticleSet {
public:
    float              PeriodSecs;                          // +0x00
    long               ParticleEffect;                      // +0x04
    long               ParticleEffectToBlendTo;             // +0x08
    long               AttackParticleEffect;                // +0x0c
    long               AttackParticleEffectToBlendTo;       // +0x10
    CArray<CDefString> StartPoint;                          // +0x14
    CArray<CDefString> EndPoint;                            // +0x20
    CObjectAugmentationParticleSet() : PeriodSecs(0.0f) { }
    ~CObjectAugmentationParticleSet();                      // 0x004E25F1
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
template <> struct CPersistTraits<CObjectAugmentationParticleSet> {
    CObjectAugmentationParticleSet DefaultVal;              // +0x00
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CObjectAugmentationParticleSet& value) const;   // 0x004E959D
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
template void __fastcall GFSerialiseMapBinaryOut<CObjectAugmentationParticleSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(
    CDataOutputStreamWriter& stream, const std::map<EObjectAugmentationType, CObjectAugmentationParticleSet, std::less<EObjectAugmentationType> >& value);