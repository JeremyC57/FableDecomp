// CSkeletalMorphResourceManager::`scalar deleting destructor'(unsigned int flags) -- destroy the CachedMorphs map
// (out of line), fall back to the CTBaseSingleton<CSkeletalMorphResourceManager> vtable, free when bit 0 is set
// Leaf: ??_GCSkeletalMorphResourceManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AE3B0, 39 bytes, `ret 4`.
// Identity: slot 0 of vtable 0x01238F84, whose RTTI locator is `.?AVCSkeletalMorphResourceManager@@`
// (rtti-vtables.tsv); the vtable stored by the inlined base destructor, 0x01238C50, is
// `.?AV?$CTBaseSingleton@VCSkeletalMorphResourceManager@@@@`. Ego_r.pdb has ??_GCSkeletalMorphResourceManager@@UAEPAXI@Z
// and the layout {vptr +0, map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > CachedMorphs +4}. The +4 member
// is that map, not a list: its _Rb_tree destructor 0x004AE200 erases through 0x004AD690 and frees the header node
// (the landed constructor 0x004AE2A0 builds the same tree). The old label named the class by its base.
// Callees: ~_Rb_tree 0x004AE200, operator delete 0x00BFE9BC.
// Shape: constructing the class emits its vtable (and with it this destructor); the delete-expression calls it through
// the vtable. The class destructor is implicit, so the map destructor call and the base vtable store inline.
class CCharString;
class C3DSkeletalMorphTarget;
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
};

namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { };
template <class _Pair> struct _Select1st { };

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    void*        _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
    _Rb_tree();
    ~_Rb_tree();                                        // 0x004AE200 for the CachedMorphs tree
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                     // +0x00
};
}

template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() { }                      // vtable 0x01238C50
protected:
    CTBaseSingleton() { }
};

class CSkeletalMorphResourceManager : public CTBaseSingleton<CSkeletalMorphResourceManager> {
public:
    std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > CachedMorphs;   // +0x04
    CSkeletalMorphResourceManager() { }
};

CSkeletalMorphResourceManager* CreateSkeletalMorphResourceManager()
{
    return new CSkeletalMorphResourceManager;
}

void DestroySkeletalMorphResourceManager(CSkeletalMorphResourceManager* manager)
{
    delete manager;
}
