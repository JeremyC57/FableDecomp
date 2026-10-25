// Behaviour test for C3DAnimationInfo::~C3DAnimationInfo (0x00A25970): the shared animation data is released once
// (deleted when the last reference goes, kept alive otherwise), a null data pointer is fine, and walking a
// populated animation map terminates.
#include <cstdio>

class CMeshDataBank;
class C3DAnimation2;
class CCPPointerInfo;
class C3DVector { public: float X, Y, Z; };

static int g_steps = 0;

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
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        ++g_steps;
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
template <class _Key, class _Tp> class map {
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

static int g_deleted = 0;

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int) {}
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
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

static int g_baseDtors = 0;

class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry() { ++g_baseDtors; }
    CMeshDataBank* Bank;
    unsigned long  Index;
};

class C3DAnimationInfo : public CMeshDataBankEntry {
public:
    class CAnimationData : public CIVCountedPointeeBase {
    public:
        unsigned char _body[0x14];
        std::map<unsigned long, CCountedPointer<C3DAnimation2> > Animations;
    };
    CIVCountedPointer<CAnimationData> Data;
    float     Duration;
    float     NonLoopingDuration;
    C3DVector MovementVector;
    float     Rotation;
    virtual ~C3DAnimationInfo();
};

C3DAnimationInfo::~C3DAnimationInfo()
{
    if (Data) {
        for (std::map<unsigned long, CCountedPointer<C3DAnimation2> >::iterator it = Data->Animations.begin(); it != Data->Animations.end(); ++it) {
        }
    }
}

int main()
{
    int ok = 1;
    // animation data with a 3-entry map (header + root with two children)
    C3DAnimationInfo::CAnimationData data;
    std::_Rb_tree_node_base header, root, l, r;
    header._M_parent = &root; header._M_left = &l; header._M_right = &r;
    root._M_parent = &header; root._M_left = &l; root._M_right = &r;
    l._M_parent = &root; l._M_left = 0; l._M_right = 0;
    r._M_parent = &root; r._M_left = 0; r._M_right = 0;
    data.Animations._M_header = &header; data.Animations._M_node_count = 3;
    data.IVRefCount = 2;

    C3DAnimationInfo* a = new C3DAnimationInfo; a->Data.Object = &data;
    C3DAnimationInfo* b = new C3DAnimationInfo; b->Data.Object = &data;
    CMeshDataBankEntry* base = a;
    delete base;                                                   // virtual: derived dtor runs
    if (g_steps != 3 || data.IVRefCount != 1 || g_deleted != 0 || g_baseDtors != 1) ok = 0;
    delete b;                                                      // last reference: Delete()
    if (data.IVRefCount != 0 || g_deleted != 1 || g_baseDtors != 2) ok = 0;
    C3DAnimationInfo* c = new C3DAnimationInfo; c->Data.Object = 0;
    delete c;                                                      // null data: no walk, no release
    if (g_deleted != 1 || g_baseDtors != 3 || g_steps != 6) ok = 0;

    if (!ok) { std::printf("00a25970_TEST FAIL\n"); return 1; }
    std::printf("00a25970_TEST PASS\n");
    return 0;
}