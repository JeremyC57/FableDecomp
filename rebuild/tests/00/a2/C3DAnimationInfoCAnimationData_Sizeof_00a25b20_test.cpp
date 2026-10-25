// Behaviour test for C3DAnimationInfo::CAnimationData::Sizeof (0x00A25B20): 8 bytes per null counted pointer,
// pointee size + 8 otherwise, 12 bytes per map entry plus the header, plus every animation's own size.
#include <cstdio>

class CCPPointerInfo;
class C3DAnimationInfo;

class C3DAnimation2 {
public:
    unsigned long Size;
    unsigned long Sizeof() const { return Size; }
};
class CBankFileAsyncData {
public:
    long Body;
    unsigned long Sizeof() const { return Body + 72; }
};

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    operator bool() const { return Data != 0; }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global::_M_increment(_M_node); }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }
template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};
}

class C3DAnimationInfo {
public:
    class CAnimationData {
    public:
        typedef std::map<unsigned long, CCountedPointer<C3DAnimation2> > CAnimationMap;
        unsigned char       _base[0x1C];
        CAnimationMap       Animations;
        CCountedPointer<C3DAnimation2>      SharedAnimation;
        CCountedPointer<CBankFileAsyncData> FileData;
        C3DAnimationInfo*   AnimInfo;
        unsigned long Sizeof() const;
    };
};

unsigned long C3DAnimationInfo::CAnimationData::Sizeof() const
{
    unsigned long size = (SharedAnimation ? SharedAnimation->Sizeof() + sizeof(SharedAnimation) : sizeof(SharedAnimation))
                       + (FileData ? FileData->Sizeof() + sizeof(FileData) : sizeof(FileData))
                       + (Animations.size() + 1) * sizeof(CAnimationMap::value_type);
    for (CAnimationMap::const_iterator it = Animations.begin(); it != Animations.end(); ++it)
        size += it->second->Sizeof();
    return size;
}

typedef std::_Rb_tree_node<C3DAnimationInfo::CAnimationData::CAnimationMap::value_type> SNode;

int main()
{
    int ok = 1;
    unsigned int entry = (unsigned int)sizeof(C3DAnimationInfo::CAnimationData::CAnimationMap::value_type);   // 12 on the target
    unsigned int cp = (unsigned int)sizeof(CCountedPointer<C3DAnimation2>);                                  // 8 on the target
    C3DAnimationInfo::CAnimationData d;
    std::_Rb_tree_node_base header;
    header._M_parent = 0; header._M_left = &header; header._M_right = &header;
    d.Animations._M_header = &header; d.Animations._M_node_count = 0;
    d.SharedAnimation.Data = 0; d.FileData.Data = 0;
    if (d.Sizeof() != cp + cp + entry) ok = 0;                                   // empty: 8 + 8 + 12

    C3DAnimation2 shared; shared.Size = 100;
    CBankFileAsyncData file; file.Body = 28;
    d.SharedAnimation.Data = &shared; d.FileData.Data = &file;
    C3DAnimation2 a1, a2, a3; a1.Size = 5; a2.Size = 7; a3.Size = 11;
    unsigned char raw[3][sizeof(SNode)];
    SNode* n1 = (SNode*)raw[0]; SNode* n2 = (SNode*)raw[1]; SNode* n3 = (SNode*)raw[2];
    n2->_M_parent = &header; n2->_M_left = n1; n2->_M_right = n3; n2->_M_value_field.second.Data = &a2;
    n1->_M_parent = n2; n1->_M_left = 0; n1->_M_right = 0; n1->_M_value_field.second.Data = &a1;
    n3->_M_parent = n2; n3->_M_left = 0; n3->_M_right = 0; n3->_M_value_field.second.Data = &a3;
    header._M_parent = n2; header._M_left = n1; header._M_right = n3;
    d.Animations._M_node_count = 3;
    unsigned long expect = (100 + cp) + (28 + 72 + cp) + 4 * entry + 5 + 7 + 11;
    if (d.Sizeof() != expect) ok = 0;

    if (!ok) { std::printf("00a25b20_TEST FAIL\n"); return 1; }
    std::printf("00a25b20_TEST PASS\n");
    return 0;
}