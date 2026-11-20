// CWaterPatchMesh::CreateFromStream(CMovableResource*, CEngineLandscapeMap*, CDataInputStream*) -- read the patch's water offset (X then Y) and build a CWaterPatchMesh for it from the landscape memory manager
// Leaf: ?CreateFromStream@CWaterPatchMesh@@SGPAV1@PAVCMovableResource@@PAVCEngineLandscapeMap@@PAVCDataInputStream@@@Z
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
#include "fable_compiler.h"
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
// NF188 (2026-11-20, q6204): CreateFromStream is a static of the shared engine/CWaterPatchMesh.h retail class (opt-in
//   FABLE_RETAIL_CWATERPATCHMESH_CREATEFROMSTREAM: the PDB class operator new ??2CWaterPatchMesh@@SIPAXIPAVCMovableResource@@@Z 0x00BEA7E0)
//   and builds the mesh with the PDB constructor ??0CWaterPatchMesh@@QAE@PBVCEngineLandscapeMap@@AAVCDataInputStream@@AAVC2DCoordI@@@Z
//   0x00BEB490 (Ego_r.pdb / FableWin.pdb); was the method carrier CWaterPatchMesh_Methods (a data-only base) with pointer parameters.
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CWATERPATCHMESH_CREATEFROMSTREAM
#define FABLE_RETAIL_C2DCOORDI_COPY
#include "engine/CWaterPatchMesh.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
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
    FABLE_FORCEINLINE long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

CWaterPatchMesh* __stdcall CWaterPatchMesh::CreateFromStream(CMovableResource* owner, CEngineLandscapeMap* map, CDataInputStream* stream)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    long x = in->ReadLong();
    long y = in->ReadLong();
    C2DCoordI offset;
    offset.X = x;
    offset.Y = y;
    return new (owner) CWaterPatchMesh(map, *stream, offset);
}