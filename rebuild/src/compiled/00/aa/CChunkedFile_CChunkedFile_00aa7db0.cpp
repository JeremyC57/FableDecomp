// CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&) -- the CBaseClassNonCopyable shell, CChunkedFile's vtable
//   0x0129E4E8, the root chunk pointer copied (the shared count block incremented when present), Version = 0, an empty FileComment
// Leaf: ??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7DB0, length 63 bytes, thiscall, `ret 4`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CCharString::CCharString(const char*, long) 0x0099EBF0 ("", -1;
//   the literal at 0x0122D70E).
// Name: FableWin.pdb 0x034693B0 and Ego_r.pdb 0x00C1C4B0 (index 30 of lib_chunked_file.obj) spell
//   ??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z; the binding table already binds it here (witness
//   0x00A98E60). NF416: was the stand-in ??0CChunkedFile@@QAE@ABUCCountedSuperChunkPointer@@@Z on a row-local model; now the shared
//   engine/CChunkedFile.h.
// Option S unit (owner ruling 2026-11-24; NF416): ONE translation unit, retail lib_chunked_file.obj, for 0x00AA7DB0 (this ctor, the
//   owner), 0x00AA7DF0 (~CChunkedFile, written below) and 0x00AA9190 (the compiler-generated ??_GCChunkedFile@@UAEPAXI@Z, ~CChunkedFile
//   expanded). FableWin.pdb and Ego_r.pdb both list all three in that .obj (Ego_r indexes 30 / 31 / 63). The two thin rows include
//   this file. Bodies in retail address order.
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CChunkedFile.h"

CChunkedFile::CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& rootChunk)
    : CBaseClassNonCopyable(), RootChunk(rootChunk), Version(0), FileComment("", -1)
{
}

// CChunkedFile::~CChunkedFile() -- 0x00AA7DF0, 74 bytes: own vtable, ~CCharString on FileComment 0x0099EAE0, the root chunk's count
//   block released (DeleteFunc on Data, then operator delete 0x00BFE9BC on the block), the pointer pair nulled, `jmp` into
//   ~CBaseClassNonCopyable 0x0099A300 (table ??1CBaseClassNonCopyable@@MAE@XZ).
CChunkedFile::~CChunkedFile()
{
}
