// C3DAnimFileLoader::C3DAnimFileLoader() -- build the "3DAF" chunked-file loader: create the ANRT root chunk type, then register the object/helpers sub-chunk types under the root, sequence/bone-mask under the object, movement-vector/timing-event under the helpers and the movement track under the movement vector, keeping each registered type
// Leaf: ??0C3DAnimFileLoader
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A999B0, 1152 bytes, plain ret.
// Callees: CChunkedFileLoader::CChunkedFileLoader(const char*) 0x00AA7510 ("3DAF"), operator new 0x00BFEA1A,
//   CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) 0x00AA8360 ("ANRT"; C3DAnimFileRootChunk vtable
//   0x0129DFF0), CCountedPointer<C3DAnimFileRootChunk>::Reset 0x00A9A420, the prototype constructors
//   C3DAnimFileObjectChunk 0x00AB0020, C3DAnimFileHelpersChunk 0x00AAE090, C3DAnimFileSequenceChunk 0x00AAF730,
//   C3DAnimFileBoneMaskChunk 0x00AB0700, C3DAnimFileMovementVectorChunk 0x00AAE2A0, C3DAnimFileTimingEventChunk
//   0x00AAE170; the registrars 0x00A98BE0 (root), 0x00A98C80 (object), 0x00A98D20 (helpers), 0x00A98DC0 (movement
//   vector), each returning the registered CCountedPointer by value; ~CChunkedFileSuperChunk 0x00AA8490,
//   ~CChunkedFileChunk 0x00AA75C0, ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC, free 0x00BFEA14.
// Shape: each prototype is an unnamed temporary argument (the out-of-line ctor's eax result is pushed), all in one
//   frame slot; the returned CCountedPointer goes through the inline operator= (other's pointer and count read into
//   locals first, pointer-equality test, release old, copy, add reference), then the result temporary is released
//   and cleared and the prototype's implicit dtor runs. ChunkTypes is the STLport vector with its real
//   _Vector_base / _STLP_alloc_proxy structure: a flat 3-pointer model schedules `push 34h` one store too early.
// Layout: retail CArray is 12 bytes (PDB 16): CChunkedFileSuperChunk 0x2c, sequence chunk BoneName +0x34 /
//   Matrices +0x40, object chunk Name +0x2c; loader members as in the PDB. Manifest labels of the prototype ctors
//   (0x00AB0700 "XSequenceChunk", 0x00AAE2A0 "HelpersChunk", 0x00AAE170 "ObjectChunk") are propagated: the
//   destructor shapes give BoneMask (CArray +0x10), MovementVector (super chunk only), TimingEvent (CCharString +0x10).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                         // 0x0099EAE0
};

#include "engine/CCountedPointer.h"

#include "engine/CArray.h"

// STLport vector<CChunkedFileChunk*> with its real base structure (end-of-storage in an allocator proxy).
template <class T> struct allocator { allocator() {} allocator(const allocator&) {} };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
};
template <class T> struct vector : _Vector_base<T> {
    explicit vector(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) {}
};

class CChunkedFileSuperChunk;
class CChunkedFileChunk { public:                  // 0x10
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
    virtual ~CChunkedFileChunk();           // 0x00AA75C0
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:   // 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;               // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes; // +0x1c
    unsigned long CurrentChunkIndex;                                      // +0x28
    protected:
    CChunkedFileSuperChunk(const char* type);                             // 0x00AA8360 (Ego_r / FableWin: protected ??0...@@IAE@PBD@Z)
    public:
    virtual ~CChunkedFileSuperChunk();                                    // 0x00AA8490
};
class CMatrix3x4 { public: float m[12]; };
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { public:        // vtable 0x0129DFF0
    bool LoopFlag;                          // +0x2c
    float Duration;                         // +0x30
    C3DAnimFileRootChunk() : CChunkedFileSuperChunk("ANRT") {}
};
class C3DAnimFileObjectChunk : public CChunkedFileSuperChunk { public:
    CCharString Name;                       // +0x2c
    long SubMeshIndex, ParentIndex, FirstChildIndex, NextSiblingIndex;
    C3DAnimFileObjectChunk();               // 0x00AB0020
};
class C3DAnimFileHelpersChunk : public CChunkedFileSuperChunk { public:
    C3DAnimFileHelpersChunk();              // 0x00AAE090
};
class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:
    long BoneIndex;                         // +0x2c
    long ParentIndex;                       // +0x30
    CCharString BoneName;                   // +0x34
    float SamplesPerSecond;                 // +0x38
    long BoneType;                          // +0x3c
    CArray<CMatrix3x4> Matrices;            // +0x40
    C3DAnimFileSequenceChunk();             // 0x00AAF730
};
class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk { public:
    CArray<bool> EnableMask;                // +0x10
    C3DAnimFileBoneMaskChunk();             // 0x00AB0700
};
#include "engine/C3DVector.h"
class C3DAnimFileMovementVectorChunk : public CChunkedFileSuperChunk { public:
    C3DVector Vector;                       // +0x2c
    C3DAnimFileMovementVectorChunk();       // 0x00AAE2A0
};
class C3DAnimFileTimingEventChunk : public CChunkedFileChunk { public:
    CCharString Name;                       // +0x10
    float Time;                             // +0x14
    C3DAnimFileTimingEventChunk();          // 0x00AAE170
};

class CChunkedFileLoader { public:                 // 0x10
    long ReadUnknownChunkTypes;             // +0x04
    char SupportedFileType[5];              // +0x08
    CChunkedFileLoader(const char* fileType);   // 0x00AA7510
    virtual ~CChunkedFileLoader();
};

struct C3DAnimFileLoader : CChunkedFileLoader {   // 0x60, vtable 0x0129E060
    CCountedPointer<C3DAnimFileRootChunk> RootChunkType;        // +0x10
    CCountedPointer<CChunkedFileChunk> ObjectChunkType;          // +0x18
    CCountedPointer<CChunkedFileChunk> SequenceChunkType;        // +0x20
    CCountedPointer<CChunkedFileChunk> BoneMaskChunkType;        // +0x28
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;         // +0x30
    CCountedPointer<CChunkedFileChunk> MovementVectorChunkType;  // +0x38
    CCountedPointer<CChunkedFileChunk> MovementTrackChunkType;   // +0x40
    CCountedPointer<CChunkedFileChunk> TimingEventChunkType;     // +0x48
    vector<CChunkedFileChunk*>         ChunkTypes;               // +0x50
    C3DAnimFileLoader();
    virtual ~C3DAnimFileLoader();
    CCountedPointer<CChunkedFileChunk> AddRootSubChunkType(const CChunkedFileChunk& prototype);            // 0x00A98BE0
    CCountedPointer<CChunkedFileChunk> AddObjectSubChunkType(const CChunkedFileChunk& prototype);          // 0x00A98C80
    CCountedPointer<CChunkedFileChunk> AddHelpersSubChunkType(const CChunkedFileChunk& prototype);         // 0x00A98D20
    CCountedPointer<CChunkedFileChunk> AddMovementVectorSubChunkType(const CChunkedFileChunk& prototype);  // 0x00A98DC0
};

C3DAnimFileLoader::C3DAnimFileLoader() : CChunkedFileLoader("3DAF")
{
    RootChunkType.Reset(new C3DAnimFileRootChunk);
    ObjectChunkType = AddRootSubChunkType(C3DAnimFileObjectChunk());
    HelpersChunkType = AddRootSubChunkType(C3DAnimFileHelpersChunk());
    SequenceChunkType = AddObjectSubChunkType(C3DAnimFileSequenceChunk());
    BoneMaskChunkType = AddObjectSubChunkType(C3DAnimFileBoneMaskChunk());
    MovementVectorChunkType = AddHelpersSubChunkType(C3DAnimFileMovementVectorChunk());
    TimingEventChunkType = AddHelpersSubChunkType(C3DAnimFileTimingEventChunk());
    MovementTrackChunkType = AddMovementVectorSubChunkType(C3DAnimFileSequenceChunk());
}