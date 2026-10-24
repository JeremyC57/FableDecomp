// C3DAnimationInfo::CAnimationData::Sizeof() -- memory used by an animation data block: shared animation, file data, the animation map and every animation in it
// Leaf: ?Sizeof@CAnimationData@C3DAnimationInfo@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25B20, 161 bytes, plain `ret`. Callers 0x00A26293, 0x00A265B7
//   (C3DAnimationInfo::Sizeof, landed: it names this address CAnimationData::Sizeof), 0x00A2BBA0 (tail jump),
//   0x00A2C436. The catalog label "C3DAnimationInfo_CalcSize" is descriptive.
// Identity: Ego_r.pdb `?Sizeof@CAnimationData@C3DAnimationInfo@@QBEKXZ`. Layout (Ego_r.pdb CAnimationData with the
//   12-byte STLport map): Animations map<unsigned long, CCountedPointer<C3DAnimation2>> +0x1C (count +0x20),
//   SharedAnimation +0x28, FileData +0x30, AnimInfo +0x38.
// Callees: C3DAnimation2::Sizeof 0x00A4C1C0 and CBankFileAsyncData::Sizeof 0x009D5620 (PDB name; the manifest's
//   CVertexBufferWin32::DoSizeof is an identical-COMDAT fold of `return field + 72`).
// Shape: each counted pointer costs `p ? p->Sizeof() + 8 : 8`, the map (size() + 1) * 12 (nodes plus header), then
//   every animation's Sizeof. The rb-tree increment is STLport 4.6's static `_Rb_global::_M_increment(node)`
//   returning the next node: the walk then runs on a copy (eax) of the iterator held in esi across the call; a
//   member `_M_increment()` that updates `_M_node` in place walks esi itself (2 bytes shorter).
class CCPPointerInfo;
class C3DAnimationInfo;

class C3DAnimation2 {
public:
    unsigned long Sizeof() const;                        // 0x00A4C1C0
};
class CBankFileAsyncData {
public:
    unsigned long Sizeof() const;                        // 0x009D5620 (identical-COMDAT fold with CVertexBufferWin32::DoSizeof)
};

template <class T> class CCountedPointer {               // 8 bytes
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    operator bool() const { return Data != 0; }
};

namespace _STL {
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
struct _Rb_global {                                       // STLport 4.6 _Rb_global<bool>::_M_increment
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
template <class _Key, class _Tp> class map {                       // STLport map, 12 bytes
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
        typedef _STL::map<unsigned long, CCountedPointer<C3DAnimation2> > CAnimationMap;
        unsigned char       _base[0x1C];                // CIVCountedPointeeBase + loader state
        CAnimationMap       Animations;                 // +0x1C
        CCountedPointer<C3DAnimation2>      SharedAnimation;   // +0x28
        CCountedPointer<CBankFileAsyncData> FileData;          // +0x30
        C3DAnimationInfo*   AnimInfo;                   // +0x38
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