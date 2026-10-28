// CMeshDataBank::~CMeshDataBank() -- close the meshes and detach the animation manager, then the members die in reverse (state block, counted pointers released, arrays destroyed and freed, the lip-sync map cleared and its header freed, the four resource banks, the loaders deleted) and the CBankFileAsync base runs
// Leaf: ??1CMeshDataBank@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A27350, 886 bytes, plain `ret`. Stores vtable 0x0129CE94 = CMeshDataBank (rtti-vtables).
// Identity: Ego_r.pdb `??1CMeshDataBank@@UAE@XZ` (virtual: the PDB carries ??_G/??_E for the class). Catalog label
//   "CGameWorld_Destructor" is wrong (the vtable store and every member offset are CMeshDataBank's, landed ctor 0x00A27030).
// Callees: CMeshDataBank::CloseMeshes 0x00A26900 (landed), C3DAnimationManager::Uninitialise 0x00A9D740 (MeshBank = 0),
//   ~CMeshBankStateBlock 0x00A2D710, operator delete 0x00BFE9BC, free 0x00BFEA14, the STLport map's _Rb_tree::_M_erase
//   0x00A2DC20, ~CCharString 0x0099EAE0, __destroy_aux<CCountedPointer<CMeshDataBankEntry>*> 0x00A2BD20 (__fastcall,
//   out of line by VC7.1's own inline choice), ~CResourceBank 0x009FC690, ~CBankFileAsync 0x009D6490 (stores vtable
//   0x0129B8D4 = CBankFileAsync; RTTI CMeshDataBank : CBankFileAsync : CBankFile).
// Layout: retail offsets from the landed ctor (CBankFile 0x164, 12-byte STLport vectors and map); member names and order
//   from struct_layouts_egor (Ego_r.pdb CMeshDataBank).
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void __cdecl operator delete(void* __p);                 // 0x00BFE9BC

class CCPPointerInfo { public:
    long  RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);           // +4
    void* Data;                                          // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};

template <class T> class scoped_ptr { public:
    T* ptr;
    ~scoped_ptr() { delete ptr; }
};

class CCharString { public:
    class CCharStringData* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <> struct __type_traits<unsigned long> { typedef __true_type has_trivial_destructor; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, (_Tp*)0); }

template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const; };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_node<_Value>* _Link_type;
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
private:
    void _M_erase(_Link_type __x);                       // 0x00A2DC20
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

class CBankFile { public:
    virtual ~CBankFile();
    unsigned char _bankFile[0x160];                      // +0x004
};
class CBankFileAsync : public CBankFile { public:
    virtual ~CBankFileAsync();                           // 0x009D6490
};
class CResourceBank { public:
    virtual ~CResourceBank();                            // 0x009FC690
    unsigned char _body[0x78];
};
class CMeshBankStateBlock { public:
    virtual ~CMeshBankStateBlock();                      // 0x00A2D710
    unsigned char _body[0x38];
};
class C3DMeshFileXLoader { public: virtual ~C3DMeshFileXLoader(); };
class C3DAnimFileXLoader { public: virtual ~C3DAnimFileXLoader(); };
class CMeshDataBank;
class C3DAnimationManager { public:
    void Uninitialise();                                 // 0x00A9D740
};
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;
class CPushBufferBuildInfo;
class CGraphicDataBank;
class CMeshDataBankEntry;
class C3DQuadricBasedMeshOptimiser;
class C3DMeshStripifier;
class C3DMeshInfo { public: virtual ~C3DMeshInfo(); unsigned char _body[0x5c]; };
class CPhysicsMeshInfo { public: virtual ~CPhysicsMeshInfo(); unsigned char _body[0xc]; };
class C3DAnimationInfo { public: virtual ~C3DAnimationInfo(); unsigned char _body[0x24]; };

class CMeshDataBank : public CBankFileAsync { public:
    class CLipSyncEntry;
    virtual ~CMeshDataBank();
private:
    void CloseMeshes();                                  // 0x00A26900
public:
    scoped_ptr<C3DMeshFileXLoader>               MeshLoader;              // +0x164
    scoped_ptr<C3DAnimFileXLoader>               AnimLoader;              // +0x168
    CCharString                                  Filename;                // +0x16c
    CResourceBank                                MeshResourceBank;        // +0x170
    CResourceBank                                MeshStatsResourceBank;   // +0x1ec
    CResourceBank                                AnimationResourceBank;   // +0x268
    CResourceBank                                PhysicsMeshResourceBank; // +0x2e4
    CCountedPointer<CMovableResourceMemoryPool>  AnimationMemoryPool;     // +0x360
    CCountedPointer<CMovableResourceMemoryPool>  ClothMemoryPool;         // +0x368
    CCountedPointer<CMovableResourceMemoryPool>  MeshDataMemoryPool;      // +0x370
    CCountedPointer<CXboxBufferMemoryPool>       XboxMemoryPool;          // +0x378
    CArray<CMeshDataBankEntry*>                  MeshesAndAnimations;     // +0x380
    CArray<C3DMeshInfo>                          MeshInfoBuffer;          // +0x38c
    CArray<CPhysicsMeshInfo>                     PhysicsMeshInfoBuffer;   // +0x398
    CArray<C3DAnimationInfo>                     AnimationInfoBuffer;     // +0x3a4
    CArray<CCountedPointer<CMeshDataBankEntry> > DynamicInfoBuffer;       // +0x3b0
    CCharString                                  AnimationEnumType;       // +0x3bc
    CCountedPointer<C3DAnimationManager>         AnimationManager;        // +0x3c0
    std::map<CCharString, CArray<CLipSyncEntry> > LipSyncAnimations;      // +0x3c8
    CGraphicDataBank*                            TextureBank;             // +0x3d4
    bool                                         DP3BumpEnable;           // +0x3d8
    CArray<unsigned long>                        GroupBlockSizes;         // +0x3dc
    unsigned long                                MaxBonesPerVertex;       // +0x3e8
    unsigned long                                MaxParticleMeshFacesToRepeat; // +0x3ec
    CCountedPointer<C3DQuadricBasedMeshOptimiser> MeshOptimiser;          // +0x3f0
    CCountedPointer<C3DMeshStripifier>           MeshStripifier;          // +0x3f8
    float                                        MaximumLODErrors[4];     // +0x400
    float                                        MiminumLODReductionPercent; // +0x410
    unsigned long                                MiminumLODReductionFaces;   // +0x414
    float                                        MinimumAnimationMatch;   // +0x418
    CCountedPointer<CPushBufferBuildInfo>        PushBufferBuildInfo;     // +0x41c
    long                                         UnloadableMeshMemory;    // +0x424
    long                                         UnloadMeshFrameDelay;    // +0x428
    CMeshBankStateBlock                          StateBlock;              // +0x42c
};

CMeshDataBank::~CMeshDataBank()
{
    CloseMeshes();
    AnimationManager->Uninitialise();
}