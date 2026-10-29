// Behaviour test for 0x00BDBB70 CEngineVSConstantLayoutBasic::CEngineVSConstantLayoutBasic(): same model as the source
// (STLport 4.6 vector layer renamed to namespace stlp), with the out-of-line callees defined here.
#include <cstdio>
#include <cstdlib>
#include <new>
class CCharString { public: int n; };


class CVertexShaderConstant {                              // Ego_r.pdb, 16 bytes
public:
    float X;                                               // +0x00
    float Y;                                               // +0x04
    float Z;                                               // +0x08
    float W;                                               // +0x0c
    CVertexShaderConstant(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};

namespace stlp {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
_OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result,
                                                        const __false_type&)                     // 0x00BDB670
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)  // 0x00BDB6D0
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);                  // 0x00BDB9F0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};

// STLport 4.6 vector.c operator= (landed 0x00BDB9F0): its body sits in this TU (noinline), which lets VC7.1 see that
// passing the local preset array to it does not capture the array (the array stays in registers up to the call).
template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const _Tp*)__x._M_start + 0, (const _Tp*)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const _Tp*)__x._M_start, (const _Tp*)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const _Tp*)__x._M_start + size(), (const _Tp*)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}
}   // namespace stlp

template <class T> class CArray : public stlp::vector<T> {  // retail CArray<T> = the 12-byte STLport vector
public:
    void push_back(const T& x) { stlp::vector<T>::push_back(x); }
};

class CBaseClass {
public:
    CBaseClass();                                          // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CShaderConstantRange : public CBaseClass {           // vtable 0x0129A04C
public:
    long Offset;                                           // +0x04 first register
    unsigned long Count;                                   // +0x08 register count
    CShaderConstantRange(long offset, unsigned long count) : Offset(offset), Count(count) { }
    CShaderConstantRange() : Offset(-1), Count(0) { }
};

class CVertexShaderConstantLayout : public CBaseClass {    // vtable 0x0129A06C, retail 0x14c bytes
public:
    CShaderConstantRange Reserved;                         // +0x04
    CShaderConstantRange User;                             // +0x10
    CShaderConstantRange Compression;                      // +0x1c
    CShaderConstantRange Ambient;                          // +0x28
    CShaderConstantRange FogColour;                        // +0x34
    CShaderConstantRange CameraPos;                        // +0x40
    unsigned long LightSize;                               // +0x4c
    CShaderConstantRange LightArray;                       // +0x50
    CShaderConstantRange LightGlobals;                     // +0x5c
    CShaderConstantRange LightAttenuations;                // +0x68
    CShaderConstantRange CombinedProjectionMatrix;         // +0x74
    CShaderConstantRange FogTransform;                     // +0x80
    CShaderConstantRange WorldMatrix;                      // +0x8c
    CShaderConstantRange SphereMapMatrix;                  // +0x98
    CShaderConstantRange TextureTransform[4];              // +0xa4
    CShaderConstantRange BoneMatrices;                     // +0xd4
    CShaderConstantRange ShadowTextureTransform;           // +0xe0
    CShaderConstantRange ShadowFadeTransform;              // +0xec
    CShaderConstantRange ShadowFadeFactors;                // +0xf8
    CShaderConstantRange ShadowedSpotlightFalloffTransform;  // +0x104
    CShaderConstantRange ShadowedSpotlightAttenuation;     // +0x110
    CShaderConstantRange ShadowedSpotlightColour;          // +0x11c
    CShaderConstantRange ScreenSpaceTransformScale;        // +0x128
    CShaderConstantRange ScreenSpaceTransformOffset;       // +0x134
    CArray<CVertexShaderConstant> ReservedPresets;         // +0x140
    CVertexShaderConstantLayout();                         // 0x0098D490
    virtual ~CVertexShaderConstantLayout();
    virtual CCharString PeekName() const;
    void SetReserved(const CShaderConstantRange& range) { Reserved = range; }
    void SetUser(const CShaderConstantRange& range) { User = range; }
    void SetAmbient(const CShaderConstantRange& range) { Ambient = range; }
    void SetFogColour(const CShaderConstantRange& range) { FogColour = range; }
    void SetCameraPos(const CShaderConstantRange& range) { CameraPos = range; }
    void SetCombinedProjectionMatrix(const CShaderConstantRange& range) { CombinedProjectionMatrix = range; }
    void SetFogTransform(const CShaderConstantRange& range) { FogTransform = range; }
    void SetSphereMapMatrix(const CShaderConstantRange& range) { SphereMapMatrix = range; }
    void SetShadowTextureTransform(const CShaderConstantRange& range) { ShadowTextureTransform = range; }
    void SetShadowFadeTransform(const CShaderConstantRange& range) { ShadowFadeTransform = range; }
    void SetShadowFadeFactors(const CShaderConstantRange& range) { ShadowFadeFactors = range; }
    void SetReservedPresets(const CArray<CVertexShaderConstant>& presets) { ReservedPresets = presets; }
};

class CEngineVSConstantLayoutBasic : public CVertexShaderConstantLayout {   // vtable 0x012A7D40
public:
    CEngineVSConstantLayoutBasic();
    virtual CCharString PeekName() const;                  // 0x00BDBF00
};

CEngineVSConstantLayoutBasic::CEngineVSConstantLayoutBasic()
{
    CArray<CVertexShaderConstant> presets;
    presets.push_back(CVertexShaderConstant(0.0f, 1.0f, 2.0f, 0.5f));
    presets.push_back(CVertexShaderConstant(256.0f, 256.0f, 256.0f, 256.0f));
    SetReserved(CShaderConstantRange(0, presets.size()));
    SetReservedPresets(presets);
    SetAmbient(CShaderConstantRange(3, 1));
    SetFogColour(CShaderConstantRange(0x12, 1));
    SetCameraPos(CShaderConstantRange(4, 1));
    SetCombinedProjectionMatrix(CShaderConstantRange(5, 4));
    SetSphereMapMatrix(CShaderConstantRange(9, 2));
    SetFogTransform(CShaderConstantRange(2, 1));
    SetShadowTextureTransform(CShaderConstantRange(0xb, 4));
    SetShadowFadeTransform(CShaderConstantRange(0xf, 1));
    SetShadowFadeFactors(CShaderConstantRange(0x10, 1));
    SetUser(CShaderConstantRange(0x13, 0x4d));
}

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CVertexShaderConstantLayout::CVertexShaderConstantLayout()
    : Reserved(-1, 0), User(-1, 0), Compression(-1, 0), Ambient(-1, 0), FogColour(-1, 0), CameraPos(-1, 0), LightSize(0),
      LightArray(-1, 0), LightGlobals(-1, 0), LightAttenuations(-1, 0), CombinedProjectionMatrix(-1, 0),
      FogTransform(-1, 0), WorldMatrix(-1, 0), SphereMapMatrix(-1, 0), BoneMatrices(-1, 0),
      ShadowTextureTransform(-1, 0), ShadowFadeTransform(-1, 0), ShadowFadeFactors(-1, 0),
      ShadowedSpotlightFalloffTransform(-1, 0), ShadowedSpotlightAttenuation(-1, 0), ShadowedSpotlightColour(-1, 0),
      ScreenSpaceTransformScale(-1, 0), ScreenSpaceTransformOffset(-1, 0) { }
CVertexShaderConstantLayout::~CVertexShaderConstantLayout() { }
CCharString CVertexShaderConstantLayout::PeekName() const { CCharString c; c.n = 0; return c; }
CCharString CEngineVSConstantLayoutBasic::PeekName() const { CCharString c; c.n = 1; return c; }

static int Range(const CShaderConstantRange& r, long offset, unsigned long count) { return r.Offset == offset && r.Count == count; }
static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    CEngineVSConstantLayoutBasic* layout = new CEngineVSConstantLayoutBasic;
    int ok = 1;
    ok = ok && layout->ReservedPresets.size() == 2;
    if (ok) {
        const CVertexShaderConstant& a = layout->ReservedPresets._M_start[0];
        const CVertexShaderConstant& b = layout->ReservedPresets._M_start[1];
        ok = Near(a.X, 0.0f) && Near(a.Y, 1.0f) && Near(a.Z, 2.0f) && Near(a.W, 0.5f)
          && Near(b.X, 256.0f) && Near(b.Y, 256.0f) && Near(b.Z, 256.0f) && Near(b.W, 256.0f);
    }
    ok = ok && Range(layout->Reserved, 0, 2) && Range(layout->Ambient, 3, 1) && Range(layout->FogColour, 0x12, 1)
         && Range(layout->CameraPos, 4, 1) && Range(layout->CombinedProjectionMatrix, 5, 4)
         && Range(layout->SphereMapMatrix, 9, 2) && Range(layout->FogTransform, 2, 1)
         && Range(layout->ShadowTextureTransform, 0xb, 4) && Range(layout->ShadowFadeTransform, 0xf, 1)
         && Range(layout->ShadowFadeFactors, 0x10, 1) && Range(layout->User, 0x13, 0x4d)
         && Range(layout->LightArray, -1, 0) && Range(layout->Compression, -1, 0) && layout->LightSize == 0;
    ok = ok && layout->PeekName().n == 1;
    delete layout;
    if (!ok) { std::printf("00bdbb70_TEST FAIL\n"); return 1; }
    std::printf("00bdbb70_TEST PASS\n");
    return 0;
}