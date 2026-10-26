// CPersistTraits<CComboMultiplierData>::TransferBinaryOut(CDataOutputStream& stream, const CComboMultiplierData& value) -- write the two attenuation floats, then the combo-multiplier map's size and every (damage attribute, multiplier) entry
// Leaf: ?TransferBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB060, length 144 bytes, thiscall `ret 8` (`this` = the traits object,
// unused). Single caller 0x006DC3B4 inside 0x006DC350 (manifest "CHeroExperienceDef_WriteTreeToStream": the
// GFSerialiseMapBinaryOut of CHeroExperienceDef::ComboMultiplierMap, map<long, CComboMultiplierData>). The value layout
// is Ego_r.pdb CComboMultiplierData with the retail 12-byte STLport map: ComboMultiplier map<EDamageAttribute, float>
// +0, ExperienceMultiplierNumSecondsForAttenuation +0xC, ExperienceMultiplierParamDecayDeltaPerSecond +0x10. The
// manifest label "TreeNode_SerializeRecursive" is descriptive (an in-order map walk, no recursion). Sibling of the
// CHeroStatIncreaseData traits 0x006DB0F0 (this chunk's row).
// Callees: CDataOutputStream::WriteSLONG(long) @ 0x00993EB0, WriteFloat(float) @ 0x00993EE0.
// Shape: the two floats by value first, then `WriteSLONG(map.size())` and the `for (it = begin(); it != end(); ++it)`
// walk (end() re-read each pass, `_M_node = _M_increment(_M_node)` inlined).
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

struct COutputStreamMethods : CDataOutputStream {
};

namespace std {

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class K, class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    K first;                                                // +0x10
    V second;                                               // +0x14
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

template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};

}

class CComboMultiplierData { public:
    std::map<EDamageAttribute, float> ComboMultiplier;      // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CComboMultiplierData> {
    void TransferBinaryOut(COutputStreamMethods& stream, const CComboMultiplierData& value) const;
};

void CPersistTraits<CComboMultiplierData>::TransferBinaryOut(COutputStreamMethods& stream, const CComboMultiplierData& value) const
{
    stream.WriteFloat(value.ExperienceMultiplierNumSecondsForAttenuation);
    stream.WriteFloat(value.ExperienceMultiplierParamDecayDeltaPerSecond);
    stream.WriteSLONG(value.ComboMultiplier.size());
    for (std::map<EDamageAttribute, float>::const_iterator it = value.ComboMultiplier.begin();
         it != value.ComboMultiplier.end(); ++it) {
        stream.WriteSLONG(it->first);
        stream.WriteFloat(it->second);
    }
}