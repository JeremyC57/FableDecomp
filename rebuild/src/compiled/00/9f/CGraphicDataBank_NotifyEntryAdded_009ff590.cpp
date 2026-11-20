// CGraphicDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- grow the graphic list
//   to cover the new entry, read its graphic header from the entry data and store a fresh CGraphicInfo for it, then let CBankFile
//   record the entry
// Leaf: ?NotifyEntryAdded@CGraphicDataBank@@MAEXKKABV?$CArray@D@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FF590, `ret 0Ch`.
// Identity: Ego_r.pdb 0x00B5D420 + FableWin.pdb 0x03117E70 ?NotifyEntryAdded@CGraphicDataBank@@MAEXKKABV?$CArray@D@@@Z (protected virtual
//   override; no landed row references 0x009FF590, exe scan).
// Callees: CArray<CGraphicInfo>::push_back() 0x00A01650 (FableWin.pdb ?push_back@?$CArray@VCGraphicInfo@@@@QAEXXZ; out of line in retail:
//   a default CGraphicInfo, then the inline vector push_back), CMemoryDataInputStream::CMemoryDataInputStream(const void*, unsigned long)
//   0x00994930, CGraphicDataBank::CGraphicHeader::Load(CDataInputStream&) 0x009FD160 (Ego_r.pdb 0x00B568D0 + FableWin.pdb 0x03121EB0),
//   CGraphicInfo::CGraphicInfo(unsigned long, CGraphicDataBank*, const CGraphicHeader&) 0x009FD700, CBankFile::NotifyEntryAdded 0x009CBC20
//   (qualified base call, Ego_r.pdb ?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z), ~CDataStream 0x00993B10 (the stream local's
//   compiler-generated destructor chain). The header's inline constructor is expanded (the field stores at 0x009FF606..0x009FF650).
// NF141 (2026-11-20, q6182): ported onto the shared headers (engine/CGraphicDataBank.h / CGraphicInfo.h retail STATICBLOCKS branch,
//   CArray.h PUSHBACK_DEFAULT_DECL, CMemoryDataInputStream.h / CDataInputStream.h retail, overlay opt-in
//   FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD); was the _quarantine CGraphicDataBank donor include under a renaming macro plus a
//   dozen local stand-ins (CBankFileNative, InputStream, MemoryInput, GraphicHeader, GraphicInfo, GraphicArray, CharArray ...).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CGraphicInfo.h"
#include "engine/CDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"

void CGraphicDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    while (index >= GraphicList.size())
        GraphicList.push_back();
    CMemoryDataInputStream input(data.GetAsCArray(), data.size());
    CGraphicHeader header;
    header.Load(input);
    GraphicList[index] = CGraphicInfo(index, this, header);
    CBankFile::NotifyEntryAdded(type, index, data);
}
