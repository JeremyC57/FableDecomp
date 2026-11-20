// C3DMeshFileMapChunk::C3DMeshFileMapChunk() -- base chunk "MMAP", MapType -1, two default strings (CRC left unset)
// Leaf: ??0C3DMeshFileMapChunk@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFE50, length 48 bytes:
//   push esi ; push "MMAP" ; mov esi,ecx ; call 0x00AA8360 (CChunkedFileSuperChunk ctor) ; lea ecx,[esi+30h] ;
//   mov [esi],0129EE34h (vptr) ; mov [esi+2Ch],-1 ; call 0x0099E4B0 (CCharString()) ; lea ecx,[esi+34h] ; call 0x0099E4B0 ; mov eax,esi ; pop esi ; ret
// Port (NF202, 2026-11-20, q12251): the row-local CCharString / CDataOutputStream / CChunkedFileSuperChunk / C3DMeshFileMapChunk models and
//   their 36-byte stand-in vtable are replaced by the shared engine/C3DMeshFileMapChunk.h (polymorphic retail class: vptr 0x0129EE34, MapType
//   +0x2C, MapDescription +0x30, Filename +0x34, CRC +0x38; retail sizeof 0x3C), the class the landed CreateMap 0x00AC0C00 uses
//   (FABLE_RETAIL_C3DMESHFILEMAPCHUNK_TYPE_CTOR there). The default constructor stores -1 in MapType (Ego_r.pdb names the +0x2C member
//   MapType; the old local name MapIndex was made up). The vtable / ??_G / DeleteData COMDATs this TU now emits are those of 0x00AC0C00.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#include "fable_string.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DMeshFileMapChunk.h"

C3DMeshFileMapChunk::C3DMeshFileMapChunk()
    : CChunkedFileSuperChunk("MMAP"), MapType((EMaterialMapType)-1)
{
}
