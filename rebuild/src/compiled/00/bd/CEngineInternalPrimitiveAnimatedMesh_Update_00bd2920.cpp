// CEngineInternalPrimitiveAnimatedMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- refresh the internal animated mesh from its game-side primitive: mesh info from mesh bank 0, scale = size / 100, start/end world position, orientation, flags, theme channel, tint, cloth flag, a fresh bone-scaling cache, alpha, mesh effect, animation interpolator, then the game-default fade distance with the override bit set
// Leaf: ?Update@CEngineInternalPrimitiveAnimatedMesh@@UAEXABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD2920, 361 bytes, thiscall virtual, `ret 8`.
// Identity: Ego_r.pdb ?Update@CEngineInternalPrimitiveAnimatedMesh@@UAEXABVCEnginePrimitive@@K@Z; the body writes the
//   CEngineInternalPrimitiveAnimatedMesh members (Anim +0x70, WorldPos +0x84, Orientation +0xa0, Cache +0xd4) and its
//   sibling 2D Update 0x00BD2BC0 has the same shape.
// Callees: CMeshDataBank::PeekAnimatedMesh 0x00A242B0 on g_pResourceManager->PeekMeshBank(0) (inline), C3DOrientationInterpolator::
//   operator= 0x006B0E00, malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport map header node), the BoneScalings map's _Rb_tree::
//   operator= 0x00BD8D00 and _M_erase 0x00BD8A10, C3DAnimationInterpolator::operator= 0x00BD85F0,
//   CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults 0x00B8FD40.
// Layout: retail CEngineInternalPrimitive* chain (Ego_r - 4 from +0xe0: RenderInfo); desc CEnginePrimitiveAnimatedMesh offsets from
//   the body (MeshIndex +0xc, Size +0x10, FadeStart +0x14, Alpha +0x18, WorldPos +0x1c, Orientation +0x38, Anim +0x6c,
//   MeshEffect +0x80, ThemeChannelDesc +0x84, TintColour +0x88, BulletTimeAffectsCloth +0x8c).
// Shape: `Cache = CCache()` builds an STLport map temporary (malloc'd header), assigns it through the out-of-line tree
//   operator= and erases/frees it; the counted mesh-effect assignment is the inline CIVCountedPointer operator=.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CRGBColour { public: unsigned char B, G, R, A; };
class C3DMeshInfo;
template <class T> class CArray { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; };

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(__a, (_Rb_tree_node<_Tp>*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_node<_Value>* _Link_type;
public:
    typedef _Alloc allocator_type;
protected:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
private:
    void _M_erase(_Link_type __x);                   // 0x00BD8A10
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Rb_tree& operator=(const _Rb_tree& __x);       // 0x00BD8D00
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t() { }
    map& operator=(const map& __x) { _M_t = __x._M_t; return *this; }
};
}

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    long IVRefCount;                                   // +0x04
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        T* old = Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
class CEngineMeshEffect : public CIVCountedPointeeBase { };

class C3DPositionInterpolator { public:
    virtual ~C3DPositionInterpolator();
    C3DVector StartPosition;                           // +0x04
    C3DVector EndPosition;                             // +0x10
};
class C3DOrientationInterpolator { public:
    virtual ~C3DOrientationInterpolator();
    CRightHandedSet StartOrientation;                  // +0x04
    CRightHandedSet EndOrientation;                    // +0x1c
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);   // 0x006B0E00
};
class C3DAnimationBlendState { public: void* _vftable; CIVCountedPointer<CIVCountedPointeeBase> Data; };
class C3DAnimationInterpolator { public:
    virtual ~C3DAnimationInterpolator();
    C3DAnimationBlendState StartState;                 // +0x04
    C3DAnimationBlendState EndState;                   // +0x0c
    C3DAnimationInterpolator& operator=(const C3DAnimationInterpolator& other);       // 0x00BD85F0
};

enum EMeshBankNo { };
class CMeshDataBank { public:
    const C3DMeshInfo* PeekAnimatedMesh(unsigned long index) const;      // 0x00A242B0
};
class CEngineResourceManager { public:
    unsigned char  _pad00[0x10];
    CMeshDataBank* MeshBanks[3];                       // +0x10
    const CMeshDataBank* PeekMeshBank(EMeshBankNo no) const { return MeshBanks[no]; }
};
extern CEngineResourceManager* g_pResourceManager;     // 0x01436E98

class CEnginePrimitive { public:
    long          Type;                                // +0x00
    long          InterpolationSet;                    // +0x04
    unsigned long ThemeChannelDesc;                    // +0x08
};
class CEnginePrimitiveAnimatedMesh : public CEnginePrimitive { public:
    unsigned long                MeshIndex;            // +0x0c
    float                        Size;                 // +0x10
    float                        FadeStart;            // +0x14
    unsigned char                Alpha;                // +0x18
    C3DPositionInterpolator      WorldPos;             // +0x1c
    C3DOrientationInterpolator   Orientation;          // +0x38
    C3DAnimationInterpolator     Anim;                 // +0x6c
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;   // +0x80
    unsigned long                ThemeChannelDesc;     // +0x84
    CRGBColour                   TintColour;           // +0x88
    bool                         BulletTimeAffectsCloth; // +0x8c
};

class CEngineInternalPrimitiveBase { public:
    virtual ~CEngineInternalPrimitiveBase();
    unsigned char     _pad04[8];
    unsigned long     EnginePrimitiveFlags;            // +0x0c
    unsigned char     _pad10[0x17];
    unsigned char     Alpha;                           // +0x27
    unsigned char     _pad28[0xc];
    unsigned char     Enable : 1;                      // +0x34
    unsigned char     NonPersistent : 1;
    unsigned char     OverrideFadeDistance : 1;
    unsigned char     PostRenderEnable : 1;
    unsigned char     _pad35[0x13];
protected:
    void SetFadeDistanceWithGamePrimitiveDefaults(float fadeStart);   // 0x00B8FD40
};
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveBase { public:
    unsigned char     _pad48[8];
    C3DMeshInfo*      MeshInfo;                        // +0x50
    C3DMeshInfo*      ShadowMeshInfo;                  // +0x54
    float             Scale;                           // +0x58
    CRGBColour        TintColour;                      // +0x5c
    bool              BulletTimeAffectsCloth;          // +0x60
    CIVCountedPointer<CIVCountedPointeeBase> SimulationInfo;   // +0x64
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;   // +0x68
    unsigned long     ThemeChannelDesc;                // +0x6c
};
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { public:
    class CCache { public:
        std::map<unsigned long, CArray<C3DVector> > BoneScalings;
    };
    C3DAnimationInterpolator   Anim;                   // +0x70
    C3DPositionInterpolator    WorldPos;               // +0x84
    C3DOrientationInterpolator Orientation;            // +0xa0
    CCache                     Cache;                  // +0xd4
    unsigned char              RenderInfo[0x48];       // +0xe0 CEngineRenderInfoAnimatedMesh (retail 4 below Ego_r)
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveAnimatedMesh::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveAnimatedMesh& mesh = static_cast<const CEnginePrimitiveAnimatedMesh&>(primitive);
    MeshInfo = const_cast<C3DMeshInfo*>(g_pResourceManager->PeekMeshBank((EMeshBankNo)0)->PeekAnimatedMesh(mesh.MeshIndex));
    Scale = mesh.Size * 0.01f;
    WorldPos.StartPosition = mesh.WorldPos.StartPosition;
    WorldPos.EndPosition = mesh.WorldPos.EndPosition;
    Orientation = mesh.Orientation;
    EnginePrimitiveFlags = flags;
    ThemeChannelDesc = mesh.ThemeChannelDesc;
    TintColour = mesh.TintColour;
    BulletTimeAffectsCloth = mesh.BulletTimeAffectsCloth;
    Cache = CCache();
    Alpha = mesh.Alpha;
    MeshEffect = mesh.MeshEffect;
    Anim = mesh.Anim;
    SetFadeDistanceWithGamePrimitiveDefaults(mesh.FadeStart);
    OverrideFadeDistance = 1;
}