// C3DAnimationInfo::~C3DAnimationInfo() -- walk the shared animation data's animation map (debug checks compiled out), release the counted data pointer, then the CMeshDataBankEntry base
// Leaf: ??1C3DAnimationInfo@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25970, 120 bytes, plain `ret`.
// Identity: the vtable stored first is C3DAnimationInfo's 0x0129CD64 and the last one CMeshDataBankEntry's 0x0129CD28
//   (rtti-vtables.tsv), so this is `??1C3DAnimationInfo@@UAE@XZ` (Ego_r.pdb); the catalog label
//   "CMeshDataBank_Entry_Destructor" names the base.
// Layout: C3DAnimationInfo (0x28) over CMeshDataBankEntry (Bank +4, Index +8): Data = CIVCountedPointer<CAnimationData>
//   +0x0C (pointee IVRefCount +4, Delete = slot 1); CAnimationData::Animations (map<unsigned long,
//   CCountedPointer<C3DAnimation2>>, Ego_r.pdb +0x1C) is walked begin..end with an empty body (its per-entry debug
//   check is compiled out; VC7.1 keeps the loop).
// Shape: the counted pointer's release caches the pointee in a local (`--old->IVRefCount` sets the flags; releasing
//   through `Object` reloads it after the decrement), then nulls the member; the inline empty base destructor's
//   vtable store is the last instruction.
class CMeshDataBank;
class C3DAnimation2;
class CCPPointerInfo;
class C3DVector { public: float X, Y, Z; };

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
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
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
    }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }
template <class _Key, class _Tp> class map {                      // STLport map, 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
}

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);                     // slot 0
    virtual void Delete();                               // slot 1
    unsigned long IVRefCount;                            // +0x04
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return (T*)Object; }
    operator bool() const { return Object != 0; }
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

class CMeshDataBankEntry {                               // vtable 0x0129CD28
public:
    virtual ~CMeshDataBankEntry() {}
    CMeshDataBank* Bank;                                 // +0x04
    unsigned long  Index;                                // +0x08
};

class C3DAnimationInfo : public CMeshDataBankEntry {    // vtable 0x0129CD64, 0x28 bytes
public:
    class CAnimationData : public CIVCountedPointeeBase {
    public:
        unsigned char _body[0x14];                       // +0x08
        _STL::map<unsigned long, CCountedPointer<C3DAnimation2> > Animations;   // +0x1C
    };
    CIVCountedPointer<CAnimationData> Data;              // +0x0C
    float     Duration;                                  // +0x10
    float     NonLoopingDuration;                        // +0x14
    C3DVector MovementVector;                            // +0x18
    float     Rotation;                                  // +0x24
    virtual ~C3DAnimationInfo();
};

C3DAnimationInfo::~C3DAnimationInfo()
{
    if (Data) {
        for (_STL::map<unsigned long, CCountedPointer<C3DAnimation2> >::iterator it = Data->Animations.begin(); it != Data->Animations.end(); ++it) {
        }
    }
}