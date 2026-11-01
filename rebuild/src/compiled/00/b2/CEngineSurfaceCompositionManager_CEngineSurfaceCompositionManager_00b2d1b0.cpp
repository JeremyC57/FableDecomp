// CEngineSurfaceCompositionManager::~CEngineSurfaceCompositionManager() -- for every cached composite surface release its graphic frame and managed texture and unhook it from the resource bank; then destroy the screen-copy shader, texture manager, morph map, file names, the three graphic banks and the cache list, ~CEngineComponent and ~CResourceBank
// Leaf: ??1CEngineSurfaceCompositionManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2D1B0, 374 bytes, thiscall, tail jump to ~CResourceBank 0x009FC690; sole caller the
//   scalar deleting dtor 0x00B2F6E0 (slot 0 of the class vtable 0x012A1238).
// Identity (proven): the body's first stores are vtable 0x012A1238 at +0 (rtti-vtables.tsv / RTTI COL
//   .?AVCEngineSurfaceCompositionManager@@ : CResourceBank @0, CEngineComponent @0x7C (ghidra_out/rtti_bases.tsv); slot 0 =
//   ??_G 0x00B2F6E0, the only caller) and 0x012A1200 at +0x7C (the CEngineComponent-in-this-class vtable); the ctor 0x00B2CB40
//   stores the same vtable. Ego_r.pdb ??1CEngineSurfaceCompositionManager@@UAE@XZ (0x009CE560) before the ctor (0x009CE6D0).
//   The own vtable stores = a user-written destructor. Catalog label right.
// Layout: engine/CEngineSurfaceCompositionManager.h retail-container offsets (flattened byte arrays, no bases or methods),
//   restated with the member types (NOTES.md "Header debt"): CResourceBank 0x7C, CEngineComponent +0x7C (0xC),
//   CachedSurfaces +0x88 (STLport list), IntermediateBankFile +0x8c / CompletedBankFile +0x398 / CustomTextureBankFile +0x6a4
//   (CGraphicDataBank 0x30C, opaque), the three CWideString file names +0x9b0..+0x9b8, GeneratedMorphs +0x9bc (STLport map),
//   DiskCacheTolerance +0x9c8, MaxPreloadTime +0x9cc, CurrentFrame +0x9d0, TextureManager +0x9d4 (0x5D4, opaque),
//   the four CPixelFormat +0xfa8..+0xfb4, ScreenCopyVertexShader +0xfb8, the two pair<long,long>[4][4] handle tables +0xfc0 /
//   +0x1040, the tail scalars +0x10c0..+0x10d8 (0x10DC).
// Layout: CCompositeSurfaceCache as the landed ctor 0x00B2C400 (retail: CEngineTextureMorph 0x14 with the STLport vector):
//   GraphicFrame +0x4c, ManagedTexture +0x50; CResource at +0 (CResourceBank::RemoveResource takes the cache as CResource&).
// Inline: CGraphicFrame::UnInitialise (Ego_r name; out-of-line copies 0x0041C150 / 0x00A00B90 / 0x00B2EEC0) with the inline
//   ~CGraphicFrameData (IsInitialised bit 4 of +0x1a -> Uninitialise 0x009FCBA0, PTexture CCountedPointer release, operator delete).
// Callees: CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC, CManagedTexture::Release 0x00A69AD0,
//   CResourceBank::RemoveResource(CResource&) 0x009FC3C0 (`add ecx,4 ; jmp CResourceList::RemoveResource 0x009FC020`),
//   ~CTextureManager 0x00A6A440, map _Rb_tree::_M_erase 0x00B2EF20 + free 0x00BFEA14, ~CWideString 0x0099B510,
//   ~CGraphicDataBank 0x009FEBC0, _List_base<CCountedPointer<CCompositeSurfaceCache> >::clear 0x00B2EA90, ~CEngineComponent
//   0x00B59730, ~CResourceBank 0x009FC690.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_pair.h, stl/_function_base.h, stl/_list.h, stl/_tree.h (the members this row needs; the
// malloc-backed allocator of this build: deallocate = free 0x00BFEA14)
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator<_Tp>& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp>
inline bool operator!=(const _List_iterator<_Tp>& __x, const _List_iterator<_Tp>& __y) { return __x._M_node != __y._M_node; }
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    ~_List_base() {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                                        // 0x00B2EA90 (_list.c, out of line)
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                       // 0x00B2EF20 (_tree.c)
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
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
};
template <class _Tp, class _Alloc>
class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
};
}
// Shader models as the landed 0x00BABE60 (RTTI: CVertexShader 0x0129B3D0 : CShaderBase 0x0129A030, 8 bytes).
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    void*         VertexDeclaration;               // +0x04
    void*         VertexShader;                    // +0x08
    const void*   FormatDecl;                      // +0x0c
    long          ReferenceCount;                  // +0x10
    long RemoveReference() { return --ReferenceCount; }
};
class CShaderBase { public:                        // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public: // vtable 0x0129B3D0
    CVertexShader();                               // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CVertexShaderData* Data;                       // +0x04
};
// Resource bank models as the landed ~CResourceBank 0x009FC690 (CResourceList / CFailedAllocationHandler local models).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"
class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler() { }                              // slot 0
    virtual bool HandleFailedAllocation(unsigned long size) = 0;         // slot 1 (pure)
};
class CResourceList : public CFailedAllocationHandler {                  // vtable 0x0129C808, 0x3C bytes
public:
    virtual ~CResourceList();                                            // 0x009FC5C0
    virtual bool HandleFailedAllocation(unsigned long size);             // slot 1  0x009FC540
    CResource     Head;                                                  // +0x04
    unsigned long ResourceCount;                                         // +0x20
    long          AllocatedMemory;                                       // +0x24
    long          MaximumMemory;                                         // +0x28
    unsigned long CurrentFrame;                                          // +0x2C
    unsigned long DebugStatsFrame;                                       // +0x30
    unsigned long UnloadDelay;                                           // +0x34
    unsigned long UnloadedThisFrame;                                     // +0x38
};
class CResourceBank : public CBaseClassNonCopyable {                     // vtable 0x0129C814, 0x7C bytes
public:
    CResourceBank();                                                     // 0x009FC5F0
    virtual ~CResourceBank();                                            // 0x009FC690
    void RemoveResource(CResource& resource);                            // 0x009FC3C0
    CResourceList ResourceList;                                          // +0x04
    CResourceList LoadingQueue;                                          // +0x40
};
class CEngineComponent {                                // engine/CEngineComponent.h offsets, 0xC bytes (as landed 0x00B47B50)
public:
    virtual ~CEngineComponent();                        // 0x00B59730
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
};
class CTexture;
class CGraphicFrameData {                               // Ego_r.pdb 0x1B (non-virtual dtor: ??_GCGraphicFrameData@@QAEPAXI@Z)
public:
    ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    void Uninitialise();                                // 0x009FCBA0
    unsigned long             ReferenceCount;           // +0x00
    void*                     Data;                     // +0x04
    CCountedPointer<CTexture> PTexture;                 // +0x08
    unsigned short            TLX;                      // +0x10
    unsigned short            TLY;                      // +0x12
    unsigned short            Width;                    // +0x14
    unsigned short            Height;                   // +0x16
    unsigned short            Pitch;                    // +0x18
    unsigned char             Alpha : 1;                // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
};
class CGraphicFrame {                                   // engine/CGraphicFrame.h: one pointer
public:
    void UnInitialise()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            FrameData = 0;
        }
    }
    CGraphicFrameData* FrameData;                       // +0x00
};
template <class T> class CIVCountedPointer {
public:
    T* Data;
};
class CManagedTextureData;
class CManagedTexture : public CBaseClass {             // vtable 0x0129C880 (as landed 0x00B2C400)
public:
    virtual ~CManagedTexture();
    bool Release();                                     // 0x00A69AD0
    CIVCountedPointer<CManagedTextureData> Data;        // +0x04
};
class CCompositeBlendDesc;
class CEngineTextureMorph {
public:
    long                             OriginalTextureIndex;   // +0x00
    long                             MorphID;                // +0x04
    std::vector<CCompositeBlendDesc> Blends;                 // +0x08
};
class CEngineSurfaceCompositionManager;
class CCompositeSurfaceCache : public CResource {
public:
    CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner);
    virtual ~CCompositeSurfaceCache();
    CEngineSurfaceCompositionManager* Owner;                 // +0x1c
    CEngineTextureMorph               CurrentMorph;          // +0x20
    CEngineTextureMorph               CachedMorph;           // +0x34
    unsigned long                     MipmapLevel;           // +0x48
    CGraphicFrame                     GraphicFrame;          // +0x4c
    CManagedTexture                   ManagedTexture;        // +0x50
    unsigned long                     BankFileIndex;         // +0x58
    unsigned long                     LastUsedFrame;         // +0x5c
    double                            TimeStamp;             // +0x60
};
class CGraphicDataBank {                                // RTTI : CBankFileAsync @0, CResourceBank @0x164; 0x30C (opaque here)
public:
    virtual ~CGraphicDataBank();                        // 0x009FEBC0
    unsigned char _body[0x308];
};
// Layout: engine/CTextureManager.h is a flattened pack(1) record (no vptr slot type, no dtor); retail's class has the RTTI base
//   CBaseClassNonCopyable and the out-of-line virtual ??1CTextureManager@@UAE@XZ, so it is restated here as an opaque 0x5D4 body.
class CTextureManager {                                 // RTTI : CBaseClassNonCopyable; 0x5D4 (opaque here)
public:
    virtual ~CTextureManager();                         // 0x00A6A440
    unsigned char _body[0x5d0];
};
class CWideStringData;
class CWideString {
public:
    ~CWideString();                                     // 0x0099B510
    CWideStringData* PStringData;
};
#include "engine/CPixelFormat.h"
class CEngineSurfaceCompositionManager : public CResourceBank, public CEngineComponent {
public:
    CEngineSurfaceCompositionManager();
    virtual ~CEngineSurfaceCompositionManager();
    std::list<CCountedPointer<CCompositeSurfaceCache> >          CachedSurfaces;           // +0x88
    CGraphicDataBank                                             IntermediateBankFile;     // +0x8c
    CGraphicDataBank                                             CompletedBankFile;        // +0x398
    CGraphicDataBank                                             CustomTextureBankFile;    // +0x6a4
    CWideString                                                  IntermediateDiskCacheFileName;   // +0x9b0
    CWideString                                                  CompletedDiskCacheFileName;      // +0x9b4
    CWideString                                                  CustomTextureBankFileName;       // +0x9b8
    std::map<std::pair<long, long>, CEngineTextureMorph>         GeneratedMorphs;          // +0x9bc
    float                                                        DiskCacheTolerance;       // +0x9c8
    float                                                        MaxPreloadTime;           // +0x9cc
    unsigned long                                                CurrentFrame;             // +0x9d0
    CTextureManager                                              TextureManager;           // +0x9d4
    CPixelFormat                                                 OpaquePixelFormat;        // +0xfa8
    CPixelFormat                                                 AlphaPixelFormat;         // +0xfac
    CPixelFormat                                                 CompressedOpaquePixelFormat;   // +0xfb0
    CPixelFormat                                                 CompressedAlphaPixelFormat;    // +0xfb4
    CVertexShader                                                ScreenCopyVertexShader;   // +0xfb8
    std::pair<long, long>                                        OpaqueTextureHandles[4][4];   // +0xfc0
    std::pair<long, long>                                        AlphaTextureHandles[4][4];    // +0x1040
    float                                                        ReuseTolerance;           // +0x10c0
    float                                                        ToleranceFalloffPercent;  // +0x10c4
    long                                                         MaxCompositeTextureSize;  // +0x10c8
    unsigned long                                                MinRenderTargetSize;      // +0x10cc
    unsigned long                                                MinMipMapLevels;          // +0x10d0
    bool                                                         Enable;                   // +0x10d4
    bool                                                         RequiresCopy;             // +0x10d5
    unsigned long                                                CacheSizeBytes;           // +0x10d8
};
FABLE_STATIC_ASSERT(sizeof(CEngineSurfaceCompositionManager) == 0x10DC);
CEngineSurfaceCompositionManager::~CEngineSurfaceCompositionManager()
{
    for (std::list<CCountedPointer<CCompositeSurfaceCache> >::iterator it = CachedSurfaces.begin(); it != CachedSurfaces.end(); ++it) {
        CCompositeSurfaceCache* cache = it->Data;
        cache->GraphicFrame.UnInitialise();
        cache->ManagedTexture.Release();
        RemoveResource(*cache);
    }
}