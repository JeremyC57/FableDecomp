// C3DMeshFileEngineMaterialChunk::~C3DMeshFileEngineMaterialChunk() -- the compiler-generated destructor: release the MapFilenames counted block, destroy Name, then run ~CChunkedFileSuperChunk
// Leaf: ??1C3DMeshFileEngineMaterialChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D270, 43 bytes, thiscall, tail `jmp ~CChunkedFileSuperChunk`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00AC6290 of vtable 0x0129EFC8 (RTTI .?AVC3DMeshFileEngineMaterialChunk@@
//   : CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb ??1C3DMeshFileEngineMaterialChunk
//   @@UAE@XZ (donor 0x033D42F0, after the ??1C3DMeshFilePrimitiveChunk / ??1C3DMeshFileClothChunk pair 0x033D41A0 / 0x033D4200 --
//   retail's landed 0x00A9D3A0 / 0x00A9D3C0). No 0x0129EFC8 store: the destructor is the implicit one. The catalog label
//   CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon is propagated.
// Layout (engine/C3DMeshFileEngineMaterialChunk.h retail offsets, 0x54): CChunkedFileSuperChunk 0x2c, MaterialIndex +0x2c, Name
//   (CCharString) +0x30, MapFilenames (CIVCountedPointer<CMapFilenames>) +0x34, the four bank indices +0x38..+0x44, MapFlags +0x48,
//   SelfIllumination +0x4c, TwoSided / Transparent / BooleanAlpha / DegenerateTriangles +0x50..+0x53.
// Callees: the pointee's virtual Delete (CIVCountedPointeeBase slot 1, +4), CCharString::~CCharString 0x0099EAE0,
//   CChunkedFileSuperChunk::~CChunkedFileSuperChunk 0x00AA8490 (tail).
// Shape: members in reverse order; the counted pointer's inline dtor is FableWin.pdb's ~CIVCountedPointer() -> Release()
//   (Object copied into a named local first -- GOTCHAS chunk 656: reading the member twice reloads it after the `dec` --
//   then `if (object) { if (object->Release() == 0) object->Delete(); Object = 0; }`: the zero store sits inside the null test).
//   The chunk headers flatten the vptr, so the chain is the landed sibling 0x00A9D3C0 model, sized against
//   engine/C3DMeshFileEngineMaterialChunk.h included under a layout name (chunk 708 pattern). DestroyOne (a destruction by
//   name, ungraded) makes VC7.1 emit this out-of-line implicit `??1` (chunk 656/657).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"                         // retail model (8 bytes: Data, Info)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"                   // vptr, slot 1 Delete, IVRefCount +4

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

#include "engine/CArray.h"


template <class T> class CIVCountedPointer {                // Ego_r.pdb: one CIVCountedPointeeBase* (4 bytes)
public:
    ~CIVCountedPointer() { Release(); }                     // FableWin.pdb ??1?$CIVCountedPointer@...@@QAE@XZ (inline here)
    void Release()                                          // FableWin.pdb ?Release@?$CIVCountedPointer@...@@QAEXXZ (inline here)
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                          // +0x00
};

#define C3DMeshFileEngineMaterialChunk C3DMeshFileEngineMaterialChunkFields
#include "engine/C3DMeshFileEngineMaterialChunk.h"          // retail offsets / sizeof 0x54 (flattened vptr: layout name only)
#undef C3DMeshFileEngineMaterialChunk
class CChunkedFileSuperChunk;

class CChunkedFileChunk : public CBaseClassNonCopyable {   // vtable 0x0129E474, 0x10 bytes
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char ChunkType[5];                                      // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0c
};

class CChunkedFileSuperChunk : public CChunkedFileChunk {   // vtable 0x0129E4F0, 0x2c bytes in retail
public:
    virtual ~CChunkedFileSuperChunk();                      // 0x00AA8490
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;               // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes; // +0x1c
    unsigned long CurrentChunkIndex;                        // +0x28
};

class C3DMeshFileEngineMaterialChunk : public CChunkedFileSuperChunk {   // vtable 0x0129EFC8, 0x54 bytes in retail
public:
    class CMapFilenames;                                    // : CIVCountedPointeeBase (DiffuseMap / BumpMap / ReflectionMap / IlluminationMap)
    C3DMeshFileEngineMaterialChunk();                       // ??0C3DMeshFileEngineMaterialChunk@@QAE@XZ
    long                              MaterialIndex;        // +0x2c
    CCharString                       Name;                 // +0x30
    CIVCountedPointer<CMapFilenames>  MapFilenames;         // +0x34
    unsigned long                     DiffuseMapBankIndex;  // +0x38
    unsigned long                     BumpMapBankIndex;     // +0x3c
    unsigned long                     ReflectionMapBankIndex;    // +0x40
    unsigned long                     IlluminationMapBankIndex;  // +0x44
    unsigned long                     MapFlags;             // +0x48
    unsigned long                     SelfIllumination;     // +0x4c
    bool                              TwoSided;             // +0x50
    bool                              Transparent;          // +0x51
    bool                              BooleanAlpha;         // +0x52
    bool                              DegenerateTriangles;  // +0x53
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFileEngineMaterialChunk) == sizeof(C3DMeshFileEngineMaterialChunkFields));

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DMeshFileEngineMaterialChunk* chunk)
{
    chunk->C3DMeshFileEngineMaterialChunk::~C3DMeshFileEngineMaterialChunk();
}