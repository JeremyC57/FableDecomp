// GFSerialiseMapBinaryOut(CDataOutputStream& stream, const map<long, CComboMultiplierData>& value) -- write the entry count, then every long key and its combo-multiplier data in key order
// Leaf: ??$GFSerialiseMapBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC350, 244 bytes (+ int3 padding to 256), `ret` (__fastcall: the
//   stream in ecx, the map in edx).
// Identity: the catalog's "CHeroExperienceDef_WriteTreeToStream" is a descriptive auto label. The value traits
//   0x006DB060 (CPersistTraits<CComboMultiplierData>::TransferBinaryOut, landed) is called only from here; the map is
//   Ego_r.pdb CHeroExperienceDef::ComboMultiplierMap, map<long, CComboMultiplierData>. Callers 0x006D8FE9, 0x006DD43F,
//   0x006DD51F (the CHeroExperienceDef transfer paths).
// Callees: malloc 0x00BFEA0E (the traits' DefaultVal inner-map header), CDataOutputStream::WriteSLONG(long) 0x00993EB0
//   (the count and each key), CPersistTraits<CComboMultiplierData>::TransferBinaryOut 0x006DB060 (each value),
//   _Rb_tree<EDamageAttribute, ..>::_M_erase 0x00450903 and free 0x00BFEA14 (the traits' destructor).
// Shape: the map writer template (twin of 0x00472841): the value traits carries a DefaultVal CComboMultiplierData, so it
//   is a frame local whose implicit constructor (STLport map: allocate the header, colour red, root 0, leftmost =
//   rightmost = header, count 0) runs first and whose destructor (clear() + header free) runs last; the long key traits
//   is empty and inlines to WriteSLONG; `++it` inlines STLport's _M_increment.
#include "engine/CDataOutputStream.h"
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

struct COutputStreamMethods : CDataOutputStream {
    void WriteSLONG(long value);                            // 0x00993EB0
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
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class Dummy> struct _Rb_global {
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
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    const V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
};
template <class V> struct _Rb_tree_base {
    typedef _Rb_tree_node<V> _Node;
    _Rb_tree_node_base* _M_header;                          // +0x00
    _Rb_tree_base() : _M_header(0) { _M_header = (_Node*)malloc(sizeof(_Node)); }
    ~_Rb_tree_base() { if (_M_header != 0) free(_M_header); }
};
template <class K, class V, class C = less<K> > class map : public _Rb_tree_base<pair<const K, V> > {
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    map() : _M_node_count(0)
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
    ~map() { clear(); }
    void _M_erase(_Rb_tree_node_base* x);                   // 0x00450903 (map<EDamageAttribute, float>)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(this->_M_header->_M_parent);
            this->_M_header->_M_left = this->_M_header;
            this->_M_header->_M_parent = 0;
            this->_M_header->_M_right = this->_M_header;
            _M_node_count = 0;
        }
    }
    const_iterator begin() const { return const_iterator(this->_M_header->_M_left); }
    const_iterator end() const { return const_iterator(this->_M_header); }
    unsigned long size() const { return _M_node_count; }
};
}

class CComboMultiplierData {                                // Ego_r.pdb, 0x14 bytes with the 12-byte STLport map
public:
    std::map<EDamageAttribute, float> ComboMultiplier;      // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    void TransferBinaryOut(COutputStreamMethods& stream, const long& value) const { stream.WriteSLONG(value); }
};
template <> struct CPersistTraits<CComboMultiplierData> {
    CComboMultiplierData DefaultVal;                        // +0x00
    void TransferBinaryOut(COutputStreamMethods& stream, const CComboMultiplierData& value) const;   // 0x006DB060
};

template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryOut(COutputStreamMethods& stream, const std::map<K, V, C>& value)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    stream.WriteSLONG(value.size());
    for (typename std::map<K, V, C>::const_iterator it = value.begin(); it != value.end(); ++it) {
        keyTraits.TransferBinaryOut(stream, it->first);
        valueTraits.TransferBinaryOut(stream, it->second);
    }
}

template void __fastcall GFSerialiseMapBinaryOut<CComboMultiplierData, long, std::less<long> >(
    COutputStreamMethods& stream, const std::map<long, CComboMultiplierData, std::less<long> >& value);