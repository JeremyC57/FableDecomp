// CDataInputStream::ReadPixelFormat() -- read the 6-byte pixel-format description (zeroed if the position would leave [0, 0x7FFFFFFF]), build a CPixelFormat from it and return it by value
// Leaf: ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994670, length 137 bytes (`ret 4`: the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) when the current source
// chunk holds fewer than 6 bytes, CPixelFormat::Initialise(const CPixelFormatInit&) @ 0x009E3640.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the inlined Read(buffer, size) of the landed TransferBinaryIn family (0x007AD7F0); the 6-byte description is
// zeroed with an intrinsic memset (dword + word) and filled by the intrinsic memcpy (dword + word through ebx/di); the
// 4-byte CPixelFormat is a named local initialised out of line and then copied into the hidden result (no NRVO).
// Identity: Ego_r.pdb / FableWin.pdb ?ReadPixelFormat@CDataInputStream@@QAE?AVCPixelFormat@@XZ (public); its landed caller
//   CGraphicDataBank::CGraphicHeader::Load 0x009FD160 binds it.
// Ported (NF205, 2026-11-20, q11815) onto the retail engine/CDataInputStream.h (inline Read; NF205 opt-in
//   FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT), engine/CPixelFormatInit.h and engine/CPixelFormat.h (FABLE_RETAIL_CPIXELFORMAT_INIT:
//   the FableWin ??0CPixelFormat@@QAE@ABVCPixelFormatInit@@@Z constructor, Initialise only); was a CDataInputStreamReader stand-in
//   (?ReadPixelFormat@CDataInputStreamReader@@QAE?AVCPixelFormat@@XZ) with local CPixelFormat / CPixelFormatInit models.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#include "fable_compiler.h"
#include "engine/CDataInputStream.h"
#include "engine/CPixelFormatInit.h"
#include "engine/CPixelFormat.h"
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

CPixelFormat CDataInputStream::ReadPixelFormat()
{
    CPixelFormatInit init;
    memset(&init, 0, 6);
    Read(&init, 6);
    CPixelFormat format(init);
    return format;
}
