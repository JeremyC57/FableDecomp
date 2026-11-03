// CEngineLightingManager::~CEngineLightingManager() -- engine lighting manager destructor: free the aligned SIMD variable block, then the shadowed-spot-light, local-light and theme-channel arrays, then the CEngineComponent and primitive-manager bases
// Leaf: ??1CEngineLightingManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B47B50, 110 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x00B4A4A0, slot 0 of vtable 0x012A2274 (RTTI
//   CEngineLightingManager : CEnginePrimitiveManager @0 (: CEnginePrimitiveRenderUtil : CBaseClassNonCopyable),
//   CEngineComponent @0xC); the body stores both vtables (0x012A2274 at +0, 0x012A223C at +0xC). Ego_r.pdb
//   ??1CEngineLightingManager@@UAE@XZ (0x009BD7B0) / ??_G (0x009BEFF0); FableWin.pdb ??1 (0x02CED320) / ??_G (0x02CED2A0)
//   right after the inline ??1CEnginePrimitiveManager@@UAE@XZ / ??1CEnginePrimitiveRenderUtil@@UAE@XZ (0x02CED240 / 0x02CED260)
//   of the same TU. The catalog label CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: CMemoryAllocator::DeallocateAligned 0x009F2B20 (static __fastcall, landed), free 0x00BFEA14,
//   CEngineComponent::~CEngineComponent 0x00B59730, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Layout: engine/CEngineLightingManager.h (retail offsets, flattened): ThemeChannels +0x3c, LocalLightList +0xac,
//   ShadowedSpotLightList +0xb8, SIMDVarBlock +0x4690 (0x46d0 bytes); modelled locally to carry the two real base classes
//   and the STLport-backed CArrays the destructor releases.
// Shape: user destructor (both vtable stores first) whose body frees and clears SIMDVarBlock; the three CArrays of
//   trivially destroyed elements free their storage in reverse member order; then the second base's out-of-line
//   destructor, and the first base chain's compiler-generated destructors collapse to the CBaseClassNonCopyable tail jump.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
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
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp> inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector (12 bytes)

#include "engine/CBaseClassNonCopyable.h"               // ~CBaseClassNonCopyable 0x0099A300

class CMemoryAllocator {
public:
    static void __fastcall DeallocateAligned(void* p);  // 0x009F2B20
};

class CEngineScene;
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable { };
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {   // Ego_r.pdb, 0xC bytes
public:
    CArray<CEngineScene*>* SceneList;                   // +0x04
    bool                   Enable;                      // +0x08
};
class CEngineComponent {                                // engine/CEngineComponent.h offsets, 0xC bytes
public:
    virtual ~CEngineComponent();                        // 0x00B59730
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
};

class C3DVector { public: float X; float Y; float Z; };
class C4DVector { public: float X; float Y; float Z; float W; };
class CRGBFloatColour { public: float R; float G; float B; float A; };
class CShaderLightAttenuation { public: float Attenuation0; float Attenuation1; float Attenuation2; float AttenuationBase; };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0; unsigned char ChannelIndex1; unsigned char ChannelBlendFactor; unsigned char Pad; };
class CEngineLocalLightElement {                        // Ego_r.pdb, 0x34 bytes
public:
    C3DVector       WorldPos;
    float           Radius;
    float           RadiusSquared;
    float           SpotRadius;
    CRGBFloatColour Colour;
    float           AttenuationDistSquared;
    float           AttenuationConstant;
    unsigned long   ChannelMask;
};
class CEngineLocalLightArray {                          // Ego_r.pdb, 0x1A4 bytes
public:
    unsigned long            LightCount;
    CEngineLocalLightElement Lights[8];
};
struct CLightingSIMDVarBlock;
enum ELightingManagerShadowMode { };

class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {
public:
    struct CThemeChannel {                              // Ego_r.pdb, 0x70 bytes
        CRGBFloatColour DiffuseColour;
        CRGBFloatColour AmbientColour;
        CRGBFloatColour BacklightColour;
        CRGBFloatColour ReflectionColour;
        CRGBFloatColour FogColour;
        float           FogStartZ;
        float           FogEndZ;
        float           ShadowFactor;
        float           FadedShadowFactor;
        C4DVector       FogTransform;
    };
    struct CManagerLocalLight;
    struct CManagerShadowedSpotLight;
    class CLocalLightElementLink;
    CEngineLightingManager();
    virtual ~CEngineLightingManager();
    C3DVector                          SunVector;                       // +0x18
    C3DVector                          MoonVector;                      // +0x24
    C3DVector                          DiffuseVector;                   // +0x30
    CArray<CThemeChannel>              ThemeChannels;                   // +0x3c
    CRGBFloatColour                    PrimitiveDiffuseColour;          // +0x48
    CRGBFloatColour                    PrimitiveAmbientColour;          // +0x58
    CRGBFloatColour                    PrimitiveBacklightColour;        // +0x68
    CRGBFloatColour                    PrimitiveFogColour;              // +0x78
    float                              PrimitiveFogStartZ;              // +0x88
    float                              PrimitiveFogEndZ;                // +0x8c
    CShaderLightAttenuation            LightAttenuation;                // +0x90
    long                               ShaderPointLightsEnabledCount;   // +0xa0
    bool                               DrawLightRadius;                 // +0xa4
    bool                               DrawSpotLightCone;               // +0xa5
    bool                               EnableLightCache;                // +0xa6
    CEngineLocalLightArray*            CurrentShaderLocalLights;        // +0xa8
    CArray<CManagerLocalLight*>        LocalLightList;                  // +0xac
    CArray<CManagerShadowedSpotLight*> ShadowedSpotLightList;           // +0xb8
    float                              LookupGridWorldX;                // +0xc4
    float                              LookupGridWorldY;                // +0xc8
    bool                               LocalLightsEnabled;              // +0xcc
    bool                               ShadowedSpotLightsEnabled;       // +0xcd
    bool                               FadedShadowDiffuseFactorEnabled; // +0xce
    CRGBFloatColour                    MistEffectColour;                // +0xd0
    CEnginePrimitiveThemeChannelDesc   CurrentThemeChannel;             // +0xe0
    unsigned long                      StateUpdateFlags;                // +0xe4
    long                               SelfIlluminationAmbientPercentage;   // +0xe8
    CLocalLightElementLink*            LightLookupGrid[16][16];         // +0xec
    CEngineLocalLightArray*            SmallObjectLookupGrid[64][64];   // +0x4ec
    CEngineLocalLightArray             EmptyLightArray;                 // +0x44ec
    CLightingSIMDVarBlock*             SIMDVarBlock;                    // +0x4690
    ELightingManagerShadowMode         ShadowMode;                      // +0x4694
    unsigned char                      CurrentShadowedSpotLightParams[0x38];   // +0x4698 CEngineLightingManagerSpotlightParams (byte-packed in Ego_r)
};

CEngineLightingManager::~CEngineLightingManager()
{
    CMemoryAllocator::DeallocateAligned(SIMDVarBlock);
    SIMDVarBlock = 0;
}