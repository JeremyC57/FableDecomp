// CPersistTraits<CHeroStatIncreaseData>::TransferBinaryOut(CDataOutputStream& stream, const CHeroStatIncreaseData& value) -- write the stat-increase map's size, then every (stat category, increase) entry in key order
// Leaf: ?TransferBinaryOut@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB0F0, length 125 bytes (+ int3 padding), thiscall `ret 8` (`this` =
// the traits object, unused). Identity from the landed GFSerialiseMapBinaryOut<CHeroStatIncreaseData,EDamageAttribute>
// 0x006DC450, whose model declares exactly this member ("native6DB0F0/PDB58AD90, void thiscall(stream&, const data&),
// inner enum keys WriteSLONG and float WriteFloat993EE0"). The manifest label "TreeNode_SerializeRecursive_v2" is
// descriptive (there is no recursion: it is an in-order STLport map walk).
// Callees: CDataOutputStream::WriteSLONG(long) @ 0x00993EB0, WriteFloat(float) @ 0x00993EE0.
// Layout: CHeroStatIncreaseData {map<EHeroExperienceStatCategory, float> StatIncreases} -- retail STLport map
// {header +0, node count +4}; node {colour, parent +4, left +8, right +0xC, key +0x10, float +0x14}.
// Shape: `stream.WriteSLONG(map.size())` then `for (it = begin(); it != end(); ++it)` with end() re-read every pass and
// STLport's iterator step inlined as `_M_node = _M_increment(_M_node)` over the static successor function (right
// subtree's leftmost, else climb while we are a right child, the header special case).
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

enum EHeroExperienceStatCategory { EHeroExperienceStatCategory_Force32 = 0x7fffffff };

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

class CHeroStatIncreaseData { public:
    std::map<EHeroExperienceStatCategory, float> StatIncreases;   // +0x00
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CHeroStatIncreaseData> {
    CHeroStatIncreaseData DefaultVal;
    void TransferBinaryOut(COutputStreamMethods& stream, const CHeroStatIncreaseData& value) const;
};

void CPersistTraits<CHeroStatIncreaseData>::TransferBinaryOut(COutputStreamMethods& stream, const CHeroStatIncreaseData& value) const
{
    stream.WriteSLONG(value.StatIncreases.size());
    for (std::map<EHeroExperienceStatCategory, float>::const_iterator it = value.StatIncreases.begin();
         it != value.StatIncreases.end(); ++it) {
        stream.WriteSLONG(it->first);
        stream.WriteFloat(it->second);
    }
}