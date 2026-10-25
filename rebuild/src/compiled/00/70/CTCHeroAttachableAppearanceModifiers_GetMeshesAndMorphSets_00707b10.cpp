// CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets(vector<pair<CMeshInfo, CEngineTextureMorphSet> >& meshesAndMorphSets) const -- one (mesh info, texture morph set) pair per modifier graphic: the replacement def's texture swaps become base-type blends
// Leaf: ?GetMeshesAndMorphSets@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00707B10, 526 bytes, __fastcall, `ret 4`.
// PDB: ?GetMeshesAndMorphSets@CTCHeroAttachableAppearanceModifiers@@QBEXAAV?$vector@U?$pair@VCMeshInfo@NGraphicAppearance@@
//   VCEngineTextureMorphSet@@@std@@...@std@@@Z (Ego_r 0x0055D1C0); catalog "RandomAppearance_ProcessMorphList" was
//   descriptive. The landed push_back 0x00709E60 / __uninitialized_copy 0x00709C90 already name this as their user.
// Callees: CEngineTextureMorphSet::AddTextureMorph 0x006CA950, malloc 0x00BFEA0E, free 0x00BFEA14,
//   __uninitialized_copy<CEngineTextureMorph> 0x005BBC93 (/Os-module COMDAT; catalog "_Uninit_copy<CArray<CBodyPartMesh>>"),
//   vector<CEngineTextureMorph>(const vector&) 0x005BBDF1 (/Os-module COMDAT; catalog "CExpressionSet::Add"),
//   vector<pair<CMeshInfo, CEngineTextureMorphSet> >::_M_insert_overflow 0x00709D60.
// Layout (retail STLport, 12-byte vectors): Graphics +0x3C (PDB +0x48) of 12-byte CModifierGraphic {CDefPointer
//   PTextureReplacement, BankIndex, RenderSizeX}; CTextureReplacementDef Entries +0x28 of {OriginalTexture, NewTexture};
//   CEngineTextureMorph 20 bytes {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends}.
// Shape: per graphic a local morph set, one AddTextureMorph per replacement entry (blend type 3, diffuse 0,0,0,255 built
//   in place by CRGBColour(r, g, b, a): retail's +2, +1, +0, +3 byte stores), a CMeshInfo with PrimaryMesh set, then
//   push_back(make_pair(meshInfo, morphSet)). Natural VC7.1 inlining gives retail's split: the make_pair copy of the
//   morph vector expands (n / 20, malloc, the 0x005BBC93 call) while the copy inside the inline push_back's _Construct
//   stays a call to 0x005BBDF1; the morph range destroy expands. The STLport helpers this TU instantiated
//   (__uninitialized_copy / fill_n, _M_insert_overflow) are same-TU noinline bodies.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_3 = 3 };
class CCompositeBlendDesc {
public:
    unsigned long       TextureIndex;               // +0x00
    ECompositeBlendType BlendType;                  // +0x04
    CRGBColour          DiffuseColour;              // +0x08
    CRGBColour          SpecularColour;             // +0x0C
    CCompositeBlendDesc(unsigned long texture, ECompositeBlendType type, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : TextureIndex(texture), BlendType(type), DiffuseColour(r, g, b, a), SpecularColour(0, 0, 0, 0) {}
};

namespace NGraphicAppearance {
class CMeshInfo {                                   // Ego_r.pdb, 12 bytes
public:
    long  Index;                                    // +0x00
    float SizeX;                                    // +0x04
    unsigned char PrimaryMesh : 1;                  // +0x08
};
}

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

#pragma auto_inline(off)
// Same-TU template bodies (the linker kept 0x005BBC93 for the morph records, 0x00709C90 / 0x00709CE0 for the pairs).
template <class _Tp> __declspec(noinline) _Tp* __fastcall __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(__result, *__first);
    return __result;
}
template <class _Tp> __declspec(noinline) _Tp* __fastcall __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __false_type&)
{
    for ( ; __n > 0; --__n, ++__first)
        _Construct(__first, __x);
    return __first;
}
#pragma auto_inline(on)
template <class _Tp> inline void __destroy_aux(_Tp* __first, _Tp* __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        __first->~_Tp();
}

template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    _Tp* begin() const { return this->_M_start; }
    _Tp* end() const { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00709D60
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1, true);
    }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CEngineTextureMorph {                         // retail 20 bytes
public:
    long OriginalTextureIndex;                      // +0x00
    long MorphID;                                   // +0x04
    std::vector<CCompositeBlendDesc> Blends;        // +0x08
};
class CEngineTextureMorphSet {                      // Ego_r.pdb: Morphs +0 (retail 12 bytes)
public:
    CArray<CEngineTextureMorph> Morphs;
    void AddTextureMorph(long textureIndex, const CCompositeBlendDesc& blend);   // 0x006CA950
};

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long RefCount;                                  // +0x04
};
class CTextureReplacementDef : public CDefBase {
public:
    class CEntry {                                  // 8 bytes
    public:
        long OriginalTexture;                       // +0x00
        long NewTexture;                            // +0x04
    };
    unsigned char _pad08[0x28 - 0x08];
    CArray<CEntry> Entries;                         // +0x28
};
template <class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

class CTCHeroAttachableAppearanceModifiers {
public:
    class CModifierGraphic {                        // 12 bytes
    public:
        CDefPointer<const CTextureReplacementDef> PTextureReplacement;   // +0x00
        long  BankIndex;                            // +0x04
        float RenderSizeX;                          // +0x08
    };
    virtual void v00();
    void* ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x3c - 0x08];
    std::vector<CModifierGraphic> Graphics;         // +0x3C (retail; PDB +0x48)
    void GetMeshesAndMorphSets(std::vector<std::pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >& meshesAndMorphSets) const;
};

void CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets(std::vector<std::pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >& meshesAndMorphSets) const
{
    for (const CModifierGraphic* it = Graphics.begin(); it != Graphics.end(); ++it) {
        CEngineTextureMorphSet morphSet;
        if (it->PTextureReplacement) {
            for (long i = 0; i < (long)it->PTextureReplacement->Entries.size(); ++i) {
                const CTextureReplacementDef::CEntry& entry = it->PTextureReplacement->Entries[i];
                morphSet.AddTextureMorph(entry.OriginalTexture, CCompositeBlendDesc(entry.NewTexture, COMPOSITE_BLEND_TYPE_3, 0, 0, 0, 255));
            }
        }
        NGraphicAppearance::CMeshInfo meshInfo;
        meshInfo.Index = it->BankIndex;
        meshInfo.SizeX = it->RenderSizeX;
        meshInfo.PrimaryMesh = true;
        meshesAndMorphSets.push_back(std::make_pair(meshInfo, morphSet));
    }
}