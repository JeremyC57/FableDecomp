// C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool)
// Grade: DIFFER(2613v3051) -- md=1333: local-model draft of the STLport-heavy body (helper/dummy/volume vectors, NameCRC loops, sort/copy instantiations); 438 bytes short: retail expands the vector push_back/insert paths and the NameCRC loops differently
// Next idea: rebuild on the shared stlport headers (fable_containers.h / fable_map.h) with the real C3DMeshStats / C3DMeshFileHelpersChunk headers so the same push_back / _M_insert_overflow inline split falls out; then per-block iteration
//   -- build a mesh's stats block from its file: name, animated flag and pool stored; from the root chunk's helpers, every helper
//   point (keyed by the CRC of its name, name collected) and every dummy object -- a dummy named "createmesh <mesh>" becomes a mesh
//   generator instead -- and every mesh volume (keyed by name CRC, the volume map sorted); the collected names packed, the buffers
//   allocated, the helper points and dummy objects copied into their pre-allocated maps and sorted, the packed names copied in; the
//   bounding sphere from the XBDS chunk and the bounding box (centre +- extent) from the XBDB chunk; returns true
// Leaf: ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2F740, 3051 bytes, thiscall, `ret 10h`, frame 0x170, no EH frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?InitialiseFromFileData@C3DMeshStats@@QAE_NABVCCharString@@ABVC3DMeshFile@@_NPAVCMovableResourceMemoryPool@@@Z
//   (0x017C4CFF / 0x031E2E40 PDB entries); its callees are the landed C3DMeshStats::PackNames 0x00A2EEF0 / AllocateBuffers 0x00A2E760.
// Draft model (DIFFER oracle, not a landing candidate): the shared chunk headers carry no FindFirst*/FindNext*/GetRoot members and
//   C3DMeshStats / C3DHelperPoint / C3DDummyObject / C3DGeneratorObject / C3DMeshVolume have no shared header, so this draft keeps
//   local models. Layout (retail, from the bytes): Name +0x0c, BoundingSphere +0x10, BoundingBox +0x20, HelperPoints +0x38 (compare,
//   data +0x3c, count +0x40, dirty +0x44), DummyObjects +0x48, MeshVolumes +0x58 (CVectorMap, dirty +0x65), MeshGenerators +0x68,
//   PackedNames +0x74, AnimatedFlag +0x7a.
#include <string.h>
// CCharString: local model (fable_string.h has no GetAsLowerCase, and a root-level header cannot be overlaid -- its includers
// resolve the repo copy first). Ego_r.pdb / FableWin.pdb ?GetAsLowerCase@CCharString@@QBE?AV1@XZ 0x0099ED10 (naming wave 1).
struct CCharStringData { char* Text; long Length; };
extern const char NullChar;                                         // the empty string 0x0129AAF4
class CCharString {
public:
    CCharString(const char* text, long length = -1);                // 0x0099EBF0
    CCharString(const CCharString& other);                          // 0x0099EC30
    ~CCharString();                                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);              // 0x0099EFB0
    CCharString GetAsLowerCase() const;                             // 0x0099ED10
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetAsCharArray() const { const char* text = &NullChar; if (PStringData) text = PStringData->Text; return text; }
    bool StartsWith(const CCharString& prefix) const { return strncmp(GetAsCharArray(), prefix.GetAsCharArray(), prefix.GetLength()) == 0; }
    CCharStringData* PStringData;
};
#include "engine/C3DVector.h"
extern const unsigned long g_crc32Table[256];                       // 0x0129A168
inline unsigned long NameCRC(const CCharString& name)               // the inline CRC of the name (descriptive)
{
    unsigned long crc = 0;
    long length = name.GetLength();
    const char* text = name.GetAsCharArray();
    if (length) for (unsigned long i = 0; i < (unsigned long)length; ++i) crc = g_crc32Table[(unsigned char)text[i] ^ (crc & 0xff)] ^ (crc >> 8);
    return crc;
}
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void* p);                                      // 0x00BFE9BC
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); operator delete(Info); } Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) {
        if (Info != other.Info) { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); operator delete(Info); }
            Data = other.Data; Info = other.Info; if (Info) ++Info->RefCount; }
        return *this;
    }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
struct C3DMatrix { float M[12]; };
class C3DMeshFileHelperPointChunk { public: unsigned char _pad[0x2c]; C3DVector Position; long SubMeshIndex; long BoneIndex; CCharString Name; };
class C3DMeshFileDummyObjectChunk { public: unsigned char _pad[0x3c]; long BoneIndex; CCharString Name; unsigned char _pad44[0x50 - 0x44];
    C3DMatrix Transform; bool Flag80[1]; };
class C3DMeshFileConvexVolumeChunk { public: unsigned char _pad[0x2c]; CCharString Name; };
class C3DMeshFileHelpersChunk { public:
    CCountedPointer<C3DMeshFileHelperPointChunk> FindFirstPoint() const;                                           // 0x00AAA270
    CCountedPointer<C3DMeshFileHelperPointChunk> FindNextPoint(const CCountedPointer<C3DMeshFileHelperPointChunk>&) const;   // 0x00AAA300
    CCountedPointer<C3DMeshFileDummyObjectChunk> FindFirstDummy() const;                                           // 0x00AAA550
    CCountedPointer<C3DMeshFileDummyObjectChunk> FindNextDummy(const CCountedPointer<C3DMeshFileDummyObjectChunk>&) const;   // 0x00AAA5E0
    CCountedPointer<C3DMeshFileConvexVolumeChunk> FindFirstVolumeW() const;                                        // 0x00AAA830
    CCountedPointer<C3DMeshFileConvexVolumeChunk> FindNextVolumeW(const CCountedPointer<C3DMeshFileConvexVolumeChunk>&) const;   // 0x00AAA8C0
};
class CChunkedFileChunk { public: unsigned char _pad[0x10]; };
class C3DMeshFileXBoundingSphereChunk { public: unsigned char _pad[0x10]; C3DVector Centre; float Radius; };
class C3DMeshFileXBoundingBoxChunk { public: unsigned char _pad[0x10]; C3DVector Centre; C3DVector Extent; };
class C3DMeshFileRootChunk { public:
    bool HasHelpers() const;                                        // 0x00A9B2C0
    CCountedPointer<C3DMeshFileHelpersChunk> GetHelpers() const;   // 0x00A9B3D0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;   // 0x00AA7F40 (CChunkedFileSuperChunk)
};
class C3DMeshFile { public: CCountedPointer<C3DMeshFileRootChunk> GetRoot() const; };   // 0x00A32620
class CMovableResourceMemoryPool;
struct C3DHelperPoint { C3DVector Position; long BoneIndex; };
struct C3DDummyObject { C3DMatrix Transform; long BoneIndex; };
struct C3DGeneratorObject { C3DDummyObject Dummy; CCharString MeshName; long Handle; bool Flag;
    C3DGeneratorObject(const C3DDummyObject& d, const CCharString& n, bool f) : Dummy(d), MeshName(n), Handle(0), Flag(f) { } };
struct C3DMeshVolume { CCharString Name; CCountedPointer<void> Data;
    C3DMeshVolume(const C3DMeshFileConvexVolumeChunk& chunk, CMovableResourceMemoryPool* pool); };   // 0x00A2E9A0
struct C3DBoundingSphere { C3DVector Centre; float Radius; };
struct C3DBoundingBox { C3DVector Min, Max; };
template<class T> struct CRetailVector {                             // STLport vector (out-of-line growth)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector();
    void _M_insert_overflow(T* position, const T& x, const bool& tag, unsigned long count, bool atEnd);
    void push_back(const T& x) { if (_M_finish != _M_end_of_storage) { if (_M_finish) new(_M_finish) T(x); ++_M_finish; }
        else { bool tag; _M_insert_overflow(_M_finish, x, tag, 1, true); } }
};
inline void* operator new(unsigned int, void* p) { return p; }
template<class K, class V> struct CPair { K first; V second; CPair(const K& k, const V& v) : first(k), second(v) { } };
struct CNameSet {                                                   // std::set<CCharString> (STLport tree, 0x14-byte header node)
    CNameSet(); ~CNameSet();
    void insert_unique(const CCharString& name);                   // 0x004AC380
};
template<class V> struct CPreAllocVectorMap { bool Compare; CPair<unsigned long, V>* Data; unsigned long Count; bool Dirty; };
struct CVolumeMap { CRetailVector<CPair<unsigned long, C3DMeshVolume> > Items; bool Compare; bool Dirty;
    void Insert(const CPair<unsigned long, C3DMeshVolume>& item) { Items.push_back(item); Dirty = true; }
    void Update(); };                                               // sort 0x00A355F0
class C3DMeshStats {
public:
    virtual ~C3DMeshStats();
    bool InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool);
    void PackNames(const CNameSet& helpers, const CNameSet& dummies, CRetailVector<char>& packed);   // 0x00A2EEF0
    void AllocateBuffers(unsigned long helpers, unsigned long dummies, unsigned long names);            // 0x00A2E760
    void SortHelperPoints(); void SortDummyObjects();               // 0x00A35150 / 0x00A351B0 (std::sort instances)
    CMovableResourceMemoryPool* MemoryPool;                        // +0x04
    long StatsMemoryHandle;                                         // +0x08
    CCharString Name;                                               // +0x0c
    C3DBoundingSphere BoundingSphere;                               // +0x10
    C3DBoundingBox BoundingBox;                                     // +0x20
    CPreAllocVectorMap<C3DHelperPoint> HelperPoints;                // +0x38
    CPreAllocVectorMap<C3DDummyObject> DummyObjects;                // +0x48
    CVolumeMap MeshVolumes;                                         // +0x58
    CRetailVector<C3DGeneratorObject> MeshGenerators;               // +0x68
    char* PackedNames;                                              // +0x74
    unsigned short PackedNamesSize;                                 // +0x78
    bool AnimatedFlag;                                              // +0x7a
};

bool C3DMeshStats::InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool)
{
    Name = name;
    AnimatedFlag = animated;
    MemoryPool = pool;
    CCountedPointer<C3DMeshFileRootChunk> root = file.GetRoot();
    CRetailVector<CPair<unsigned long, C3DHelperPoint> > helperPoints;
    CRetailVector<CPair<unsigned long, C3DDummyObject> > dummyObjects;
    CNameSet helperNames;
    CNameSet dummyNames;
    if (root->HasHelpers()) {
        CCountedPointer<C3DMeshFileHelpersChunk> helpers = root->GetHelpers();
        for (CCountedPointer<C3DMeshFileHelperPointChunk> point = helpers->FindFirstPoint(); !point.IsNull(); point = helpers->FindNextPoint(point)) {
            C3DHelperPoint helper;
            helper.Position = point->Position;
            helper.BoneIndex = point->BoneIndex;
            helperPoints.push_back(CPair<unsigned long, C3DHelperPoint>(NameCRC(point->Name), helper));
            helperNames.insert_unique(point->Name);
        }
        for (CCountedPointer<C3DMeshFileDummyObjectChunk> dummy = helpers->FindFirstDummy(); !dummy.IsNull(); dummy = helpers->FindNextDummy(dummy)) {
            C3DDummyObject object;
            object.Transform = dummy->Transform;
            object.BoneIndex = dummy->BoneIndex;
            bool flag = dummy->Flag80[0];
            if (dummy->Name.GetAsLowerCase().StartsWith(CCharString("createmesh "))) {
                MeshGenerators.push_back(C3DGeneratorObject(object, CCharString(dummy->Name.GetAsCharArray() + 11), flag));
            } else {
                dummyObjects.push_back(CPair<unsigned long, C3DDummyObject>(NameCRC(dummy->Name), object));
                dummyNames.insert_unique(dummy->Name);
            }
        }
        for (CCountedPointer<C3DMeshFileConvexVolumeChunk> volume = helpers->FindFirstVolumeW(); !volume.IsNull(); volume = helpers->FindNextVolumeW(volume)) {
            C3DMeshVolume meshVolume(*volume.Data, pool);
            MeshVolumes.Insert(CPair<unsigned long, C3DMeshVolume>(NameCRC(volume->Name), meshVolume));
        }
        MeshVolumes.Update();
    }
    CRetailVector<char> packed;
    PackNames(helperNames, dummyNames, packed);
    unsigned long packedSize = packed._M_finish - packed._M_start;
    AllocateBuffers(helperPoints._M_finish - helperPoints._M_start, dummyObjects._M_finish - dummyObjects._M_start, packedSize);
    for (CPair<unsigned long, C3DHelperPoint>* h = helperPoints._M_start; h != helperPoints._M_finish; ++h) HelperPoints.Data[h - helperPoints._M_start] = *h;
    SortHelperPoints();
    HelperPoints.Dirty = false;
    for (CPair<unsigned long, C3DDummyObject>* d = dummyObjects._M_start; d != dummyObjects._M_finish; ++d) DummyObjects.Data[d - dummyObjects._M_start] = *d;
    SortDummyObjects();
    DummyObjects.Dirty = false;
    if (packedSize) memmove(PackedNames, packed._M_start, packedSize);
    {
        CCountedPointer<CChunkedFileChunk> chunk = root->FindFirstSubChunkOfType("XBDS");
        C3DMeshFileXBoundingSphereChunk* sphere = (C3DMeshFileXBoundingSphereChunk*)chunk.Data;
        if (sphere) { BoundingSphere.Centre = sphere->Centre; BoundingSphere.Radius = sphere->Radius; }
    }
    {
        CCountedPointer<CChunkedFileChunk> chunk = root->FindFirstSubChunkOfType("XBDB");
        C3DMeshFileXBoundingBoxChunk* box = (C3DMeshFileXBoundingBoxChunk*)chunk.Data;
        if (box) {
            BoundingBox.Min = box->Centre - box->Extent;
            BoundingBox.Max = box->Centre + box->Extent;
        }
    }
    return true;
}
