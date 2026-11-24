// C3DMeshFileMaterialExtChunk::GetChunkInfo() const -- "Self Illumination: " + GFIntToCharString(SelfIllumination); the number string is built first (right operand), the literal temporary destroyed first
// Leaf: ?GetChunkInfo@C3DMeshFileMaterialExtChunk@@UBE?BVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFD70, length 80 bytes.
// Callees: 0x0099F830 __fastcall GFIntToCharString, 0x0099EBF0 CCharString(const char*, long), 0x0099F570 __fastcall operator+(const CCharString&, const CCharString&), 0x0099EAE0 ~CCharString x2
//
// Body port (NF321, 2026-11-23): defined on the real class of engine/C3DMeshFileMaterialExtChunk.h (FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK; was
//   ?GetChunkInfo@C3DMeshFileMaterialExtChunk_Methods@@QBE?AVCCharString@@XZ) with the PDB `const CCharString` return; the retail RTTI vftable
//   placement names this PDB member here. The local CCharString model is fable_string.h's CCharString, whose
//   FABLE_RETAIL_CCHARSTRING_CONCAT_STRING declares the free ??H@YI?AVCCharString@@ABV0@0@Z (0x0099F570, the spelling the landed binder
//   0x00A381D0 binds); GFIntToCharString is engine/fable_script_text.h's ?GFIntToCharString@@YI?AVCCharString@@J@Z (landed self row 0x0099F830).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/C3DMeshFileMaterialExtChunk.h"

const CCharString C3DMeshFileMaterialExtChunk::GetChunkInfo() const
{
    return CCharString("Self Illumination: ", -1) + GFIntToCharString(SelfIllumination);
}
