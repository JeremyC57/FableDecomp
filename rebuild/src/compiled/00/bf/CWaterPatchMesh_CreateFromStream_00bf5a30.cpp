// CWaterPatchMesh::CreateFromStream(CMovableResource*, CEngineLandscapeMap*, CDataInputStream*) -- read the patch's water offset (X then Y) and build a CWaterPatchMesh for it from the landscape memory manager
// Leaf: CreateFromStream
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF5A30, 235 bytes, `ret 0Ch` (three stack arguments; ecx is never read).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path, x2),
//   CWaterPatchMesh::operator new(unsigned int, CMovableResource*) @ 0x00BEA7E0 (landed; __fastcall size/owner),
//   CWaterPatchMesh::CWaterPatchMesh(CEngineLandscapeMap*, CDataInputStream*, const C2DCoordI*) @ 0x00BEB490.
// Label: the manifest's "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated and wrong -- the body is the
//   out-of-line copy of the water-patch creation CEngineLandscapePatch::LoadForegroundPatch inlines at 0x00BF4088
//   (same 0x20-byte allocation from the owning patch, same ctor arguments). It has no direct callers; the name is
//   descriptive. Modelled as a __stdcall static.
// Layout: engine/CWaterPatchMesh.h (0x20 bytes, Offset C2DCoordI +8; data-only, so a method carrier derives from it);
//   engine/CDataInputStream.h for the inline Read. C2DCoordI {X, Y} per the PDB.
// Shape: the two inline reads land in the dead stream-argument slot and a frame word, then are copied into the
//   coordinate local; the class operator new is null-tested before the ctor (`xor eax,eax` on failure).
#include "engine/CDataInputStream.h"
#include "engine/CWaterPatchMesh.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

struct CMovableResource;
struct C2DCoordI {
    long X;     // +0
    long Y;     // +4
};

struct CWaterPatchMesh_Methods : CWaterPatchMesh {
    CWaterPatchMesh_Methods(CEngineLandscapeMap* map, CDataInputStream* stream, const C2DCoordI* offset);   // 0x00BEB490
    static void* __fastcall operator new(unsigned int size, CMovableResource* owner);                        // 0x00BEA7E0
    static void __fastcall operator delete(void* block, CMovableResource* owner);
    static CWaterPatchMesh* __stdcall CreateFromStream(CMovableResource* owner, CEngineLandscapeMap* map, CDataInputStream* stream);
};

CWaterPatchMesh* __stdcall CWaterPatchMesh_Methods::CreateFromStream(CMovableResource* owner, CEngineLandscapeMap* map, CDataInputStream* stream)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    long x = in->ReadLong();
    long y = in->ReadLong();
    C2DCoordI offset;
    offset.X = x;
    offset.Y = y;
    return new (owner) CWaterPatchMesh_Methods(map, stream, &offset);
}