// CGraphicDataBank::CGraphicHeader::Load(CDataInputStream& stream) -- read the 0x1c-byte graphic header record raw, then replace its pixel format with the one decoded by ReadPixelFormat
// Leaf: ?Load@CGraphicHeader@CGraphicDataBank@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD160, length 120 bytes (padded to 128).
// Identity: Ego_r.pdb 0x00B568D0 + FableWin.pdb 0x03121EB0 ?Load@CGraphicHeader@CGraphicDataBank@@QAEXAAVCDataInputStream@@@Z (the nested
//   PDB class, public); the load twin of CGraphicHeader::Save 0x009FD1E0; called from CGraphicDataBank::NotifyEntryAdded 0x009FF590 (which
//   binds this spelling) and the /Gr thunk 0x009FD210. (The catalog label "COpinionTransientOffset::TransferBinaryIn" was wrong.)
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (the inline Read's overflow arm),
//          CDataInputStream::ReadPixelFormat() @ 0x00994670.
// Shape: the inline bounds-checked CDataInputStream::Read (64-bit position guard, `rep movsd` fast path); the by-value CPixelFormat
//   result is built in the dead stream-argument slot and copied as one dword.
// Ported (NF205, 2026-11-20, q11815) onto engine/CGraphicDataBank.h (nested CGraphicHeader, NF141 FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD)
//   and the retail engine/CDataInputStream.h (NF205 opt-in FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT); was a local CGraphicHeader stand-in
//   over a CDataInputStreamReader friend hack (?Load@CGraphicHeader@@QAEXAAUCDataInputStreamReader@@@Z).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READPIXELFORMAT
#include "fable_compiler.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CDataInputStream.h"

void CGraphicDataBank::CGraphicHeader::Load(CDataInputStream& stream)
{
    stream.Read(this, sizeof(CGraphicHeader));
    PixelFormat = stream.ReadPixelFormat();
}
