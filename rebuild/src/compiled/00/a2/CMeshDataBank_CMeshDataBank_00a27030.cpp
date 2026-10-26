// CMeshDataBank::CMeshDataBank() -- CBankFile base, then the members in order (null loaders, filename, four resource banks,
//   null memory pools and info arrays, an empty lip-sync map), then the body: new X-file mesh/anim loaders, a new mesh optimiser
//   and stripifier, the LOD defaults (0, 0.025, 0.04, 0.065; 12.5%, 5 faces; anim match 0.2), no texture bank, group block size 18,
//   3 bones per vertex, DP3 bump off, a new animation manager observed by this bank, zero unload delays on the animation /
//   physics / mesh-stats banks, unloadable memory 0, unload delay 5 frames, 64 particle faces, and the static-map flag set
// Leaf: ??0CMeshDataBank
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A27030, 800 bytes (plain `ret`). Vtable 0x0129CE94 = CMeshDataBank (rtti-vtables).
// Callees: CBankFile::CBankFile 0x009D5F80, CCharString() 0x0099E4B0, CResourceBank() 0x009FC5F0, malloc 0x00BFEA0E (the
//   STLport map header, 0x20 bytes), CMeshBankStateBlock() 0x00A2D650 (manifest "CMeshDataBank_Initialize"), operator new
//   0x00BFEA1A, C3DMeshFileXLoader() 0x00AA5C80 (0xC0), C3DAnimFileXLoader() 0x00AA4710 (0x5C), C3DQuadricBasedMeshOptimiser()
//   0x00AA41C0 (4; manifest "C3DAnimationUtil"), C3DMeshStripifier() 0x00A8DF70 (0x1C; manifest
//   "CAIStateGroup_HoverHornetQueen"), C3DAnimationManager() 0x00AA0F60 (0x4C), CCountedPointer<T>::Reset 0x00A29F50 /
//   0x00A29FC0 / 0x00A29800 (manifest "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", per-type twins),
//   vector<unsigned long>::_M_insert_overflow 0x00464ACE, CObservable::AddExclusiveObserver 0x00A9D730,
//   CBankFile::SetIsStaticMap(bool) 0x009D5230 (manifest "$E2").
// Layout (retail, 0x468 bytes): CBankFile is 0x164 (PDB 0x174) and the PDB's 0x64 bytes before MeshLoader are absent, so the
//   members start at +0x164 (PDB MeshLoader +0x1D8); 12-byte STLport CArrays and a 12-byte map shift the tail further
//   (Filename +0x16C, banks +0x170/+0x1EC/+0x268/+0x2E4, pools +0x360, arrays +0x380, AnimationManager +0x3C0, LipSync map
//   +0x3C8, TextureBank +0x3D4, GroupBlockSizes +0x3DC, MeshOptimiser +0x3F0, LOD floats +0x400, StateBlock +0x42C), so the
//   generated engine/CMeshDataBank.h (PDB offsets) does not fit. CResourceBank::ResourceList.UnloadDelay is bank +0x38.
// Shape: the map's header is allocated in a base (_Rb_tree_base) ctor so the node count is stored after the malloc;
//   scoped_ptr::reset keeps the old pointer, stores, then deletes it; push_back is the inline STLport form with a
//   __true_type user ctor.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() { } };

namespace N_009d5f80 { struct CBankFile {
    virtual void v00();
    unsigned char _bankFile[0x160];                 // +0x004
    CBankFile();                                    // 0x009D5F80
    void SetIsStaticMap(bool isStaticMap);          // 0x009D5230
}; } using N_009d5f80::CBankFile;

class CCharString { public:
    class CCharStringData* PStringData;
    CCharString();                                  // 0x0099E4B0
};

class CResourceList { public:
    unsigned char _pad_0x00[0x34];
    unsigned long UnloadDelay;                      // +0x34
    unsigned long UnloadedThisFrame;                // +0x38
};

class CResourceBank { public:
    void* __vftable;
    CResourceList ResourceList;                     // +0x04
    unsigned char LoadingQueue[0x3c];               // +0x40
    CResourceBank();                                // 0x009FC5F0
    void SetUnloadDelay(unsigned long delay) { ResourceList.UnloadDelay = delay; }
};

template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};

template <class T> class CArray { public:                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CULongArray : CArray<unsigned long> {
    void _M_insert_overflow(unsigned long* position, const unsigned long& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00464ACE
    void push_back(const unsigned long& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) unsigned long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    T* operator->() const { return Data; }
};

class CMeshDataBank;

class C3DMeshFileXLoader { public:
    virtual ~C3DMeshFileXLoader();
    C3DMeshFileXLoader();                           // 0x00AA5C80
    unsigned char _pad[0xbc];
};
class C3DAnimFileXLoader { public:
    virtual ~C3DAnimFileXLoader();
    C3DAnimFileXLoader();                           // 0x00AA4710
    unsigned char _pad[0x58];
};
class C3DQuadricBasedMeshOptimiser { public:
    C3DQuadricBasedMeshOptimiser();                 // 0x00AA41C0
    unsigned char _pad[4];
};
class C3DMeshStripifier { public:
    C3DMeshStripifier();                            // 0x00A8DF70
    unsigned char _pad[0x1c];
};
class C3DAnimationManager { public:
    C3DAnimationManager();                          // 0x00AA0F60
    void AddExclusiveObserver(CMeshDataBank* observer);   // 0x00A9D730 (CObservable base)
    unsigned char _pad[0x4c];
};
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;
class CPushBufferBuildInfo;
class CGraphicDataBank;
class CMeshDataBankEntry;
class C3DMeshInfo;
class CPhysicsMeshInfo;
class C3DAnimationInfo;

struct CMeshOptimiserPointer : CCountedPointer<C3DQuadricBasedMeshOptimiser> {
    void Reset(C3DQuadricBasedMeshOptimiser* item);  // 0x00A29F50
};
struct CMeshStripifierPointer : CCountedPointer<C3DMeshStripifier> {
    void Reset(C3DMeshStripifier* item);            // 0x00A29FC0
};
struct CAnimationManagerPointer : CCountedPointer<C3DAnimationManager> {
    void Reset(C3DAnimationManager* item);          // 0x00A29800
};

template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) { }
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
};

struct CRbNodeBase {
    unsigned char _M_color;
    CRbNodeBase* _M_parent;
    CRbNodeBase* _M_left;
    CRbNodeBase* _M_right;
};
struct CRbNode : CRbNodeBase {
    unsigned char _value[0x10];
};

struct CRbTreeBase {                               // STLport _Rb_tree_base: the header node is allocated here
    _STLP_alloc_proxy<CRbNode> _M_header;
    CRbTreeBase() : _M_header(0) { _M_header._M_data = (CRbNode*)malloc(sizeof(CRbNode)); }
};

struct CLipSyncMap : CRbTreeBase {                 // map<CCharString, CArray<CLipSyncEntry>>: retail 12 bytes
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CLipSyncMap() : _M_node_count(0)
    {
        _M_header._M_data->_M_color = 0;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
    }
};

class CMeshBankStateBlock { public:
    unsigned char _pad[0x3c];
    CMeshBankStateBlock();                          // 0x00A2D650
};

class CMeshDataBank : public CBankFile { public:
    scoped_ptr<C3DMeshFileXLoader>              MeshLoader;              // +0x164
    scoped_ptr<C3DAnimFileXLoader>              AnimLoader;              // +0x168
    CCharString                                 Filename;                // +0x16c
    CResourceBank                               MeshResourceBank;        // +0x170
    CResourceBank                               MeshStatsResourceBank;   // +0x1ec
    CResourceBank                               AnimationResourceBank;   // +0x268
    CResourceBank                               PhysicsMeshResourceBank; // +0x2e4
    CCountedPointer<CMovableResourceMemoryPool> AnimationMemoryPool;     // +0x360
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;         // +0x368
    CCountedPointer<CMovableResourceMemoryPool> MeshDataMemoryPool;      // +0x370
    CCountedPointer<CXboxBufferMemoryPool>      XboxMemoryPool;          // +0x378
    CArray<CMeshDataBankEntry*>                 MeshesAndAnimations;     // +0x380
    CArray<C3DMeshInfo>                         MeshInfoBuffer;          // +0x38c
    CArray<CPhysicsMeshInfo>                    PhysicsMeshInfoBuffer;   // +0x398
    CArray<C3DAnimationInfo>                    AnimationInfoBuffer;     // +0x3a4
    CArray<CCountedPointer<CMeshDataBankEntry> > DynamicInfoBuffer;      // +0x3b0
    CCharString                                 AnimationEnumType;       // +0x3bc
    CAnimationManagerPointer                    AnimationManager;        // +0x3c0
    CLipSyncMap                                 LipSyncAnimations;       // +0x3c8
    CGraphicDataBank*                           TextureBank;             // +0x3d4
    bool                                        DP3BumpEnable;           // +0x3d8
    CULongArray                                 GroupBlockSizes;         // +0x3dc
    unsigned long                               MaxBonesPerVertex;       // +0x3e8
    unsigned long                               MaxParticleMeshFacesToRepeat; // +0x3ec
    CMeshOptimiserPointer                       MeshOptimiser;           // +0x3f0
    CMeshStripifierPointer                      MeshStripifier;          // +0x3f8
    float                                       MaximumLODErrors[4];     // +0x400
    float                                       MiminumLODReductionPercent; // +0x410
    unsigned long                               MiminumLODReductionFaces;   // +0x414
    float                                       MinimumAnimationMatch;   // +0x418
    CCountedPointer<CPushBufferBuildInfo>       PushBufferBuildInfo;     // +0x41c
    long                                        UnloadableMeshMemory;    // +0x424
    long                                        UnloadMeshFrameDelay;    // +0x428
    CMeshBankStateBlock                         StateBlock;              // +0x42c
    virtual void v00();
    CMeshDataBank();
};

CMeshDataBank::CMeshDataBank()
{
    MeshLoader.reset(new C3DMeshFileXLoader);
    AnimLoader.reset(new C3DAnimFileXLoader);
    MeshOptimiser.Reset(new C3DQuadricBasedMeshOptimiser);
    MeshStripifier.Reset(new C3DMeshStripifier);
    MaximumLODErrors[0] = 0.0f;
    MaximumLODErrors[1] = 0.025f;
    MaximumLODErrors[2] = 0.04f;
    MaximumLODErrors[3] = 0.065f;
    MiminumLODReductionPercent = 12.5f;
    MiminumLODReductionFaces = 5;
    MinimumAnimationMatch = 0.2f;
    TextureBank = 0;
    GroupBlockSizes.push_back(18);
    MaxBonesPerVertex = 3;
    DP3BumpEnable = false;
    AnimationManager.Reset(new C3DAnimationManager);
    AnimationManager->AddExclusiveObserver(this);
    AnimationResourceBank.SetUnloadDelay(0);
    PhysicsMeshResourceBank.SetUnloadDelay(0);
    MeshStatsResourceBank.SetUnloadDelay(0);
    UnloadableMeshMemory = 0;
    UnloadMeshFrameDelay = 5;
    MaxParticleMeshFacesToRepeat = 64;
    SetIsStaticMap(true);
}